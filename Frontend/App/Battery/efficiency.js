//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    div{
        display: flex;
        flex-wrap: wrap;
        align-items: center;
        background-color: rgba(33, 255, 33);
        margin-left: auto;
        margin-right: auto;
        width: 500px;
    }
  </style>
  <body>
    <p>Hoeveel bedraagd het rendement? </p>
    <div>
      <p> filler for the actual efficiency Object </p>
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
     this.$display = this._shadowRoot.querySelector('div');
  }

   connectedCallback() {
    //
  }
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/