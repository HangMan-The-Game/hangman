#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char keys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'I','J','K','L'},
  {'Q','R','S','T'},
  {'*','°','#','?'},
};

/* char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'Z','Y','M','D'},
}; */

//* == Selezione | # == Giù | ° == Su

/* byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22}; */
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
/* byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30}; */

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
/* Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS); */
LiquidCrystal_I2C lcd(0x27, 16, 2);

char key;
int dato;
int lello;

char input, input2;
String parola = "";
String b [] = {"PAOLO", "MAMMA", "CAINGERO", "AACC"}; // parole da generare
String a = ""; //parola da indovinare
String hidden = "";
int i = 0, tent = 6, generated = 0;
bool flag = false;

void setup(){
  lcd.init();                      
  lcd.backlight();


  //Game Load
  a = b[generated];

  //Serial.println(a + " | " + generated);

  for(int i = 0; i < a.length(); i++){
    hidden += "*";
  }
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("1 Start");
  lcd.setCursor(0,1);
  lcd.print("2 Difficulty");

  key = firstKpd.getKey();
  
  if(key){
      if(key == '1'){
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
        lcd.setCursor(0, 0);
        lcd.print(hidden);

        if(input){
          if(flag){
            i = 0;
          }
          if(i == 0){
            flag = false;
          }
          lcd.setCursor(i, 1);
          lcd.print(input);
          parola += input;

          //controllo di ogni lettera inserita
          for(int s = 0; s < a.length(); s++){
            if(input == a[s]){
              hidden[s] = input;
              delay(500);
              tent++;
            } else{
              //tent -= 1; // da fixare
            }
          }
          tent++;
          i++;
          delay(500);
          if(hidden == a){
            i = a.length();
            lello = 0;
            dato = 0;
          }
          if(tent == i){
            if(hidden == a){
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " | " + hidden + " - WIN");
              delay(2000);
              lcd.clear();
              lello = 0;
              dato = 0;
            } else{
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " | " + hidden + " - LOST");
              delay(2000);
              lcd.clear();
              lello = 0;
              dato = 0;
            }
          }
          if(i == a.length()){
            flag = true;

            //check se la parola composta anche non in ordine è corretta
            lcd.clear();
            if(hidden == a){
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - WIN");
              delay(2000);
              lcd.clear();
              lello = 0;
              dato = 0;
            } else{
              lcd.setCursor(0, 0);
              lcd.print("HangMan");
              lcd.setCursor(0, 1);
              lcd.print(a + " - LOST");
              delay(2000);
              lcd.clear();
              lello = 0;
              dato = 0;
            }
          }

        }
      }

      key = firstKpd.getKey();
        if(key == '3'){
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
    if(key == '2'){ 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1E 2M 3H");
      lcd.setCursor(0,1);
      lcd.print("4 MENU");
      dato = 1;
      while(dato){
        key = firstKpd.getKey();
        if(key == '1'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Easy Mode");
          lcd.setCursor(0, 1);
          lcd.print("4 MENU");
        }
        if(key == '2'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Medium Mode");
          lcd.setCursor(0, 1);
          lcd.print("4 MENU");
        }
        if(key == '3'){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Hard Mode");
          lcd.setCursor(0, 1);
          lcd.print("4 MENU");
        }
        if(key == '4'){
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