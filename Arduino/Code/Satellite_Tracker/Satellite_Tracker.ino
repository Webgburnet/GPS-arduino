#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 /*
   Cet exemple de code montre comment utiliser un tableau d'objets TinyGPSCustom
   pour surveiller tous les satellites visibles.

   Les numéros de satellite, l’altitude, l’azimut et le rapport signal sur bruit ne sont pas connus.
   normalement suivi par TinyGPS ++, mais en utilisant TinyGPSCustom, nous contournons ce problème.

   Le code simple montre également comment utiliser les tableaux d'objets TinyGPSCustom,
   chacun surveillant un champ différent de la phrase $ GPGSV.

   Il nécessite l’utilisation de SoftwareSerial et suppose que vous avez un
   Périphérique GPS série à 4800 bauds raccordé aux broches 2 (RX) et 3 (TX).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/* 
 De http://aprs.gids.nl/nmea/:
   
  $ GPGSV
  
  Satellites GPS en vue
  
  par exemple. $ GPGSV, 3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00 * 74
      $ GPGSV, 3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00 * 74
      $ GPGSV, 3,3,11,22,42,067,42,24,14,311,43,27,05,244,00 ,,,, * 4D

  1 = nombre total de messages de ce type dans ce cycle
  2 = numéro du message
  3 = nombre total de SV en vue
  4 = numéro SV PRN
  5 = Altitude en degrés, 90 maximum
  6 = Azimut, degrés du nord vrai 000 à 359
  7 = SNR, 00-99 dB (nul si non suivi)
  8-11 = Information sur la deuxième SV, comme dans le champ 4-7
  12-15 = Informations sur la troisième SV, comme dans le champ 4-7
  16-19 = Informations sur la quatrième SV, comme dans le champ 4-7
*/

static const int MAX_SATELLITES = 40;

TinyGPSCustom totalGPGSVMessages(gps, "GPGSV", 1); // $GPGSV phrase, premier élément
TinyGPSCustom messageNumber(gps, "GPGSV", 2);      // $GPGSV phrase, deuxieme élément
TinyGPSCustom satsInView(gps, "GPGSV", 3);         // $GPGSV phrase, troisieme élément
TinyGPSCustom satNumber[4]; // à initialiser plus tard
TinyGPSCustom elevation[4];
TinyGPSCustom azimuth[4];
TinyGPSCustom snr[4];

struct
{
  bool active;
  int elevation;
  int azimuth;
  int snr;
} sats[MAX_SATELLITES];

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("Surveillance de la position du satellite et de la force du signal à l'aide de TinyGPSCustom"));
  Serial.print(F("Testing TinyGPS++ library v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
  
  // Initialise tous les objets TinyGPSCustom non initialisés
  for (int i=0; i<4; ++i)
  {
    satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
    elevation[i].begin(gps, "GPGSV", 5 + 4 * i); // offsets 5, 9, 13, 17
    azimuth[i].begin(  gps, "GPGSV", 6 + 4 * i); // offsets 6, 10, 14, 18
    snr[i].begin(      gps, "GPGSV", 7 + 4 * i); // offsets 7, 11, 15, 19
  }
}

void loop()
{
  // envoie les caractères entrants
  if (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (totalGPGSVMessages.isUpdated())
    {
      for (int i=0; i<4; ++i)
      {
        int no = atoi(satNumber[i].value());
        // Serial.print(F("SatNumber is ")); Serial.println(no);
        if (no >= 1 && no <= MAX_SATELLITES)
        {
          sats[no-1].elevation = atoi(elevation[i].value());
          sats[no-1].azimuth = atoi(azimuth[i].value());
          sats[no-1].snr = atoi(snr[i].value());
          sats[no-1].active = true;
        }
      }
      
      int totalMessages = atoi(totalGPGSVMessages.value());
      int currentMessage = atoi(messageNumber.value());
      if (totalMessages == currentMessage)
      {
        Serial.print(F("Sats=")); Serial.print(gps.satellites.value());
        Serial.print(F(" Nums="));
        for (int i=0; i<MAX_SATELLITES; ++i)
          if (sats[i].active)
          {
            Serial.print(i+1);
            Serial.print(F(" "));
          }
        Serial.print(F(" Elevation="));
        for (int i=0; i<MAX_SATELLITES; ++i)
          if (sats[i].active)
          {
            Serial.print(sats[i].elevation);
            Serial.print(F(" "));
          }
        Serial.print(F(" Azimuth="));
        for (int i=0; i<MAX_SATELLITES; ++i)
          if (sats[i].active)
          {
            Serial.print(sats[i].azimuth);
            Serial.print(F(" "));
          }
        
        Serial.print(F(" SNR="));
        for (int i=0; i<MAX_SATELLITES; ++i)
          if (sats[i].active)
          {
            Serial.print(sats[i].snr);
            Serial.print(F(" "));
          }
        Serial.println();

        for (int i=0; i<MAX_SATELLITES; ++i)
          sats[i].active = false;
      }
    }
  }
}
