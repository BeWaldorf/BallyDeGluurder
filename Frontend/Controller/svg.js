//#region IMPORTS
//#endregion IMPORTS

//#region TEMPLATE
let template = document.createElement('template');
template.innerHTML = /*html*/`
  <style>
    #arrow_up {
      fill: url(#img_arw_U);
    }

    #arrow_left {
      fill: url(#img_arw_L);
    }

    #arrow_right {
      fill: url(#img_arw_R);
    }

    #arrow_down {
      fill: url(#img_arw_D);
    }

    #middle_btn {
      fill: url(#img_start_B);
    }    
  </style>
  <body>
    <svg width="400" height="200">
        <!--width="400" height="200"  viewBox="0 0 1000 180"-->
        <!-- Create image definitions-->
        <defs>
          <pattern id='img_arw_U' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/Arrow_U.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>

          <pattern id='img_arw_L' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/Arrow_L.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>

          <pattern id='img_arw_R' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/Arrow_R.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>

          <pattern id='img_arw_D' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/Arrow_D.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>

          <pattern id='img_start_B' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/Flash.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>

          <pattern id='img_stop_B' width="1" height="1" viewBox="0 0 100 100" preserveAspectRatio="none">
            <image href='./Controller/images/FlashExclamantion.png' width="100" height="100" preserveAspectRatio="none"></image>
          </pattern>
        </defs>

        <!-- Create an ellipse -->
        <ellipse cx="200" cy="100" rx="160" ry="80" fill="transparent" stroke="black" stroke-width="2" />

        <!-- Create four areas -->
        <path id="arrow_up" d="M150 75 L100 38 C100 38 193 0 300 38 L250 75 Z" />
        <path id="arrow_right" d="M250 75 L300 38 C300 32 435 100 300 162 L300 162 L250 125 Z" />
        <path id="arrow_left" d="M150 75 L150 125 L100 162 C95 165 -33 100 100 37 Z" />
        <path id="arrow_down" d="M150 125 L250 125 L300 162 C300 162 206 200 100 162 Z" />

        <!-- Create deviders -->
        <line x1="245" y1="80" x2="300" y2="38" style="stroke:black; stroke-width:2" />
        <line x1="245" y1="120" x2="300" y2="162" style="stroke:black; stroke-width:2" />
        <line x1="155" y1="80" x2="100" y2="38" style="stroke:black; stroke-width:2" />
        <line x1="155" y1="120" x2="100" y2="162" style="stroke:black; stroke-width:2" />

        <!-- Create box -->
        <path id="middle_btn" d="M150 75 L250 75 L250 125 L150 125 Z" fill="rgb(248 248 248)"/>
        <line x1="150" y1="76" x2="250" y2="76" style="stroke:black; stroke-width:2" />
        <line x1="150" y1="124" x2="250" y2="124" style="stroke:black; stroke-width:2" />
        <line x1="150" y1="76" x2="150" y2="124" style="stroke:black; stroke-width:2" />
        <line x1="250" y1="76" x2="250" y2="124" style="stroke:black; stroke-width:2" />
    </svg>
  </body>
`;
//#endregion TEMPLATE

//#region CLASS
window.customElements.define('svg-r', class extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ 'mode': 'open' });
    this._shadowRoot.appendChild(template.content.cloneNode(true));  

    //getting variables from our HTML to assign code to
    this.$arrow_right=this._shadowRoot.querySelector('#arrow_right');
    this.$arrow_left=this._shadowRoot.querySelector('#arrow_left');
    this.$arrow_up=this._shadowRoot.querySelector('#arrow_up');
    this.$arrow_down=this._shadowRoot.querySelector('#arrow_down');
    this.$middle_btn=this._shadowRoot.querySelector('#middle_btn');

    //establishing a connection with MQTT
    const url = 'ws://23.97.138.160:9001'
    // MQTT Client Setup with Authentication
    const options = {
        // Authentication
        username: 'bally',
        password: 'BallyDeGluurder',
    }
    this.client = mqtt.connect(url, options);
  }

   connectedCallback() {
    //right Button
    this.$arrow_right.addEventListener('mousedown',()=>{
      moveCar("r", 250, this.client);
    });
    this.$arrow_right.addEventListener('mouseup',()=>{
      moveCar("s", 250, this.client);
    });

    //left Button
    this.$arrow_left.addEventListener('mousedown',()=>{
      moveCar("l", 250, this.client);
    });
    this.$arrow_left.addEventListener('mouseup',()=>{
      moveCar("s", 250, this.client);
    });

    //up Button
    this.$arrow_up.addEventListener('mousedown',()=>{
      moveCar("f", 250, this.client);
    });
    this.$arrow_up.addEventListener('mouseup',()=>{
      moveCar("s", 250, this.client);
    });

    //down Button
    this.$arrow_down.addEventListener('mousedown',()=>{
      moveCar("b", 250, this.client);
    });
    this.$arrow_down.addEventListener('mouseup',()=>{
      moveCar("s", 250, this.client);
    });

    //additional Flash functionality; not quite sure what we will be using this for
    let flashInt = false;
    this.$middle_btn.addEventListener('click',()=>{
      if (!flashInt){
        this.client.publish('bally/flash', JSON.stringify({
          "flash": "0"
        }));
        flashInt = true;
        this.$middle_btn.setAttribute('style', "fill: url(#img_stop_B");
      } else if (flashInt == true) 
      {
        this.client.publish('bally/flash', JSON.stringify({
          "flash": "1"
        }));
        flashInt = false;
        this.$middle_btn.setAttribute('style', "fill: url(#img_start_B");
      }
    });

    function moveCar(direction, speed, ourClient){
      //console.log(direction);
      ourClient.publish('bally/directions', JSON.stringify({
        "direction": direction,
        "speed": speed
      }));
    }
  }
});

//#endregion CLASS