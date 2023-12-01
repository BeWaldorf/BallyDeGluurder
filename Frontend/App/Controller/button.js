//#region IMPORTS
import '../app.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
  </style>
  <body>
  <div>filler Text is dit en dit en dit en dit en dit en dit en dit</div>
  </body>
`;
//#endregion TEMPLATE


//#region CLASS
window.customElements.define('button-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     this.$btn = this._shadowRoot.querySelector('div');
  }

   connectedCallback() {
    this.$btn.addEventListener("click", (event)=> {
      console.log("button was pressed");
    });
    //this.$btn.innerHTML = this.dispatchEvent();
     // this.addEventListener("ux", (e) => {
     //  console.log(e);
     //  this.$menu.setAttribute('custom', e.detail.checked)
     // });
  }
});

//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/