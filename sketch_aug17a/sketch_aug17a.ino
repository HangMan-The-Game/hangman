void setup() {
  Serial.begin(9600);
  Serial.println("Welcome");
}

void loop() {
  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n');
    Serial.println(receivedString);
  }
}
