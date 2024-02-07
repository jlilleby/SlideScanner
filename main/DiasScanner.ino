#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int prevSlidePin = D1;
const int nextSlidePin = D2;
const int shutterTriggerPin = D3;

const int timeForSwitch = 1000;    // Example time for switch in milliseconds
const int timeForInterval = 2000;  // Example time between each slide action in milliseconds
const int timeForSettle = 500;     // Example time for slide to settle in milliseconds
const int timeForComplete = 3000;  // Example time for camera complete operation in milliseconds
const int totalSlides = 10;        // Example total number of slides
int currentSlide = totalSlides;    // Initialize current slide to the total number of slides

ESP8266WebServer server(80);

bool loopRunning = false;

void handleRoot() {
  String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Slide Scanner Control</title><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css\"></head><body><div class=\"container\"><h2>Slide Scanner Control</h2><form id=\"slideForm\"><div class=\"form-group\"><label for=\"timeForSwitch\">Time for cassette to switch from one slide to another:</label><input type=\"number\" class=\"form-control\" id=\"timeForSwitch\" min=\"0\" value=\"" + String(timeForSwitch) + "\"></div><div class=\"form-group\"><label for=\"timeForInterval\">Time between each slide action wait to send image to storage:</label><input type=\"number\" class=\"form-control\" id=\"timeForInterval\" min=\"0\" value=\"" + String(timeForInterval) + "\"></div><div class=\"form-group\"><label for=\"timeForSettle\">Time for slide to settle into place before shutter is triggered:</label><input type=\"number\" class=\"form-control\" id=\"timeForSettle\" min=\"0\" value=\"" + String(timeForSettle) + "\"></div><div class=\"form-group\"><label for=\"timeForComplete\">Time for camera to complete operation before a new slide can be inserted:</label><input type=\"number\" class=\"form-control\" id=\"timeForComplete\" min=\"0\" value=\"" + String(timeForComplete) + "\"></div><div class=\"form-group\"><label for=\"totalSlides\">Total number of slides to be scanned in the cassette:</label><input type=\"number\" class=\"form-control\" id=\"totalSlides\" min=\"0\" value=\"" + String(totalSlides) + "\"></div></form><button id=\"prevSlide\" class=\"btn btn-primary\">Previous Slide</button><button id=\"nextSlide\" class=\"btn btn-primary\">Next Slide</button><button id=\"ShutterTrigger\" class=\"btn btn-primary\">Trigger Sensor</button><button id=\"startLoop\" class=\"btn btn-success\">Start Loop</button><button id=\"stopLoop\" class=\"btn btn-danger\">Stop Loop</button><button id=\"SaveValues\" class=\"btn btn-success\">Save Values</button></div></body></html>";
  server.send(200, "text/html", html);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void prevSlide() {
  digitalWrite(prevSlidePin, HIGH); // Assuming active HIGH
  delay(timeForSwitch);
  digitalWrite(prevSlidePin, LOW);
}

void nextSlide() {
  digitalWrite(nextSlidePin, HIGH); // Assuming active HIGH
  delay(timeForSwitch);
  digitalWrite(nextSlidePin, LOW);
}

void shutterTrigger() {
  digitalWrite(shutterTriggerPin, HIGH); // Assuming active HIGH
  delay(timeForComplete);
  digitalWrite(shutterTriggerPin, LOW);
}


void startLoop() {
  if (!loopRunning) {
    loopRunning = true;
    // Start the loop logic
    while (loopRunning && currentSlide > 0) {
      // Perform actions for each iteration of the loop
      nextSlide(); // Example: Advance to the next slide
      delay(timeForSettle); // Allow time for the slide to settle
      shutterTrigger(); // Trigger the shutter
      delay(timeForInterval); // Wait before moving to the next slide
      
      currentSlide--; // Decrement the current slide number
      
      // Check if the loop should stop
      if (currentSlide == 0) {
        stopLoop();
      }
    }
  }
}

void stopLoop() {
  loopRunning = false;
  // Additional cleanup or actions to stop the loop
}

void saveValues() {
  // Code to save values entered in text fields
}

void setup() {
  pinMode(prevSlidePin, INPUT_PULLUP);
  pinMode(nextSlidePin, INPUT_PULLUP);
  pinMode(shutterTriggerPin, INPUT_PULLUP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();

}
