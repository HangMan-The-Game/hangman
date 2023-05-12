#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include <Keypad.h>

const byte ROWS = 4; //righe
const byte COLS = 4; //colonne

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
  {'Z','Y','|','?'},
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
Adafruit_GFX_Button start, diff, easy, med, hard, backhome;

String menu = "home";
int mode = 1;

String ndiff = "Easy";

String b [] = {
  #include "/Users/alerunza/Documents/Arduino/sketch_mar7b/parole.txt"
};

String a = ""; //parola da indovinare
int nword = 50, randomized = 0;
String hidden = "", wrong = "";
String prendi;
int i = 0, tent = 6, generated = 0, z = 0, g = 0, y = 0, scelta = 1, lello = 0;
bool flag = false, guessed = false;

//BUZZ SECTION

int speakerPin = 53;

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
  tft.setCursor(185, 90);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Medium");
  } else if(mode == 3){
    tft.print("- Hard");
  }

  start.initButton(&tft, 90, 100, 140, 40, WHITE, BLUE, WHITE, "START", 4);
  diff.initButton(&tft, 120, 150, 240, 40, WHITE, GREEN, WHITE, "DIFFICULTY", 4);

  start.drawButton(false);
  diff.drawButton(false);
}

// Array of button addresses to behave like a list
Adafruit_GFX_Button *buttons[] = {
  &start, &diff, &easy, &med, &hard, &backhome, NULL
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
      startgame();
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
    }
    if(buttons[4]->isPressed()){
      mode = 3;
    }
    if(buttons[5]->isPressed()){
      menu = "home";

      home();
    }
  }
  Serial.println(menu + " mode: " + mode);
  Serial.println(a + " | " + generated + " - scelta: " + mode);
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
  tft.setCursor(185, 90);
  if(mode == 1){
    tft.print("- Easy");
  } else if(mode == 2){
    tft.print("- Medium");
  } else if(mode == 3){
    tft.print("- Hard");
  }

  start.initButton(&tft, 90, 100, 140, 40, WHITE, BLUE, WHITE, "START", 4);
  diff.initButton(&tft, 120, 150, 240, 40, WHITE, GREEN, WHITE, "DIFFICULTY", 4);

  start.drawButton(false);
  diff.drawButton(false);
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
  while (z == 0) {
    tent = 6;
    Serial1.println("9");
    hidden = "";
    wrong = "";
    lello = 1;
    guessed = false;
    generated = randomized;
    switch(mode){
      case 1:
        a = b[generated];
        //a = "CIAQ";
        ndiff = "Easy";
        break;
      case 2:
        a = b[generated];
        //a = "CIAB";
        ndiff = "Medium";
        break;
      case 3:
        a = b[generated];
        //a = "CIIAJ";
        ndiff = "Hard";
        break;
      default:
        a = "CIAQ";
        break;
    }
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
  tft.print(ndiff + " Mode");
  Serial.println(randomized);

  tft.setCursor(20, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);

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
              Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                Serial1.println(tent);
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
          tft.setCursor(50, 220);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WIN");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 220);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }

    if (input2) {
      if(input2 == '?'){
        //guess();
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
              wrong += input2;
              Serial1.println(tent);
              if (h == a.length()) {
                tent--;
                Serial1.println(tent);
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
          tft.setCursor(50, 220);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WIN");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 220);
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

/* void guess(){
  lello = 1;
  tft.fillScreen(BLACK);
  tft.setCursor(120, 25);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.print("Guess Mode");
  tft.setCursor(140, 60);
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.print("Guess the Word");

  tft.setCursor(20, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(6);
  tft.print(hidden);

  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  backhome.initButton(&tft, 390, 270, 120, 40, WHITE, BLUE, WHITE, "HOME", 4);
  backhome.drawButton(false);

  while(lello){
    input = firstKpd.getKey();
    input2 = secondKpd.getKey();

    update_button_list(buttons);
    if(buttons[5]->isPressed()){
      menu = "home";
      home();
      lello = 0;
    }

    for (int s = 0; s < a.length(); s++) {
      if(hidden[s] == "_"){
        i = s;
      }
    }
    
    if (input) {

      hidden[i] = input;
      delay(500);
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      i++;

      delay(500);
      if (i == a.length()) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 180);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WIN");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 180);
          tft.setTextColor(RED);
          tft.setTextSize(4);
          tft.print(a + " - LOST");
          delay(1000);
          z = 0;
          lello = 0;
        }
      }
    }

    if (input2) {

      hidden[i] = input2;
      delay(500);
      tft.setCursor(20, 100);
      tft.setTextColor(BLUE, BLACK);
      tft.setTextSize(6);
      tft.print(hidden);
      i++;

      delay(500);
      if (i == a.length()) {
        flag = true;
        if (hidden == a) {
          tft.setCursor(50, 180);
          tft.setTextColor(GREEN);
          tft.setTextSize(4);
          tft.print(a + " - WIN");
          delay(1000);
          z = 0;
          lello = 0;
        } else {
          tft.setCursor(50, 180);
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


  update_button_list(buttons);
} */

void difficolta(){
  start.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
  diff.initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 4);
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
