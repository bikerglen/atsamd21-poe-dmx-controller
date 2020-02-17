/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>
#include <timer.h>
#include <SERCOM.h>
#include <Uart.h>
#include <wiring_private.h>
#include "samd21dmx.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 180, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
auto effectsTimer = timer_create_default(); 

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode (11, OUTPUT);
  digitalWrite (11, LOW);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  dmx.begin ();
  
  effects_Init ();
  effectsTimer.every (20, effects_Run);
}

int guiMode = 0;
String request = String (128);

void loop()
{
  int pageId = 0;
  int reqPageId = 0;

  effectsTimer.tick ();
  
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {
    Serial.println("new client");
    request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\r') {
          if (c == '\n') {
            if (request.length () == 0) {
              break;
            } else {
              // determine URL and send HTML or CSS response, parse parameters
              reqPageId = ParseRequest (request);
              if (reqPageId >= 0) {
                pageId = reqPageId;
                // Serial.printf ("pageId is now %d\n\r", pageId);
              }
              request = "";
            }
          } else if (request.length () < 128) {
            request += c;
          }
        }
      }
    }

    if (pageId == 0) {
      // send HTML response
      SendHtmlPage (client, guiMode);
    } else if (pageId = 1) {
      // send CSS response
      SendStyleSheet (client);
    }

    // give the web browser time to receive the data
    client.flush();
    // delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


int ParseRequest (String request)
{
  int pageId = -1;
  String path;
  String params;
  String keyValuePair;
  String key;
  String value;

  int newMode = -1;
  int newColor = -1;
  int newDirection = -1;
  int newCycleTime = -1;
  int newWashOffset = -1;
  int newRandomDelay = -1;
  int newWhiteLevel = -1;
  
  if (request.substring (0, 4) == "GET ") {
    // Serial.println ("GET");
    int space = request.indexOf (' ', 4);
    int qmark = request.indexOf ('?', 4);
    // Serial.printf ("  space: %d, qmark: %d\n\r", space, qmark);
    if (qmark < 0) {
      // no parameters
      path = request.substring (4, space);
      params = "";
    } else {
      // some parameters
      path = request.substring (4, qmark);
      params = request.substring (qmark+1, space);
    }
    
    // Serial.printf ("  path is '%s'\n\r", path.c_str());
    // Serial.printf ("  parameters is '%s'\n\r", params.c_str());

    if (path == "/style.css") {
      pageId = 1;
    } else {
      pageId = 0;
    }
    
    for (int i = 0; i < params.length (); ) {
      int kve = params.indexOf ('&', i);
      if (kve < 0) {
        keyValuePair = params.substring (i);
        i = params.length ();
      } else {
        keyValuePair = params.substring (i, kve);
        i = kve + 1;
      }
      int eq = keyValuePair.indexOf ('=');
      if (eq < 0) {
        key = keyValuePair;
        value = "";
      } else {
        key = keyValuePair.substring (0, eq);
        value = keyValuePair.substring (eq+1);
      }
      // Serial.printf ("  key = '%s' and value = '%s'\n\r", key.c_str(), value.c_str());
      
      if (key == "mode") {
        newMode = atoi (value.c_str());
        if ((newMode < 0) || (newMode > 7)) newMode = -1;
      } else if (key == "color") {
        newColor = hexatoi (value.c_str());
        if ((newColor < 0) || (newColor > 0xFFFFFF)) newMode = -1;
      } else if (key == "direction") {
        newDirection = atoi (value.c_str());
      } else if (key == "cycle_time") {
        newCycleTime = hexatoi (value.c_str());
      } else if (key == "wash_offset") {
        newWashOffset = hexatoi (value.c_str());
      } else if (key == "random_delay") {
        newRandomDelay = atoi (value.c_str());
      } else if (key == "level") {
        newWhiteLevel = atoi (value.c_str());
      }
    }

    // Update effects generator
    if (pageId == 0) {
      // Serial.printf ("  newMode:        %d\n\r", newMode);
      // Serial.printf ("  newColor:       %06x\n\r", newColor);
      // Serial.printf ("  newDirection:   %d\n\r", newDirection);
      // Serial.printf ("  newCycleTime:   %d\n\r", newCycleTime);
      // Serial.printf ("  newWashOffset:  %d\n\r", newWashOffset);
      // Serial.printf ("  newRandomDelay: %d\n\r", newRandomDelay);
      // Serial.printf ("  newWhiteLevel:  %d\n\r", newWhiteLevel);

      // Set Parameters
      if (newColor >= 0) {
        SetFixedColor (newColor);
      } else if (newDirection >= 0) {
        SetDirection (newDirection);
      } else if (newCycleTime >= 0) {
        SetStep (newCycleTime);
      } else if (newWashOffset >= 0) {
        SetOffset (newWashOffset);
      } else if (newRandomDelay >= 0) {
        SetRandomDelay (newRandomDelay);
      } else if (newWhiteLevel >= 0) {
        SetWhiteLevel (newWhiteLevel);
      }
      
      // Set Mode for Effects
      if (newMode >= 1) {
        SetMode (newMode);
      }

      // Set Mode for Web GUI
      guiMode = newMode;
    }
  } 

  return pageId;
}


int hexatoi (const char *a)
{
  int r = 0;
  char ch;
  while (ch = *a++) {
    if ((ch >= '0') && (ch <= '9')) {
      r = (r << 4) | (ch - '0');
    } else if ((ch >= 'a') && (ch <= 'f')) {
      r = (r << 4) | (ch - 'a' + 10);
    } else if ((ch >= 'A') && (ch <= 'F')) {
      r = (r << 4) | (ch - 'A' + 10);
    }
  }

  return r;
}
