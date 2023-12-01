//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    div.level{
        background-color: rgba(250, 100, 100);
        width: 50px;
        height: 50px;
        margin: 5px;
    }
    div{
        display: flex;
        flex-wrap: wrap;
        align-items: center;
        background-color: rgba(33, 255, 33);;
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
window.customElements.define('powerdisplay-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     this.$display = this._shadowRoot.querySelector('div');
     this._list = ["level", "level","level", "level"];
  }

   connectedCallback() {
    //make our powercell icons
    this._list.forEach((level)=>{
        let newElement = document.createElement('div');
        newElement.setAttribute('class',level);
        this.$display.appendChild(newElement);
    })
  }
});

//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/