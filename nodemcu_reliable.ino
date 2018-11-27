
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>

#include <WiFiUdp.h>

#include <functional>

#include <Wire.h>

WiFiServer server(80); //Initialize the server on Port 80

// Variable to store the HTTP request
String header;

// prototypes

boolean connectWifi();





// Change this before you flash

//#######################################

const char* ssid     = "Hello_IoT"; //enter your ssid/ wi-fi(case sensitive) router name - 2.4 Ghz only

const char* password = "12345678";     // enter ssid password (case sensitive)


const int relayPin1 = 16;

const int relayPin2 = 4;



//#######################################

boolean wifiConnected = false;


void setup()

{


WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); 
Serial.begin(9600);
server.begin(); // Start the HTTP Server 
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);


  

   pinMode(relayPin1, OUTPUT);

   pinMode(relayPin2, OUTPUT);
   digitalWrite(relayPin1, LOW); //Initial state is ON
    digitalWrite(relayPin2, LOW); //Initial state is ON


  
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
Serial.println("Complete setup");
}

 

void loop()

{


WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

             if (header.indexOf("GET /LED-OFF") >= 0) {
              Serial.println("kitchenLights Off");
             }
             else if (header.indexOf("GET /LED-ON") >= 0) {
              Serial.println("kitchenLights On");
             }


              // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<!DOCTYPE HTML>\r\n<html>");
            client.println("<br><input type=\"button\" name=\"b1\" value=\"Turn LED ON\" onclick=\"location.href='/LED-ON'\">");
            client.println("<br><br><br>");
            client.println("<input type=\"button\" name=\"bi\" value=\"Turn LED OFF\" onclick=\"location.href='/LED-OFF'\">");
            client.println("</html>\n");

                        // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
        // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
