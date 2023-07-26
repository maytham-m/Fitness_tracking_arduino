int bandPin = 0;
int Vo;
float R1 = 10000;
int breathingCapacity;
int breathingRate;
int threshold = 50; // You'll need to adjust this
int prevBreathingCapacity = 0;
unsigned long startTime;
int breathCount = 0;

void setup() {
  Serial.begin(9600);
  startTime = millis();
}

void loop() {
  Vo = analogRead(bandPin);
  breathingCapacity = 635-Vo;

  // Count breaths
  if (prevBreathingCapacity < threshold && breathingCapacity >= threshold) {
    breathCount++;
  }
  prevBreathingCapacity = breathingCapacity;

  // Every minute, calculate and display breathing rate, then reset
  if (millis() - startTime >= 10000) {
    breathingRate = breathCount*6; // breaths per minute
    breathCount = 0;
    startTime = millis();

    Serial.print("Breathing rate: ");
    Serial.print(breathingRate);
    Serial.println();
  }

  Serial.print("Lung Capacity: ");
  Serial.print(breathingCapacity);
  Serial.println();

  delay(500);
}
