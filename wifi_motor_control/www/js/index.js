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

      this.forwardButton.addEventListener('focus', function() {
        console.log ("forward button : focus");
      });

      this.forwardButton.addEventListener('blur', function() {
        console.log ("forward button : blur");
      });

      this.forwardButton.addEventListener('click', function() {
        console.log ("forward button is clicked");
      });

      this.backwardButton.addEventListener('mousedown', function() {
        console.log ("backward button: mousedown");
        that.setState("backward");
      });

      this.backwardButton.addEventListener('click', function() {
        console.log ("backward button is clicked");
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
