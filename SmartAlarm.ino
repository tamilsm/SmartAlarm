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

const int hour = 10;
const int mins = 05;
char* alarmTime = "10:03:00";


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

const int numReadings = 3;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int oldAvg = 0;

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
  rtc.setTime(hour, mins, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(21, 10, 2019);   // Set the date
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
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

  float mag = sqrt(x*x + y*y + z*z);

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = mag;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(abs(oldAvg-average));
  oldAvg = average;
  
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
