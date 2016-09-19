(function() {
  // PHPのURL
  var PHP_URL = "ledSet.php";
  // ボタン
  var btn;

  $(function() {
    btn = $("#btn");
    btn.on("click", clickHandler);

    // LEDをOFFにする
    ledSet("OFF");
  });

  /**
   * Click Handler
   */
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

  /**
   * PHPにLEDの状態を送信します
   * @param	state	0:OFF, 1:ON
   */
  function ledSet(state) {
    localStorage.setItem('state', state);
    $.ajax({
      url: PHP_URL,
      type: "POST",
      dataType: "json",
      data: {
        state: state
      },
      success: function(data, dataType) {
        //successのブロック内は、Ajax通信が成功した場合に呼び出される

        //PHPから返ってきたデータの表
	console.log ("success: " + data);
      },
      error: function(XMLHttpRequest, textStatus, errorThrown) {
	console.log ("error : "  + errorThrown);
        //通常はここでtextStatusやerrorThrownの値を見て処理を切り分けるか、単純に通信に失敗した際の処理を記述します。

        //this;
        //thisは他のコールバック関数同様にAJAX通信時のオプションを示します。

        //エラーメッセージの表示
//        alert('Error : ' + errorThrown);
      }
    })
  }
})();
