void setup() {
  Serial.begin(9600);

  pinMode(0, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);

  digitalWrite(0, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);
}

void loop() {
  int val = 0;
  if (Serial.available() > 0) {
    bool flag = false;
    char receivedString = Serial.readString().charAt(0);
    Serial.println(receivedString);
    // if(receivedString == '1'){
    //   Serial.println("on if");
    //   digitalWrite(22, HIGH);
    // } else{
    //   Serial.println("ciao");
    // } if(receivedString == '0'){
    //   Serial.println("off if");
    //   digitalWrite(22, LOW);
    // }
    if(receivedString == '1'){
      flag = true;
      val = 5;
      digitalWrite(27, LOW);
    }
    if(receivedString == '2'){
      flag = true;
      val = 4;
      digitalWrite(12, LOW);
    }
    if(receivedString == '3'){
      flag = true;
      val = 3;
      digitalWrite(13, LOW);
    }
    if(receivedString == '4'){
      flag = true;
      val = 2;
      digitalWrite(21, LOW);
    }
    if(receivedString == '5'){
      flag = true;
      val = 1;
      digitalWrite(0, LOW);
    }
    if(receivedString == '6'){
      flag = true;
      val = 1;
      digitalWrite(26, LOW);
    }

    if(receivedString == '9'){
      digitalWrite(0, HIGH);
      digitalWrite(21, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(27, HIGH);
      digitalWrite(26, HIGH);
    }
  
    // if(flag && val > 0){
    //   for(int i = 0; i < val; i++){
    //     digitalWrite(22, HIGH);
    //     delay(500);
    //     digitalWrite(22, LOW);
    //     delay(500);
    //   }
    // }
  }
}

/* void loop() {
  digitalWrite(0, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  digitalWrite(21, LOW);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(500);
} */