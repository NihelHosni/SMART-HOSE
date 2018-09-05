#include <SoftwareSerial.h>
#include "WiFly.h"
#include <SPI.h>
#include <SD.h>
#define SSID      "Connectify-me"
#define KEY       "07775010"
// check your access point's security mode, mine was WPA20-PSK
// if yours is different you'll need to change the AUTH constant, see the file WiFly.h for avalable security codes
#define AUTH      WIFLY_AUTH_WPA2_PSK
// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   20
int flag = 0;
char d;
int pinNumber;
// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
//SoftwareSerial wiflyUart(2, 3); // create a WiFi shield serial object
SoftwareSerial wiflyUart(10, 11); // create a WiFi shield serial object
WiFly wifly(&wiflyUart); // pass the wifi siheld serial object to the WiFly class
char ip[16];
char request[4];
char Buff[512] = {0};
File webFile;
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char FileName[10];
char req_index = 0;              // index into HTTP_req buffer
void setup() 
{
 // pinMode(11,OUTPUT);
 // digitalWrite(11,LOW);
  // pinMode(3,INPUT);

  attachInterrupt(digitalPinToInterrupt(2), blink1, RISING); 
  attachInterrupt(digitalPinToInterrupt(3), blink2, RISING);
  attachInterrupt(digitalPinToInterrupt(18), blink3, RISING);
  
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  // 
    pinMode(24,OUTPUT);
  digitalWrite(24,LOW);
  
    pinMode(26,OUTPUT);
  digitalWrite(26,LOW);
  
      pinMode(28,OUTPUT);
  digitalWrite(28,LOW);
  
    pinMode(30,OUTPUT);
  digitalWrite(30,LOW);
  
  
        pinMode(32,OUTPUT);
  digitalWrite(32,LOW);
  
    pinMode(34,OUTPUT);
  digitalWrite(34,LOW);
  
  
  
  
  
  wiflyUart.begin(9600); // start wifi shield uart port
  Serial.begin(9600); // start the arduino serial port
  Serial.println("--------- WIFLY Webserver --------");
  // wait for initilization of wifly
  delay(1000);
  wifly.reset(); // reset the shield 
  Serial.println("-- reset the shield --");
  delay(1000);
  //set WiFly params
  wifly.sendCommand("set ip local 80\r"); // set the local comm port to 80
  delay(100);
  wifly.sendCommand ("set ip protocol 18\r");
  delay(100);
  wifly.sendCommand("set comm remote 0\r"); // do not send a default string when a connection opens
  delay(100);
  wifly.sendCommand("get everything\r"); // do not send a default string when a connection opens
  delay(100);
  wifly.sendCommand("set comm open *OPEN*\r"); // set the string that the wifi shield will output when a connection is opened
  delay(100);
  Serial.println("Join " SSID );
  if (wifly.join(SSID, KEY, AUTH)) {
    Serial.println("OK");
  } else {
    Serial.println("Failed");
  }
  wifly.sendCommand("get ip\r");
  wiflyUart.setTimeout(500);
  if(!wiflyUart.find("IP="))
  {
    Serial.println("can not get ip");
    while(1);
  }else
  {
    Serial.print("IP:");
  }
  char c;
  int index = 0;
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) { // print the response from the get ip command
    if(c == ':')
    {
      ip[index] = 0; 
      break;
    }
    ip[index++] = c;
    Serial.print((char)c);
  }
  Serial.println();
  Serial.println("Content of char c");
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) { Serial.print((char)c); }
  Serial.println();
  Serial.println("Web server ready");
      // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(53)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
}

void loop()
{

     if(wifly.available()) 
  { // the wifi shield has data available

      Serial.println("available!");
                  // Vidage de la mémoire de reception Wifly
            // Serial.println("Content of char d after available");
            // while (wifly.receive((uint8_t *)&d, 1, 300) > 0) { Serial.print((char)d); }
            // Serial.println();

      if(wiflyUart.find("*OPEN*")) // see if the data available is from an open connection by looking for the *OPEN* string
      // if(true) // see if the data available is from an open connection by looking for the *OPEN* string
    {
      Serial.println("New Browser Request!");
      delay(1000); // delay enough time for the browser to complete sending its HTTP request string
      if(wiflyUart.find("pin=")) // look for the string "pin=" in the http request, if it's there then we want to control the LED
      { 
              Serial.println("LED Control");
               pinNumber = (wiflyUart.read()-48);
                  // if (pinNumber ==0)
                  // {
                        // digitalWrite(24,HIGH);                  
                        // Serial.println("LED 24 OFF");
                  // }
                  // else 
                  // {
                        // digitalWrite(24,LOW);
                        // Serial.println("LED 24 ON");
                  // }
               pinNumber = wiflyUart.read();
               
               pinNumber = (wiflyUart.read()-48);
                  // if (pinNumber ==0)
                  // {
                        // digitalWrite(26,HIGH);                  
                        // Serial.println("LED 12 OFF");
                  // }
                  // else 
                  // {
                        // digitalWrite(26,LOW);
                        // Serial.println("LED 12 ON");
                  // }
               pinNumber = wiflyUart.read();  
               
               pinNumber = (wiflyUart.read()-48);
                  // if (pinNumber ==0)
                  // {
                        // digitalWrite(28,HIGH);                  
                        // Serial.println("LED 12 OFF");
                  // }
                  // else 
                  // {
                        // digitalWrite(28,LOW);
                        // Serial.println("LED 12 ON");
                  // }
               pinNumber = wiflyUart.read();
               
                              pinNumber = (wiflyUart.read()-48);
                  if (pinNumber ==0)
                  {
                        digitalWrite(30,HIGH);                  
                        Serial.println("LED 12 OFF");
                  }
                  else 
                  {
                        digitalWrite(30,LOW);
                        Serial.println("LED 12 ON");
                  }
               pinNumber = wiflyUart.read();
                              pinNumber = (wiflyUart.read()-48);
                  if (pinNumber ==0)
                  {
                        digitalWrite(32,HIGH);                  
                        Serial.println("LED 12 OFF");
                  }
                  else 
                  {
                        digitalWrite(32,LOW);
                        Serial.println("LED 12 ON");
                  }
               pinNumber = wiflyUart.read();
                              pinNumber = (wiflyUart.read()-48);
                  if (pinNumber ==0)
                  {
                        digitalWrite(34,HIGH);                  
                        Serial.println("LED 12 OFF");
                  }
                  else 
                  {
                        digitalWrite(34,LOW);
                        Serial.println("LED 12 ON");
                  }
               // pinNumber = wiflyUart.read();

        
        String pinStateToSend = "Pin ";
        if(digitalRead(24)) // check if the pin is ON or OFF
        {
         pinStateToSend+="0";  // the pin is off
        }
        else
        {
          pinStateToSend+="1"; // the pin is on
        }
        if(digitalRead(26)) // check if the pin is ON or OFF
        {
         pinStateToSend+="0";  // the pin is off
        }
        else
        {
          pinStateToSend+="1"; // the pin is on
        }  
        if(digitalRead(28)) // check if the pin is ON or OFF
        {
         pinStateToSend+="0";  // the pin is off
        }
        else
        {
          pinStateToSend+="1"; // the pin is on
        }
            // Vidage de la mémoire de reception Wifly
            Serial.println("Content of char d after PinState");
            while (wifly.receive((uint8_t *)&d, 1, 300) > 0) { Serial.print((char)d); }
            Serial.println();

            
        // build HTTP header Content-Length string.
        String contentLength="Content-Length: ";
        contentLength+=pinStateToSend.length(); // the value of the length is the lenght of the string the Arduino is replying to the browser with.
        // send HTTP header
        wiflyUart.println("HTTP/1.1 200 OK");
        wiflyUart.println("Content-Type: text/html; charset=UTF-8");
        wiflyUart.println(contentLength); // length of HTML code
        wiflyUart.println("Connection: close");
        wiflyUart.println();
        // send response
        wiflyUart.print(pinStateToSend);
        Serial.println("Arduino status sent to browser");
        // Serial.println(pinStateToSend);
      }
      else
          { 
            // Vidage de la mémoire de reception Wifly
            Serial.println("Content of char d before Page");
            while (wifly.receive((uint8_t *)&d, 1, 300) > 0) { Serial.print((char)d); }
            Serial.println();     
            
            // envoie de la page HTML initiale
                Serial.println("Initial HTML page");
                        // Serial.println("Sending index.htm");
                        wiflyUart.println("HTTP/1.1 200 OK");
                        wiflyUart.println("Content-Type: text/html");
                        wiflyUart.println("Content-Length: 7395"); // length of HTML code file.size() 
                        wiflyUart.println("Connnection: close");
                        wiflyUart.println();

                        for (int fi =1; fi<10;fi++)
                        {
                        String webfilename = "index";
                        webfilename+=(char)(fi+48);
                        webfilename+=".htm";

                        
                        Serial.println(webfilename);
        
                    if (SD.exists(webfilename)) {

    
                       webFile = SD.open(webfilename);        // open web page file
                        Serial.println(webFile.size());
                       
                         if (webFile) {
                                while(webFile.available()) {
                                    // wiflyUart.print((char) webFile.read()); // send web page to client
                                    wifly.write(webFile.read()); // send web page to client
                                }
                            webFile.close();
                            }
                                
                        }
        
    }
                        

                   
                   Serial.println("awake after sd read !");
                   wiflyUart.flush();
                   
            // Vidage de la mémoire de reception Wifly
            // Serial.println("Content of char d after Page sent");
            // while (wifly.receive((uint8_t *)&d, 1, 300) > 0) { Serial.print((char)d); }
            // Serial.println();
        }
       Serial.println("end if OPEN !");

    }
    else {
        // wiflyUart.flush();
        Serial.println("Here im in else OPEN!");
                    // Vidage de la mémoire de reception Wifly
            Serial.println("Content of buffer");
            while (wifly.receive((uint8_t *)&d, 1, 300) > 0) { Serial.print((char)d); }
            Serial.println();
        
        // send HTTP header
        // wiflyUart.println("HTTP/1.1 200 OK");
        // wiflyUart.println("Content-Type: text/html; charset=UTF-8");
        // wiflyUart.println("Connection: close");
        // wiflyUart.println();
    }
    Serial.println("end if available !");

    // delay(1); // delay enough time for the browser to complete sending its HTTP request string
  } 
}

void blink1() {
      digitalWrite(24, !digitalRead(24)); // toggle pin 
}



void blink2() {
      digitalWrite(26, !digitalRead(26)); // toggle pin 
}


void blink3() {
      digitalWrite(28, !digitalRead(28)); // toggle pin 
}



























