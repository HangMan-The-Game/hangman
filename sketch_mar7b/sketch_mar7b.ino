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

byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};

Keypad customKeypad = Keypad(makeKeymap(tasti), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//dichiarazioni

char inserimento;
char pippo = ' ';
String parola = "";
String b [] = {"PANE", "CANE", "GIO", "CIA", "ALE"}; // parole da generare
String a = ""; // parola da indovinare
int i = 0, tentativi = 0, z = 0, generated = 0;

void setup(){
  randomSeed(analogRead(0)); // trovare un'altra soluzione con il millis()
  lcd.init();
  lcd.backlight();
  generated = generaNum(); //variabile a cui viene assegnato il valore in uscita della funzione che genera il numero
}

void loop(){
  inserimento = customKeypad.getKey();
  a = b[generated];
  lcd.setCursor(0, 0);
  lcd.print("HangMan - " + a);
  
//  for(int x = 10; x < a.length()+10; x++){
//    lcd.setCursor(x, 0);
//    lcd.print("*");
//  }
  
  if(inserimento){
    lcd.setCursor(i, 1);
    Serial.print(inserimento);
    lcd.print(inserimento);
    parola += inserimento;
    
/*     for(int y = 0; y < a.length(); y++){
      if(parola[i] == a[y]){
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("giusto! " + parola[i]);
        delay(1500);
      }
    } */

    i++;
    if(i == a.length()){
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print(parola);

      if(parola == a){
        lcd.setCursor(8, 1);       //ULTIMA PARTE 
        lcd.print("WIN");
        delay(2000);
        lcd.clear();
      }
      else{
        lcd.setCursor(8, 1);
        lcd.print("LOST");
        delay(2000);
        lcd.clear();
      }
       
      /* BISOGNA FARE I MENU (MENU INIZIALE, DI GIOCO E MENU DELA SCELTA DELLA DIFFICOLTA')
      BISOGNA FARE I CONTROLLI PER OGNI LETTERA.
      BISOGNA FARE UN CONTROLLO PER CLEARARE LA PARTE INIZIARE Riga(lcd.print("HangMan - " + a);) DI CODICE .
      BISOGNA FARE LA STRINGA[VETTORE] CHE SERVE PER BANNARE LE PAROLE GIA USCITE DALLA STRINGA[VETTORE] CON TUTTE LE PAROLE. */
    
/*       if(parola == a){
        lcd.clear();
        lcd.print("Hai Vinto");
        lcd.setCursor(0, 1);
        lcd.print(parola);
      } 
      else{
        lcd.clear();
        lcd.print("Hai Perso");
        lcd.setCursor(0, 1);
        lcd.print(parola);
      } */
    }

  }
}

int generaNum(){      //funzione che genera un numero a caso seguendo un range 
  int num;
  num = random (0, 4);
  return num;
}