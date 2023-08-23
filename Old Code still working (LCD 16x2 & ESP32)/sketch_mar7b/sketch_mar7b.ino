#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4;  //righe
const byte COLS = 4;  //colonne

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
  {'B','N','M','D'},
};

byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22};
byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30};

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//BISOGNA FARE I MENU (MENU INIZIALE, DI GIOCO E MENU DELA SCELTA DELLA DIFFICOLTA')
//BISOGNA FARE I CONTROLLI PER OGNI LETTERA. (FATTO, MA DA FINIRE)
//BISOGNA FARE LA STRINGA[VETTORE] CHE SERVE PER BANNARE LE PAROLE GIA USCITE DALLA STRINGA[VETTORE] CON TUTTE LE PAROLE.
//METTERE UN BUZZ SUONO BELLO SE HA INDOVINATO LA LETTERA E BRUTTO SE LA SBAGLIA

//dichiarazioni
char input, input2;
String parola = "";
String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole.txt"
};

/* String b[] = {
  "TOVAGLIA", "TAZZA", "TAPPETO", "UOVO", "UVA", "UCCELLO", "VALIGIA", "VETRO", "VASSOIO", "VINO", "VIDEOCAMERA"
}; */

String a = "";  //parola da indovinare
String hidden = "";
int i = 0, tent = 6, generated = 0, z = 0, g = 0, y = 0;
bool flag = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));  //trovare un'altra soluzione con il millis()
  lcd.init();
  lcd.backlight();
}

void loop() {
  input = firstKpd.getKey();
  input2 = secondKpd.getKey();

  while (z == 0) {
    tent = 6;
    hidden = "";
    generated = random(200);
    a = b[generated];
    //a = "CIAQ";
    g = 0;
    Serial.println(a + " | " + generated);
    for (int i = 0; i < a.length(); i++) {
      hidden += "*";
    }
    z = 1;
  }


  lcd.setCursor(0, 0);
  /*   lcd.print("HangMan - ");
  lcd.setCursor(10, 0); */
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
          delay(1500);
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

/*   if(g >= 2) {
    char indovina = firstKpd.getKey();
    lcd.setCursor(0, 1);
    lcd.print("A o B");
    if(indovina) {
      if(indovina == 'A') {
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        parola = "";
        indovina = "";
        while(parola.length() == a.length()){
          if(indovina){
            lcd.setCursor(i, 1);
            parola += indovina;
            i++;
          }
        }
      }
      if(indovina == 'B'){
        y = 1;
      }
    }
  } */

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
      } else {
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

/*   if(g >= 2) {
    char indovina = firstKpd.getKey();
    lcd.setCursor(0, 1);
    lcd.print("A o B");
    if(indovina) {
      if(indovina == 'A') {
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        parola = "";
        indovina = "";
        while(parola.length() == a.length()){
          if(indovina){
            lcd.setCursor(i, 1);
            parola += indovina;
            i++;
          }
        }
      }
      if(indovina == 'B'){
        y = 1;
      }
    }
  } */

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
      } else {
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
}