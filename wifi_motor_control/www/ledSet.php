<?php
header("Content-type: text/plain; charset=UTF-8");

var_dump($argv);

if (isset($_POST['state']))
{
  $state = $_POST['state'];

  //$state = 1;

  // ledLog.txtにデータを保存
  $fp = fopen("ledLog.txt", "w");
  fwrite($fp, $state);
  fclose($fp);
}
?>