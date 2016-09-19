# arduino
###wifi_test###
Sample that Arduino contorls ESP8266.  
this sketch access the http server (/localhost/~taro/wifitest) and getting the data.  
###wifi_tcp_server_test###
this sketch act as TCP server and receive the TCP message.    
PC sends TCP message like below.  
`$echo hello arduino | nc 192.168.9.113 8090`
###simple_dc_motor###
sample of controling DC motors.  
 - use motor driver TA7291SG
 - power input for motors : 2.6V (two AA batteries.)

###wifi_motor_control###
sample of controling DC motors with wifi.

