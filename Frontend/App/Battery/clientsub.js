//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
  </style>
  <body>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('clientsub-z', class extends HTMLElement {
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

        // Subscribe to the topic
        this.client.subscribe('bally/battery', function (err) {
            if (!err) {
                console.log('Succesvol geabonneerd op bally/battery');
            } else {
                console.error('Fout bij het abonneren:', err);
            }
        });
    }

    connectedCallback() {
        
        let thing = this.client.subscribe('bally/battery');
        let ourComponent = this;
        this.client.on('message', function (topic, payload, packet) {
            //put recieved payload in variable as string
            let message = payload.toString();

            //make a bubbling methode from this object, so both 'powerdisplay' and 'efficiency' can acces this.
            const clientSub = new CustomEvent('clientsub-event', { detail: message, bubbles: true, composed: true });
            ourComponent.dispatchEvent(clientSub);
        });

        this.client.on('error', function (error) {
            console.log(error);
        });
    }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Zoran Bovin
*-------------------------------------------------------------------------------------------------
*/