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
  ClearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Inizia");
  lcd.setCursor(0, 1);
  lcd.print("Difficolta");
  if(inserimento){
    if(inserimento == 'A'){
      Serial.println(inserimento);
      ClearScreen();
      lcd.setCursor(0, 0);
      lcd.print("Iniziando...");
      delay(1500);
      break;
      //Start();
    }
    if(inserimento == 'B'){
      ClearScreen();
      lcd.setCursor(0, 0);
      lcd.print("Scegli Difficolta");
      lcd.setCursor(0, 1);
      lcd.print("1C");
      lcd.setCursor(3, 1);
      lcd.print("2D");
      lcd.setCursor(6, 1);
      lcd.print("3E");

      switch(inserimento){
        case 'C':
          ClearScreen();
          a = diff1[0];
          lcd.print("1C");
          delay(1500);
          break;
        case 'D':
          ClearScreen();
          a = diff2[0];
          lcd.print("2D");
          delay(1500);
          break;
        case 'E':
          ClearScreen();
          a = diff3[0];
          lcd.print("3E");
          delay(1500);
          break;
        default: 
          ClearScreen();
          lcd.setCursor(0, 1);
          lcd.print("Error");
          delay(1500);
          break;
      }
    }
  }
  //BeginMenu();
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
      Serial.println("Inserito: " + inserimento);
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
          delay(1500);
        } else{
          ClearScreen();
          lcd.setCursor(0, 0);
          lcd.print("Hai Perso");
          lcd.setCursor(0, 1);
          lcd.print(parola);
          delay(1500);
        }
      }
    }
  }while(i <= a.length());
}