import './style.css'
import { io, Socket } from "socket.io-client";
import type { ServerToClientEvents, ClientToServerEvents, RoomInfo, GameState } from "@shared/types";
import { GAME_CONSTANTS } from "../../shared/types";

const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io("http://localhost:3000");

// DOM
const lobbyScreen = document.getElementById('lobbyScreen')!;
const waitScreen = document.getElementById('waitScreen')!;
const gameScreen = document.getElementById('gameScreen')!;
const gameOverScreen = document.getElementById('gameOverScreen')!;
const nicknameInput = document.getElementById('usernameInput') as HTMLInputElement;
const roomsListDiv = document.getElementById('roomsList')!;
const currentRoomIdSpan = document.getElementById('currentRoomIdDisplay')!;
const statusMsg = document.getElementById('statusMsg')!;
const countdownDisplay = document.getElementById('countdownDisplay') as HTMLDivElement;
const leaveLobbyBtn = document.getElementById('leaveLobbyBtn')!;
const exitToMenuBtn = document.getElementById('exitToMenuBtn')!;
const finalScoreDisplay = document.getElementById('finalScoreDisplay')!;
const winnerText = document.getElementById('winnerText')!;
const rematchBtn = document.getElementById('rematchBtn') as HTMLButtonElement;
const rematchCountSpan = document.getElementById('rematchCount')!;
const canvas = document.getElementById('gameCanvas') as HTMLCanvasElement;
const ctx = canvas.getContext('2d')!;

canvas.width = GAME_CONSTANTS.WIDTH;
canvas.height = GAME_CONSTANTS.HEIGHT;

let currentGameState: GameState | null = null;
let amIHost = false;

let player1Name = "Player 1";
let player2Name = "Player 2";

// SPA (single page app)
function showScreen(screenName: 'lobby' | 'wait' | 'game' | 'gameOver') {
  lobbyScreen.style.display = 'none';
  waitScreen.style.display = 'none';
  gameScreen.style.display = 'none';
  gameOverScreen.style.display = 'none';
  
  if (screenName === 'lobby') lobbyScreen.style.display = 'block';
  if (screenName === 'wait') waitScreen.style.display = 'block';
  if (screenName === 'game') gameScreen.style.display = 'block';
  if (screenName === 'gameOver') gameOverScreen.style.display = 'block';
}

function validateNick(): string | null {
    const nick = nicknameInput.value.trim();

    if (nick.length > 10) {
        alert("Your Nickname is too long ( MAX 10 characters ) !");
        return null;
    }

    if (nick.length === 0) {
        alert("Enter your Nickname!");
        return null;
    }

    return nick;
}

function handleExit() {
    socket.emit('leave_room');

    currentRoomIdSpan.innerText = '';
    currentGameState = null;
    amIHost = false;

    showScreen('lobby');
}

leaveLobbyBtn.addEventListener('click', () => {
    handleExit();
});

exitToMenuBtn.addEventListener('click', () => {
    handleExit();
});

document.getElementById('createRoomBtn')?.addEventListener('click', () => {
    const nick = validateNick();
    
    if (nick) {
        socket.emit("create_room", nick);
    }
});

function joinRoom(roomId: string) {
    const nick = validateNick();

    if (nick) {
        socket.emit("join_room", roomId, nick);
    }
}

const keysPressed = {
    ArrowUp: false,
    ArrowDown: false
};

let lastSentDirection: 'up' | 'down' | 'stop' = 'stop';

function updateMovement() {
    if (gameScreen.style.display !== 'block') return;

    let newDirection: 'up' | 'down' | 'stop' = 'stop';

    if (keysPressed.ArrowUp && !keysPressed.ArrowDown) {
        newDirection = 'up';
    } else if (keysPressed.ArrowDown && !keysPressed.ArrowUp) {
        newDirection = 'down';
    }

    if (newDirection !== lastSentDirection) {
        lastSentDirection = newDirection;
        socket.emit('player_move', newDirection);
    }
}

document.addEventListener('keydown', (e) => {
    if (e.code === 'ArrowUp' || e.code === 'ArrowDown') {
        e.preventDefault();
        keysPressed[e.code] = true;
        updateMovement();
    }
});

document.addEventListener('keyup', (e) => {
    if (e.code === 'ArrowUp' || e.code === 'ArrowDown') {
        keysPressed[e.code] = false;
        updateMovement();
    }
});

function drawGame() {
    if (!currentGameState || gameScreen.style.display !== 'block') {
        requestAnimationFrame(drawGame);
        return;
    }

    ctx.fillStyle = '#000000';
    ctx.fillRect(0, 0, GAME_CONSTANTS.WIDTH, GAME_CONSTANTS.HEIGHT);

    ctx.strokeStyle = '#333';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(GAME_CONSTANTS.WIDTH / 2, 0);
    ctx.lineTo(GAME_CONSTANTS.WIDTH / 2, GAME_CONSTANTS.HEIGHT);
    ctx.stroke();

    const p1Color = amIHost ? '#4cc9f0' : '#f72585'; 
    const p2Color = !amIHost ? '#4cc9f0' : '#f72585';
    const p1Label = amIHost ? "P1 (TY)" : "P1";
    const p2Label = !amIHost ? "P2 (TY)" : "P2";
    const radius = GAME_CONSTANTS.PADDLE_WIDTH / 2;
    
    ctx.font = 'bold 14px Arial';
    ctx.textAlign = 'center';

    ctx.fillStyle = p1Color;
    ctx.beginPath();
    ctx.roundRect(
        10, 
        currentGameState.paddles.player1Y, 
        GAME_CONSTANTS.PADDLE_WIDTH, 
        GAME_CONSTANTS.PADDLE_HEIGHT, 
        radius
    );
    ctx.fill();

    ctx.fillStyle = '#FFFFFF';
    ctx.textAlign = 'left';
    ctx.fillText(
        p1Label, 
        (GAME_CONSTANTS.PADDLE_WIDTH / 2),
        currentGameState.paddles.player1Y - 10
    );


    ctx.fillStyle = p2Color;
    ctx.beginPath();
    ctx.roundRect(
        GAME_CONSTANTS.WIDTH - 10 - GAME_CONSTANTS.PADDLE_WIDTH, 
        currentGameState.paddles.player2Y, 
        GAME_CONSTANTS.PADDLE_WIDTH, 
        GAME_CONSTANTS.PADDLE_HEIGHT, 
        radius
    );
    ctx.fill();

    ctx.fillStyle = '#FFFFFF';
    ctx.textAlign = 'right';
    ctx.fillText(
        p2Label, 
        (GAME_CONSTANTS.WIDTH - GAME_CONSTANTS.PADDLE_WIDTH) + (GAME_CONSTANTS.PADDLE_WIDTH / 2), 
        currentGameState.paddles.player2Y - 10
    );


    currentGameState.balls.forEach(ball => {
        ctx.fillStyle = ball.color;
        ctx.beginPath();
        ctx.arc(ball.x, ball.y, GAME_CONSTANTS.BALL_RADIUS, 0, Math.PI * 2);
        ctx.fill();
    });

    ctx.font = 'bold 40px Arial';
    ctx.textAlign = 'left';
    
    ctx.fillStyle = p1Color; 
    ctx.fillText(currentGameState.scores.player1.toString(), GAME_CONSTANTS.WIDTH / 2 - 60, 60);
    
    ctx.fillStyle = p2Color;
    ctx.fillText(currentGameState.scores.player2.toString(), GAME_CONSTANTS.WIDTH / 2 + 30, 60);

    requestAnimationFrame(drawGame);
}

requestAnimationFrame(drawGame);

socket.on('connect_error', (err) => {
    console.error("Błąd połączenia:", err.message);
    statusMsg.innerText = "Connection error.";
});

socket.on('room_list_update', (rooms: RoomInfo[]) => {
    roomsListDiv.innerHTML = '';
    if (rooms.length === 0) {
        roomsListDiv.innerHTML = '<p>No available rooms.</p>';
        return;
    }
    rooms.forEach(room => {
        const div = document.createElement('div');
        div.className = 'room-item';
        
        div.innerHTML = `
            <span class="room-host-info">
                HOST: <span class="host-badge">${room.hostName}</span>
            </span>
        `;
        
        const btn = document.createElement('button');
        btn.innerText = 'PLAY';
        btn.onclick = () => joinRoom(room.id);
        
        div.appendChild(btn);
        roomsListDiv.appendChild(div);
    });
});

socket.on('room_joined', (roomId) => {
    currentRoomIdSpan.innerText = roomId;
    showScreen('wait');
});

socket.on('game_start', (data) => {
    amIHost = data.isHost;
    player1Name = data.p1Nickname;
    player2Name = data.p2Nickname;
    
    console.log(`Start gry! P1: ${player1Name}, P2: ${player2Name}`);
    showScreen('game');
});

socket.on('game_update', (state) => {
    currentGameState = state;
});

socket.on('room_error', (msg) => {
    alert(msg);
    showScreen('lobby');
});

socket.on('game_over', ({ winnerId, winnerNickname, score }) => {
    finalScoreDisplay.innerText = `${score.player1} : ${score.player2}`;

    if (socket.id === winnerId) {
        winnerText.innerText = `WINNER: ${winnerNickname}`;
        winnerText.style.color = "#4cc9f0";
    } else {
        winnerText.innerText = `WINNER: ${winnerNickname}`;
        winnerText.style.color = "#e74c3c";
    }

    rematchBtn.disabled = false;
    if (rematchCountSpan) {
        rematchCountSpan.innerText = "0/2";
    }

    showScreen('gameOver');
});

rematchBtn.addEventListener('click', () => {
    rematchBtn.disabled = true;
    socket.emit('request_rematch');
});

socket.on('rematch_update', (count) => {
    rematchCountSpan.innerText = `${count}/2`;
});

socket.on('start_countdown', () => {
    showScreen('game');

    countdownDisplay.style.display = 'block';
    countdownDisplay.innerText = "3";
    
    let timeLeft = 3;

    const timer = setInterval(() => {
        timeLeft --;

        if (timeLeft > 0) {
            countdownDisplay.innerText = timeLeft.toString();
        } else if (timeLeft === 0) {
            countdownDisplay.innerText = "GO!";
        } else {
            countdownDisplay.style.display = 'none';
            clearInterval(timer);
        }
    }, 1000);
});