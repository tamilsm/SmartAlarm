/*
*/
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
const int xpin = A1; // x-axis of the accelerometer
const int ypin = A2; // y-axis
const int zpin = A3; // z-axis

int x = 0;
int y = 0;
int z = 0;

float sum = 0;
int count = 0;
int maxCount = 5;
float avg = 0;
float oldAvg = 100;

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

void setup() {
  //Initialize serial and wait for port to open
  Serial.begin(9600);

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
}

void loop() {

  //Serial.print("Time:  ");
  //Serial.println(rtc.getTimeStr());
  
  //Serial.print("Date: ");
  //Serial.println(rtc.getDateStr());

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
//  Serial.print("Distance: ");
//  Serial.println(newDist);

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

  // the sensor values:
  x = abs(analogRead(xpin));
  y = abs(analogRead(ypin));
  z = abs(analogRead(zpin));
  
//  Serial.print(x);
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(y);
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(z);
//  Serial.println();
  float mag = sqrt(x*x + y*y + z*z);
//  Serial.println(mag);

  if (count < maxCount){
    sum = sum + mag;
    count = count + 1;
  }
  else{
    count = 0;
    avg = sum/maxCount;
    sum = 0;
    Serial.println(abs(avg-oldAvg));
    oldAvg = avg;
  }
  
//  // print the sensor values:
//  Serial.print(analogRead(xpin));
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(analogRead(ypin));
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(analogRead(zpin));
//  Serial.println();
  
  char* alarmTime = "12:00:05";
  char* timie = rtc.getTimeStr();
  if (strcmp(timie,alarmTime)==0){
    //Serial.println("heree");
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
    //Serial.println("asdfasdf");
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
