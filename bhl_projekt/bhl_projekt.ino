#include<Servo.h>

#define PIN A0
#define STEP_PIN 9
#define DIR_PIN 10
#define Pill_1 1
#define Pill_2 2
#define PILL_TAKEN_PIN 12
#define REMINDER_DIODE 10
#define DIODE A0
#define PIR_PIN 12
#define DIODE_THRESHOLD 700

#define Pill_1_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_1_dir 1 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1
#define Pill_2_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_2_dir 0 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1



void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  Serial.begin(9600);

  bool pill_1_state = 0;
  bool pill_2_state = 0;
  
  pill_1_state = dose_Pill(2, 1);
  pill_2_state = dose_Pill(1, 2);

  while(pill_1_state == 1 && pill_2_state == 1){
    if(PIR_movement() != 0)
    {
      buzzer;
    }
    
    if(wzieto_piksa()){
      pill_1_state = 0;
      pill_2_state = 0;
    }
  }

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

bool dose_Pill(int label, int n_pill) {  //8000 imp=360deg
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
        while (analogRead(DIODE) > DIODE_THRESHOLD)
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
        while (analogRead(DIODE) > DIODE_THRESHOLD)
        {
        }

      }
      break;

    default:
      break;
  }
  return 1;
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

bool PIR_movement(void) {
  bool state = 0;
  if (PIR_PIN == HIGH) {
    return 1;
  }
  else
  {
    return 0;
  }



}
