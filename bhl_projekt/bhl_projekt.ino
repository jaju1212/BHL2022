#include<Servo.h>

#define PIN A0
#define STEP_PIN 9
#define DIR_PIN 10

#define PILL_TAKEN_PIN 12
#define REMINDER_DIODE 10

#define STEP_DEG 4.5

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

}

void loop() {
  
  delay(1000);


}

//Function prototypes

bool checkIfTaken(void) {
  bool val = digitalRead(PILL_TAKEN_PIN);
  return val;
}

void reminderDiodeState(int state) {
  digitalWrite(REMINDER_DIODE, state);
}

void stepperAngle(int pulse, int dir) {
  if (dir == 0) {
    digitalWrite(STEP_PIN, LOW);
  }
  else
  {
    digitalWrite(STEP_PIN, HIGH);
  }

  for(int i = 0; i < pulse; i++){
    digitalWrite(STEP_PIN, HIGH);
    delay(1);
    digitalWrite(STEP_PIN, LOW);
  }



}
