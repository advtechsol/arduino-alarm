// outputs
#define ledPin     13
#define alarmPin   12
#define armedPin   11

// inputs
#define intPin0    2
#define intPin1    3
#define anaPin     A0


volatile bool flagArmed = false;
volatile bool flagAlarm = false;
const int anaThreshold = 20;
volatile int anaValue = 0;

void analogMonitor() {
  anaValue = analogRead(anaPin);
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(armedPin, OUTPUT);
  pinMode(intPin0, INPUT_PULLUP);
  pinMode(intPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin0), activateAlarm, CHANGE);
  attachInterrupt(digitalPinToInterrupt(intPin1), activateAlarm, CHANGE);
}

void loop()
{
  
}
