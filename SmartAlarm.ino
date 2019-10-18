#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
//Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>


#include <DS3231.h>

//DS3231  rtc(SDA, SCL);
DS3231  rtc(12, SCL);

// defines pins numbers for UltraSOnic
const int trigPin = 10;
const int echoPin = 11;
// defines variables
long duration;
int distance = 0;
int newDist;

//Buzz Buzz
const int buzzer = 13; //buzzer to arduino pin 9
bool ring = false;

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

// most mcufriend shields use these pins and Portrait mode:
//uint8_t YP = A1;  // must be an analog pin, use "An" notation!
//uint8_t XM = A0;  // must be an analog pin, use "An" notation!
//uint8_t YM = 7;   // can be a digital pin
//uint8_t XP = 6;   // can be a digital pin
//uint8_t SwapXY = 0;

uint16_t TS_LEFT = 893;
uint16_t TS_RT  = 208;
uint16_t TS_TOP = 900;
uint16_t TS_BOT = 340;
char *name = "Unknown controller";


#define MINPRESSURE 20
#define MAXPRESSURE 850

#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor;
uint8_t Orientation = 0;    //PORTRAIT

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


String inString = "";    // string to hold input

void setup(void)
{
//    uint16_t tmp;
//    tft.begin(9600);
//
//    tft.reset();
//    identifier = tft.readID(); // No.
    
    identifier = 0;
    
//    name = "ILI9341 DealExtreme";
//    SwapXY = 1;
//    switch (Orientation) {      // adjust for different aspects
//        case 0:   break;        //no change,  calibrated for PORTRAIT
//        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
//        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
//        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
//    }
//
    Serial.begin(9600);
//    ts = TouchScreen(XP, YP, XM, YM, 300);     //call the constructor AGAIN with new values.

    // Buzzz
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
    
    // UltraSonic
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    rtc.begin(); // Initialize the rtc object
    // The following lines can be uncommented to set the date and time
    rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
    rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(1, 10, 2019);   // Set the date
    
    tft.begin(identifier);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
//    tft.setCursor(50,100);
//    tft.setRotation(3);
//    tft.setTextSize(5);
//    Serial.println(rtc.getTimeStr()); 
//    tft.println(rtc.getTimeStr());
}

void loop()
{

    Serial.print("Time:  ");
    Serial.println(rtc.getTimeStr());
    
    Serial.print("Date: ");
    Serial.println(rtc.getDateStr());

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    newDist = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    if (newDist < 10){
      Serial.print("Distance: ");
      Serial.println(newDist);

      if(newDist != distance){
          tft.setTextColor(WHITE,BLACK);
          tft.setCursor(0,90);
          tft.setRotation(3);
          tft.setTextSize(2);
          tft.println(distance);
          distance = newDist;
        }
    }
    
    tft.setTextColor(WHITE,BLACK);
    tft.setCursor(40,90);
    tft.setRotation(3);
    tft.setTextSize(5);
    tft.println(rtc.getTimeStr());
    
    tft.setTextColor(WHITE,BLACK);
    tft.setCursor(75,135);
    tft.setRotation(3);
    tft.setTextSize(3);
    tft.println(rtc.getDateStr());
    
    delay(1000); 
    alarm();
}

void alarm(){
  while(ring)
    tone(buzzer, 1500); // Send 1KHz sound signal...
    delay(150);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(200);        // ...for 1sec
}
// Serial input

//    if (Serial.available() > 0) {
//      int inChar = Serial.read();
//      if (isDigit(inChar)) {
//        // convert the incoming byte to a char and add it to the string:
//        inString += (char)inChar;
//      }
//      // if you get a newline, print the string, then the string's value:
//      if (inChar == '\n') {
//        Serial.print("Value:");
//        Serial.println(inString.toInt());
//        Serial.print("String: ");
//        Serial.println(inString);
//        // clear the string for new input:
//      
//        inString = "";
//      }
//      
//    }


// touch point


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//TSPoint tp;

//    uint16_t xpos, ypos;  //screen coordinates
//    tp = ts.getPoint();   //tp.x, tp.y are ADC values

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
//    pinMode(XM, OUTPUT);
//    pinMode(YP, OUTPUT);
//    pinMode(XP, OUTPUT);
//    pinMode(YM, OUTPUT);

//    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
//        // is controller wired for Landscape ? or are we oriented in Landscape?
//        if (SwapXY != (Orientation & 1)) SWAP(tp.x, tp.y);
//        xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
//        ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
//
//        
//       Serial.print("X = "); Serial.print(tp.x);
//       Serial.print("\tY = "); Serial.print(tp.y);
//       Serial.print("\tPressure = "); Serial.println(tp.z);
//    }
