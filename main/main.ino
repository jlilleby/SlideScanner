#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Jones iPhone";
const char *password = "1234567890";

ESP8266WebServer server(80);

const int projectorForwardPin = D2;  // Connect to the projector forward trigger
const int projectorReversePin = D3;  // Connect to the projector reverse trigger
const int cameraTriggerPin = D4;     // Connect to the camera shutter trigger

int totalSlides = 10000;  // Set the total number of slides
int currentSlide = 1;     // Current slide number
int delayBetweenSlides = 5000;  // Delay in milliseconds between slides

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  pinMode(projectorForwardPin, OUTPUT);
  pinMode(projectorReversePin, OUTPUT);
  pinMode(cameraTriggerPin, OUTPUT);

  digitalWrite(projectorForwardPin, LOW);
  digitalWrite(projectorReversePin, LOW);
  digitalWrite(cameraTriggerPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
  
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  // Your main loop code goes here
}

void handleRoot() {
  String message = "<html><body>";
  message += "<h1>Slide Scanner Control</h1>";
  message += "<button onclick=\"reverseToPreviousSlide()\">Reverse to Previous Slide</button>";
  message += "<button onclick=\"advanceToNextSlide()\">Advance to Next Slide</button>";
  message += "<button onclick=\"triggerShutter()\">Trigger Shutter</button>";
  message += "<button onclick=\"startSlideLoop()\">Start Slide Loop</button>";
  message += "<button onclick=\"stopSlideLoop()\">Stop Slide Loop</button>";
  message += "</body></html>";
  
  server.send(200, "text/html", message);
}

void reverseToPreviousSlide() {
  // Add code to reverse to the previous slide
  currentSlide = max(1, currentSlide - 1);
}

void advanceToNextSlide() {
  // Add code to advance to the next slide
  currentSlide = min(totalSlides, currentSlide + 1);
}

void triggerShutter() {
  // Add code to trigger the camera shutter
  digitalWrite(cameraTriggerPin, HIGH);
  delay(100);
  digitalWrite(cameraTriggerPin, LOW);
}

void startSlideLoop() {
  // Add code to start the slide loop
  for (int i = currentSlide; i <= totalSlides; i++) {
    // Advance to the next slide
    advanceToNextSlide();
    
    // Trigger the shutter
    triggerShutter();

    // Delay between slides
    delay(delayBetweenSlides);
  }
}

void stopSlideLoop() {
  // Add code to stop the slide loop
  // You might need to add specific logic to stop the loop if it's running
}