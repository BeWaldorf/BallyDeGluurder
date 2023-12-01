//#region IMPORTS
//import './app.js';
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
        <p>
    </div>
  </body>
`;
//#endregion TEMPLATE


//#region CLASS
window.customElements.define('screen-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
  }

   connectedCallback() {
  }
});
//#endregion CLASS