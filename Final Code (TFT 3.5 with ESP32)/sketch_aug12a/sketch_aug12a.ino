#include <WiFi.h>
#include <FirebaseESP32.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "OPPO A53s"
#define WIFI_PASSWORD "michele06"

/* #define WIFI_SSID "iPhone di Ale"
#define WIFI_PASSWORD "alerunza" */

#define API_KEY "f2YtnVuyxOV7omZ0zXoaGwcDHgt5Miy12OhW8v8p"
#define DATABASE_URL "https://hangmangame-it-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;

int x;

int numeroDiParoleFacili = 176, numeroDiParoleMedie = 228, numeroDiParoleDifficili = 71;  

String lang = "ITA";

int receivedValue = 1, after = 1;
String mode = "Facile";
bool xGenerated = false;

void setup() {
  Serial.begin(9600);
  delay(2000);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);

  randomSeed(analogRead(0));
}

void loop() {
  if (Serial.available() > 0) {
    receivedValue = Serial.parseInt();
    after = receivedValue;
  }

  if (after != 0) {
    handleReceivedValue();
  }

  if (Firebase.ready() && xGenerated) {
    fetchDataAndSend();
  }
}

void handleReceivedValue() {
  switch (after) {
    case 1:
      mode = "Facile";
      generateWord();
      break;
    case 2:
      mode = "Medio";
      generateWord();
      break;
    case 3:
      mode = "Difficile";
      generateWord();
      break;
    case 4:
      lang = "ITA";
      generateWord();
      break;
    case 5:
      lang = "ENG";
      generateWord();
      break;
  }
}

void generateWord() {
  if (mode == "Facile") {
    x = random(1, numeroDiParoleFacili + 1);
  } else if (mode == "Medio") {
    x = random(1, numeroDiParoleMedie + 1);
  } else if (mode == "Difficile") {
    x = random(1, numeroDiParoleDifficili + 1);
  }
  xGenerated = true;
}

void fetchDataAndSend() {
  String path = lang + "/" + mode +"/Parole/" + String(x);
  String pathConsigli = lang + "/" + mode + "/Consigli/" + String(x);
  String parola = Firebase.getString(fbdo, path) ? fbdo.to<const char *>() : "";
  String consiglio = Firebase.getString(fbdo, pathConsigli) ? fbdo.to<const char *>() : "";

  String parolaEconsiglio = parola + "\n" + consiglio + "\n";
  Serial.print(parolaEconsiglio);
  xGenerated = false;
  delay(500);
}
