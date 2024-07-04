#include <Mouse.h>
#include <Keyboard.h>


const int xAxisPin = A0;
const int yAxisPin = A1;
const int DebuggingStopPin = 9;
int responseDelay = 5;        // Milliseconds between mouse updates
int max_range = 500;    
int Ycenter = 95;     // Joystick's neutral position value
int Xcenter = 94;     // Joystick's neutral position value
int analog_debugging_active = 0;
int general_debugging_active = 1;
int YdeadZone = 10; // Deadzone around joystick center 
int XdeadZone = 10; // Deadzone around joystick center 
int mouseSpeed = 1; 
int max_distance = 2;
int mouse_enabled_dip_switch_pin = 2;
int mouse_enabled_dip_switch_state = LOW;
int speed_setting_dip_switch_pin = 3;
int speed_setting_dip_switch_state = LOW;
int firstBottomButtonPin = 0;
int previousFirstBottomButtonPinState = HIGH;
int secondBottomButtonPin = 1;
int previousSecondBottomButtonPinState = HIGH;
int thirdBottomButtonPin = 11;
int previousThirdBottomButtonPinState = HIGH;
int mouseButtonPin = 1;
int previousMouseButtonPinState = HIGH;
int joystickButtonPin = 20;
int previousJoystickButtonState = HIGH;



void setup() {
  Mouse.begin();
  pinMode(joystickButtonPin, INPUT_PULLUP);
  pinMode(mouseButtonPin, INPUT_PULLUP);
  // pinMode(firstBottomButtonPin, INPUT_PULLUP);
  // pinMode(secondBottomButtonPin, INPUT_PULLUP);
  // pinMode(thirdBottomButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //   // Turn off TX LED
  // pinMode(LED_BUILTIN_TX, INPUT);

  // // Turn off RX LED
  // pinMode(LED_BUILTIN_RX, INPUT);
}

void loop() {

  mouseSpeed = 1;
  move_mouse();
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on 

/// ANALOG STICK PRESS - SIMULATES SCREEN TOUCH/MOUSE CLICK
  int joystickButtonState = digitalRead(joystickButtonPin);
  if (joystickButtonState != previousJoystickButtonState) {
    if (joystickButtonState == LOW) { 
      Mouse.press(MOUSE_LEFT);
      Serial.println("Joystick pressed"); // Pressed when LOW
    } else {
      Mouse.release(MOUSE_LEFT);
      Serial.println("Joystick released");
    }
    previousJoystickButtonState = joystickButtonState;
  }
/// TOP RED BUTTON - SIMULATES SCREEN TOUCH/MOUSE CLICK
  int mouseButtonPinState = digitalRead(mouseButtonPin);
  if (mouseButtonPinState != previousMouseButtonPinState) {
    if (mouseButtonPinState == LOW) { 
      Mouse.press(MOUSE_LEFT);
      Serial.println("Mouse Button pressed"); // Pressed when LOW
    } else {
      Mouse.release(MOUSE_LEFT);
      Serial.println("Mouse Button released");
    }
    previousMouseButtonPinState = mouseButtonPinState;
  }



  delay(responseDelay);
} 


void move_mouse(){
  int yreading = analogRead(yAxisPin);
  yreading = map(yreading, 0, 2047, 60, 200); // Scale to output range
  // Apply deadzone
  int ydistance = yreading - Ycenter;
  int ydebug_distance = ydistance;
  if (abs(ydistance) < YdeadZone) {
    ydistance = 0;
  }
  if (analog_debugging_active) {
    Serial.print("------YAXIS Info ");
    Serial.print("Reading: ");
    Serial.print(yreading);
    Serial.print(" Distance with Deadzone: ");
    Serial.print(ydistance);
    Serial.print(" Distance w/o Deadzone: ");
    Serial.print(ydebug_distance);
    Serial.println("------End YAXIS");
  }
  int yadjusted_distance = ydistance;
  if (ydistance != 0){
    if (yadjusted_distance < 0){
      yadjusted_distance = (-max_distance) * mouseSpeed;
    }
    else {
      yadjusted_distance = max_distance * mouseSpeed;
    }
  }
  int yFinalReading = yadjusted_distance;

  int xreading = analogRead(xAxisPin);
  xreading = map(xreading, 0, 2047, 60, 200); // Scale to output range
  // Apply deadzone
  int xdistance = xreading - Xcenter;
  int xdebug_distance = xdistance;
  if (abs(xdistance) < XdeadZone) {
    xdistance = 0;
  }
  if (analog_debugging_active) {
    Serial.print("------XAXIS Info ");
    Serial.print("Reading: ");
    Serial.print(xreading);
    Serial.print(" Distance with Deadzone: ");
    Serial.print(xdistance);
    Serial.print(" Distance w/o Deadzone: ");
    Serial.print(xdebug_distance);
    Serial.println("------End XAXIS");
  }
  int xadjusted_distance = xdistance;
  if (xdistance != 0){
    if (xadjusted_distance < 0){
      xadjusted_distance = (-max_distance) * mouseSpeed;
    }
    else {
      xadjusted_distance = max_distance * mouseSpeed;
    }
  }
  int xFinalReading = xadjusted_distance;
  Mouse.move(xFinalReading, -yFinalReading, 0);
  
  if (analog_debugging_active) {
    Serial.print("X Distance: ");
    Serial.print(xFinalReading);
    Serial.print(" || Y Distance: ");
    Serial.println(-yFinalReading);
  }
}
