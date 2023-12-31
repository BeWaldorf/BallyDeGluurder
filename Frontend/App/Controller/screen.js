//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
        div {
        height: 400px;
        width: 800px;
        margin-left: auto;
        margin-right: auto;
        margin-top: 10px;
        border: 1px 1px;
        border-color: black;
        border-radius: 5px;
        border-style: groove;
        box-shadow:
            5px 5px 5px 1px rgba(0, 0, 0, 0.2),
            -5px -5px 5px 1px rgba(0, 0, 0, 0.2),
            5px -5px 5px 1px rgba(0, 0, 0, 0.2),
            -5px 5px 5px 1px rgba(0, 0, 0, 0.2);
        }
  </style>
  <body>
    <div>
        <img id="liveStream" src="" alt="stream offline" style="color:red"/>
    </div>
  </body>
`;
//#endregion TEMPLATE


//#region CLASS
window.customElements.define('screen-z', class extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ 'mode': 'open' });
    this._shadowRoot.appendChild(template.content.cloneNode(true));


    //establishing a connection with MQTT
    const url = 'ws://23.97.138.160:9001'
    // MQTT Client Setup with Authentication
    const options = {
      // Authentication
      username: 'bally',
      password: 'BallyDeGluurder',
    }
    this.client = mqtt.connect(url, options);



    var img = document.getElementById('liveStream');
    var socket = new WebSocket('ws://23.97.138.160:3000');

    socket.onmessage = function (event) {
      var reader = new FileReader();
      reader.onload = function () {
        img.src = reader.result;
      };
      reader.readAsDataURL(new Blob([event.data]));
    };
  }

  connectedCallback() {

  }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Zoran Bovin
*-------------------------------------------------------------------------------------------------
*/