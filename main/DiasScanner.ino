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
    String html = "<!DOCTYPE html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "<meta charset=\"UTF-8\">\n"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                "<title>Slide Scanner Control</title>\n"
                "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css\">\n"
                "</head>\n"
                "<body>\n"
                "<div class=\"container\">\n"
                "<h2>Slide Scanner Control</h2>\n"
                "<form id=\"slideForm\">\n"
                "<div class=\"form-group\">\n"
                "<label for=\"timeForSwitch\">Time for cassette to switch from one slide to another:</label>\n"
                "<input type=\"number\" class=\"form-control\" id=\"timeForSwitch\" min=\"0\" value=\"" + String(timeForSwitch) + "\">\n"
                "</div>\n"
                "<div class=\"form-group\">\n"
                "<label for=\"timeForInterval\">Time between each slide action wait to send image to storage:</label>\n"
                "<input type=\"number\" class=\"form-control\" id=\"timeForInterval\" min=\"0\" value=\"" + String(timeForInterval) + "\">\n"
                "</div>\n"
                "<div class=\"form-group\">\n"
                "<label for=\"timeForSettle\">Time for slide to settle into place before shutter is triggered:</label>\n"
                "<input type=\"number\" class=\"form-control\" id=\"timeForSettle\" min=\"0\" value=\"" + String(timeForSettle) + "\">\n"
                "</div>\n"
                "<div class=\"form-group\">\n"
                "<label for=\"timeForComplete\">Time for camera to complete operation before a new slide can be inserted:</label>\n"
                "<input type=\"number\" class=\"form-control\" id=\"timeForComplete\" min=\"0\" value=\"" + String(timeForComplete) + "\">\n"
                "</div>\n"
                "<div class=\"form-group\">\n"
                "<label for=\"totalSlides\">Total number of slides to be scanned in the cassette:</label>\n"
                "<input type=\"number\" class=\"form-control\" id=\"totalSlides\" min=\"0\" value=\"" + String(totalSlides) + "\">\n"
                "</div>\n"
                "</form>\n"
                "<button id=\"prevSlide\" class=\"btn btn-primary\">Previous Slide</button>\n"
                "<button id=\"nextSlide\" class=\"btn btn-primary\">Next Slide</button>\n"
                "<button id=\"ShutterTrigger\" class=\"btn btn-primary\">Trigger Sensor</button>\n"
                "<button id=\"startLoop\" class=\"btn btn-success\">Start Loop</button>\n"
                "<button id=\"stopLoop\" class=\"btn btn-danger\">Stop Loop</button>\n"
                "<button id=\"SaveValues\" class=\"btn btn-success\">Save Values</button>\n"
                "</div>\n"
                "</body>\n"
                "</html>\n";
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
