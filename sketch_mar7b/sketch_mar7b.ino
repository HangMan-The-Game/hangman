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

/* char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'B','N','M','D'},
}; */

byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};
/* byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30}; */

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//BISOGNA FARE I MENU (MENU INIZIALE, DI GIOCO E MENU DELA SCELTA DELLA DIFFICOLTA')
//BISOGNA FARE I CONTROLLI PER OGNI LETTERA. (FATTO, MA DA FINIRE)
//BISOGNA FARE LA STRINGA[VETTORE] CHE SERVE PER BANNARE LE PAROLE GIA USCITE DALLA STRINGA[VETTORE] CON TUTTE LE PAROLE.
//METTERE UN BUZZ SUONO BELLO SE HA INDOVINATO LA LETTERA E BRUTTO SE LA SBAGLIA

//dichiarazioni
char input, input2;
String parola = "";
/* String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole.txt"
}; */

String b [] = {
  "TOVAGLIA", "TAZZA", "TAPPETO", "UOVO", "UVA", "UCCELLO", "VALIGIA", "VETRO", "VASSOIO", "VINO", "VIDEOCAMERA"
};

//String b [] = {"PAOLO", "MAMMA", "CAINGERO", "AACC"}; // parole da generare
String a = ""; //parola da indovinare
String hidden = "";
int i = 0, tent = 6, generated = 0, z = 0, g = 0;
bool flag = false;

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(0)); //trovare un'altra soluzione con il millis()
  lcd.init();
  lcd.backlight();
}

void loop(){
  input = firstKpd.getKey();
  //input2 = secondKpd.getKey();
  
  while(z == 0){
    tent = 6;
    hidden = "";
    generated = random(10);
    //a = b[generated];
    a = "CIAQ";
    Serial.println(a + " | " + generated);
    for(int i = 0; i < a.length(); i++){
      hidden += "*";
    }
    z = 1;
  }

  
  lcd.setCursor(0, 0);
/*   lcd.print("HangMan - ");
  lcd.setCursor(10, 0); */
  lcd.print(hidden + " - " + tent);

/*   for(int x = 10; x < a.length()+10; x++){
    lcd.setCursor(x, 0);
    lcd.print("*");
  } */

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
    int h=0;
    //controllo di ogni lettera inserita
    for(int s = 0; s < a.length(); s++){
      if(input == a[s]){
        hidden[s] = input;
        delay(500);
        g++;
        Serial.println(g);
      }
    else if(a.charAt(s)!=input){
        h++;
      }
    }
    if(h==a.length()){
      tent--;
    }
    h=0;

    i++;
    delay(500);

/*     if(hidden == a){
      lcd.setCursor(0, 0);
      lcd.print("HangMan");
      lcd.setCursor(0, 1);
      lcd.print(a + " - WIN");
      delay(2000);
      lcd.clear();
      z = 0;
    } else{
      lcd.setCursor(0, 0);
      lcd.print("HangMan");
      lcd.setCursor(0, 1);
      lcd.print(a + " - LOST1 ");
      delay(2000);
      lcd.clear();
      z = 0;
    } */

    if(tent < 1 || g == a.length()){
    /* if(i == a.length()){ */
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
        z = 0;
      } else{
        lcd.setCursor(0, 0);
        lcd.print("HangMan");
        lcd.setCursor(0, 1);
        lcd.print(a + " - LOST2 ");
        delay(2000);
        lcd.clear();
        z = 0;
      }
    }

  }
  if(input2){
    if(flag){
      i = 0;
    }
    if(i == 0){
      flag = false;
    }
    lcd.setCursor(i, 1);
    lcd.print(input2);
    parola += input2;

    //controllo di ogni lettera inserita
    for(int s = 0; s < a.length(); s++){
      if(input2 == a[s]){
        hidden[s] = input2;
        delay(500);
        tent++;
      } else{
        //tent -= 1; // da fixare
      }
    }
    i++;
    delay(500);
    if(hidden == a){
      i = a.length();
    }
    if(tent == i){
      if(hidden == a){
        lcd.setCursor(0, 0);
        lcd.print("HangMan");
        lcd.setCursor(0, 1);
        lcd.print(a + " | " + hidden + " - WIN");
        delay(2000);
        lcd.clear();
      } else{
        lcd.setCursor(0, 0);
        lcd.print("HangMan");
        lcd.setCursor(0, 1);
        lcd.print(a + " | " + hidden + " - LOST");
        delay(2000);
        lcd.clear();
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
        z = 0;
      } else{
        lcd.setCursor(0, 0);
        lcd.print("HangMan");
        lcd.setCursor(0, 1);
        lcd.print(a + " - LOST");
        delay(2000);
        lcd.clear();
        z = 0;
      }
    }

  }
}

/*     for(int y = 0; y < a.length(); y++){
      if(parola[i] == a[y]){
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("giusto! " + parola[i]);
        delay(1500);
      }
    } */

    /*       if(tent < 0){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Sei MORTO");
        delay(2000);
      } */

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

int generaNum(){      //funzione che genera un numero a caso seguendo un range 
  return random(0, 199);
}
