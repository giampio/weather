#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 2 

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// WIFI Setting
const String ssid = "*******";
const String password = "********";

// Endpoint of API
const String api = "http://192.168.1.24:3000/api/v1/weather";

// Interval to send data to server
long previousMillis = 0;
long interval = 30000;

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Static ip
  IPAddress ip(192, 168, 1, 150);   
  IPAddress gateway(192, 168, 1, 1);   
  IPAddress subnet(255, 255, 255, 0);   
  WiFi.config(ip, gateway, subnet);

  // Inizialization Wifi 
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");  
}
 
void loop() {
 
  // Wait a few seconds between measurements.
  delay(3000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 3 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // Save the last time you send data
    previousMillis = currentMillis;   

    // Send data to server    
    sendJSON(t, h);
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  //Serial.println("New client");
  while(!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); 
    client.println("Failed to read from DHT sensor!");
    return;
  }

  //Serial.println(request);
  if (request.indexOf("/send") != -1) {
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); 

    // Send JSON to client
    client.println(sendJSON(t, h));
  } 
  else
  {    
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); 
       
    //Serial.print("T = ");
    //Serial.print(t, 1);
    //Serial.print(" deg. C, H = ");
    //Serial.print(h, 1);
    //Serial.println("%");
  
    // Send HTML to client
    client.println(showHTML(t, h));
  }
    
  //Serial.println("Client disconnected");
  //Serial.println("");
}

String showHTML(float t, float h){
  String html = "<!DOCTYPE html> <html>\n";
  html +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html +="<meta charset=\"UTF-8\">\n";
  html +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  html +="<title>ESP8266 Weather Station</title>\n";
  html +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  html +="body{margin-top: 50px;}\n";
  html +="h1 {margin: 50px auto 30px;}\n";
  html +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  html +=".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  html +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  html +=".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  html +=".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  html +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  html +=".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n";
  html +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  html +=".data{padding: 10px;}\n";
  html +=".animate{transition:all .1s;-webkit-transition:all .1s}\n";
  html +=".action-button{position:relative;padding:10px 40px;margin:0 10px 10px 0;border-radius:3px;font-family:Lato,sans-serif;font-size:18px;color:#fff;text-decoration:none;}\n";
  html +=".green{background-color:#2ecc71;border-bottom:5px solid #27ae60;text-shadow:0 -2px #27ae60;}\n";
  html +=".action-button:active{transform:translate(0,5px);-webkit-transform:translate(0,5px);border-bottom:1px solid;}\n";  
  html +=".reload{margin-top: 20px;}\n";
  html +="</style>\n";
  html +="</head>\n";
  html +="<body>\n";
  
   html +="<div id=\"webpage\">\n";
   
   html +="<h1>ESP8266 Weather Station</h1>\n";
   html +="<div class=\"data\">\n";
   html +="<div class=\"side-by-side temperature-icon\">\n";
   html +="<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"";
   html +="width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n";
   html +="<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
   html +="c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
   html +="c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
   html +="c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
   html +="c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
   html +="</svg>\n";
   html +="</div>\n";
   html +="<div class=\"side-by-side temperature-text\">Temperature</div>\n";
   html +="<div class=\"side-by-side temperature\">";
   html += t;
   html +="<span class=\"superscript\">°C</span></div>\n";
   html +="</div>\n";
   html +="<div class=\"data\">\n";
   html +="<div class=\"side-by-side humidity-icon\">\n";
   html +="<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"";
   html +="width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n";
   html +="<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
   html +="c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
   html +="</svg>\n";
   html +="</div>\n";
   html +="<div class=\"side-by-side humidity-text\">Humidity</div>\n";
   html +="<div class=\"side-by-side humidity\">";
   html += h;
   html +="<span class=\"superscript\">%</span></div>\n";
   html +="<div class=\"reload\"><a class=\"action-button shadow animate green\" href=\"javascript:location.reload();\">Reload</a>\n";
   html +="</div>\n";
   
  html +="</div>\n";
  html +="</body>\n";
  html +="</html>\n";
  return html;
}


String sendJSON(float t, float h){   
	String json;

	// Check if any reads failed and exit early (to try again).
	if (!isnan(h) && !isnan(t)) {
	  // Prepare array
	  StaticJsonDocument<256> doc;
	  doc["temperature"] = t;
	  doc["humidity"] = h;    
	  
	  // Serialize JSON
	  serializeJson(doc, json);

	  // Declare object of class HTTPClient
	  HTTPClient http;  

	  // Connect to server
	  http.begin(api);
	  http.addHeader("Content-Type", "application/json");  
			   
	  // Send the request
	  int httpCode = http.POST(json);   

	  // Get the response payload
	  String response = http.getString(); 
	  
	  // Close connection
	  http.end();
	}

	return json;
}
