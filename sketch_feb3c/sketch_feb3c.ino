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

char inserimento, difficolta;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(tasti), rowPins, colPins, ROWS, COLS);

//LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int tentativi = 0;
//long rand;

String diff1[] = {"cane"};
String diff2[] = {"ciao"};
String diff3[] = {"pane"};

String a = diff1[0];
//String risposta(a.size(), '_');
String parola = "";
char indovina;
int i = 0, c = 0, z;
bool ciccio = false;

void setup() {
  lcd.init();
  lcd.backlight();
  //lcd.begin(16, 2);
  Serial.begin(9600);
  c = 0;
  //randomSeed(analogRead(0));
  customKeypad.addEventListener(change);
}

void loop() {
  //rand = random(3);
  inserimento = customKeypad.getKey();
  delay(100);
/*   lcd.setCursor(0, 0);
  lcd.print("MICH");
  Serial.print(customKeypad.); */

  menuInit();

  if(inserimento){
    if(inserimento == 'A'){
      Serial.println(inserimento);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iniziando...");
      delay(1500);
      Start();
      while(ciccio){}
    }
    if(inserimento == 'B'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Difficolta");
      lcd.setCursor(0, 1);
      lcd.print("1C");
      lcd.setCursor(3, 1);
      lcd.print("2D");
      lcd.setCursor(6, 1);
      lcd.print("3E");
      menuDiff();
      }
      while(ciccio){}
    }
}

void change(){
  ciccio = !ciccio;
}

void menuInit(){
  lcd.setCursor(0, 0);
  lcd.print("Inizia");
  lcd.setCursor(0, 1);
  lcd.print("Difficolta");
  c++;
  if(c <= 1){
    lcd.setCursor(0, 1);
    lcd.clear();
  }
}

void menuDiff(){

  z = 0;

  do{
    char diff = customKeypad.getKey();
    delay (100);

    if(diff == 'C'){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("1C");
      a = diff1[0];
      delay(1000);
      z++;
    }

    if(diff == 'D'){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("2C");
      a = diff2[0];
      delay(1000);
      z++;
    }

    if(diff == 'E'){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("3E");
      a = diff3[0];
      delay(1000);
      z++;
    }
   
  }while(z<1);

}

void Start(){
  lcd.clear();

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
          lcd.clear();
          lcd.print("Hai Vinto");
          lcd.setCursor(0, 1);
          lcd.print(parola);
          delay(1500);
        } else{
          lcd.clear();
          lcd.print("Hai Perso");
          lcd.setCursor(0, 1);
          lcd.print(parola);
          delay(1500);
        }
      }
    }
      
      while(true){
        if(customKeypad.keyStateChanged()){
          break;
        }
      }
  }while(i <= a.length());
}