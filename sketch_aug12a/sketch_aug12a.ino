#include <WiFi.h>
#include <FirebaseESP32.h>

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

#define WIFI_SSID "Wind3 2,4"
#define WIFI_PASSWORD "siiwiyjv2kkjx2d"

#define API_KEY "f2YtnVuyxOV7omZ0zXoaGwcDHgt5Miy12OhW8v8p"
#define DATABASE_URL "https://hangmangame-it-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


String main="";

int a, b, x, y;

String letturaDato;
int idDato;

void setup(){
  Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;


  Firebase.begin(DATABASE_URL, API_KEY);

  Firebase.setDoubleDigits(5);
}

void loop(){  
  x=random(0,9);
  y=random(10,19);
  
  if (Firebase.ready()) {
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/test/a", x);
    Firebase.setInt(fbdo, "/test/b", y);
    delay(200);

    Serial.println("Stringa: " + Firebase.getString(fbdo, "/Facile/8") ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
    letturaDato = fbdo.to<String>();

    Serial.printf("A: %s\n", Firebase.getInt(fbdo, "/test/a") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    a=fbdo.to<int>();

    Serial.printf("B: %s\n", Firebase.getInt(fbdo, "/test/b") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    b=fbdo.to<int>();
    delay(2500);
  }
}