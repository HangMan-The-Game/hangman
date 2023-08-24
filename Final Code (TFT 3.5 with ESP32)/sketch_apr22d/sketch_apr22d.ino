#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include <Keypad.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char keys[ROWS][COLS] = {
  {'A','B','C','D'},
  {'I','J','K','L'},
  {'Q','R','S','T'},
  {'Y','Z','Z','='},
};

char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'Z','^','?','|'},
};

byte rowPins[ROWS] = {29, 28, 27, 26};
byte colPins[COLS] = {25, 24, 23, 22};
byte rowPins2[ROWS] = {37, 36, 35, 34};
byte colPins2[COLS] = {33, 32, 31, 30};

Keypad firstKpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad secondKpd = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS, COLS);

char input, input2;

#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

/* #include <UTFTGLUE.h>
UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0); */

const int XP = 6, XM = A2, YP = A1, YM = 7;  //ID=0x9486
const int TS_LEFT = 948, TS_RT = 233, TS_TOP = 139, TS_BOT = 921;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
MCUFRIEND_kbv tft;
Adafruit_GFX_Button start, diff, easy, med, hard, backhome, vs;

String menu = "home";
int mode = 1;

String ndiff = "Easy";

String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word.txt"
};
String c [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word2.txt"
};
String d [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word3.txt"
};

String a = ""; //parola da indovinare
String consiglio = ""; // consiglio da indovinare
String parolaRicevuta = "", consiglioRicevuto = "";
int nword = 50, randomized = 0;
String hidden = "", wrong = "", beforeguess = "";
String prendi;
int i = 0, tent = 6, generated = 0, z = 0, g = 0, y = 0, scelta = 1, lello = 0, mamma = 0, tenterrato = 0;
bool flag = false, guessed = false, guessmode = false, guessmodewrong = false;

bool nuoveStringheDisponibili = false;

unsigned long previousMillis = 0;
const unsigned long interval = 1500;

//BUZZ SECTION

int speakerPin = 38;

int pixel_x, pixel_y;  //Touch_getXY() updates global vars
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, tft.width());  //.kbv makes sense to me
    pixel_y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

void setup() {

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);

  digitalWrite(39, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);

  pinMode(speakerPin, OUTPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial1.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.setCursor(185, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.print("HangMan");
  tft.setCursor(185, 100);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Medium");
  } else if(mode == 3){
    tft.print("- Hard");
  }

  start.initButton(&tft, 90, 110, 140, 40, WHITE, BLUE, WHITE, "START", 4);
  diff.initButton(&tft, 90, 160, 140, 40, WHITE, GREEN, WHITE, "MODE", 4);
  vs.initButton(&tft, 90, 210, 140, 40, WHITE, RED, WHITE, "1vs1", 4);

  start.drawButton(false);
  diff.drawButton(false);
  vs.drawButton(false);
}

// Array of button addresses to behave like a list
Adafruit_GFX_Button *buttons[] = {
  &start, &diff, &easy, &med, &hard, &backhome, &vs, NULL
  };

bool update_button(Adafruit_GFX_Button *b, bool down) {
  b->press(down && b->contains(pixel_x, pixel_y));
  if (b->justReleased()) {
    b->drawButton(false);
  }
  if (b->justPressed()) {
    b->drawButton(true);
  }
  return down;
}

bool update_button_list(Adafruit_GFX_Button **pb) {
  bool down = Touch_getXY();
  for (int i = 0; pb[i] != NULL; i++) {
    update_button(pb[i], down);
  }
  return down;
}

void loop() {
  randomized = random(nword);
  update_button_list(buttons);  //use helper function
  for (int i = 0; buttons[i] != NULL; i++) {
    if (buttons[i]->isPressed()) {
      Serial.println(i);
      buttons[i]->drawButton(true);
    }
    if (buttons[i]->justReleased()) {
      buttons[i]->drawButton(false);
    }
    if(buttons[0]->isPressed()){
      menu = "start";
      if (!parolaRicevuta.length() == 0 && !consiglioRicevuto.length() == 0){
        startgame();
        parolaRicevuta = "";
        consiglioRicevuto = "";
      }
    }
    if(buttons[1]->isPressed()){
      menu = "diff";
      difficolta();
    }
    if(buttons[2]->isPressed()){
      mode = 1;
    }
    if(buttons[3]->isPressed()){
      mode = 2;

      Serial.println("Parola Ricevuta: " + parolaRicevuta);
      Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
    }
    if(buttons[4]->isPressed()){
      mode = 3;
    }
    if(buttons[5]->isPressed()){
      menu = "home";

      home();
    }
    if(buttons[6]->isPressed()){
      menu = "1vs1";

      versus();
    }
  }


    // Serial.println("yo millis");

    /* while (Serial1.available()) {
      Serial1.read();
    } */

    if (Serial1.available() > 0) {
      String receivedString = Serial1.readStringUntil('\n');
      receivedString.trim();
      // Serial.println(receivedString);
      if (parolaRicevuta.length() == 0) {
        parolaRicevuta = receivedString;
      } else if (consiglioRicevuto.length() == 0) {
        consiglioRicevuto = receivedString;
        Serial.println("Parola Ricevuta: " + parolaRicevuta);
        Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
        nuoveStringheDisponibili = true;

        /* parolaRicevuta = "";
        consiglioRicevuto = ""; */
      }
    }
  


  /* if (Serial1.available() > 0) {
    String receivedString = Serial1.readStringUntil('\n');
    receivedString.trim();
    if (parolaRicevuta.length() == 0) {
      parolaRicevuta = receivedString;
    } else if (consiglioRicevuto.length() == 0) {
      consiglioRicevuto = receivedString;
      Serial.println("Parola Ricevuta: " + parolaRicevuta);
      Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
      nuoveStringheDisponibili = true;
    }
  } */

/*   static String buffer = "";

  while (Serial1.available() > 0) {
    char c = Serial1.read();
    if (c == '\n') {
      buffer.trim();
      if (parolaRicevuta.length() > 0) {
        if (consiglioRicevuto.length() == 0) {
          consiglioRicevuto = buffer;
          Serial.println("Parola Ricevuta: " + parolaRicevuta);
          Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
          nuoveStringheDisponibili = true;
        }
      } else {
        parolaRicevuta = buffer;
      }
      buffer = "";
    } else {
      buffer += c;
    }
  } */

/*   if (Serial1.available() > 0) {
    String ricevuto = Serial1.readStringUntil('\n');
    ricevuto.trim();
    
    Serial.println("Ricevuto: [" + ricevuto + "]");

    int pos = ricevuto.lastIndexOf('\n');
    Serial.println("Posizione del carattere '\\n': " + String(pos));

    if (pos != -1) {
      parolaRicevuta = ricevuto.substring(0, pos);
      consiglioRicevuto = ricevuto.substring(pos + 1);
      
      Serial.println("Parola Ricevuta: " + parolaRicevuta);
      Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);

      nuoveStringheDisponibili = true;
    }
  } */

  /* Serial.println(menu + " mode: " + mode);
  Serial.println(a + " | " + generated + " - scelta: " + mode); */
}

void home(){
  easy.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  med.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  hard.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  tft.fillScreen(BLACK);

  tft.setCursor(185, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.print("HangMan");
  tft.setCursor(185, 100);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Medium");
  } else if(mode == 3){
    tft.print("- Hard");
  }

  start.initButton(&tft, 90, 110, 140, 40, WHITE, BLUE, WHITE, "START", 4);
  diff.initButton(&tft, 90, 160, 140, 40, WHITE, GREEN, WHITE, "MODE", 4);
  vs.initButton(&tft, 90, 210, 140, 40, WHITE, RED, WHITE, "1vs1", 4);

  start.drawButton(false);
  diff.drawButton(false);
  vs.drawButton(false);
  update_button_list(buttons);  //use helper function
  for (int i = 0; buttons[i] != NULL; i++) {
    if (buttons[i]->isPressed()) {
      Serial.println(i);
      buttons[i]->drawButton(true);
    }
    if (buttons[i]->justReleased()) {
      buttons[i]->drawButton(false);
    }
  }
}

void startgame(){
  Serial.println(a + " " + consiglio);
  Serial.println("Parola Ricevuta: " + parolaRicevuta);
  Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
  while (z == 0) {
    Serial.println("RESET");
    tent = 6;
    tenterrato = 0;
    hidden = "";
    wrong = "";
    lello = 1;
    guessed = false;
    generated = randomized;
    switch(mode){
      case 1:
        // a = "BICICLETTA";
        if (nuoveStringheDisponibili) {
          a = parolaRicevuta; 
          consiglio = consiglioRicevuto; 
          nuoveStringheDisponibili = false;
        }
        // a = b[generated];
        //a = "CIAQ";
        ndiff = "Easy";
        break;
      case 2:
        if (nuoveStringheDisponibili) {
          a = parolaRicevuta; 
          consiglio = consiglioRicevuto; 
          nuoveStringheDisponibili = false;
        }
        // a = c[generated];
        //a = "CIAB";
        ndiff = "Medium";
        break;
      case 3:
        if (nuoveStringheDisponibili) {
          a = parolaRicevuta; 
          consiglio = consiglioRicevuto; 
          nuoveStringheDisponibili = false;
        }
        // a = d[generated];
        //a = "CIIAJ";
        ndiff = "Hard";
        break;
      default:
        a = "TOMATO";
        break;
    }
    g = 0;
    Serial.println(a + " | " + generated + " - scelta: " + mode + " - lunghezza: " + a.length());
    for (int i = 0; i < a.length(); i++) {
      hidden += "_";
    }
    z = 1;
  }

  if(tenterrato == 1){
    tent = 1;
    tenterrato = 0;
  }

  tft.fillScreen(BLACK);

  tft.setCursor(20, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print(ndiff + " Mode");
  //Serial.println(randomized);

  tft.setCursor(20, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      if (flag) {
        i = 0;
      }
      if (i == 0) {
        flag = false;
      }
      int h = 0;
    
      bool is_present = false, is_wrong = false;

      for (int s = 0; s < a.length(); s++) {
        if (input == a[s]) {
          hidden[s] = input;
          delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(BLUE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println("Lettere indovinate: " + String(g));
          is_present = true;
        } else if (a.charAt(s) != input){
            h++;
          }
      }
      if (!is_present) {
        for (int i = 0; i < wrong.length(); i++) {
          if (input == wrong[i]) {
            is_wrong = true;
          }
        }
        
        if (!is_wrong) {
          if(input == wrong[i]){
            is_wrong = true;
          } else{
              wrong += input;
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;

                if(((mode == 2) || (mode == 3)) && (tent == 2)){
                  tft.setCursor(20, 250);
                  tft.setTextColor(WHITE);
                  tft.setTextSize(2);
                  tft.print(consiglio);
                }

                int lunghezzaPar = a.length();
                if((g <= lunghezzaPar-3) && (mode == 1) && (tent == 2)){
                  int randomico;

                  do {
                    randomico = random(lunghezzaPar);
                  } while (hidden[randomico] != '_');

                  char lettPresa;
                  int lettCont = 0;

                  Serial.println("Lunghezza " + String(lunghezzaPar));
                  Serial.println("Random " + String(randomico));

                  char letteraDaSostituire = a[randomico];
                  bool sostituita = false;
              
                  for (int c = 0; c < lunghezzaPar; c++) {
                    if (letteraDaSostituire == a[c] && hidden[c] == '_') {
                      hidden[c] = letteraDaSostituire;
                      sostituita = true;
                    }
                  }

                  if (!sostituita) {
                    hidden[randomico] = letteraDaSostituire;
                  }

                  // hidden[randomico] = a[randomico];
                }
                
                // Serial1.println(tent);
                tone(speakerPin, 500);
                delay(1000);
                noTone(speakerPin);
              }
          }
        }
      }

      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      tft.setCursor(450, 10);
      tft.setTextSize(4);
      tft.print(tent);
      tft.setCursor(20, 180);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(4);
      tft.print(wrong);
      
      h = 0;

      //i++;
      delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON"); 
          delay(1000);

          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 240);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }

    bool special = false;

    if (input2) {
      if(input2 == '?'){
        beforeguess = hidden;
        guess();
      }
      if(input2 == '|'){
        lello = 0;
        z = 0;
        special = true;
      }
      if (flag) {
        i = 0;
      }
      if (i == 0) {
        flag = false;
      }
      int h = 0;
      
      bool is_present = false, is_wrong = false;

      for (int s = 0; s < a.length(); s++) {
        if (input2 == a[s]) {
          hidden[s] = input2;
          delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(BLUE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println("Lettere indovinate: " + String(g));
          is_present = true;
        } else if (a.charAt(s) != input2){
            h++;
          }
      }
      if (!is_present) {
        for (int i = 0; i < wrong.length(); i++) {
          if (input2 == wrong[i]) {
            is_wrong = true;
          }
        }
        if (!is_wrong) {
          if(input2 == wrong[i]){
            is_wrong = true;
          } else{
            if(special){
              break;
            }else{
              wrong += input2;
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;

                if(((mode == 2) || (mode == 3)) && (tent == 3)){
                  tft.setCursor(20, 250);
                  tft.setTextColor(WHITE);
                  tft.setTextSize(2);
                  tft.print(consiglio);
                }

                int lunghezzaPar = a.length();

                if((g <= lunghezzaPar-3) && (mode == 1) && (tent == 2)){
                  int randomico;
                  
                  do {
                    randomico = random(lunghezzaPar);
                  } while (hidden[randomico] != '_');

                  char lettPresa;
                  int lettCont = 0;

                  Serial.println("Lunghezza " + String(lunghezzaPar));
                  Serial.println("Random " + String(randomico));

                  char letteraDaSostituire = a[randomico];
                  bool sostituita = false;
              
                  for (int c = 0; c < lunghezzaPar; c++) {
                    if (letteraDaSostituire == a[c] && hidden[c] == '_') {
                      hidden[c] = letteraDaSostituire;
                      sostituita = true;
                    }
                  }

                  if (!sostituita) {
                    hidden[randomico] = letteraDaSostituire;
                  }

                  // hidden[randomico] = a[randomico];
                }

                // Serial1.println(tent);
                tone(speakerPin, 500);
                delay(1000);
                noTone(speakerPin);
              }
            }
          }
        }
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      tft.setCursor(450, 10);
      tft.setTextSize(4);
      tft.print(tent);
      tft.setCursor(20, 180);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(4);
      tft.print(wrong);
      
      h = 0;

      //i++;
      delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 240);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }
  }

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 390, 290, 120, 40, WHITE, BLUE, WHITE, "HOME", 4);
  backhome.drawButton(false);

  update_button_list(buttons);
  if (buttons[5]->isPressed()) {
    Serial.println(5);
    buttons[5]->drawButton(true);
  } 
  if (buttons[5]->justReleased()) {
    buttons[5]->drawButton(false);
  }
}

void versus(){
  ndiff = "1vs1";
  mamma = 0;
  a = "";
  tft.fillScreen(BLACK);

  tft.setCursor(20, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(RED);
  tft.setTextSize(3);

  tft.print(ndiff + " Mode");

  tft.setCursor(16, 50);
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.print("Input Mode");

  tft.setCursor(16, 120);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  tft.print(a);
  // tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  while(mamma == 0){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      a += input;
      delay(500);
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(a);
    }
    if (input2) {
      if(input2 == '^'){
        mamma = 1;
      } else {
        a += input2;
      }
      delay(500);
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(a);
    }
  }

  while (z == 0) {
    Serial.println("RESET");
    tent = 6;
    // Serial1.println("9");
    hidden = "";
    wrong = "";
    lello = 1;
    guessed = false;
    g = 0;
    Serial.println(a + " | " + generated + " - scelta: " + mode);
    for (int i = 0; i < a.length(); i++) {
      hidden += "_";
    }
    z = 1;
  }

  tft.fillScreen(BLACK);

  tft.setCursor(20, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print("1vs1 Mode");

  tft.setCursor(20, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  // tft.print(a);
  tft.print(hidden);

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      if (flag) {
        i = 0;
      }
      if (i == 0) {
        flag = false;
      }
      int h = 0;
    
      bool is_present = false, is_wrong = false;

      for (int s = 0; s < a.length(); s++) {
        if (input == a[s]) {
          hidden[s] = input;
          delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(BLUE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println(g);
          is_present = true;
        } else if (a.charAt(s) != input){
            h++;
          }
      }
      if (!is_present) {
        for (int i = 0; i < wrong.length(); i++) {
          if (input == wrong[i]) {
            is_wrong = true;
          }
        }
        
        if (!is_wrong) {
          if(input == wrong[i]){
            is_wrong = true;
          } else{
              wrong += input;
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                // Serial1.println(tent);
                tone(speakerPin, 500);
                delay(1000);
                noTone(speakerPin);
              }
          }
        }
      }

      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      tft.setCursor(450, 10);
      tft.setTextSize(4);
      tft.print(tent);
      tft.setCursor(20, 180);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(4);
      tft.print(wrong);
      
      h = 0;

      //i++;
      delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON"); 
          delay(1000);

          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 240);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }

    bool special = false;

    if (input2) {
      if(input2 == '?'){
        beforeguess = hidden;
        guess();
      }
      if(input2 == '|'){
        lello = 0;
        z = 0;
        special = true;
      }
      if (flag) {
        i = 0;
      }
      if (i == 0) {
        flag = false;
      }
      int h = 0;
      
      bool is_present = false, is_wrong = false;

      for (int s = 0; s < a.length(); s++) {
        if (input2 == a[s]) {
          hidden[s] = input2;
          delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(BLUE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println(g);
          is_present = true;
        } else if (a.charAt(s) != input2){
            h++;
          }
      }
      if (!is_present) {
        for (int i = 0; i < wrong.length(); i++) {
          if (input2 == wrong[i]) {
            is_wrong = true;
          }
        }
        if (!is_wrong) {
          if(input2 == wrong[i]){
            is_wrong = true;
          } else{
            if(special){
              break;
            }else{
              wrong += input2;
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                // Serial1.println(tent);
                tone(speakerPin, 500);
                delay(1000);
                noTone(speakerPin);
              }
            }
          }
        }
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      tft.setCursor(450, 10);
      tft.setTextSize(4);
      tft.print(tent);
      tft.setCursor(20, 180);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(4);
      tft.print(wrong);
      
      h = 0;

      //i++;
      delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 240);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }
  }

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 390, 290, 120, 40, WHITE, BLUE, WHITE, "HOME", 4);
  backhome.drawButton(false);

  update_button_list(buttons);
  if (buttons[5]->isPressed()) {
    Serial.println(5);
    buttons[5]->drawButton(true);
  } 
  if (buttons[5]->justReleased()) {
    buttons[5]->drawButton(false);
  }
}

void guess(){
  guessmode = true;
  lello = 1;
  tft.fillScreen(BLACK);
  tft.setCursor(20, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print("Guess Mode");
  tft.setCursor(20, 50);
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.print("Guess the Word");

  tft.setCursor(20, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  /* backhome.initButton(&tft, 390, 270, 120, 40, WHITE, BLUE, WHITE, "HOME", 4);
  backhome.drawButton(false); */

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    /* update_button_list(buttons);
    if(buttons[5]->isPressed()){
      menu = "home";
      home();
      lello = 0;
    } */

    /* for (int s = 0; s < a.length(); s++) {
      if(hidden[s] == "_"){
        i = s;
      }
    } */
    
    if(input) {
      if(input == '='){
        startgame();
      }
      boolean substitutionMade = false;
      boolean allLettersGuessed = true;

      for (int s = 0; s < beforeguess.length() - 1; s++) {
          if (beforeguess.charAt(s) == '_') {
              allLettersGuessed = false;
              break;
          }
      }

      for (int s = 0; s < beforeguess.length(); s++) {
          if (beforeguess.charAt(s) == '_') {
              beforeguess.setCharAt(s, input);
              i = s;
              substitutionMade = true;
              break;
          }
      }

      if (allLettersGuessed && substitutionMade) {
          i++;
      }
      
      if (beforeguess.indexOf('_') == -1) {
          i = hidden.length();
      }

      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(beforeguess);
      delay(500);

      if (i == a.length()) {
        flag = true;
        if (beforeguess == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON");
          hidden = a;
          delay(1000);
          z = 0;
          lello = 0;
        } else {	
          delay(1000);
          Serial.println("ERRATO");
          tenterrato = 1;
          startgame();	
          z = 0;
          lello = 0;
          guessmode = false;	
        }
      }
    }

    if (input2) {
      boolean substitutionMade = false;
      boolean allLettersGuessed = true;

      for (int s = 0; s < beforeguess.length() - 1; s++) {
          if (beforeguess.charAt(s) == '_') {
              allLettersGuessed = false;
              break;
          }
      }

      for (int s = 0; s < beforeguess.length(); s++) {
          if (beforeguess.charAt(s) == '_') {
              beforeguess.setCharAt(s, input2);
              i = s;
              substitutionMade = true;
              break;
          }
      }

      if (allLettersGuessed && substitutionMade) {
          i++;
      }
      
      if (beforeguess.indexOf('_') == -1) {
          i = hidden.length();
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(beforeguess);
      delay(500);

      if (i == a.length()) {
        flag = true;
        if (beforeguess == a) {
          tft.setCursor(50, 240);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WON");
          hidden = a;
          delay(1000);
          z = 0;
          lello = 0;
        } else {	
          delay(1000);
          Serial.println("ERRATO");
          tenterrato = 1;
          startgame();
          z = 0;
          lello = 0;	
          guessmode = false;	
        }
      }
    }
  }

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);


  update_button_list(buttons);
}

void difficolta(){
  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  tft.fillScreen(BLACK);

  tft.setCursor(185, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.print("HangMan");

  easy.initButton(&tft, 90, 120, 120, 40, WHITE, GREEN, WHITE, "EASY", 4);
  med.initButton(&tft, 250, 120, 190, 40, WHITE, GREEN, WHITE, "MEDIUM", 4);
  hard.initButton(&tft, 410, 120, 120, 40, WHITE, GREEN, WHITE, "HARD", 4);
  backhome.initButton(&tft, 90, 250, 120, 40, WHITE, BLUE, WHITE, "HOME", 4);

  easy.drawButton(false);
  med.drawButton(false);
  hard.drawButton(false);
  backhome.drawButton(false);

  update_button_list(buttons);  //use helper function
  for (int i = 2; buttons[i] != NULL; i++) {
    if (buttons[i]->isPressed()) {
      Serial.println(i);
      buttons[i]->drawButton(true);
    }
    if (buttons[i]->justReleased()) {
      buttons[i]->drawButton(false);
    }
  }
}
