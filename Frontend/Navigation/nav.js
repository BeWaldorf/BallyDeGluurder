//#region IMPORTS
//import './app.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    a{
      text-decoration:none;  
      color: white;  
      margin: 2px;
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
     this.$nav = this._shadowRoot.querySelector('nav');
     this._list = ["././index.html", "././battery.html"];
  }

   connectedCallback() {
    this._list.forEach((link)=>{
        let newElement = document.createElement('a');
        newElement.setAttribute('href',link);
        newElement.setAttribute('title',link.split('/').pop().split('.')[0]);
        newElement.textContent = link.split('/').pop().split('.')[0]; //works //used to just be "link" but this ~ extracts ~
        this.$nav.appendChild(newElement);

        /*
        newElement.setAttribute('title',link.split('/').pop().split('.')[0]);
        let txtContent = link.split('/').pop().split('.')[0];
        if (txtContent == "index"){
          newElement.textContent = "home";
        } else {
          newElement.textContent = this.textContent;
        }
        newElement.textContent = link.split('/').pop().split('.')[0]; //works //used to just be "link" but this ~ extracts ~
        */
    })
  }
});

//#endregion CLASS