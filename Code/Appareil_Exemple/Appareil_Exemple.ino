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

double latitude=0;
double longitude=0;
int annee=0;
int mois=0;
int jour=0;
int heure=0;
int minutes=0;
int seconde=0;
int milliseconde=0;

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
  }
  
//  Serial.print("Emplacement : ");
//  Serial.print(latitude, 6);
//  Serial.print(",");
//  Serial.print(longitude, 6);
//  Serial.print("  Date/Heure: ");
//  Serial.print(jour);
//  Serial.print("/");
//  Serial.print(mois);
//  Serial.print("/");
//  Serial.print(annee);
//  Serial.print(" ");
//  if (heure < 10) Serial.print("0");
//  Serial.print(heure);
//  Serial.print(":");
//  if (minutes < 10) Serial.print("0");
//  Serial.print(minutes);
//  Serial.print(":");
//  if (seconde < 10) Serial.print("0");
//  Serial.print(seconde);
//  Serial.print(".");
//  if (milliseconde < 10) Serial.print("0");
//  Serial.print(milliseconde);
//  Serial.println();
   
}

void displayInfo()
{
  latitude=gps.location.lat();
  longitude=gps.location.lng();
  annee=gps.date.year();
  mois=gps.date.month();
  jour=gps.date.day();
  heure=gps.time.hour();
  minutes=gps.time.minute();
  seconde=gps.time.second();
  milliseconde=gps.time.centisecond();
  
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
    Serial.print(jour);
    Serial.print("/");
    Serial.print(mois);
    Serial.print("/");
    Serial.print(annee);
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.print(" ");
  if (gps.time.isValid())
  {
    if (heure < 10) Serial.print("0");
    Serial.print(heure);
    Serial.print(":");
    if (minutes < 10) Serial.print("0");
    Serial.print(minutes);
    Serial.print(":");
    if (seconde < 10) Serial.print("0");
    Serial.print(seconde);
    Serial.print(".");
    if (milliseconde < 10) Serial.print("0");
    Serial.print(milliseconde);
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.println();
}
