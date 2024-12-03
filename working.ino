#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int trigPin = 9;
const int echoPin = 10;
const int relayPin = 2;
const int mutePin = 3; // Define the pin for the mute switch
const int buzzerPin = 4; // Define the pin for the buzzer
const int RDP = 13;
const int darkYellowLEDPin = 6;
const int yellowLEDPin = 5;
const int greenLEDPin = 8;
const int blueLEDPin = 7;
const int modeSwitchPin = 11; // Define the pin for the mode toggle switch
 // Define the pin for the mode switch button
const int slideSwitchPin = 12; // Define the pin for the manual mode slide switch

bool mute = false;
bool lastButtonState = HIGH; // Previous state of the mute button
unsigned long lastDebounceTime = 0; // Last time the mute button was toggled
unsigned long debounceDelay = 50; // The debounce time; increase if the output flickers

bool autoMode = true; // Variable to track the current mode (default is auto)
bool lastModeButtonState = HIGH; // Previous state of the mode button
unsigned long lastModeDebounceTime = 0; // Last time the mode button was toggled

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(relayPin, OUTPUT);
  pinMode(mutePin, INPUT_PULLUP); // Set the mute switch pin as input with pull-up resistor
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output

  pinMode(RDP, OUTPUT);
  pinMode(darkYellowLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  pinMode(modeSwitchPin, INPUT_PULLUP); // Set the mode switch button pin as input with pull-up resistor
  pinMode(slideSwitchPin, INPUT_PULLUP); // Set the slide switch pin as input with pull-up resistor

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  long duration;
  int distance;
  int percent;

  // Check the state of the mute switch with debounce
  int muteReading = digitalRead(mutePin);
  if (muteReading != lastButtonState) {
    lastDebounceTime = millis(); // reset the debouncing timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (muteReading == LOW && !mute) {
      mute = true; // Set the mute state
      noTone(buzzerPin); // Ensure no tone is playing
    }
  }
  lastButtonState = muteReading;
  // Check the state of the mode toggle switch
  autoMode = digitalRead(modeSwitchPin) == HIGH; // Auto mode when switch is HIGH, manual mode when LOW

  

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  percent = 100 - map(distance, 0, 20 , 0, 100); // Mapping distance to percentage (0-100%)

  display.clearDisplay();

  // Print water level percentage
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(9.5, 11);
  display.print("WATER LEVEL \n");
  display.print("--------------");
  display.print("\n");

  // Draw a simple bar graph representing the water level
  int barHeight = map(percent, 0, 100, 0, SCREEN_HEIGHT - 15); // Adjusted height
  int barWidth = 35;
  int barX = 90;
  int barY = 13; // Adjusted starting point

  // Calculate top of the bar
  int barTop = barY + (SCREEN_HEIGHT - 14.1 - barHeight);

  // Ensure bar height does not exceed screen height
  if (barHeight > (SCREEN_HEIGHT - 14.1)) {
    barHeight = SCREEN_HEIGHT - 14.1;
  }

  // Draw outer border rectangle
  display.drawRect(barX - 1, barY - 1, barWidth + 2, SCREEN_HEIGHT - 16 + 2, SSD1306_WHITE);

  // Draw filled inner rectangle representing the bar
  display.fillRect(barX, barTop, barWidth, barHeight,SSD1306_WHITE);

  // Draw the dashed border box for percentage
  int boxWidth = 80;
  int boxHeight = 33;
  int boxX = 2;
  int boxY = SCREEN_HEIGHT - boxHeight - 3;

  // Draw top and bottom borders with dashes
  for (int i = boxX; i < boxX + boxWidth; i += 4) {
    display.drawPixel(i, boxY, SSD1306_WHITE);
    display.drawPixel(i + 1, boxY, SSD1306_WHITE);
    display.drawPixel(i, boxY + boxHeight, SSD1306_WHITE);
    display.drawPixel(i + 1, boxY + boxHeight, SSD1306_WHITE);
  }

  // Draw left and right borders
  for (int i = boxY; i < boxY + boxHeight; i += 4) {
    display.drawPixel(boxX, i, SSD1306_WHITE);
    display.drawPixel(boxX, i + 1, SSD1306_WHITE);
    display.drawPixel(boxX + boxWidth, i, SSD1306_WHITE);
    display.drawPixel(boxX + boxWidth, i + 1, SSD1306_WHITE);
  }

  // Display the percentage inside the box
  display.setCursor(boxX + 34, boxY + 8);
  display.setTextSize(1);
  display.print(percent);
  display.print("%");

  // Display mode
  display.setTextSize(1);
  display.setCursor(9.5, 0);
  if (autoMode) {
    display.print("MODE:AUTO");
  } else {
    display.print("MODE:MANUAL");
  }

  // Display refill status and warnings
  display.setTextSize(1);
  display.setCursor(17, SCREEN_HEIGHT - 16);
  
  if (digitalRead(relayPin) == LOW) {
    display.print("Refilling");
  } else if (!autoMode) {
    if (percent <= 15) {
      display.setCursor(20, SCREEN_HEIGHT - 16);
      display.print("LOW TANK");
    } else if (percent >= 95) {
      display.setCursor(20, SCREEN_HEIGHT - 16);
      display.print("OVERFLOW");
    }
  } 

  // Buzzer logic
  unsigned int frequencies[] = {500, 400, 500, 400, 500, 400, 500, 400, 500, 400, 500, 400, 500, 400, 500, 400};
  unsigned int frequencies1[] = {1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000};

  if (percent >= 95) {
    if (!mute) {
      digitalWrite(buzzerPin, HIGH);
      for (int i = 0; i < 16; i++) {
        tone(buzzerPin, frequencies1[i], 250);
        delay(100);
      }
    }
  } else if (percent <= 15) {
    if (!mute) {
      digitalWrite(buzzerPin, HIGH);
      for (int i = 0; i < 16; i++) {
        tone(buzzerPin, frequencies[i], 250);
        delay(100);
      }
    }
  } else {
    mute = false; // Reset the mute state when outside the range
    noTone(buzzerPin);
    digitalWrite(buzzerPin, LOW); // Ensure the buzzer is off
  }

  // Relay control
  if (autoMode) {
    if (percent <= 16 && percent>=0) {
      digitalWrite(relayPin, LOW);
      noTone(buzzerPin);
    } else if (percent >= 92) {
      digitalWrite(relayPin, HIGH);
    }
  } else {
    // Manual mode: control the relay with the slide switch
    if (digitalRead(slideSwitchPin) == LOW) {
      digitalWrite(relayPin, LOW);
    } else {
      digitalWrite(relayPin, HIGH);
    }
  }

  // Update LED states progressively
  digitalWrite(RDP, percent > 0);
  digitalWrite(darkYellowLEDPin, percent > 25);
  digitalWrite(yellowLEDPin, percent > 48);
  digitalWrite(greenLEDPin, percent > 68);
  digitalWrite(blueLEDPin, percent > 90);

  display.display();
  delay(1000);
}
