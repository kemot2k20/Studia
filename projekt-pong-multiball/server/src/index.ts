import express from 'express';
import http from 'http';
import { Server } from 'socket.io';
import cors from 'cors';
import { ClientToServerEvents, ServerToClientEvents, GameRoom, RoomInfo } from '../../shared/types';
import { Game } from './game';

const app = express();
app.use(cors());

const server = http.createServer(app);
const io = new Server<ClientToServerEvents, ServerToClientEvents>(server, {
  cors: { origin: "*", methods: ["GET", "POST"] }
});

// pokoje i aktywne gry 
const rooms = new Map<string, GameRoom>();
const activeGames = new Map<string, Game>();

// socketID -> roomID
const playerRooms = new Map<string, string>();

const broadcastRoomList = () => {
    const availableRooms: RoomInfo[] = [];

    rooms.forEach((room) => {
      if (room.gameState === 'waiting') {
        availableRooms.push({ id: room.id, hostName: room.players[0].nickname, playersCount: room.players.length });
      }
    });
    
    io.emit('room_list_update', availableRooms);
};

function handleDisconnect(socketId: string) {
    const roomIdToDelete = playerRooms.get(socketId);

    if (!roomIdToDelete) {
        playerRooms.delete(socketId);
        return; 
    }

    const roomToDelete = rooms.get(roomIdToDelete);

    if (roomToDelete) {
        const game = activeGames.get(roomIdToDelete);
        if (game) {
            game.stop();
            activeGames.delete(roomIdToDelete);
        }

        const otherPlayer = roomToDelete.players.find(p => p.id !== socketId);
        if (otherPlayer) {
            io.to(otherPlayer.id).emit('room_error', 'Opponent left the game.');
            
            playerRooms.delete(otherPlayer.id);
        }
        
        rooms.delete(roomIdToDelete);
        broadcastRoomList();
    }
    playerRooms.delete(socketId);
}

io.on('connection', (socket) => {
  console.log(`Użytkownik połączony: ${socket.id}`);
  
  broadcastRoomList();

  socket.on('create_room', (nickname) => {
    const roomId = Math.random().toString(36).substring(2, 7).toUpperCase();
    
    const newRoom: GameRoom = {
      id: roomId,
      players: [{ id: socket.id, nickname, score: 0 }],
      gameState: 'waiting'
    };

    rooms.set(roomId, newRoom);
    socket.join(roomId); // dolacza socket do pokoju roomId

    playerRooms.set(socket.id, roomId);

    socket.emit('room_joined', roomId);
    
    broadcastRoomList();
  });

  socket.on('join_room', (roomId, nickname) => {
    const room = rooms.get(roomId);

    if (!room || room.players.length >= 2) {
      socket.emit('room_error', 'Room not available.');
      return;
    }

    room.players.push({ id: socket.id, nickname, score: 0 });
    socket.join(roomId);

    playerRooms.set(socket.id, roomId);
    
    socket.emit('room_joined', roomId);

    if (room.players.length === 2) {
      room.gameState = 'playing';
      
      const hostId = room.players[0].id;
      const guestId = room.players[1].id;
      const hostName = room.players[0].nickname;
      const guestName = room.players[1].nickname;
      
      const newGame = new Game(io, roomId, hostId, guestId, hostName, guestName);
      
      activeGames.set(roomId, newGame);
      newGame.start();
      broadcastRoomList();
    }
  });

  socket.on('player_move', (direction) => {
    const roomId = playerRooms.get(socket.id);

    if (!roomId) return;

    const game = activeGames.get(roomId);
    if (game) {
      game.handleInput(socket.id, direction);
    }
  });

  socket.on('leave_room', () => {
        console.log(`Gracz ${socket.id} opuszcza pokój.`);
        handleDisconnect(socket.id); 
    });

  socket.on('disconnect', () => {
        console.log(`Gracz ${socket.id} rozłączył się.`);
        handleDisconnect(socket.id);
    });

  socket.on('request_rematch', () => {
        const roomId = playerRooms.get(socket.id);
        if (!roomId) return;

        const room = rooms.get(roomId);
        if (!room) return;

        if (!room.rematchVotes) {
            room.rematchVotes = [];
        }

        if (!room.rematchVotes.includes(socket.id)) {
            room.rematchVotes.push(socket.id);
        }

        io.to(roomId).emit('rematch_update', room.rematchVotes.length);

        if (room.rematchVotes.length === 2) {
            console.log(`Pokój ${roomId}: Restart gry!`);
            
            room.rematchVotes = [];
            room.players.forEach(p => p.score = 0);

            const hostId = room.players[0].id;
            const guestId = room.players[1].id;
            const hostName = room.players[0].nickname;
            const guestName = room.players[1].nickname;

            if (activeGames.has(roomId)) {
                activeGames.get(roomId)?.stop();
                activeGames.delete(roomId);
            }

            const newGame = new Game(io, roomId, hostId, guestId, hostName, guestName);
            activeGames.set(roomId, newGame);
            
            newGame.start();
        }
    });
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Serwer działa na http://localhost:${PORT}`);
});