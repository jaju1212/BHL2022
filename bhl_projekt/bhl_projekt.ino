#include<Servo.h>

#define PIN A0
#define STEP_PIN 9
#define DIR_PIN 10
#define Pill_1 1
#define Pill_2 2
#define PILL_TAKEN_PIN 12
#define REMINDER_DIODE 10
#define DIODE A0

#define Pill_1_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_1_dir 1 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1
#define Pill_2_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_2_dir 0 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1



void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  
  Serial.begin(9600);

  //dose_Pill(1, 2);
  //dose_Pill(2, 1);

}

void loop() {
Serial.println(analogRead(A0));
delay(100);


}

//Function prototypes

bool checkIfTaken(void) {
  bool val = digitalRead(PILL_TAKEN_PIN);
  return val;
}

void reminderDiodeState(int state) {
  digitalWrite(REMINDER_DIODE, state);
}

void dose_Pill(int label, int n_pill) {  //8000 imp=360deg
  switch (label) {
    case 1:
      for (int j = 0; j < n_pill; j++)
      {
        digitalWrite(DIR_PIN, HIGH);

        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }
        
        digitalWrite(DIR_PIN, LOW);
        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }
        while(analogRead(DIODE)==HIGH)
        {
          
        }
      }
      break;
    case 2:
      for (int j = 0; j < n_pill; j++)
      {
        digitalWrite(DIR_PIN, LOW);

        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }
        
        digitalWrite(DIR_PIN, HIGH);
        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }
        while(digitalRead(DIODE)==HIGH)
        {
          
        }
      }
      break;

    default:
      break;
  }



}
//void dose_Pill_2(int n_pil) {  //8000 imp=360deg
//  for (int j = 0; j < n_pill; j++)
//  {
//    digitalWrite(DIR_PIN, LOW);
//
//    for (int i = 0; i < 3333; i++)
//    {
//      digitalWrite(STEP_PIN, HIGH);
//      delayMicroseconds(100);
//      digitalWrite(STEP_PIN, LOW);
//      delayMicroseconds(100);
//    }
//    digitalWrite(DIR_PIN, HIGH);
//    for (int i = 0; i < 3333; i++)
//    {
//      digitalWrite(STEP_PIN, HIGH);
//      delayMicroseconds(100);
//      digitalWrite(STEP_PIN, LOW);
//      delayMicroseconds(100);
//    }
//  }
//}
