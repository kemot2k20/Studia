import { Server } from 'socket.io';
import { GameState, GAME_CONSTANTS, ServerToClientEvents, Ball } from '../../shared/types';

const delay = (ms: number) => new Promise(resolve => setTimeout(resolve, ms));

export class Game {
    private io: Server<any, ServerToClientEvents>;
    private roomId: string;
    private players: [string, string]; 
    private hostName: string;
    private guestName: string;
    
    private gameLoopInterval: NodeJS.Timeout | null = null;
    private spawnerTimeout: NodeJS.Timeout | null = null; 
    private isSpawning = false;

    private state: GameState = {
        paddles: {
            player1Y: GAME_CONSTANTS.HEIGHT / 2 - GAME_CONSTANTS.PADDLE_HEIGHT / 2,
            player2Y: GAME_CONSTANTS.HEIGHT / 2 - GAME_CONSTANTS.PADDLE_HEIGHT / 2
        },
        scores: { player1: 0, player2: 0 },
        balls: []
    };

    private playerInputs: { [socketId: string]: number } = {};

    constructor(io: Server, roomId: string, hostId: string, guestId: string, hostName: string, guestName: string) {
        this.io = io;
        this.roomId = roomId;
        this.players = [hostId, guestId];
        this.playerInputs[hostId] = 0;
        this.playerInputs[guestId] = 0;
        this.hostName = hostName;
        this.guestName = guestName;
    }

    public start() {
        this.io.to(this.players[0]).emit('game_start', { 
            isHost: true, 
            p1Nickname: this.hostName, 
            p2Nickname: this.guestName 
        });

        this.io.to(this.players[1]).emit('game_start', { 
            isHost: false, 
            p1Nickname: this.hostName, 
            p2Nickname: this.guestName 
        });

        this.io.to(this.roomId).emit('start_countdown');

        setTimeout(() => {
            // 60 fps
            this.gameLoopInterval = setInterval(() => {
                this.update();
                this.broadcast();
            }, 1000 / 60);

            this.spawnWave(false);

        }, 3000);
    }

    public stop() {
        if (this.gameLoopInterval) clearInterval(this.gameLoopInterval);
        if (this.spawnerTimeout) clearTimeout(this.spawnerTimeout);
        this.isSpawning = false;
    }

    public handleInput(socketId: string, dir: 'up' | 'down' | 'stop') {
        let move = 0;
        if (dir === 'up') move = -1;
        if (dir === 'down') move = 1;
        this.playerInputs[socketId] = move;
    }

    private async spawnWave(multi: boolean) {
        if (this.isSpawning) return;
        this.isSpawning = true;

        const speed = 5;
        let cnt = 1;
        if (multi) {
            const vec = [1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 4, 5];
            const randomIndex = Math.floor(Math.random() * vec.length);
            cnt = vec[randomIndex];
        }

        for (let i = 0; i < cnt; i++) {
            if (!this.gameLoopInterval) break;

            this.createSingleBall(speed);
            if (i < cnt - 1) {
                await delay(GAME_CONSTANTS.LAUNCH_COOLDOWN);
            }
        }

        this.isSpawning = false;

        if (this.gameLoopInterval) {
            this.spawnerTimeout = setTimeout(() => {
                this.spawnWave(true);
            }, 3500);
        }
    }

    private createSingleBall(speed: number) {
        this.state.balls.push({
            id: Math.random().toString(),
            x: GAME_CONSTANTS.WIDTH / 2,
            y: GAME_CONSTANTS.HEIGHT / 2,
            vx: speed * (Math.random() > 0.5 ? 1 : -1),
            vy: (Math.random() * speed * 2) - speed,
            color: `hsl(${Math.random() * 360}, 100%, 50%)`
        });
    }

    private finishGame(winnerId: string) {
        this.stop();
        
        const winnerNickname = (winnerId === this.players[0]) ? this.hostName : this.guestName;

        this.io.to(this.roomId).emit('game_over', { 
            winnerId, 
            winnerNickname,
            score: this.state.scores 
        });
        
        console.log(`Koniec gry w pokoju ${this.roomId}. Wygrał: ${winnerNickname}`);
    }

    private update() {
        const hostId = this.players[0];
        const guestId = this.players[1];

        if (this.playerInputs[hostId] !== 0) {
            this.state.paddles.player1Y += this.playerInputs[hostId] * GAME_CONSTANTS.PADDLE_SPEED;
            this.state.paddles.player1Y = Math.max(0, Math.min(GAME_CONSTANTS.HEIGHT - GAME_CONSTANTS.PADDLE_HEIGHT, this.state.paddles.player1Y));
        }

        if (this.playerInputs[guestId] !== 0) {
            this.state.paddles.player2Y += this.playerInputs[guestId] * GAME_CONSTANTS.PADDLE_SPEED;
            this.state.paddles.player2Y = Math.max(0, Math.min(GAME_CONSTANTS.HEIGHT - GAME_CONSTANTS.PADDLE_HEIGHT, this.state.paddles.player2Y));
        }

        if (!this.state.balls.length && !this.isSpawning) {
            if (this.spawnerTimeout) clearTimeout(this.spawnerTimeout);
            this.spawnWave(true);
        }

        for (let i = this.state.balls.length - 1; i >= 0; i--) {
            const ball = this.state.balls[i];
            
            ball.x += ball.vx;
            ball.y += ball.vy;

            if (ball.y <= 0 + GAME_CONSTANTS.BALL_RADIUS || ball.y >= GAME_CONSTANTS.HEIGHT - GAME_CONSTANTS.BALL_RADIUS) {
                ball.vy *= -1;
            }

            let paddleHit = null;
            
            if (ball.x - GAME_CONSTANTS.BALL_RADIUS < GAME_CONSTANTS.PADDLE_WIDTH + 10 &&
                ball.x > GAME_CONSTANTS.PADDLE_WIDTH &&
                ball.y >= this.state.paddles.player1Y &&
                ball.y <= this.state.paddles.player1Y + GAME_CONSTANTS.PADDLE_HEIGHT) {
                paddleHit = 'player1';
            }
            
            else if (ball.x + GAME_CONSTANTS.BALL_RADIUS > GAME_CONSTANTS.WIDTH - 10 - GAME_CONSTANTS.PADDLE_WIDTH &&
                     ball.x < GAME_CONSTANTS.WIDTH - GAME_CONSTANTS.PADDLE_WIDTH &&
                     ball.y >= this.state.paddles.player2Y &&
                     ball.y <= this.state.paddles.player2Y + GAME_CONSTANTS.PADDLE_HEIGHT) {
                paddleHit = 'player2';
            }

            if (paddleHit) {
                const paddleY = (paddleHit === 'player1') ? this.state.paddles.player1Y : this.state.paddles.player2Y;
                const paddleCenter = paddleY + (GAME_CONSTANTS.PADDLE_HEIGHT / 2);
                let collidePoint = (ball.y - paddleCenter) / (GAME_CONSTANTS.PADDLE_HEIGHT / 2);
                collidePoint = Math.max(-1, Math.min(1, collidePoint));
                
                const angleRad = collidePoint * (Math.PI / 4);
                const currentSpeed = Math.sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
                let newSpeed = currentSpeed * 1.10;
                const MAX_SPEED = 30; 
                if (newSpeed > MAX_SPEED) newSpeed = MAX_SPEED;

                const direction = (paddleHit === 'player1') ? 1 : -1;

                ball.vx = direction * newSpeed * Math.cos(angleRad);
                ball.vy = newSpeed * Math.sin(angleRad);
                
                if (paddleHit === 'player1') {
                    ball.x = GAME_CONSTANTS.PADDLE_WIDTH + 10 + GAME_CONSTANTS.BALL_RADIUS + 2;
                } else {
                    ball.x = GAME_CONSTANTS.WIDTH - 10 - GAME_CONSTANTS.PADDLE_WIDTH - GAME_CONSTANTS.BALL_RADIUS - 2;
                }
            }

            if (ball.x < -20) {
                this.state.scores.player2 ++;
                this.state.balls.splice(i, 1);
                if (this.state.scores.player2 >= GAME_CONSTANTS.WIN_SCORE) {
                    this.finishGame(this.players[1]);
                    return;
                }
                continue;
            }

            if (ball.x > GAME_CONSTANTS.WIDTH + 20) {
                this.state.scores.player1 ++;
                this.state.balls.splice(i, 1);
                if (this.state.scores.player1 >= GAME_CONSTANTS.WIN_SCORE) {
                    this.finishGame(this.players[0]);
                    return;
                }
                continue;
            }
        }
    }

    private broadcast() {
        this.io.to(this.roomId).emit('game_update', this.state);
    }
}