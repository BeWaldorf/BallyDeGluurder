//#region IMPORTS
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
    </div>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('efficiency-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     this.$text = this._shadowRoot.querySelector('div');
  }

   connectedCallback() {
    this.$text.innerHTML = "We kunnen nog " + "20" + " minuten rond rijden!";
  }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/