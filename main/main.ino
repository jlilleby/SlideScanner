#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Jones iPhone";          // Change it to your WiFi network SSID
const char* password = "1234567890";  // Change it to your WiFi password

ESP8266WebServer server(80);

bool state = false;  // Initial state is off
bool loopRunning = false;

const int totalSlides = 10;        // Example total number of slides
int currentSlide = totalSlides;    // Initialize current slide to the total number of slides

void handleRoot() {
  server.send(200, "text/html",
              "<html><body>"
              "<h1>ESP8266 Web Interface</h1>"
              "<form action='/ReverseSlide' method='get'>"
              "<button>Reverse Slide</button>"
              "</form>"
              "<form action='/ForwardSlide' method='get'>"
              "<button>Forward Slide</button>"
              "</form>"
              "<form action='/Shutter' method='get'>"
              "<button>Trigger Shutter</button>"
              "</form>"
              "<form action='/StartLoop' method='get'>"
              "<button>Start</button>"
              "</form>"
              "<form action='/StopLoop' method='get'>"
              "<button>Stop</button>"
              "</form>"
              "</body></html>");
}
/*
void ReverseSlides() {
  state = !state;  // Toggle the state
  digitalWrite(D1, state ? HIGH : LOW);  // Turn on or off D1 pin
  digitalWrite(D2, state ? HIGH : LOW);  // Turn on or off D1 pin
  digitalWrite(D6, state ? HIGH : LOW);  // Turn on or off D1 pin
  digitalWrite(D7, state ? HIGH : LOW);  // Turn on or off D1 pin

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}*/

void ReverseSlides() {
  digitalWrite(D1, HIGH); // Assuming active HIGH

  delay(500);
  digitalWrite(D1, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void ForwardSlides() {
  digitalWrite(D2, HIGH); // Assuming active HIGH
  Serial.println("Advancing to the next slide...");
  delay(500);
  digitalWrite(D2, LOW);

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void ShutterTrigger() {
  digitalWrite(D6, HIGH); // Assuming active HIGH
  Serial.println("Triggering shutter...");
  delay(250);
  digitalWrite(D6, LOW);

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void StartLoop() {
  if (!loopRunning) {
    loopRunning = true;
    // Start the loop logic
    while (loopRunning && currentSlide > 0) {
      // Perform actions for each iteration of the loop
      ForwardSlides(); // Example: Advance to the next slide
      delay(2000); // Allow time for the slide to settle
      ShutterTrigger(); // Trigger the shutter
      delay(1000); // Wait before moving to the next slide
      
      currentSlide--; // Decrement the current slide number
      Serial.println(currentSlide);

        
      
      // Check if the loop should stop
      if (currentSlide == 0) {
        StopLoop();
      }
    }
  }
  server.sendHeader("Location", "/");
  /*server.send(302, "text/plain", ""); */
}

void StopLoop() {
  state = !state;  // Toggle the state
  Serial.println("Loop stopped");
  digitalWrite(D1, LOW);  // Turn on or off D1 pin
  digitalWrite(D2, LOW);  // Turn on or off D1 pin
  digitalWrite(D6, LOW);  // Turn on or off D1 pin
  digitalWrite(D7, LOW);  // Turn on or off D1 pin

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  pinMode(D1, OUTPUT);  // Initialize D1 pin as an output
  digitalWrite(D1, LOW);  // Initially turn off D1 pin
  pinMode(D2, OUTPUT);  // Initialize D1 pin as an output
  digitalWrite(D2, LOW);  // Initially turn off D1 pin
  pinMode(D6, OUTPUT);  // Initialize D1 pin as an output
  digitalWrite(D6, LOW);  // Initially turn off D1 pin
  pinMode(D7, OUTPUT);  // Initialize D1 pin as an output
  digitalWrite(D7, LOW);  // Initially turn off D1 pin

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/ReverseSlide", ReverseSlides);
  server.on("/ForwardSlide", ForwardSlides);
  server.on("/Shutter", ShutterTrigger);
  server.on("/StartLoop", StartLoop);
  server.on("/StopLoop", StopLoop);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
