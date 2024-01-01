/*
*-------------------------------------------------------------------------------------------------
* Documentation: 
* Written by Philip Mulders
*
* components ending on -r are by Philip Mulders
* components ending on -z  are by Zoran
*
*-------------------------------------------------------------------------------------------------
*/

//#region IMPORTS
import './Battery/efficiency.js';
import './Battery/powerdisplay.js';
import './Controller/svg.js';
import './Controller/screen.js';
import './Navigation/nav.js';
import './Navigation/navitem.js';
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
      *{
        margin: 0px;
        padding: 0px;
      }
      body {
        font-family: Verdana, sans-serif;
        font-optical-sizing: auto;
        font-size: 15px;
        background-color: rgb(50, 50, 50);
      }

      h1 {
        text-align: center;
        font-size: 50px;
        color: rgb(248, 248, 248);
      }

      div#navbar {
        margin-left: 10px;
      }

      #container {
        text-align: center;
        border: 1px 1px;
        border-color: black;
        border-radius: 30px;
        border-style: groove;
        margin: 0px 20px;
        background-color: rgb(248, 248, 248);
        transition: filter 2s;
      }

      #container:hover {
        filter: brightness(85%);
        animation-delay: 5ms;
      } 
  </style>
  <body>
    <h1>Bally de gluurende auto</h1>
    <div id="navbar"><nav-r></nav-r></div>
    <div id="container" containerMode="Controller">
    </div>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('app-r', class extends HTMLElement {
    constructor() {
      super();
      this._shadowRoot = this.attachShadow({ 'mode': 'open' });
      this._shadowRoot.appendChild(template.content.cloneNode(true)); 
      this.$container = this._shadowRoot.getElementById("container");
    }

    connectedCallback() {
      killAllChildren(this.$container); //remove all childeren

      let newElement = document.createElement('screen-z');
      this.$container.appendChild(newElement);

      newElement = document.createElement('svg-r');
      this.$container.appendChild(newElement);

      //following function removes all childeren
      function killAllChildren(mother){
        while (mother.firstChild) {
          mother.removeChild(mother.lastChild);
        }
      }

      this.addEventListener('nav-clicked-event', (e) => {
        this.$container.setAttribute("containerMode", e.detail);

        //change the containers contents based on the mode it is in.
        if (e.detail == "Controller"){
          killAllChildren(this.$container); //remove all childeren
          let newElement = document.createElement('screen-z');
          this.$container.appendChild(newElement);
  
          newElement = document.createElement('svg-r');
          this.$container.appendChild(newElement);
        } else if (e.detail == "Battery"){
          killAllChildren(this.$container); //remove all childeren
          newElement = document.createElement('powerdisplay-z');
          this.$container.appendChild(newElement);
  
          newElement = document.createElement('efficiency-z');
          this.$container.appendChild(newElement);
        }
      });
    }
});
//#endregion CLASS

/*
*
* Documentation: Written by Philip Mulders
*
* components ending on -r are by Philip Mulders
* components ending on -z  are by Zoran Bovin
*
*/