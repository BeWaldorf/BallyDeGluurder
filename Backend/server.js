import express from 'express';
import { WebSocketServer } from 'ws';
import http from 'http';

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

wss.on('connection', (ws) => {
    console.log('Client connected');
    
    ws.send("server");

    ws.on('message', (message) => {
        console.log(message);        
        wss.clients.forEach(function each(client) {
           client.send(message);
        });
        
    });

    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

server.listen(3000, () => {
    console.log('Server is running on port 3000');
});