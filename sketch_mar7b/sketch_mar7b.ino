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
String b [] = {"GIOA", "CIAO", "ALE"}; // parole da generare
String a = ""; // parola da indovinare
String nascosta = "";
int i = 0, tentativi = 0, z = 0, generated = 0;

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(0)); // trovare un'altra soluzione con il millis()
  lcd.init();
  lcd.backlight();
  generated = generaNum(); //variabile a cui viene assegnato il valore in uscita della funzione che genera il numero
  
  a = b[generated];

  for(int i = 0; i < a.length(); i++){ //da testare se va o meno, su cpp va
    nascosta += "*";
  }

}

void loop(){
  inserimento = customKeypad.getKey();

  lcd.setCursor(0, 0);
  lcd.print("HangMan - ");
  lcd.setCursor(11, 0);
  lcd.print(nascosta);

/*   for(int x = 10; x < a.length()+10; x++){
    lcd.setCursor(x, 0);
    lcd.print("*");
  } */

  if(inserimento){
    lcd.setCursor(i, 1);
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

    //controllo di ogni lettera inserita
    for(int s = 0; s < a.length(); s++){
      if(inserimento == a[s]){
        nascosta[s] = inserimento;
        Serial.println(nascosta);
        delay(1500);
      } else{
        
      }
    }

    i++;

    if(i == a.length()){
      //check se la parola composta anche non in ordine è corretta
      lcd.clear();
      if(nascosta == a){
        lcd.setCursor(3, 1);
        lcd.print(nascosta + " - WIN");
        delay(2000);
      }
/*       if(parola == a){
        lcd.setCursor(3, 1);
        lcd.print(" - WIN");
        delay(2000);
        lcd.clear();
      }
      else{
        lcd.setCursor(3, 1);
        lcd.print(" - LOST");
        delay(2000);
        lcd.clear();
      } */
       
      //BISOGNA FARE I MENU (MENU INIZIALE, DI GIOCO E MENU DELA SCELTA DELLA DIFFICOLTA')
      //BISOGNA FARE I CONTROLLI PER OGNI LETTERA.
      //BISOGNA FARE UN CONTROLLO PER CLEARARE LA PARTE INIZIARE Riga(lcd.print("HangMan - " + a); DI CODICE .
      //BISOGNA FARE LA STRINGA[VETTORE] CHE SERVE PER BANNARE LE PAROLE GIA USCITE DALLA STRINGA[VETTORE] CON TUTTE LE PAROLE.
    }

  }
}

int generaNum(){      //funzione che genera un numero a caso seguendo un range 
  return  random (0, 3);
}