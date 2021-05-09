#include <TinyGPS++.h>
/*
   Cet exemple de croquis devrait être le premier que vous essayez d’essayer lorsque vous testez TinyGPS ++.
   (TinyGPSPlus) installation. En utilisation normale, vous alimentez des objets TinyGPS ++ à partir de
   un périphérique GPS NMEA série, mais cet exemple utilise des chaînes statiques pour plus de simplicité.
*/

// Un exemple de flux NMEA.
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

// L'objet TinyGPS ++
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);

  Serial.println("Démonstration de base de TinyGPS ++ (aucun périphérique requis)");
  Serial.print("Test de la bibliothèque TinyGPS ++ v."); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();

  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      displayInfo();

  Serial.println();
  Serial.println("Ok ! ");
}

void loop()
{
}

void displayInfo()
{
  Serial.print("Emplacement : "); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.print(" Date/Heure: ");
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
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print("INVALIDE");
  }

  Serial.println();
}
