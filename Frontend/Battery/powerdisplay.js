//#region IMPORTS
//import './app.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    div.level{
        background-color: red;
        width: 50px;
        height: 50px;
        margin: 5px;
        border: 5px 5px solid white;
    }
    div{
        display: flex;
        flex-wrap: wrap;
        align-items: center;
        background-color: green;
        border: 5px 5px solid white;
        margin-left: auto;
        margin-right: auto;
        width: 500px;
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
window.customElements.define('powerdisplay-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     this.$display = this._shadowRoot.querySelector('div');
     this._list = ["level", "level","level", "level"];
  }

   connectedCallback() {
    this._list.forEach((level)=>{
        let newElement = document.createElement('div');
        newElement.setAttribute('class',level);
        this.$display.appendChild(newElement);
    })
  }
});

//#endregion CLASS