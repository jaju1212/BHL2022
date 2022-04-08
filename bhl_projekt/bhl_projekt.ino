
#define PIN A0
#define PILL_TAKEN_PIN 12
#define REMINDER_DIODE 10


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
}

void loop() {
  int adc = digitalRead(PIN);
  delay(100);
  Serial.println(adc);
  
  
}


bool checkIfTaken(void){
  bool val = digitalRead(PILL_TAKEN_PIN);
  return val;
}

void reminderDiode(void){
  
  
}
