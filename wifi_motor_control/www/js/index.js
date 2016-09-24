(function(exports) {

  const PHP_URL = "ledSet.php";

  /**
   * Main class of smart-home
   * @class Home
   */
  function Home() {}

  Home.prototype = {
    forwardButton: document.getElementById('forward-button'),
    backwardButton: document.getElementById('backward-button'),
    leftForwardButton: document.getElementById('left-forward-button'),
    rightForwardButton: document.getElementById('right-forward-button'),

    setState : function(state) {
      $.ajax({
        url: PHP_URL,
        type: "POST",
        dataType: "json",
        data: {
          state: state
        },
        success: function(data, dataType) {
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
          console.log ("error : "  + errorThrown);
        }
      })
    },

    init: function() {
      var that = this;
      this.setState("stop");

      window.addEventListener('mouseup', function() {
        console.log ("window  : mouseup");
        that.setState("stop");
      });

      this.forwardButton.addEventListener('mousedown', function() {
        console.log ("forward button : mousedown");
        that.setState("forward");
      });

      this.backwardButton.addEventListener('mousedown', function() {
        console.log ("backward button: mousedown");
        that.setState("backward");
      });

      this.leftForwardButton.addEventListener('mousedown', function() {
        console.log ("left-forward button : mousedown");
        that.setState("left-forward");
      });

      this.rightForwardButton.addEventListener('mousedown', function() {
        console.log ("right-forward button : mousedown");
        that.setState("right-forward");
      });

    }
  };

  exports.Home = Home;
}(window));

window.home = new Home();
window.home.init();

/*
(function() {

  var PHP_URL = "ledSet.php";
  var back;
  var forwardButton =  document.getElementById('forward-button');
  var backardButton =  document.getElementById('backward-button');

  $(function() {

    ledSet("OFF");
  });
  function clickHandler() {
    // ON -> OFF
    if(btn.attr("class") == "on") {
      btn.attr("class", "off");
      ledSet("OFF");
    }
    // OFF -> ON
    else {
      btn.attr("class", "on");
      ledSet("ON");
    }
  }

})();
*/
