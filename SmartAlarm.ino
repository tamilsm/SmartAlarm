// defines LCD things
#include <SPI.h>
#include <TFT_ST7735.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define __CS  10
#define __DC  8
#define __RST 9

TFT_ST7735 tft = TFT_ST7735(__CS, __DC, __RST);

#include <DS3231.h>

DS3231  rtc(SDA, SCL);
//DS3231  rtc(12, SCL);

// Init a Time-data structure
Time  t;

// defines pins numbers for UltraSonic
const int trigPin = 6;
const int echoPin = 7;

// defines UltraSonic variables
long duration;
int newDist;

// Accelorororometer
const int xpin = 2; // x-axis of the accelerometer
const int ypin = 3; // y-axis
const int zpin = 4; // z-axis

//Buzz Buzz
const int buzzer = A0; //buzzer to arduino pin 9
bool ring = false;



// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// other stuff
#include "ThingSpeak.h"
#include "WiFiEsp.h"
#include "secrets.h"

char ssid[] = SECRET_SSID;    //  your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(A1, A2); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = 884447;
const char * myWriteAPIKey = "2DT9LZ4U2C1SJYEE";

int number = 0;

String inString = "";    // string to hold input

void setup(void)
{
    //Initialize serial and wait for port to open
    Serial.begin(9600);  
  
    // initialize serial for ESP module  
    setEspBaudRate(ESP_BAUDRATE);
  
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo native USB port only
    }
  
    Serial.print("Searching for ESP8266..."); 
    // initialize ESP module
    WiFi.init(&Serial1);
  
    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue
      while (true);
    }
    Serial.println("found it!");
      
    ThingSpeak.begin(client);  // Initialize ThingSpeak
  
    // Buzzz
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
    
    // UltraSonic
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    rtc.begin(); // Initialize the rtc object
    // The following lines can be uncommented to set the date and time
    rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
    rtc.setTime(1, 0, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(21, 10, 2019);   // Set the date
    
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(BLACK);
//    tft.setCursor(50,100);
//    tft.setRotation(3);
//    tft.setTextSize(5);
//    Serial.println(rtc.getTimeStr()); 
//    tft.println(rtc.getTimeStr());
}

void loop()
{
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  Serial.println(ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey));
  // change the value
  number++;
  if(number > 99){
    number = 0;
  }



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
    newDist = duration * 0.034 / 3;
    Serial.print("Distance: ");

    // Get data from the DS3231
    t = rtc.getTime();
    
    tft.setTextColor(WHITE,BLACK);
    tft.setCursor(18,50);
    tft.setRotation(3);
    tft.setTextSize(2);
    tft.println(rtc.getTimeStr());
    
    tft.setTextColor(WHITE,BLACK);
    tft.setCursor(40,75);
    tft.setRotation(3);
    tft.setTextSize(1);
    tft.println(rtc.getDateStr());

    // print the sensor values:
    Serial.print((analogRead(xpin)- 331.5)/65*9.8);
    // print a tab between values:
    Serial.print("\t");
    Serial.print((analogRead(ypin)- 329.5)/68.5*9.8);
    // print a tab between values:
    Serial.print("\t");
    Serial.print((analogRead(zpin)- 340)/68*9.8);
    Serial.println();
    
    char* alarmTime = "12:00:05";
    char* timie = rtc.getTimeStr();
    if (strcmp(timie,alarmTime)==0){
      Serial.println("heree");
      ring = true;
      tft.setTextColor(BLUE,BLACK);
      tft.setCursor(13,18);
      tft.setRotation(3);
      tft.setTextSize(3);
      tft.println("SNOOZE");
    }
    if (newDist < 3){
      ring = false;
      tft.setTextColor(BLACK,BLACK);
      tft.setCursor(13,18);
      tft.setRotation(3);
      tft.setTextSize(3);
      tft.println("SNOOZE");
    }
    if(ring){
      Serial.println("asdfasdf");
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(70);        
      noTone(buzzer);     // Stop sound...
      delay(70);    
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(70);        
      noTone(buzzer);     // Stop sound...
      delay(70); 
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(70);        
      noTone(buzzer);     // Stop sound...
      delay(70);    
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(70);        
      noTone(buzzer);     // Stop sound...
      delay(70); 
    }
    delay(500); 
}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial1.begin(baudrate);
}
