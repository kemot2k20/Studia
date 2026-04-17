export interface Player {
    id: string;
    nickname: string;
    score: number;
}

export interface GameRoom {
    id: string;
    players: Player[];
    gameState: 'waiting' | 'playing' | 'finished';

    rematchVotes?: string[];
}

export interface RoomInfo {
    id: string;
    hostName: string;
    playersCount: number;
}

export const GAME_CONSTANTS = {
    WIDTH: 1000,
    HEIGHT: 600,
    PADDLE_WIDTH: 20,
    PADDLE_HEIGHT: 100,
    BALL_RADIUS: 10,
    PADDLE_SPEED: 10,
    WIN_SCORE: 20,
    LAUNCH_COOLDOWN: 500
};

export interface Ball {
    id: string;
    x: number;
    y: number;
    vx: number;
    vy: number;
    color: string;
}

export interface GameState {
    paddles: {
        player1Y: number;
        player2Y: number;
    };
    scores: {
        player1: number;
        player2: number;
    };
    balls: Ball[];
}

export interface ClientToServerEvents {
    create_room: (nickname: string) => void;
    join_room: (roomId: string, nickname: string) => void;
    leave_room: () => void;
    player_move: (direction: 'up' | 'down' | 'stop') => void;
    request_rematch: () => void;
}

export interface ServerToClientEvents {
    room_joined: (roomId: string) => void;
    room_error: (message: string) => void;

    room_list_update: (rooms: RoomInfo[]) => void;

    game_start: (data: { 
        isHost: boolean; 
        p1Nickname: string; 
        p2Nickname: string; 
    }) => void;
    
    game_update: (state: GameState) => void;

    game_over: (data: { 
        winnerId: string; 
        winnerNickname: string; 
        score: { player1: number, player2: number } 
    }) => void;

    start_countdown: () => void;
    rematch_update: (count: number) => void;
}