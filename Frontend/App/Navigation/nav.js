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
    //innitialize the nav links
    this._list.forEach((link)=>{
      let newElement = document.createElement('navitem-r');
      //set the name and title. From the title, the container mode will change from our NavItem's methode.
      newElement.setAttribute('title', link);
      //append the child
      this.$nav.appendChild(newElement);
    });
    

    //event secondary event listener when an nav item is clicked. This one changes the selecte attribute
    this.addEventListener('nav-clicked-event', (e) => {
      //remove all previous children
      while (this.$nav.firstChild) {
        this.$nav.removeChild(this.$nav.lastChild);
      }

      //add the new BETTER children
      this._list.forEach((link)=>{
        let newElement = document.createElement('navitem-r');
        //set the name and title. From the title, the container mode will change from our NavItem's methode.
        newElement.setAttribute('title', link);
        //the following 2 change the attribute "select" which influences your nav tab's CSS
        newElement.setAttribute('select', "false");
        if (link == e.detail){newElement.setAttribute('select', "true");}
        //append the child
        this.$nav.appendChild(newElement);
      });
    });
   }
  
});
//#endregion CLASS

/*
*-------------------------------------------------------------------------------------------------
* Made by Philip Mulders
*-------------------------------------------------------------------------------------------------
*/