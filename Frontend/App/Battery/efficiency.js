//#region IMPORTS
import './clientsub.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    div{
        display: flex;
        background-color: rgba(33, 255, 33);
        margin-left: auto;
        margin-right: auto;
        margin-bottom: 5px;
        margin-top: 5px;
        padding: 10px;
        width: 500px;        
    }
  </style>
  <body>
    <p>Hoeveel bedraagd het rendement? </p>
    <div>
     We kunnen nog ??? minuten om rond te rijden!
    </div>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('efficiency-z', class extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ 'mode': 'open' });
    this._shadowRoot.appendChild(template.content.cloneNode(true));
    this.$div = this._shadowRoot.querySelector('div');
    this._shadowRoot.appendChild(document.createElement('clientsub-z'));
  }

  connectedCallback() {

    this.addEventListener('clientsub-event', (e) => {
      let messageT = e.detail;
      //console.log(messageT);
      //split recieved message to get time left
      let messageArr = messageT.split(",");
      let timeLeft = messageArr[3].split(":");
      let timeLeftVal = timeLeft[1].slice(0, -1);
      this.$div.innerHTML = "We kunnen nog " + timeLeftVal + " minuten om rond te rijden!";
    });
  }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Zoran Bovin
*-------------------------------------------------------------------------------------------------
*/