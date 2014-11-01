/*

  easyRollCall v1.0
 This Arduino firmware is part of a student project.
 It is a terminal scanning student NFC cards to easily computerize the roll call.
 
 Circuit:
 * Arduino Ethernet
 * Adafruit PN532 NFC/RFID Controller Shield
 
 created October 2014
 by Benjamin Gammaire and Irfann Karimjy
 
 */

// uncomment if you want to enable debug
//#define DEBUG

// dependencies for ethernet
#include <SPI.h>
#include <Ethernet.h>

// dependencies for NFC
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// Ethernet configuration
// Declare a MAC address for the controller
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Fill in an available IP address on your network here,
// for auto-configuration:
IPAddress ip(192, 168, 0, 19);
IPAddress subnet(255, 255, 0, 0);
// Initialize the Ethernet client library
EthernetClient client;

// Initialize NFC
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

// initialize a table with the ID of each student
int student_ids_array_lenght = 3;
String student_ids[] = {"43 AA 23 D4", "93 EE 20 D4", "23 0B 1C D4"};

boolean processing_scan = false;

void setup(void) {
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);
  
  Ethernet.begin(mac);
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    #ifdef DEBUG
    Serial.println("failed to get an IP address using DHCP, trying manually");
    #endif
    Ethernet.begin(mac, ip, subnet);
  }
   nfc.begin();
}

void loop(void) {
  // If a NFC card is scanned
  if (nfc.tagPresent() && !processing_scan) {
      processing_scan = true;
      NfcTag tag = nfc.read();
      String id = tag.getUidString();
      #ifdef DEBUG
      Serial.println(id);
      #endif
      // loop throught student_ids array
      for (int i = 0; i <= student_ids_array_lenght ; i++) {
        // If the scanned card is own by one of our student
        if (id == student_ids[i]){  
          Serial.println(student_ids[i]);
          // Send a request to the server
          sendGET();
          // Give the server time to answer
          delay(2000);
          if (client.connected() && client.available()) {
            #ifdef DEBUG
            Serial.println("connected ready to print");
            #endif
            String c = client.readString();
            delay(1000);
            #ifdef DEBUG
            Serial.print(c);
            Serial.println("end reading");
            #endif
            
            int check_server_response = c.indexOf("string_send_by_your_server"); //change the value of the string verified by the indexOf with the value your server is sending back
            if(check_server_response != -1){
              // Here you can add actions if your request is sucessfull, like blinking a green led
              Serial.println("Student save in server");
            } else {
              Serial.println("Fail to save student in server");
            }
            //close connection, flush datas
            client.stop();
            processing_scan = false;
            // Break the loop
            break;
          }
        }
        #ifdef DEBUG
        else {
          Serial.println("Unknown student id");
        }
        #endif
      }
      processing_scan = false;
   }
 } 
  //client function to send GET request data.
  void sendGET(){ 
   if (client.connect("www.yourserver.com", 80)) { //change the server's name
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /the-url/youwanttoget HTTP/1.1");
    client.println("User-Agent: arduino-ethernet");
    client.println("Host: www.yourserver.com");
    client.println("Connection: close");
    client.println();
    } 
    else {
      // if you couldn't make a connection:
      Serial.println("connection failed");
      Serial.println("disconnecting.");
      client.stop();
    }
  }
