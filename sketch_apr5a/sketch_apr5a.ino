#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char keys[ROWS][COLS] = {
  {'A','B','C','D'},
  {'I','J','K','L'},
  {'Q','R','S','T'},
  {'Y','Z','K','L'},
};

char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'Z','Y','M','D'},
};

byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22};
byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30};

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char key;
int dato;
int lello;

char input, input2;
String parola = "";
String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole.txt"
};

String c [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole2.txt"
};

String d [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole3.txt"
};
String a = ""; //parola da indovinare
int nword = 66;
String hidden = "";
int i = 0, tent = 6, generated = 0, z = 0, g = 0, y = 0, scelta = 1;
bool flag = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));  //trovare un'altra soluzione con il millis()
  lcd.init();
  lcd.backlight();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("1 Start");
  lcd.setCursor(0,1);
  lcd.print("2 Difficulty");

  key = firstKpd.getKey();

  while (z == 0) {
    tent = 6;
    hidden = "";
    generated = random(nword);
    switch(scelta){
      case 1:
        a = b[generated];
        break;
      case 2:
        a = c[generated];
        break;
      case 3:
        a = d[generated];
        break;
      default:
        a = b[generated];
        break;
    }
    //a = "CIAQ";
    g = 0;
    Serial.println(a + " | " + generated + " - scelta: " + scelta);
    for (int i = 0; i < a.length(); i++) {
      hidden += "*";
    }
    z = 1;
  }
  
  if(key){
      if(key == 'A'){
      lcd.clear();
      dato = 1;
      while(dato){
      lcd.setCursor(0,0);
      lcd.print("Starting...");
      lcd.setCursor(0,1);
      lcd.print("3 MENU");

      delay(1500);
      lcd.clear();
      lello = 1;
      while(lello){
        key = 0;
        input = firstKpd.getKey();
        input2 = secondKpd.getKey();
        lcd.setCursor(0, 0);
        lcd.print(hidden + " - " + tent);

        if (input) {
          if (flag) {
            i = 0;
          }
          if (i == 0) {
            flag = false;
          }
          lcd.setCursor(i, 1);
          lcd.print(input);
          parola += input;
          int h = 0;
          
          //controllo di ogni lettera inserita
          for (int s = 0; s < a.length(); s++) {
            if (input == a[s]) {
              if (input == parola[s]) {
                Serial.println("RIPROVA");
                //delay(1500);
              }
              hidden[s] = input;
              delay(500);
              g++;
              Serial.println(g);
            } else if (a.charAt(s) != input) {
              h++;
            }
          }
          if (h == a.length()) {
            tent--;
          }
          h = 0;

          //i++;
          delay(500);
          if (tent < 1 || hidden == a) {
            /* if(i == a.length()){ */
            flag = true;

            //check se la parola composta anche non in ordine è corretta
            lcd.clear();
            if (hidden == a) {
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - WIN");
              delay(2000);
              lcd.clear();
              z = 0;
              dato = 0;
              key = 0;
              lello = 0;
            } else {
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - LOST2 ");
              delay(2000);
              lcd.clear();
              z = 0;
              dato = 0;
              key = 0;
              lello = 0;
            }
          }
        }
        if (input2) {
          if (flag) {
            i = 0;
          }
          if (i == 0) {
            flag = false;
          }
          lcd.setCursor(i, 1);
          lcd.print(input2);
          parola += input2;
          int h = 0;
          //controllo di ogni lettera inserita

          for (int s = 0; s < a.length(); s++) {
            if (input2 == a[s]) {
              if (input2 == parola[s]) {
                Serial.println("RIPROVA");
                delay(1500);
              }
              hidden[s] = input2;
              delay(500);
              g++;
              Serial.println(g);
            } else if (a.charAt(s) != input2) {
              h++;
            }
          }
          if (h == a.length()) {
            tent--;
          }
          h = 0;

          //i++;
          delay(500);

          if (tent < 1 || hidden == a) {
            /* if(i == a.length()){ */
            flag = true;

            //check se la parola composta anche non in ordine è corretta
            lcd.clear();
            if (hidden == a) {
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - WIN");
              delay(2000);
              lcd.clear();
              z = 0;
              dato = 0;
              key = 0;
              lello = 0;
            } else {
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - LOST2 ");
              delay(2000);
              lcd.clear();
              z = 0;
              dato = 0;
              key = 0;
              lello = 0;
            }
          }
        }
      }
      key = firstKpd.getKey();
        if(key == 'C'){
          dato = 0;
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Return to MENU");
          delay(2000);
          lcd.clear();
          key = 0;
        }
      }
    }
  }
  if(key == 'B'){
    lcd.clear();
    lello = 1;
    while(lello){
      lcd.setCursor(0, 0);
      lcd.print("1E 2M 3H");
      lcd.setCursor(0, 1);
      lcd.print("4 MENU");
      dato = 1;
      while(dato){
        key = firstKpd.getKey();
        if(key == 'A'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Easy Mode");
          delay(1500);
          lcd.clear();
          dato = 0;
          nword = 66;
          scelta = 1;
          z = 0;
        }
        if(key == 'B'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Medium Mode");
          delay(1500);
          lcd.clear();
          dato = 0;
          nword = 59;
          scelta = 2;
          z = 0;
        }
        if(key == 'C'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Hard Mode");
          delay(1500);
          lcd.clear();
          dato = 0;
          nword = 74;
          scelta = 3;
          z = 0;
        }
        if(key == 'I'){
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Return to MENU");
          delay(2000);
          lcd.clear();
          key = 0;
          dato = 0;
          lello = 0;
        }
      }
    }
  }
}