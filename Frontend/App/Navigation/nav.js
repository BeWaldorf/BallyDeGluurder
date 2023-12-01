//#region IMPORTS
import './navitem.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    nav{
      display: flex;
      justify-content: space-evenly;
    }
  </style>
  <body>
    <nav>
    </nav>
  </body>
`;
//#endregion TEMPLATE


//#region CLASS
window.customElements.define('nav-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     //specify the element we want to append our children to
     this.$nav = this._shadowRoot.querySelector('nav');
     
     //construct an array of links.
     this._list = ["Controller", "Battery"];
  }

   connectedCallback() {
    //create a new item for each link in our array of links.
    this._list.forEach((link)=>{
        let newElement = document.createElement('navitem-r');
        //set the name, title, and as a result, the container mode we will use, using the navItem's methode.
        newElement.setAttribute('title', link);
        //append the child
        this.$nav.appendChild(newElement);
    })
  }
});

//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/