// outputs
#define ledPin     13
#define alarmPin   12
#define armedPin   11

// inputs
#define intPin0    2
#define intPin1    3
#define anaPin     A0

// Output values
volatile bool flagArmed = false;
volatile bool flagAlarm = false;

// ADC related variables
const int anaThreshold = 20;
volatile int anaValue = 0;

// Serial related variables
const int buffer_size = 10;
int buffer_i = 0;
char buffer[buffer_size];

// Previous values to compare
volatile bool prevPin0 = false;
volatile bool prevPin1 = false;
volatile int prevAna = 0;

void checkIntDiferences() {
  if ((prevPin0 =! digitalRead(intPin0)) ||
  (prevPin1 =! digitalRead(intPin1))) {
    prevPin0 = digitalRead(intPin0);
    prevPin1 = digitalRead(intPin1);
    if (flagArmed == true) flagAlarm = true;
  }
}

void checkAnaDiferences() {
  anaValue = analogRead(anaPin);
  int anaDiference = anaValue - prevAna;
  if (abs(anaDiference) > anaThreshold) {
    prevAna = anaValue;
    if (flagArmed) flagAlarm = true;
  }
}

void processComand (char c) {
  buffer[buffer_i] = c;
  buffer_i++;
  if (buffer_i > buffer_size) buffer_i=0;
  if ((buffer_i >= 2) && ((c == '\n') || (c == '\r'))) {
    buffer[buffer_i] = '\0';
    if (memcmp("arm",    buffer, 3)==0) flagArmed=true;
    if (memcmp("disarm", buffer, 3)==0) flagArmed=false;
    buffer_i=0;
    for(int i=0; i<buffer_size; i++) buffer[i]=0; 
  }

}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(armedPin, OUTPUT);
  pinMode(intPin0, INPUT_PULLUP);
  pinMode(intPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin0), checkIntDiferences, CHANGE);
  attachInterrupt(digitalPinToInterrupt(intPin1), checkIntDiferences, CHANGE);
}

void loop() {
  digitalWrite(armedPin, flagArmed);
  digitalWrite(alarmPin, flagAlarm);
  checkAnaDiferences();
  while (Serial.available()) {
    processCommand(Serial.read());
  }
}
