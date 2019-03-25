#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   Cet exemple d'esquisse illustre l'utilisation normale d'un objet TinyGPS ++ (TinyGPSPlus).
   Il nécessite l’utilisation de SoftwareSerial et suppose que vous avez un
   Un périphérique GPS série à 9600 bauds raccordé aux broches 2 (rx) et 3 (tx).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// L'objet TinyGPS ++
TinyGPSPlus gps;

// La connexion série au périphérique GPS
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println("Une démonstration simple de TinyGPS ++ avec un module GPS connecté");
  Serial.print("Test de la bibliothèque TinyGPS ++ v. "); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
}

void loop()
{
// Cette esquisse affiche des informations chaque fois qu'une nouvelle phrase est correctement codée.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("Aucun GPS détecté: vérifiez le câblage.");
    while(true);
  }
}

void displayInfo()
{
  double latitude=gps.location.lat();
  double longitude=gps.location.lng();
  Serial.print("Emplacement : "); 
  if (gps.location.isValid())
  {
    Serial.print(latitude, 6);
    Serial.print(",");
    Serial.print(longitude, 6);
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.print("  Date/Heure: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.print(" ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.println();
}
