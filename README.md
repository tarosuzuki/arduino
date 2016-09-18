# arduino
###wifi_test###
Sample that Arduino contorls ESP8266.  
this sketch access the http server (/localhost/~taro/wifitest) and getting the data.  
###wifi_tcp_server_test###
this sketch act as TCP server and receive the TCP message.    
PC sends TCP message like below.  
`$echo hello arduino | nc 192.168.9.113 8090`
