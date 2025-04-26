#include "FastLED.h"
#include "ArduinoJson.h"


//define LEDs per string
#define NUMm_LEDS 12
#define NUMm_Strings 8
#define TCB_OE_Pin 23 //ESP32 Pin for TCB OE

//ESP32 Pins for TXA
#define TXA_1 27
#define TXA_2 25
#define TXA_3 32
#define TXA_4 17
#define TXA_5 16
#define TXA_6 22 //unused
#define TXA_7 21 //unused
#define TXA_8 33

//define Connected Fans/Strings for readability Index (-1)
#define FAN_BACK 1
#define FAN_FRONT_1 0
#define FAN_FRONT_2 4
#define FAN_FRONT_3 5
#define FAN_CPU_1 2
#define FAN_CPU_2 3
#define FAN_CUSTOM_1 6  //Unused
#define FAN_CUSTOM_2 7  //Unused    

struct FanConfig {
  public:
  int index;          // Index des Strings
  int min;            // Minimalwert
  int max;            // Maximalwert
  String hwKey;
  String sensorType;
  String sensorName ;
  int value;      
  void (*animationFunction)(int, int); // Funktionszeiger für Animation
};


// Konfiguration der min/max werte für die einzelnen Strings und Access Code des Json-Schlüssels
void setRGBRingoneColorBlending(int value,int strip);
void setRGBRingoneColorFading(int value,int strip);

// Konfiguration der min/max werte für die einzelnen Strings und Access Code des Json-Schlüssels
FanConfig fanConfigs[8] = {
  {FAN_BACK     ,0, 75,      "Nuvoton NCT6687D",       "Temperature",  "CPU Core",          0,  setRGBRingoneColorFading},       
  {FAN_FRONT_1  ,50, 80,  "Nuvoton NCT6687D",       "Temperature",  "CPU Core",     0,  setRGBRingoneColorFading},  
  {FAN_FRONT_2  ,0, 95,   "Nuvoton NCT6687D",       "Temperature",  "VRM MOS",      0,  setRGBRingoneColorFading},      
  {FAN_FRONT_3  ,0, 70,   "AMD Radeon RX 7600 XT",  "Temperature",  "GPU Core",     0,  setRGBRingoneColorFading},   
  {FAN_CPU_1    ,35, 80,"","", "",0,setRGBRingoneColorFading},
  {FAN_CPU_2    ,35, 80,"", "","",0,setRGBRingoneColorFading},
  {FAN_CUSTOM_1 ,35, 80,"","", "",0,setRGBRingoneColorFading},
  {FAN_CUSTOM_2 ,35, 80,"", "","",0,setRGBRingoneColorFading} 
};


CRGB leds[NUMm_Strings][NUMm_LEDS];



void fill(CRGB color,int string);
bool stringComplete = false; // whether the string is complete
void setRGBRing(int step,int pointer);
int mappedparsing(String hw, String sensorName, String sensorType, int min, int max);
void serialEvent();
String generateJsonFromFanConfigs();
// Define the array of leds

// Initialize the JSON document with a specific size
StaticJsonDocument<2048> doc;
String inputString="";
String jsonStringStream = ""; // Variable to hold the JSON string
String receiveBuffer = ""; // Variable to hold the received string

void setup() {
   
    Serial.begin(115200);
    pinMode(TCB_OE_Pin,OUTPUT);
    digitalWrite(TCB_OE_Pin,HIGH );

    //Sort and define Your Setup here
    FastLED.addLeds<WS2812, TXA_1,GRB>(leds[0], 12);  // A1 //F3
    FastLED.addLeds<WS2812, TXA_2,GRB>(leds[1], 12);  // A2 //Back
    FastLED.addLeds<WS2812, TXA_3,GRB>(leds[2], 12);  // A3  //
    FastLED.addLeds<WS2812, TXA_4,GRB>(leds[3], 12);  // A4 //F1
    FastLED.addLeds<WS2812, TXA_5,GRB>(leds[4], 12);  // A5 //F2
    FastLED.addLeds<WS2812, TXA_6,GRB>(leds[5], 12);  // A6 //Unused
    FastLED.addLeds<WS2812, TXA_7,GRB>(leds[6], 12);  // A7 //Unused
    FastLED.addLeds<WS2812, TXA_8,GRB>(leds[7], 12);  // A8
    //Serial.print("\r\n"); //clear commands
    delay(1000);

// uncomment for list of your Hardware on start
//Serial.print("liste\r\n");
//Serial.print(Serial.readStringUntil('\n'));
jsonStringStream= generateJsonFromFanConfigs(); // Send JSON to Serial
delay(1000);

}
 


void loop() { 

    //serialEvent(); // Check for serial input
   if(stringComplete) {

    String receiveBuffertemp=receiveBuffer; // Speichere den empfangenen String in receiveBuffer
    receiveBuffer = ""; 
    stringComplete = false; 
    receiveBuffertemp.replace("\r", ""); // Entferne '\r'
    receiveBuffertemp.replace("\n", ""); // Entferne '\n'
    deserializeJson(doc, receiveBuffertemp); // JSON-Daten deserialisieren
  
  // Clear the string for new input
 
   for (int i = 0; i < NUMm_Strings; i++)
   if (fanConfigs[i].hwKey != "") 
   fanConfigs[i].value = mappedparsing(fanConfigs[i].hwKey, fanConfigs[i].sensorName, fanConfigs[i].sensorType , fanConfigs[i].min, fanConfigs[i].max);
   for (int i = 0; i < NUMm_Strings; i++) {
    if (fanConfigs[i].hwKey != "") { 
        fanConfigs[i].animationFunction(fanConfigs[i].value , fanConfigs[i].index);
        // Serial.print("DEBUG: "+ String(i) +": " + String(fanConfigs[i].value) + "\r\n");

      }
    }
    FastLED.show();
}


  delay(1000);
Serial.print(jsonStringStream);

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
      inputString += inChar;
      if (inChar == '\n') {
        stringComplete = true;
          receiveBuffer=inputString;
          inputString="";
      } 
  
  }
}

int mappedparsing(String hw, String sensorName, String sensorType, int min, int max) {

  for (JsonObject hardware : doc.as<JsonArray>()) {
      String hardwareName = hardware["Hardware"].as<String>();
    
      if (hardwareName == hw) {
             
          JsonArray sensors = hardware["Sensors"];
          for (JsonObject sensor : sensors) {
            
              if (sensor["SensorName"].as<String>() == sensorName && sensor["SensorType"].as<String>() == sensorType) {
                  String temp = sensor["Value"].as<String>();
                  temp.replace(",", "."); 
                  int check = constrain(map((int)round(temp.toFloat()), min, max, 0, 255), 0, 255);
                  return check;
              }
          }
      }
  }

  return 0;
}


void fill(CRGB color,int string){
  for (int i = 0; i < NUMm_LEDS; i++) {
    leds[string][i] = color;
  }
}

//Other First Animations
void setRGBRing(int temp,int strip){
int step=temp;
int rgbset=0;
  for (int i = 0; i < NUMm_LEDS; i++) {
  
    if (i<4){
      leds[strip][i] = CRGB::Green;
      }else 
    if (i<8){
      leds[strip][i] = CRGB::Yellow;
    }else 
     leds[strip][i] = CRGB::Red;
    
   
      if(rgbset!=step &&rgbset!=step+1 &&rgbset!=step-1 )
{
     fadeToBlackBy(leds[strip]+i,1,200); // Fade to black
   
}   
  }}


//Prozent Color Blending as 1 Stripe one Color
void setRGBRingoneColorBlending(int value,int strip){
  int rgbset = 0;

  for (int i = 0; i < NUMm_LEDS; i++) {
  

    // Berechne den Fortschritt für die Farbüberblendung
    uint8_t progress = map(value, 0, 255, 0, 255);

    // Farben definieren
    CRGB colorStart, colorEnd;

    if (progress < 85) {
        // Grün -> Gelb
        colorStart = CRGB::Green;
        colorEnd = CRGB::Yellow;
       
       
       
       
        leds[strip][i] = blend(colorStart, colorEnd, progress * 3); // 0–85 wird auf 0–255 skaliert
    } else if (progress < 170) {
        // Gelb -> Orange
        colorStart = CRGB::Yellow;
        colorEnd = CRGB::Orange;
        leds[strip][i] = blend(colorStart, colorEnd, (progress - 85) * 3); // 85–170 wird auf 0–255 skaliert
    } else {
        // Orange -> Rot
        colorStart = CRGB::Orange;
        colorEnd = CRGB::Red;
        leds[strip][i] = blend(colorStart, colorEnd, (progress - 170) * 3); // 170–255 wird auf 0–255 skaliert
    }
}
}

//Prozent Color Fading as 1 Stripe one Color, Blue Dot Scaled
void setRGBRingoneColorFading(int value, int strip) {

  // Berechne den Fortschritt für die Farbüberblendung
  uint8_t progress = value;// map(value, 0, 255, 0, 255);


  // Farben definieren
  CRGB colorStart, colorEnd;
 
  for (int i = 0; i < NUMm_LEDS; i++) {


      // Berechne die aktuelle Farbe basierend auf dem Fortschritt
      if (progress < 85) {
          // Grün -> Gelb
          colorStart = CRGB::Green;
          colorEnd = CRGB::Yellow;
          leds[strip][i] = blend(colorStart, colorEnd, progress * 3); // 0–85 wird auf 0–255 skaliert
      } else if (progress < 170) {
          // Gelb -> Orange
          colorStart = CRGB::Yellow;
          colorEnd = CRGB::Orange;
          leds[strip][i] = blend(colorStart, colorEnd, (progress - 85) * 3); // 85–170 wird auf 0–255 skaliert
      } else {
          // Orange -> Rot
          colorStart = CRGB::Orange;
          colorEnd = CRGB::Red;
          leds[strip][i] = blend(colorStart, colorEnd, (progress - 170) * 3); // 170–255 wird auf 0–255 skaliert
      }

      // Helle LEDs für die aktuelle Stufe hervorheben
      int highlightIndex = constrain(map(value, 0, 255, 0, NUMm_LEDS - 1),0,11);
      if (i == highlightIndex || i == (highlightIndex + 1) % NUMm_LEDS || i == (highlightIndex -1) % NUMm_LEDS) {
          leds[strip][i] = CRGB::Blue; 
          Serial.print("DEBUG: "+ String(strip) +": " + String(i)+ "\r\n");
      }
  }
}
//Create JSON from FanConfigs
String generateJsonFromFanConfigs() {
  String jsonString = "{"; // JSON-Objekt starten

  String processedKeys[NUMm_Strings]; // Array, um bereits verarbeitete hwKeys zu speichern
  int processedCount = 0; // Zähler für verarbeitete hwKeys
  bool firstHwKey = true; // Flag, um das erste hwKey zu erkennen

  for (int i = 0; i < NUMm_Strings; i++) {
      if (fanConfigs[i].index != -1 && fanConfigs[i].hwKey != "") { // Nur gültige Einträge
          String hwKey = fanConfigs[i].hwKey;

          // Prüfe, ob der hwKey bereits verarbeitet wurde
          bool isDuplicate = false;
          for (int k = 0; k < processedCount; k++) {
              if (processedKeys[k] == hwKey) {
                  isDuplicate = true;
                  break;
              }
          }

          if (isDuplicate) {
              continue; // Überspringe doppelte hwKeys
          }

          // Füge den hwKey zur Liste der verarbeiteten Keys hinzu
          processedKeys[processedCount++] = hwKey;

          // Komma nur hinzufügen, wenn es nicht der erste hwKey ist
          if (!firstHwKey) {
              jsonString += ", ";
          }
          firstHwKey = false;

          jsonString += "\"" + hwKey + "\": ["; // hwKey als JSON-Schlüssel

          // Alle sensorNames und sensortypes mit demselben hwKey sammeln
          bool firstSensorName = true;
          for (int j = 0; j < NUMm_Strings; j++) {
              if (fanConfigs[j].hwKey == hwKey && fanConfigs[j].hwKey != "") { // Nur gültige Einträge
                  if (!firstSensorName) {
                      jsonString += ", ";
                  } 
                  jsonString += "{";
                  jsonString += "\"SensorName\": \"" + fanConfigs[j].sensorName + "\", ";
                  jsonString += "\"SensorType\": \"" + fanConfigs[j].sensorType + "\"";
                  jsonString += "}";
                  firstSensorName = false;
              }
          }

          jsonString += "]"; // Liste schließen
      }
  }

  jsonString += "}\r\n"; 
  return jsonString; 
}
void identifyFans(int index) {

    for(int i=0; i<NUMm_Strings; i++){
      for (int c = 0; c < i+1; c++) {
      
        for (int a = 0; a < NUMm_LEDS; a++) {
        leds[i][a] = CRGB::White;
        
      }
      FastLED.show();
      delay(500) ;
      for (int a = 0; a < NUMm_LEDS; a++) {
        leds[i][a] = CRGB::Black;
      
    
      }
      FastLED.show();
      delay(500) ;
     
    }
    delay(1000);
    
      }
}