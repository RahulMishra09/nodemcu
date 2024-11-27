#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Create an instance of the server
ESP8266WebServer server(80);

// Function to add CORS headers
void addCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");  // Allow requests from any origin
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define the HTTP GET request handlers
  server.on("/", []() {
    addCORSHeaders();
    server.send(200, "text/html", "<h1>Send Message</h1><form action='/send' method='GET'><input type='text' name='msg'><input type='submit' value='Send'></form>");
  });

  server.on("/send", []() {
    addCORSHeaders();
    String message = server.arg("msg");
    Serial.println(message);  // Send the message to the Arduino via Serial
    server.send(200, "text/html", "<h1>Message Sent!</h1><a href='/'>Go Back</a>");
  });

  // Handle preflight requests (OPTIONS method for CORS)
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      addCORSHeaders();
      server.send(204);  // No content for OPTIONS requests
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Handle incoming client requests
}