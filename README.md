Easy Roll Call
=============

This Arduino firmware is part of a student project.
 It is an arduino based terminal, scanning student NFC cards to easily computerize the roll call.

What it does :
* Sending a http GET request to a server depending on the NFC's id of the scanned card.

To make it work you’ll need this material :
* An arduino board with ethernet shield or an arduino ethernet
* An Adafruit PN532 NFC/RFID Controller Shield www.adafruit.com/product/789
* MiFare Classic (13.56MHz RFID/NFC) Card www.adafruit.com/product/359

Software :
* arduino IDE : www.arduino.cc/en/main/software
* arduino libraries for NFC : 
	* https://github.com/don/NDEF
	* https://github.com/Seeed-Studio/PN532

Code :
* Modify the server's name you want to send request to in easyrollcall.ino
