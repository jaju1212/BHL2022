
#include <WiFiNINA.h>
#include "mario.h"
#include "secrets.h"
#include <ThingSpeak.h>

#define STEP_PIN 9
#define DIR_PIN 10
#define Pill_1 1 //indeks pigułki
#define Pill_2 23
#define PILL_TAKEN_PIN 12 //krańcówka
#define REMINDER_DIODE 3 //dioda przypominająca
//buzzer pin 6

#define DIODE A0 //fotorezystor
#define PIR_PIN 12
#define DIODE_THRESHOLD 920 //próg ADC wykrywania spadającej pigułki

#define Pill_1_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_1_dir 1 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1
#define Pill_2_deg 150 //kąt o ktory ma się obrocic wał silnika, aby pobrac tabletke z dozownika 1
#define Pill_2_dir 0 //kierunek w ktorym ma sie obrocic wal silnika, aby pobrac tabletke 1

//Kanały Thingspeak
#define PILL_STATE_CHANNEL 7
#define PILL_1_1_FIELD 1
#define PILL_1_2_FIELD 3
#define PILL_1_3_FIELD 5

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

int DEMO = 1;

int krancowka_state = 0;

unsigned long ch_num = 1699255;
const char * APIwrite = SECRET_WRITE_APIKEY;
const char * APIread = SECRET_READ_APIKEY;

int status = WL_IDLE_STATUS;

int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / (int)tempo;

int divider = 0, noteDuration = 0;

volatile int ISR_flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(REMINDER_DIODE, OUTPUT);
  reminderDiodeState(0);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PILL_TAKEN_PIN, INPUT_PULLUP);

//    WIFISetup();

  attachInterrupt(digitalPinToInterrupt(PILL_TAKEN_PIN), ISR_handler, CHANGE);

//   delay(15000);


  //prezentacja
  if (DEMO == 1) {
    two_pills_dosing(2, 1);
    delay(5000);
    two_pills_dosing(0, 1);
    delay(5000);
    two_pills_dosing(1, 0);
  }

}


void loop() {
  
//  int val = ThingDownload(2);
//  Serial.println(val);
//  delay(1000);
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

        stepperShake();
        
        digitalWrite(DIR_PIN, HIGH);


        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }

        stepperShake();

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

        stepperShake();
        
        digitalWrite(DIR_PIN, LOW);

        for (int i = 0; i < 3333; i++)
        {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(100);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(100);
        }

        stepperShake();

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

void ISR_handler(void) {
  ISR_flag = 1;

  krancowka_state = digitalRead(PILL_TAKEN_PIN);
  Serial.println(krancowka_state);

}

int ThingSend(int field, int value) {
  ThingSpeak.setField(field, value);

  int x = ThingSpeak.writeFields(ch_num, APIwrite);

  if (x == 200) {
    Serial.println("data sent");
    return true;
  }
  else
  {
    Serial.println("error while sending data");
    Serial.println(x);
    return false;
  }

}

//void pillTaken()
//void dose_Pill_2(int n_pil) {  //8000 imp=360deg
//  for (int j = 0; j < n_pill; j++)
//  {
//    digitalWrite(DIR_PIN, LOW);
//
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
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
  {
    if (ISR_flag == 0) {

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

void two_pills_dosing(int pill_1_quantity, int pill_2_quantity) {
  bool pill_1_state = 0;
  bool pill_2_state = 0;

  pill_1_state = dose_Pill(1, pill_1_quantity);
  pill_2_state = dose_Pill(2, pill_2_quantity);

  //  ThingSend(PILL_STATE_CHANNEL, pill_1_state);

  while (pill_1_state == 1 && pill_2_state == 1) {
    //dopóki pigułki nie zostały zabrane z koszyczka
    playMario();


    reminderDiodeState(1);

    if (ISR_flag == 1) {
      pill_1_state = 0;
      pill_2_state = 0;
      ThingSend(PILL_STATE_CHANNEL, pill_1_state);
      reminderDiodeState(0);
      ISR_flag = 0;
    }


  }
}

void WIFISetup(void) {
  ThingSpeak.begin(client);

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Lacze sie z siecia ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    Serial.println(WiFi.status());
    delay(5000);

  }
  Serial.println("\nConnected.");


  delay(5000);
  ThingSpeak.setStatus("");
}

void stepperShake(void) {
  //potrząsanie
  int state = 0;
  for (int j = 0; j < 2; j++) {
    state = !state;
    digitalWrite(DIR_PIN, state);
    for (int i = 0; i < 44; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(10);

    }
  }
}

int ThingDownload(int field){
  int val = ThingSpeak.readIntField(ch_num, field);
  return val;    
}
