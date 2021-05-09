#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/*
   Cet exemple illustre la capacité de TinyGPS ++ à extraire des fichiers personnalisés.
   champs de toute phrase NMEA. TinyGPS ++ a des installations intégrées pour
   extraire la latitude, la longitude, l’altitude, etc., à partir du $ GPGGA et
   Phrases GPRMC $. Mais avec le type TinyGPSCustom, vous pouvez extraire
   autres champs NMEA, même à partir de phrases NMEA non standard.

   Il nécessite l’utilisation de SoftwareSerial et suppose que vous avez un
   Périphérique GPS série à 4800 bauds raccordé aux broches 4 (RX) et 3 (TX).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;

// L'objet TinyGPS ++
TinyGPSPlus gps;

// La connexion série au périphérique GPS
SoftwareSerial ss(RXPin, TXPin);

/*
   En déclarant les objets TinyGPSCustom comme ceci, nous annonçons que nous
   sont intéressés par les 15ème, 16ème et 17ème champs de la $ GPGSA
   phrase, respectivement le PDOP (F ("dilution positionnelle de la précision")),
   HDOP (F ("horizontal ...")) et VDOP (F ("vertical ...")).

   (Le décompte commence par le champ qui suit immédiatement le nom de la phrase,
   c'est-à-dire $ GPGSA. Pour plus d'informations sur les phrases NMEA, consultez votre
   Documentation du module GPS et / ou http://aprs.gids.nl/nmea/.)

   Si votre module GPS ne supporte pas la phrase $ GPGSA, alors vous
   ne produira aucun résultat de ce programme.
*/

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA phrase, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

void setup() 
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println("Démonstration de l'extraction d'un champ NMEA à l'aide de TinyGPSCustom");
  Serial.print("Test de la bibliothèque TinyGPS ++ v."); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
}

void loop() 
{
  // Chaque fois que quelque chose est mis à jour, tout imprimer.
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  {
    Serial.print("ALT=");   Serial.print(gps.altitude.meters()); 
    Serial.print(" PDOP="); Serial.print(pdop.value()); 
    Serial.print(" HDOP="); Serial.print(hdop.value()); 
    Serial.print(" VDOP="); Serial.print(vdop.value());
    Serial.print(" SATS="); Serial.println(gps.satellites.value());
  }

  while (ss.available() > 0)
    gps.encode(ss.read());
}
