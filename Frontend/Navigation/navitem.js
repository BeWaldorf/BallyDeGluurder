//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    p{
        margin: 5px;
        color: white;
    }
  </style>
  <body>
    <p>error, no reference.</p>
  </body>
`;
//#endregion TEMPLATE


//#region CLASS
window.customElements.define('navitem-r', class extends HTMLElement {
   constructor() {
     super();
     this._shadowRoot = this.attachShadow({ 'mode': 'open' });
     this._shadowRoot.appendChild(template.content.cloneNode(true));  
     this.$link = this._shadowRoot.querySelector('p');
  }

   connectedCallback() {
    //set the inner HTML to that of the title
    this.$link.innerHTML = this.title;

    //Create an event allowing us to change the container mode by transferring it under "e.details"
    this.$link.addEventListener("click", (e) => {
      let setContainer = this.title;
      const navClicked = new CustomEvent('nav-clicked-event', {detail: setContainer, bubbles: true, composed: true });

      this.dispatchEvent(navClicked);
    });
  }
});

//#endregion CLASS