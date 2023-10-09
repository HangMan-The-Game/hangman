#include <WiFi.h>
#include <FirebaseESP32.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "OPPO A53s"
#define WIFI_PASSWORD "michele06"

//#define WIFI_SSID "iPhone di Ale"
//#define WIFI_PASSWORD "alerunza" 

//#define WIFI_SSID "Ed"
//#define WIFI_PASSWORD "cavl1431" 

#define API_KEY "f2YtnVuyxOV7omZ0zXoaGwcDHgt5Miy12OhW8v8p"
#define DATABASE_URL "https://hangmangame-it-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;

int x;

int numeroDiParoleFacili = 176, numeroDiParoleMedie = 228, numeroDiParoleDifficili = 71;  

String lang = "ITA";
String receivedStr = "";
int receivedValue = 1, after = 1;
String mode = "Facile", nick = "";
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

bool isAllUppercase(String s) {
    for(int i = 0; i < s.length(); i++) {
        if(islower(s[i])) {
            return false;
        }
    }
    return true;
}

void loop() {
if (Serial.available() > 0) {
  receivedStr = Serial.readStringUntil('\n');
  receivedStr.trim();
  bool isNumber = true;

  for (int i = 0; i < receivedStr.length(); i++) {
    if (!isdigit(receivedStr[i])) {  // Utilizza isdigit() invece di isDigit()
      isNumber = false;
      break;
    }
  }

  if (isNumber) {
    receivedValue = receivedStr.toInt();
    after = receivedValue;
  } else {
    switch (receivedStr[0]) {
      case 'f':
        if (receivedStr == "fac") {
          handleReceivedValue();
        } else {
          nick = receivedStr;
        }
        break;
      case 'm':
        if (receivedStr == "med") {
          handleReceivedValue();
        } else {
          nick = receivedStr;
        }
        break;
      case 'd':
        if (receivedStr == "dif") {
          handleReceivedValue();
        } else {
          nick = receivedStr;
        }
        break;
      case 'i':
        if (receivedStr == "it" || receivedStr == "en") {
          handleReceivedValue();
        } else {
          nick = receivedStr;
        }
        break;
      default:
        nick = receivedStr;
    }
  }
}


  if (after != 0) {
    handleReceivedValue();
  }

  if (Firebase.ready() && xGenerated) {
    fetchDataAndSend();
  }
}


void handleReceivedValue() {
  if (receivedStr == "fac") {
    mode = "Facile";
    generateWord();
  } else if (receivedStr == "med") {
    mode = "Medio";
    generateWord();
  } else if (receivedStr == "dif") {
    mode = "Difficile";
    generateWord();
  } else if (receivedStr == "it") {
    lang = "ITA";
    generateWord();
  } else if (receivedStr == "en") {
    lang = "ENG";
    generateWord();
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
  String parola = "", consiglio = "", parolaEconsiglio = "";
  if (!nick.isEmpty() && after > 1) {
    String pathNick = "Classifica/" + nick + "/" + String(after);
    Firebase.setString(fbdo, pathNick, nick);
  }
  String path = lang + "/" + mode +"/Parole/" + String(x);
  String pathConsigli = lang + "/" + mode + "/Consigli/" + String(x);
  parola = Firebase.getString(fbdo, path) ? fbdo.to<const char *>() : "";
  consiglio = Firebase.getString(fbdo, pathConsigli) ? fbdo.to<const char *>() : "";

  if(isAllUppercase(parola)) {
    parolaEconsiglio = parola + "\n" + consiglio + "\n";
    Serial.print(parolaEconsiglio);
  }
  xGenerated = false;
  delay(1000);
}
