#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MenuBackend.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char keys[ROWS][COLS] = {
  {'1','2','3','D'},
  {'I','J','K','L'},
  {'Q','R','S','T'},
  {'*','°','#','?'},
};

char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'Z','Y','M','D'},
};

//* == Selezione | # == Giù | ° == Su

byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22};
byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30};

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char key;
int dato;

void setup(){
  lcd.init();                      
  lcd.backlight();
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
          lcd.setCursor(0,1);
          lcd.print("Easy Mode");
        }
        if(key == '2'){
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Medium Mode");
        }
        if(key == '3'){
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Hard Mode");
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