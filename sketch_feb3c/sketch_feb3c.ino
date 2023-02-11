#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char tasti[ROWS][COLS] = {
  {'A', 'B', 'C', 'D'},
  {'E', 'F', 'G', 'H'},
  {'I', 'L', 'M', 'N'},
  {'O', 'P', 'Q', 'R'}
};

char inserimento;

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {3, 2, 1, 0};

Keypad customKeypad = Keypad(makeKeymap(tasti), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
//LiquidCrystal_I2C lcd(0x27, 16, 2);

int tentativi = 0;
//long rand;

String diff1[] = {"cane"};
String diff2[] = {"ciao"};
String diff3[] = {"pane"};

String a = diff1[0];
//String risposta(a.size(), '_');
String parola = "";
char indovina;
int i = 0;

void setup() {
/*   lcd.init();
  lcd.backlight(); */
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  //rand = random(3); //(max = 3)
  inserimento = customKeypad.getKey();
  /* lcd.setCursor(0,0);
  lcd.print("Ciao"); */

  lcd.setCursor(0, 0);
  lcd.print("Inizia");
  lcd.setCursor(0, 1);
  lcd.print("Difficolta");
  if(inserimento){
    if(inserimento == 'A'){
      Serial.println(inserimento);
      ClearScreen();
      lcd.setCursor(0, 0);
      lcd.print("Inizia...");
      delay(1500);
      //Start();
    }
    if(inserimento == 'B'){
      ClearScreen();
      lcd.setCursor(0, 0);
      lcd.print("Scegli Difficolta");
      lcd.setCursor(0, 1);
      lcd.print("1C");
      lcd.setCursor(4, 1);
      lcd.print("2D");
      lcd.setCursor(6, 1);
      lcd.print("3E");

        switch(inserimento){
          case 'C':
            ClearScreen();
            a = diff1[0];
            lcd.print("1C");
            break;
          case 'D':
            ClearScreen();
            a = diff2[0];
            lcd.print("2D");
            break;
          case 'E':
            ClearScreen();
            a = diff3[0];
            lcd.print("3E");
            break;
          default: 
            ClearScreen();
            lcd.setCursor(0, 1);
            lcd.print("Error");
            break;
      }
    }
  }
  //BeginMenu();
}

void BeginMenu(){
  lcd.setCursor(0, 0);
/*   for(int i = 1; i <= 3; i++){
    lcd.print("Begins in " + i + "...");
    delay(1000);
  } */

/*   lcd.print("Iniziando...");
  delay(1000);
  ClearScreen();

  delay(1000); */
  lcd.print("Inizia");
  lcd.setCursor(0, 1);
  lcd.print("Difficolta");
  Serial.print(inserimento);
  if(inserimento == 'A'){
    ClearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Inizia...");
    delay(1500);
    Start();
  }
  if(inserimento == 'B'){
    ClearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Scegli Difficolta");
    lcd.setCursor(0, 1);
    lcd.print("1C");
    lcd.setCursor(4, 1);
    lcd.print("2D");
    lcd.setCursor(6, 1);
    lcd.print("3E");

    switch(inserimento){
      case 'C':
        ClearScreen();
        a = diff1[0];
        lcd.print("1C");
        break;
      case 'D':
        ClearScreen();
        a = diff2[0];
        lcd.print("2D");
        break;
      case 'E':
        ClearScreen();
        a = diff3[0];
        lcd.print("3E");
        break;
      default: 
        ClearScreen();
        lcd.setCursor(0, 1);
        lcd.print("Error");
        break;
    }
  }
}

void ClearScreen(){
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.clear();
}

void Start(){
  ClearScreen();

  lcd.setCursor(0, 0);
  lcd.print("HangMan - ");
  for(int x = 10; x < a.length()+10; x++){
    lcd.setCursor(x, 0);
    lcd.print("*");
  }
  do{
    if(inserimento){
      lcd.setCursor(i, 1);
      Serial.print(inserimento);
      lcd.print(inserimento);
      parola[i] = inserimento;
      i++;
      if(i == a.length()){
        if(parola == a){
          ClearScreen();
          lcd.setCursor(0, 0);
          lcd.print("Hai Vinto");
          lcd.setCursor(0, 1);
          lcd.print(parola);
        } else{
          ClearScreen();
          lcd.setCursor(0, 0);
          lcd.print("Hai Perso");
          lcd.setCursor(0, 1);
          lcd.print(parola);
        }
      }
    }
  }while(parola == a.length());
}