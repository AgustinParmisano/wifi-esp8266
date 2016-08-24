# wifi-esp8266

#Common AT Commands:
 
	AT
	AT+CWMODE=3
	AT+CWJAP="ssid",""password"
	AT+CIPMUX=1
	AT+CIPSERVER=1,8080
	AT+CIPSTART="TCP","IP/URL","puerto"
	AT+CIPSEND="Numero de caracteres"

#To send AT Commands use send_at_cmds.ino

	To change esp8266 baud rate use: AT+CIOBAUD=<baudrate>
	Recomended esp8266 baud rate: 19200

 
