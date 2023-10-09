#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>  
#include <Keypad.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

char keys[ROWS][COLS] = {
  {'A','B','C','D'},
  {'I','J','K','L'},
  {'Q','R','S','T'},
  {'Y','Z','(','='},
};

char keys2[ROWS][COLS] = {
  {'E','F','G','H'},
  {'M','N','O','P'},
  {'U','V','W','X'},
  {'(','^','?','|'},
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

uint16_t coloreModalita = 0x07E0;

/* #include <UTFTGLUE.h>
UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0); */

const int XP = 6, XM = A2, YP = A1, YM = 7;  //ID=0x9486
const int TS_LEFT = 948, TS_RT = 233, TS_TOP = 139, TS_BOT = 921;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
MCUFRIEND_kbv tft;
Adafruit_GFX_Button start, diff, easy, med, hard, backhome, vs, lang, ita, eng, options, backop, play, logout;

String menu = "home";
int mode = 1;

String language = "ITA";

String ndiff = "Easy";

/* String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word.txt"
};
String c [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word2.txt"
};
String d [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/ITA/word3.txt"
}; */

String a = "", nickname = ""; //parola da indovinare
String consiglio = ""; // consiglio da indovinare
String parolaRicevuta = "", consiglioRicevuto = "", parolaBuffer = "", consiglioBuffer = "";
int nword = 50, randomized = 0;
String hidden = "", wrong = "", beforeguess = "";
String prendi;
int i = 0, tent = 6, generated = 0, z = 0, g = 0, y = 0, scelta = 1, lello = 0, mamma = 0, tenterrato = 0, punti = 0;;
bool flag = false, guessed = false, guessmode = false, guessmodewrong = false;
int selectedDifficulty = 1; // 1 = easy, 2 = medium, 3 = hard
int selectedLanguage = 1; // 1 = ita, 2 = eng
bool needsUpdate = false, needsUpdateLang = false;
bool nuoveStringheDisponibili = false;

unsigned long lastUpdate = 0;
const long updateInterval = 250;

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

bool isValidWord(const String& str) {
    bool hasChar = false;
    for (unsigned int i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        if (c != ' ') {
            hasChar = true;
            if (islower(c)) {
                return false;
            }
        }
    }
    return hasChar;
}

bool isAllUppercase(String s) {
    for(int i = 0; i < s.length(); i++) {
        if(islower(s[i])) {
            return false;
        }
    }
    return true;
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

  tft.setCursor(145, 10);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("HANGMAN");
  tft.setCursor(175, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("THE GAME");

  play.initButton(&tft, 245, 150, 175, 40, WHITE, RED, WHITE, "PLAY", 4);
  options.initButton(&tft, 245, 200, 175, 40, WHITE, RED, WHITE, "OPTIONS", 4);

  play.drawButton(false);
  options.drawButton(false);

  /* tft.setCursor(185, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.print("HANGMAN");
  tft.setCursor(185, 100);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Medium");
  } else if(mode == 3){
    tft.print("- Hard");
  }

  tft.setCursor(185, 250);
  tft.print("- " + language);

  start.initButton(&tft, 90, 110, 140, 40, WHITE, BLUE, WHITE, "START", 4);
  diff.initButton(&tft, 90, 160, 140, 40, WHITE, GREEN, WHITE, "MODE", 4);
  vs.initButton(&tft, 90, 210, 140, 40, WHITE, RED, WHITE, "1vs1", 4);
  lang.initButton(&tft, 90, 260, 140, 40, WHITE, CYAN, WHITE, "LANG", 4);

  start.drawButton(false);
  diff.drawButton(false);
  vs.drawButton(false);
  lang.drawButton(false); */
}

// Array of button addresses to behave like a list
Adafruit_GFX_Button *buttons[] = {
  &start, &diff, &easy, &med, &hard, &backhome, &vs, &lang, &ita, &eng, &options, &backop, &play, &logout, NULL
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
  unsigned long currentMillis = millis();
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
      menu = "insNick";
      if(nickname.length() > 0){
        startgame();
      } else{
        preGame();
      }
    }
    /* if(buttons[0]->isPressed()){
      menu = "start";
      if (!parolaRicevuta.length() == 0 && !consiglioRicevuto.length() == 0){
        startgame();
        parolaRicevuta = "";
        consiglioRicevuto = "";
      }
    } */
    if(buttons[1]->isPressed()){
      menu = "diff";
      difficolta();
    }
    if(buttons[2]->isPressed()){
      mode = 1;
      selectedDifficulty = 1;
      needsUpdate = true;
      Serial1.println("fac"); 
    }
    if(buttons[3]->isPressed()){
      mode = 2;
      Serial1.println("med"); 
      selectedDifficulty = 2;
      needsUpdate = true;
      Serial.println("Parola Med: " + parolaRicevuta);
      Serial.println("Consiglio Med: " + consiglioRicevuto);
    }
    if(buttons[4]->isPressed()){
      mode = 3;
      Serial1.println("dif"); 
      selectedDifficulty = 3;
      needsUpdate = true;
      Serial.println("Parola Diff: " + parolaRicevuta);
      Serial.println("Consiglio Diff: " + consiglioRicevuto);
    }
    if(buttons[5]->isPressed()){
      menu = "home";

      home();
    }
    if(buttons[6]->isPressed()){
      menu = "1vs1";

      versus();
    }    
    if(buttons[7]->isPressed()){
      menu = "LANG";

      lingua();
    }
    if(buttons[8]->isPressed()){
      language = "ITA";
      selectedLanguage = 1;
      needsUpdateLang = true;
      Serial1.println("it");
    }
    if(buttons[9]->isPressed()){
      language = "ENG";
      selectedLanguage = 2;
      needsUpdateLang = true;
      Serial1.println("en");
    }
    if(buttons[10]->isPressed()){
      menu = "options";

      impostazioni();
    }
    if(buttons[11]->isPressed()){
      menu = "options";

      impostazioni();
    }
    if(buttons[12]->isPressed()){
      menu = "play";
      giocamenu();
    }
    if(buttons[13]->isPressed()){
      menu = "play";
      nickname = "";
      punti = "";
      home();
    }

    if (needsUpdate) {
        if (currentMillis - lastUpdate >= updateInterval) {
            drawDifficultyButtons();
            needsUpdate = false;
            lastUpdate = currentMillis;
        }
    }
    
    if (needsUpdateLang) {
        if (currentMillis - lastUpdate >= updateInterval) {
            drawLanguageButtons();
            needsUpdateLang = false;
            lastUpdate = currentMillis;
        }
    }

/*     if (needsUpdate) { 
        drawDifficultyButtons();
        needsUpdate = false;
    } */

    Serial1.println(mode);
    if (Serial1.available() > 0) {
        String receivedString = Serial1.readStringUntil('\n');
        receivedString.trim();

        if (receivedString.length() > 0) {
            if(parolaRicevuta.length() == 0 && isAllUppercase(receivedString)) {
                parolaRicevuta = receivedString;
            } 
            else if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() == 0) {
                consiglioRicevuto = receivedString;
            }

            if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() > 0) {
                Serial.println("Parola Ricevuta: " + parolaRicevuta);
                Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
                    
                parolaBuffer = parolaRicevuta;
                consiglioBuffer = consiglioRicevuto;

                nuoveStringheDisponibili = true;
                    
                parolaRicevuta = "";
                consiglioRicevuto = "";
            }
        }
    }



  /* Serial.println(menu + " mode: " + mode);
  Serial.println(a + " | " + generated + " - scelta: " + mode); */
}
}

void home(){
  Serial1.println(mode);
  if(language == "ITA"){
    Serial1.println("it");
  } else if(language == "ENG"){
    Serial1.println("en");
  }

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  lang.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  tft.fillScreen(BLACK);

  if(nickname.length() > 0){
    tft.setCursor(10, 290);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print(nickname + ":" + String(punti));
    logout.initButton(&tft, 410, 285, 80, 40, WHITE, RED, WHITE, "LOGOUT", 2);
    logout.drawButton(false);
  } else{
    logout.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4); 
  }

  tft.setCursor(145, 10);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("HANGMAN");
  tft.setCursor(175, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("THE GAME");

  play.initButton(&tft, 245, 150, 175, 40, WHITE, RED, WHITE, "PLAY", 4);
  options.initButton(&tft, 245, 200, 175, 40, WHITE, RED, WHITE, "OPTIONS", 4);

  play.drawButton(false);
  options.drawButton(false);

/*   parolaRicevuta = "";
  consiglioRicevuto = ""; */

    if (Serial1.available() > 0) {
        String receivedString = Serial1.readStringUntil('\n');
        receivedString.trim();

        if (receivedString.length() > 0) {
            if(parolaRicevuta.length() == 0 && isAllUppercase(receivedString)) {
                parolaRicevuta = receivedString;
            } 
            else if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() == 0) {
                consiglioRicevuto = receivedString;
            }

            if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() > 0) {
                Serial.println("Parola Ricevuta: " + parolaRicevuta);
                Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
                    
                parolaBuffer = parolaRicevuta;
                consiglioBuffer = consiglioRicevuto;

                nuoveStringheDisponibili = true;
                    
                parolaRicevuta = "";
                consiglioRicevuto = "";
            }
        }
    }



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

void preGame(){
  z = 0;
  mamma = 0;
  nickname = "";
  punti = 0;
  tft.fillScreen(BLACK);

  tft.setCursor(145, 10);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("HANGMAN");

  tft.setCursor(16, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("Your Nickname");

  tft.setCursor(16, 120);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(6);
  tft.print(nickname);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  logout.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  while(mamma == 0){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      if(input == '='){
        if (nickname.length() > 0) {
          nickname = nickname.substring(0, nickname.length() - 1);
          tft.fillRect(20, 100, tft.width() - 40, 50, BLACK);
        }
      } else if (input == '('){} else{
        nickname += input;
      }
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(6);
      tft.print(nickname);
    }

    if (input2) {
      if(input2 == '^'){
        mamma = 1;
      } else if(input2 == '|' || input2 == '?' || input2 == '('){}else {
        nickname += input2;
      }
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(6);
      tft.print(nickname);
    }
  }
  /* while(z == 0){
    tft.fillScreen(BLACK);
    tft.setCursor(20, 100);
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(6);
    tft.print(nickname);
    delay(2500);
    z = 1;
  } */
  // Serial1.println(nickname);
  if (parolaRicevuta.length() > 2 && parolaRicevuta != nickname && isValidWord(parolaRicevuta)) {
    startgame();
  } else {
      home();
      Serial1.println(mode);
  }
}

void startgame(){
/*   Serial1.println(mode);
  if(language == "ITA"){
    Serial1.println("it");
  } else if(language == "ENG"){
    Serial1.println("en");
  } */
  Serial1.println(nickname);
  // Serial.println(a + " " + consiglio);
  Serial.println("Parola Buffer: " + parolaBuffer);
  Serial.println("Consiglio Buffer: " + consiglioBuffer);
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
          a = parolaBuffer; 
          consiglio = consiglioBuffer; 
          nuoveStringheDisponibili = false;
        }
        // a = b[generated];
        //a = "CIAQ";
        ndiff = "Easy";
        break;
      case 2:
        if (nuoveStringheDisponibili) {
          a = parolaBuffer; 
          consiglio = consiglioBuffer; 
          nuoveStringheDisponibili = false;
        }
        // a = c[generated];
        //a = "CIAB";
        ndiff = "Medium";
        break;
      case 3:
        if (nuoveStringheDisponibili) {
          a = parolaBuffer; 
          consiglio = consiglioBuffer; 
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
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print(ndiff + " Mode");
  tft.setCursor(20, 45);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print(nickname + ":" + String(punti));
  //Serial.println(randomized);

  tft.setCursor(20, 100);
  tft.setTextColor(WHITE);
  tft.setTextSize(6);
  tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  logout.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      if(input == '('){}
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
          if(input == hidden[s]){
            is_present = true;
          }else{
            hidden[s] = input;
            // delay(500);
            punti += 1;
            tft.setCursor(20, 45);
            tft.setTextColor(WHITE, BLACK);
            tft.setTextSize(3);
            tft.print(nickname + ":" + String(punti));
            g++;
            tft.setCursor(20, 100);
            tft.setTextColor(WHITE, BLACK);
            tft.setTextSize(6);
            tft.print(hidden);
            tft.setCursor(450, 10);
            tft.setTextSize(4);
            tft.print(tent);
            Serial.println("Lettere indovinate: " + String(g));
            is_present = true;
          }
        } else if (a.charAt(s) != input){
          if(input == '=' || input == '('){} else{
            h++;
          }
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
              if(input == '=' || input == '('){} else{
                wrong += input;
              }
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;

                if(((mode == 2) || (mode == 3)) && (tent == 3)){
                  tft.setCursor(20, 230);
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
                // delay(1000);
                noTone(speakerPin);
              }
          }
        }
      }

      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
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
      // delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          punti += 3;
          delay(750);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(RED);
          tft.print(" - LOST");
          delay(750);
          z = 0;
          lello = 0;
        }
      }
    }

    bool special = false;

    if (input2) {
      if(input2 == '('){}
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
          if(input2 == hidden[s]){
            is_present = true;
          }else{
            hidden[s] = input2;
            // delay(500);
            punti += 1;
            tft.setCursor(20, 45);
            tft.setTextColor(WHITE, BLACK);
            tft.setTextSize(3);
            tft.print(nickname + ":" + String(punti));
            g++;
            tft.setCursor(20, 100);
            tft.setTextColor(WHITE, BLACK);
            tft.setTextSize(6);
            tft.print(hidden);
            tft.setCursor(450, 10);
            tft.setTextSize(4);
            tft.print(tent);
            Serial.println("Lettere indovinate: " + String(g));
            is_present = true;
          }
        } else if (a.charAt(s) != input2){
            if(input2 == '(' || input2 == '^'){} else{
              h++;
            }
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
              if(input2 == '(' || input2 == '^'){} else{
                wrong += input2;
              }
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                
                if(((mode == 2) || (mode == 3)) && (tent == 3)){
                  tft.setCursor(20, 230);
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
                // delay(1000);
                noTone(speakerPin);
              }
            }
          }
        }
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
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
      // delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          punti += 3;
          delay(750);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(RED);
          tft.print(" - LOST");
          delay(750);
          z = 0;
          lello = 0;
        }
      }
    }
  }

  Serial.println("Punti di: " + nickname + " - " + punti);
  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 410, 285, 100, 40, WHITE, BLUE, WHITE, "HOME", 3);
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
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  tft.print(ndiff + " Mode");

  tft.setCursor(16, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("Input Mode");

  tft.setCursor(16, 120);
  tft.setTextColor(WHITE);
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
      if(input == '=' || input == '('){}else{
        a += input;
      }
      // delay(500);
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(6);
      tft.print(a);
    }
    if (input2) {
      if(input2 == '^'){
        mamma = 1;
      } else if(input2 == '|' || input2 == '?' || input2 == '('){}else {
        a += input2;
      }
      // delay(500);
      g++;
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
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
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("1vs1 Mode");

  tft.setCursor(20, 100);
  tft.setTextColor(WHITE);
  tft.setTextSize(6);
  // tft.print(a);
  tft.print(hidden);

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    if (input) {
      if(input == '=' || input == '('){}
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
          // delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(WHITE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println(g);
          is_present = true;
        } else if (a.charAt(s) != input){
            if(input == '=' || input == '('){} else{
              h++;
            }
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
              if(input == '=' || input == '('){} else{
                wrong += input;
              }
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                // Serial1.println(tent);
                tone(speakerPin, 500);
                // delay(1000);
                noTone(speakerPin);
              }
          }
        }
      }

      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
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
      // delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          delay(750);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(RED);
          tft.print(" - LOST");
          delay(750);
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
      if(input2 == '('){}
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
          // delay(500);
          g++;
          tft.setCursor(20, 100);
          tft.setTextColor(WHITE, BLACK);
          tft.setTextSize(6);
          tft.print(hidden);
          tft.setCursor(450, 10);
          tft.setTextSize(4);
          tft.print(tent);
          Serial.println(g);
          is_present = true;
        } else if (a.charAt(s) != input2){
            if(input2 == '(' || input2 == '^'){} else{
              h++;
            }
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
              if(input2 == '(' || input2 == '^'){} else{
                wrong += input2;
              }
              // Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                // Serial1.println(tent);
                tone(speakerPin, 500);
                // delay(1000);
                noTone(speakerPin);
              }
            }
          }
        }
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
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
      // delay(500);
      if (tent < 1 || hidden == a) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          delay(750);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(RED);
          tft.print(" - LOST");
          delay(750);
          z = 0;
          lello = 0;
        }
      }
    }
  }

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 410, 280, 100, 40, WHITE, BLUE, WHITE, "HOME", 3);
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
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.print("HangMan | ");
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("Guess Mode");
  tft.setCursor(20, 57);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("Guess the Word");

  tft.setCursor(20, 100);
  tft.setTextColor(WHITE);
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
        /* startgame(); */
      }
      if(input == '('){}
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
              if(input == '=' || input == '('){}else{
              beforeguess.setCharAt(s, input);
              i = s;
              substitutionMade = true;
              break;
            }
          }
      }

      if (allLettersGuessed && substitutionMade) {
          i++;
      }
      
      if (beforeguess.indexOf('_') == -1) {
          i = hidden.length();
      }

      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(6);
      tft.print(beforeguess);
      // delay(500);

      if (i == a.length()) {
        flag = true;
        if (beforeguess == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          punti += 10;
          hidden = a;
          delay(750);
          z = 0;
          lello = 0;
        } else {	
          delay(750);
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
      if(input2 == '|'){
        lello = 0;
        z = 0;
      }
      if(input2 == '?' || input2 == '^' || input2 == '('){}
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
            if(input2 == '|' || input2 == '?' || input2 == '^' || input2 == '('){}else{
              
              beforeguess.setCharAt(s, input2);
              i = s;
              substitutionMade = true;
              break;
            }
          }
      }

      if (allLettersGuessed && substitutionMade) {
          i++;
      }
      
      if (beforeguess.indexOf('_') == -1) {
          i = hidden.length();
      }
      
      tft.setCursor(20, 100);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(6);
      tft.print(beforeguess);
      // delay(500);

      if (i == a.length()) {
        flag = true;
        if (beforeguess == a) {
          tft.setCursor(20, 275);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print(a);
          tft.setTextColor(GREEN);
          tft.print(" - WON");
          punti += 10;
          hidden = a;
          delay(750);
          z = 0;
          lello = 0;
        } else {	
          delay(750);
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
  Serial.println("Guess Punti di: " + nickname + " - " + punti);
  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);


  update_button_list(buttons);
}

void difficolta(){
  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  lang.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  
  tft.fillScreen(BLACK);

  tft.setCursor(185, 10);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print("HANGMAN");

  drawDifficultyButtons();
  backop.initButton(&tft, 240, 250, 110, 40, WHITE, BLUE, WHITE, "BACK", 4);

  backop.drawButton(false);

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

void drawDifficultyButtons() {
    if (selectedDifficulty == 1) {
        easy.initButton(&tft, 70, 120, 110, 40, WHITE, GREEN, WHITE, "EASY", 4);
        med.initButton(&tft, 240, 120, 180, 40, WHITE, RED, WHITE, "MEDIUM", 4);
        hard.initButton(&tft, 410, 120, 110, 40, WHITE, RED, WHITE, "HARD", 4);
    } else if (selectedDifficulty == 2) {
        easy.initButton(&tft, 70, 120, 110, 40, WHITE, RED, WHITE, "EASY", 4);
        med.initButton(&tft, 240, 120, 180, 40, WHITE, GREEN, WHITE, "MEDIUM", 4);
        hard.initButton(&tft, 410, 120, 110, 40, WHITE, RED, WHITE, "HARD", 4);
    } else if (selectedDifficulty == 3) {
        easy.initButton(&tft, 70, 120, 110, 40, WHITE, RED, WHITE, "EASY", 4);
        med.initButton(&tft, 240, 120, 180, 40, WHITE, RED, WHITE, "MEDIUM", 4);
        hard.initButton(&tft, 410, 120, 110, 40, WHITE, GREEN, WHITE, "HARD", 4);
    }

    easy.drawButton(false);
    med.drawButton(false);
    hard.drawButton(false);
}

void lingua(){
  play.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  vs.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  lang.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  
  tft.fillScreen(BLACK);

  tft.setCursor(185, 10);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print("HANGMAN");
  
  tft.setCursor(80, 40);
  tft.setTextColor(WHITE);
  tft.setTextSize(3.5);
  tft.print("Choose the Language");

  drawLanguageButtons();
  backop.initButton(&tft, 245, 250, 110, 40, WHITE, BLUE, WHITE, "BACK", 4);

  backop.drawButton(false);

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

void drawLanguageButtons() {
    if (selectedLanguage == 1) {
      ita.initButton(&tft, 180, 140, 110, 40, WHITE, GREEN, WHITE, "ITA", 4);
      eng.initButton(&tft, 310, 140, 110, 40, WHITE, RED, WHITE, "ENG", 4);
    } else if (selectedLanguage == 2) {
      ita.initButton(&tft, 180, 140, 110, 40, WHITE, RED, WHITE, "ITA", 4);
      eng.initButton(&tft, 310, 140, 110, 40, WHITE, GREEN, WHITE, "ENG", 4);
    }

    ita.drawButton(false);
    eng.drawButton(false);
}

void impostazioni(){
  play.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  options.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  easy.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  med.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  hard.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  ita.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  eng.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backop.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  tft.fillScreen(BLACK);
  tft.setCursor(145, 10);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("HANGMAN");
  tft.setCursor(185, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("OPTIONS");

  tft.setCursor(350, 120);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Med");
  } else if(mode == 3){
    tft.print("- Hard");
  }
  
  tft.setCursor(50, 170);
  tft.print(language + " -");

  diff.initButton(&tft, 245, 130, 175, 40, WHITE, RED, WHITE, "MODE", 4);
  lang.initButton(&tft, 245, 180, 175, 40, WHITE, RED, WHITE, "LANG", 4);
  backhome.initButton(&tft, 245, 230, 175, 40, WHITE, BLUE, WHITE, "HOME", 4);

  diff.drawButton(false);
  lang.drawButton(false);
  backhome.drawButton(false);

  /* parolaRicevuta = "";
  consiglioRicevuto = ""; */



    if (Serial1.available() > 0) {
        String receivedString = Serial1.readStringUntil('\n');
        receivedString.trim();

        if (receivedString.length() > 0) {
            if(parolaRicevuta.length() == 0 && isAllUppercase(receivedString)) {
                parolaRicevuta = receivedString;
            } 
            else if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() == 0) {
                consiglioRicevuto = receivedString;
            }

            if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() > 0) {
                Serial.println("Parola Ricevuta: " + parolaRicevuta);
                Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
                    
                parolaBuffer = parolaRicevuta;
                consiglioBuffer = consiglioRicevuto;

                nuoveStringheDisponibili = true;
                    
                parolaRicevuta = "";
                consiglioRicevuto = "";
            }
        }
    }



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

void giocamenu(){
  play.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  options.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

  tft.fillScreen(BLACK);
  tft.setCursor(145, 10);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("HANGMAN");
  tft.setCursor(178, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("THE GAME");

  start.initButton(&tft, 245, 130, 175, 40, WHITE, RED, WHITE, "SOLO", 4);
  vs.initButton(&tft, 245, 180, 175, 40, WHITE, RED, WHITE, "1vs1", 4);
  backhome.initButton(&tft, 245, 230, 175, 40, WHITE, BLUE, WHITE, "HOME", 4);

  start.drawButton(false);
  vs.drawButton(false);
  backhome.drawButton(false);

  /* parolaRicevuta = "";
  consiglioRicevuto = ""; */

    if (Serial1.available() > 0) {
        String receivedString = Serial1.readStringUntil('\n');
        receivedString.trim();

        if (receivedString.length() > 0) {
            if(parolaRicevuta.length() == 0 && isAllUppercase(receivedString)) {
                parolaRicevuta = receivedString;
            } 
            else if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() == 0) {
                consiglioRicevuto = receivedString;
            }

            if (parolaRicevuta.length() > 0 && consiglioRicevuto.length() > 0) {
                Serial.println("Parola Ricevuta: " + parolaRicevuta);
                Serial.println("Consiglio Ricevuto: " + consiglioRicevuto);
                    
                parolaBuffer = parolaRicevuta;
                consiglioBuffer = consiglioRicevuto;

                nuoveStringheDisponibili = true;
                    
                parolaRicevuta = "";
                consiglioRicevuto = "";
            }
        }
    }



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