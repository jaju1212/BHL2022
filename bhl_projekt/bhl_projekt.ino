#include<Servo.h>
#include <WiFiNINA.h>
//#include <ThingSpeak.h>
#include "mario.h"

#define STEP_PIN 9
#define DIR_PIN 10
#define Pill_1 1 //indeks pigułki
#define Pill_2 2
#define PILL_TAKEN_PIN 12 //krańcówka
#define REMINDER_DIODE 3 //dioda przypominająca

#define DIODE A0 //fotorezystor
#define PIR_PIN 12
#define DIODE_THRESHOLD 700 //próg ADC wykrywania spadającej pigułki

#define Pill_1_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_1_dir 1 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1
#define Pill_2_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_2_dir 0 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1

char ssid[] = "Ecuador";
char pass[] = "slodkiekotki69";

// change this to make the song slower or faster
int tempo = 200;

// change this to whichever pin you want to use
int buzzer = 6;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
//int melody[] = {

int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / (int)tempo;

int divider = 0, noteDuration = 0;


volatile int ISR_flag = 0;

//int status = WL_IDLE_STATUS;
//WiFiClient client;


void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(REMINDER_DIODE, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  
  pinMode(PILL_TAKEN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PILL_TAKEN_PIN), ISR_handler, LOW);
  
  
//
//  while (status != WL_CONNECTED)
//  {
//    Serial.print("Lacze sie z siecia");
//    Serial.println(ssid);
//    status = WiFi.begin(ssid, pass);
//    delay(5000);
//  }
//  ThingSpeak.begin(client);

  Serial.begin(9600);

  bool pill_1_state = 0;
  bool pill_2_state = 0;
  
  pill_1_state = dose_Pill(0, 0);
  pill_2_state = dose_Pill(0, 0);

  while(pill_1_state == 1 && pill_2_state == 1){
    
      playMario();
      
      reminderDiodeState(1);
    
    
    if(checkIfTaken() != 0){
      pill_1_state = 0;
      pill_2_state = 0;
    }
  }
}

void loop() {
  
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

void ISR_handler(void){
  ISR_flag = 1;
}

//void pillTaken()
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

void playMario()
{
    // actualTime = millis();

    // if (actualTime - temp_time > tempo)
    // {

        // iterate over the notes of the melody.
        // Remember, the array is twice the number of notes (notes + durations)
        for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
        {
           if(ISR_flag == 0){

            // calculates the duration of each note
            divider = melody[thisNote + 1];
            if (divider > 0)
            {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
            }
            else if (divider < 0)
            {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
            }

            // we only play the note for 90% of the duration, leaving 10% as a pause
            tone(buzzer, melody[thisNote], noteDuration * 0.9);

            // Wait for the specief duration before playing the next note.
            delay(tempo);

            // stop the waveform generation before the next note.
            noTone(buzzer);
           }
           else
           {
               return;
           }
        }
}
