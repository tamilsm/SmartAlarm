#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
//Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>

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

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//TSPoint tp;

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


void setup(void)
{
    uint16_t tmp;
    tft.begin(9600);

    tft.reset();
    identifier = tft.readID(); // No.
    
    identifier = 0;
    
    name = "ILI9341 DealExtreme";
//    SwapXY = 1;
//    switch (Orientation) {      // adjust for different aspects
//        case 0:   break;        //no change,  calibrated for PORTRAIT
//        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
//        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
//        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
//    }
//
//    Serial.begin(9600);
//    ts = TouchScreen(XP, YP, XM, YM, 300);     //call the constructor AGAIN with new values.
    tft.begin(identifier);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
    tft.println(" COLOR GRADES");
}

void loop()
{
    uint16_t xpos, ypos;  //screen coordinates
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
}
