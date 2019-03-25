#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   Cet exemple de code suit les altitudes de satellites à l'aide d'objets TinyGPSCustom.

   Les numéros de satellite et les altitudes ne sont généralement pas suivis par TinyGPS ++, mais
   en utilisant TinyGPSCustom, nous contournons cela.

   Il nécessite l’utilisation de SoftwareSerial et suppose que vous possédez une
   Périphérique GPS série à 9600 bauds raccordé aux broches 2 (RX) et 3 (TX).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
static const int MAX_SATELLITES = 40;
static const int PAGE_LENGTH = 40;

// L'objet TinyGPS ++
TinyGPSPlus gps;

// La connexion série au périphérique GPS
SoftwareSerial ss(RXPin, TXPin);

TinyGPSCustom totalGPGSVMessages(gps, "GPGSV", 1); // phrase $ GPGSV, premier élément
TinyGPSCustom messageNumber(gps, "GPGSV", 2);      // phrase $ GPGSV, deuxième élément
TinyGPSCustom satNumber[4]; // à initialiser plus tard
TinyGPSCustom elevation[4];
bool anyChanges = false;
unsigned long linecount = 0;

struct
{
  int elevation;
  bool active;
} sats[MAX_SATELLITES];

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("Affiche les altitudes des satellites GPS à mesure qu'elles changent"));
  Serial.print(F("Test de la bibliothèque TinyGPS ++ v.")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
  
  // Initialise tous les objets TinyGPSCustom non initialisés
  for (int i=0; i<4; ++i)
  {
    satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
    elevation[i].begin(gps, "GPGSV", 5 + 4 * i); // offsets 5, 9, 13, 17
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
        if (no >= 1 && no <= MAX_SATELLITES)
        {
          int elev = atoi(elevation[i].value());
          sats[no-1].active = true;
          if (sats[no-1].elevation != elev)
          {
            sats[no-1].elevation = elev;
            anyChanges = true;
          }
        }
      }
      
      int totalMessages = atoi(totalGPGSVMessages.value());
      int currentMessage = atoi(messageNumber.value());
      if (totalMessages == currentMessage && anyChanges)
      {
        if (linecount++ % PAGE_LENGTH == 0)
          printHeader();
        TimePrint();
        for (int i=0; i<MAX_SATELLITES; ++i)
        {
          Serial.print(F(" "));
          if (sats[i].active)
            IntPrint(sats[i].elevation, 2);
          else
            Serial.print(F("   "));
          sats[i].active = false;
        }
        Serial.println();
        anyChanges = false;
      }
    }
  }
}

void IntPrint(int n, int len)
{
  int digs = n < 0 ? 2 : 1;
  for (int i=10; i<=abs(n); i*=10)
    ++digs;
  while (digs++ < len)
    Serial.print(F(" "));
  Serial.print(n);
  Serial.print(F(" "));
}

void TimePrint()
{
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F(" "));
  }
  else
  {
    Serial.print(F("(Inconnue)"));
  }
}

void printHeader()
{
  Serial.println();
  Serial.print(F("Heure     "));
  for (int i=0; i<MAX_SATELLITES; ++i)
  {
    Serial.print(F(" "));
    IntPrint(i+1, 2);
  }
  Serial.println();
  Serial.print(F("---------"));
  for (int i=0; i<MAX_SATELLITES; ++i)
    Serial.print(F("----"));
  Serial.println();
}
