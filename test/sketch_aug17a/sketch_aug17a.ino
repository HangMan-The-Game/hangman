void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Welcome");
}

void loop() {
  if (Serial1.available() > 0) {
    String receivedString = Serial1.readStringUntil('\n');
    Serial.println(receivedString);
  }
}
