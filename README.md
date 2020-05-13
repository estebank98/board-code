# board-code

Basically, with this code, the behavior of the board is managed (NodeMCU: ESP8266 in this case) by the peripherals that connect to it, with the JSON values they get from the website http://plazaapp.herokuapp.com/luces/status

These are modified on the main page http://plazaapp.herokuapp.com, therefore, the board connects to the Internet, gets the JSON and, according to the values, turns on, off or changes the RGB values ​​of the LEDs that in this case were used.

The NodeMCU: ESP8266 board is used to facilitate connection to a WIFI network, since it comes with this built-in functionality, thanks to the ESP8266 module.

## Requirements
- Arduino board or any other manufacturer with the possibility of connecting to the internet.
- Arduino IDE
- USB Cable

## Common errors
<ul>
  <li>https://arduino.stackexchange.com/questions/54373/nodemcu-arduino-ide-dynamicjsonbuffer-was-not-declared-in-this-scope/54377</li>
  <li>https://stackoverflow.com/questions/50080260/arduino-ide-cant-find-esp8266wifi-h-file</li>
  <li>https://github.com/FirebaseExtended/firebase-arduino/issues/354</li>
</ul>
