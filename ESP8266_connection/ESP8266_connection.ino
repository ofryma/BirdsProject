#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "PinConfig.h"
#include "WiFi_settings.h"


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;



int str2int(String str){
  int i;
  int value = 0;
  int number = 0;
  char v [] = {"0"};
  int min_val = 48;
  int max_val = 48 + 9;
  for (i=0 ; i<sizeof(str)/sizeof(v[0]) ; i++){

    if (int(str[i]) >= min_val && int(str[i])<= max_val){
      value = int(str[i]) - 48;
      if( value >= 0 && value <= 9){
        number = number * 10;
        number = number + value;
      }

    }
  }
  return number;
}

#include "TimeStamp.h"
#include "Button.h"
#include "Pump.h"

Button Light1;


Pump* pumps [] = {&Pump1 , &Pump2};

int one_time = 1;
int counter = 0;
int last_counter = 0;

void setup() {

  Serial.begin(115200);


  // setup all the buttons and features
  Light1.Initialize("Random Mode", "light1" , "off" ,D3 , &Light1);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  WiFiClient client = server.available(); // Listen for incoming clients
  // if (one_time == 1){
  //   one_time = 0;
  //   time_clock.get_time(&time_clock , http_get_request("http://worldtimeapi.org","/api/timezone/Asia/Jerusalem" , client));
  //   checks(client);
  // }


  // every 10 min checks:
    if (mins() < counter){
      counter = mins();
      last_counter = counter;
    }
    if(counter - last_counter >= 10){
      last_counter = counter;
      checks(client);
    }else if(mins() > counter){
      counter = mins();
    }




  if (client) { // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();



            // Display the HTML web page
            client.println("<!DOCTYPE html><html lang='en' dir='ltr'><head><meta charset='utf-8'><title>OfryHome</title></head>");


            // CSS to style the on/off buttons

            client.println(CSS());
            client.println(HTML_BODY());

            client.println("</html>");



            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
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


String Header(String h_level , String text) {
  return "<h" + h_level + " class='header'>" + text + "</h" + h_level + ">";
}


String CSS() {

  String style = "";
  style = style +
          "<style>" +
          "body{background-color: #EFEFEF;}" +
          ".header{font-size: 5rem;color: #277BC0;text-shadow: 2px 2px 5px black;}" +
          ".btn{border-radius: 25px;border-width: thick;font-size: 30px;width: 100%;height: 12rem;}" +
          ".btn-on{background-color: #3B9AE1;}" +
          ".btn-off{background-color: #F1EFDC;box-shadow: 8px 8px 35px black;}" +
          ".buttns-container{border-width: medium;}" +
          ".pump-info{display: inline-block;}" +
          ".controler{margin: 7px;}" +
          ".float{display: inline-block;}" +
          ".general-text{font-size: 2.5rem;color: black;}" +
          ".btn-controler{width: 2.5rem; height: 2.5rem;}" +
          "</style>";


  return style;



}

String HTML_BODY() {

  String Body = "";

  Body = Body +
         "<body>" +
         "<div class='buttns-container'>" +

        Header("2" , "Feeding Mode") +
        Light1.ToggleButtonElement(&Light1) +
        my_button.ToggleButtonElement(&my_button) +


         "</div>" +




         "</body>";

  return Body;



}

String http_get_request(String endpoint , String route ,WiFiClient open_client){

    String json;

    String req = endpoint + route;
    HTTPClient http;

    http.useHTTP10(true);
    http.begin(open_client , req);
    http.GET();
    json = http.getString();


    http.end();

    return json;
    
}

void checks(WiFiClient open_client){
  checkPumps(open_client);
}

void checkPumps(WiFiClient open_client){


  int i=0;
  time_clock.get_time(&time_clock , http_get_request("http://worldtimeapi.org","/api/timezone/Asia/Jerusalem" , open_client));
  for (i=0 ; i < sizeof(pumps)/sizeof(Pump*); i++){
      Serial.print((*pumps[i]).button.name);
      Serial.print(" -> state: ");
      Serial.print((*pumps[i]).button.state);
      Serial.print(", ");
      Serial.print( (*pumps[i]).pump_freq + (*pumps[i]).last_pump.day_of_year - time_clock.day_of_year);
      Serial.println(" days to next pump");
    if ((*pumps[i]).button.state == "off") {
      continue;
    }
    if(time_clock.day_of_year - (*pumps[i]).pump_freq >= (*pumps[i]).last_pump.day_of_year){

      (*pumps[i]).last_pump.day_of_year = time_clock.day_of_year;
      (*pumps[i]).activate(pumps[i]);
    }

  }
}

int secs(){
  return millis()/1000;
}
int mins(){
  return secs()/60;
}
int hours(){
  return mins()/60;
}
