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
        this.client.subscribe('bally/battery');

        this.client.on('message', (topic, payload, packet) => {
            //put received payload in variable as a string
            let message = payload.toString();

            //make a bubbling method from this object, so both 'powerdisplay' and 'efficiency' can access this.
            const clientSub = new CustomEvent('clientsub-event', { detail: message, bubbles: true, composed: true });
            this.dispatchEvent(clientSub);
        });

        this.client.on('error', (error) => {
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