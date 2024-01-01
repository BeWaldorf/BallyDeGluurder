//#region IMPORTS
import './clientsub.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    div.level{
        background-color: rgba(200, 50, 50);
        width: 50px;
        height: 50px;
        margin: 5px;
    }
    div{
        display: flex;
        background-color: rgba(33, 255, 33);
        margin-left: auto;
        margin-right: auto;
        width: 300px;
    }
  </style>
  <body>
    <p>Hoeveel hebben we nog in onze tank? </p>
    <div>
    </div>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('powerdisplay-z', class extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ 'mode': 'open' });
    this._shadowRoot.appendChild(template.content.cloneNode(true));
    this.$display = this._shadowRoot.querySelector('div');
    this._shadowRoot.appendChild(document.createElement('clientsub-z'));
  }

  connectedCallback() {

    let percentage = 1;

    //remove the old battery parts
    while (this.$display.firstChild) {
      this.$display.removeChild(this.$display.lastChild);
    }

    //make our powercell icons
    for (let i = 0; i < percentage; i++) {
      let newElement = document.createElement('div');
      newElement.setAttribute('class', "level");
      this.$display.appendChild(newElement);
    }

    this.addEventListener('clientsub-event', (e) => {
      //split recieved message to get percentage
      let messageT = e.detail;
      let messageArr = messageT.split(",");
      let percent = messageArr[1].split(":");
      console.log(percent[1]);

      percentage = Math.floor(percent[1] / 20);
      console.log(percentage);

      while (this.$display.firstChild) {
        this.$display.removeChild(this.$display.lastChild);
      }
      for (let i = 0; i < percentage; i++) {
        let newElement = document.createElement('div');
        newElement.setAttribute('class', "level");
        this.$display.appendChild(newElement);
      }
    });
  }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Zoran Bovin
*-------------------------------------------------------------------------------------------------
*/