#include "FastLED.h"
#include "ArduinoJson.h"


//define LEDs per string
#define NUMm_LEDS 12
#define NUMm_Strings 8
#define TCB_OE_Pin 23 //ESP32 Pin for TCB OE

//ESP32 Pins for TXA
#define TXB_1 27
#define TXB_2 25
#define TXB_3 32
#define TXB_4 17
#define TXB_5 16
#define TXB_6 22 //unused
#define TXB_7 21 //unused
#define TXB_8 33

//define Connected Fans/Strings for readability Index (-1)
#define FAN_BACK 4
#define FAN_FRONT_1 0
#define FAN_FRONT_2 7
#define FAN_FRONT_3 5
#define FAN_CPU_1 1
#define FAN_CPU_2 2
#define FAN_CUSTOM_1 3  //Unused
#define FAN_CUSTOM_2 6  //Unused    

struct FanConfig {
  public:
  int index;          // Index des Strings
  int min;            // Minimalwert
  int max;            // Maximalwert
  String jsonKey;     // JSON-Schlüssel
  void (*animationFunction)(int, int); // Funktionszeiger für Animation
};

// Konfiguration der min/max werte für die einzelnen Strings und Access Code des Json-Schlüssels
void setRGBRingoneColorBlending(int value,int strip);
void setRGBRingoneColorFading(int value,int strip);

FanConfig fanConfigs[8] = {
  {FAN_FRONT_1,35, 75, "CPU", setRGBRingoneColorFading},        //CPU grün=35-75% gelb=75-85% rot=85-100%
  {FAN_FRONT_2,50, 95, "VRM MOS",setRGBRingoneColorFading},     //VRM MOS grün=50-95% gelb=95-100% rot=100%     
  {FAN_FRONT_3,30, 37, "System",setRGBRingoneColorFading},      //System grün=20-30% gelb=30-40% rot=40-100%
  {FAN_BACK,35, 80, "CPU",setRGBRingoneColorFading},  
  {FAN_CPU_1,35, 80, "CPU",setRGBRingoneColorFading},
  {FAN_CPU_2,35, 80, "CPU",setRGBRingoneColorFading},
  {FAN_CUSTOM_1,35, 80, "CPU",setRGBRingoneColorFading},
  {FAN_CUSTOM_2,35, 80, "CPU",setRGBRingoneColorFading}, //  

};


CRGB leds[NUMm_Strings][NUMm_LEDS];



void fill(CRGB color,int string);
bool stringComplete = false; // whether the string is complete
void setRGBRing(int step,int pointer);
int mappedparsing(String key, int min,int max);
void serialEvent();
// Define the array of leds

JsonDocument doc;
String inputString="";
int values[NUMm_Strings]= {0}; // Array to hold the values for each string

void setup() {
    Serial.begin(115200);
    pinMode(TCB_OE_Pin,OUTPUT);
    digitalWrite(TCB_OE_Pin,HIGH );

    //Sort and define Your Setup here
    FastLED.addLeds<WS2812, TXB_1,GRB>(leds[FAN_BACK], 12);  // A1 //F3
    FastLED.addLeds<WS2812, TXB_2,GRB>(leds[FAN_CPU_1], 12);  // A2 //Back
    FastLED.addLeds<WS2812, TXB_3,GRB>(leds[FAN_CPU_2], 12);  // A3  //
    FastLED.addLeds<WS2812, TXB_4,GRB>(leds[FAN_FRONT_1], 12);  // A4 //F1
    FastLED.addLeds<WS2812, TXB_5,GRB>(leds[FAN_FRONT_2], 12);  // A5 //F2
    FastLED.addLeds<WS2812, TXB_6,GRB>(leds[FAN_FRONT_3], 12);  // A6 //Unused
    FastLED.addLeds<WS2812, TXB_7,GRB>(leds[FAN_CUSTOM_2], 12);  // A7 //Unused
    FastLED.addLeds<WS2812, TXB_8,GRB>(leds[FAN_CUSTOM_1], 12);  // A8

    delay(1000);
}
 
void loop() { 
  /*fill(CRGB::Blue,FAN_FRONT_1); 
  fill(CRGB::Red,FAN_FRONT_2);
  fill(CRGB::Green,FAN_FRONT_3);
  fill(CRGB::White,FAN_BACK);  
  fill(CRGB::Pink,FAN_CPU_1);
  fill(CRGB::Orange,FAN_CPU_2);
  fill(CRGB::Purple,FAN_CUSTOM_1);
  fill(CRGB::Yellow,FAN_CUSTOM_2);  
  FastLED.show();*/
  //delay(1000);
serialEvent(); // Check for serial input
   String s ="";
   if(stringComplete) {
   deserializeJson(doc, inputString);
   inputString = ""; // Clear the string for new input
   stringComplete = false; 
        for (int i = 0; i < NUMm_Strings; i++) {
        if (fanConfigs[i].index != -1 ) {
         fanConfigs[i].animationFunction(values[fanConfigs[i].index], fanConfigs[i].index);
        }
    }

}
  delay(100);
FastLED.show();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
      inputString += inChar;
      if (inChar == '}') {
        stringComplete = true;

        for (int i = 0; i < NUMm_Strings; i++) {
          if (fanConfigs[i].index != -1) { // Nur Einträge mit gültigem Index
              values[fanConfigs[i].index] = mappedparsing(fanConfigs[i].jsonKey, fanConfigs[i].min, fanConfigs[i].max);
          }
        } 
    }
}
}

int mappedparsing(String key, int min,int max){
  String temp= doc[key];
  temp.replace(",",".");
  int check= constrain(map((int) round(temp.toFloat()), min,max,0,100),0,100);
  return check;
}

void fill(CRGB color,int string){
  for (int i = 0; i < NUMm_LEDS; i++) {
    leds[string][i] = color;
  }
}

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
  }
}

//Prozent Color Blending as 1 Stripe one Color
void setRGBRingoneColorBlending(int value,int strip){
  int rgbset = 0;

  for (int i = 0; i < NUMm_LEDS; i++) {
  

    // Berechne den Fortschritt für die Farbüberblendung
    uint8_t progress = map(value, 0, 100, 0, 255);

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

//Blue "Clock" Dot goes Around the Fan, 
void setRGBRingoneColorFading(int value, int strip) {

  // Berechne den Fortschritt für die Farbüberblendung
  uint8_t progress = map(value, 0, 100, 0, 255);


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

      // Blaue LEDs für die aktuelle Stufe hervorheben
      int highlightIndex = map(value, 0, 100, 0, NUMm_LEDS - 1); // Mappe Prozent auf LED-Index
      if (i == highlightIndex || i == (highlightIndex + 1) % NUMm_LEDS || i == (highlightIndex -1) % NUMm_LEDS) {
          leds[strip][i] = CRGB::Blue; // Setze den blauen Cursor
      }
  }
}


/*{
  "CPU": "36,5",
  "System": "32",
  "VRM MOS": "46,5",
  "PCH": "54",
  "CPU Socket": "32",
  "GPU Memory": "36,5",
  "GPU Fan": "46,5",
  "CPU Fan": "32",
  "System Fan #1": "15",
  "System Fan #2": "41"
  }*/
