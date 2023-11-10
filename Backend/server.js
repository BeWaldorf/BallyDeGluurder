import express from 'express';
import { WebSocketServer } from 'ws';

const PORT = 3000;
const APP = express();
const WSS = new WebSocketServer({ port: 8080 });
const sensorData = [];

// #region WEBSERVER
// ---------------------------------------
APP.use("/embedded", express.static("../frontend/dashboard"));
// ---------------------------------------
// #endregion


// #region REST API
// ---------------------------------------
/* ---ACTIVATE MIDDLEWARE--- */
APP.use(express.json());


/* ---ENDPOINTS--- */
APP.get("/getData", (req, res) => {
    let newData=[];
    sensorData.forEach(data=>{
        newData.push(data.temperature)
    });
    res.send(JSON.stringify(newData));
});

//http://192.168.0.4:2022/update-sensor
APP.post("/update-sensor", (req, res) => {
    console.log("Full data: ", req.body)
    const { temperature, humidity } = req.body;
    console.log("temperature: ", temperature);
    console.log("humidity: ", humidity);
    sensorData.push(req.body);

    WSS.on('connection', ws => {
        console.log('New client connected!')
        ws.send(humidity)

        ws.on('close', () => {
            console.log('Client has disconnected!')
        })
        
        ws.onerror =  () => {
            console.log('websocket error')
        }
    })
    return res.status(200).send()  //OK
});
// ---------------------------------------
// #endregion

// #region WEBSOCKETS
// ---------------------------------------
WSS.on('connection', ws => {
    console.log('New client connected!')
    ws.send('connection established')

    ws.on('close', () => {
        console.log('Client has disconnected!')
    })
    
    ws.onerror =  () => {
        console.log('websocket error')
    }
})
// ---------------------------------------
// #endregion


APP.listen(PORT, () => {
    console.log(`App running at http://localhost:${PORT}`);
});