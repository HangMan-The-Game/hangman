#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int upButton = 11;
int downButton = 12;
int selectButton = 13;
int menu = 1;
int diff = 1;

int z = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  updateMenu();
}

void loop() {
  if (!digitalRead(downButton)){
    menu++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)){
    eseguiMenu();
    updateMenu();
    delay(100);
    while (!digitalRead(selectButton));
  }
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Inizia");
      lcd.setCursor(0, 1);
      lcd.print(" Difficolta");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Inizia");
      lcd.setCursor(0, 1);
      lcd.print(">Difficolta");
      break;
    case 3:
      menu = 2;
      break;
  }
}

void eseguiMenu() {
  switch (menu) {
    case 1:
      iniziaGioco();
      break;
    case 2:
      //difficolta(); // da fare
      break;
  }
}

void iniziaGioco() {
  lcd.clear();
  lcd.print("Iniziando...");
  delay(1500);
}
