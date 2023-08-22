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

int x, y;

int receivedValue = 0;

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
}

void loop() {
  x = random(1, 176);

/*   if (Firebase.ready()) {
    String path = "/Facile/Parole/" + String(x);
    String pathConsigli = "/Facile/Consigli/" + String(x);
    Serial.println("Stringa: " + Firebase.getString(fbdo, path) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
    Serial.println("Stringa Consigli: " + Firebase.getString(fbdo, pathConsigli) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
    delay(1500);
  } */

  if (Firebase.ready()) {
    String path = "/Facile/Parole/" + String(x);
    String pathConsigli = "/Facile/Consigli/" + String(x);
    String parola = Firebase.getString(fbdo, path) ? fbdo.to<const char *>() : "";
    String consiglio = Firebase.getString(fbdo, pathConsigli) ? fbdo.to<const char *>() : "";

    String parolaEconsiglio = parola + "\n" + consiglio + "\n";
    Serial.print(parolaEconsiglio);

    delay(500);
  }

  /* if (Serial.available() > 0) {
    receivedValue = Serial.parseInt();
    Serial.print("Dato ricevuto dall'Arduino: ");
    Serial.println(receivedValue);
  } */
}
