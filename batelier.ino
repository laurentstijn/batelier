//Libs
#include <genieArduino.h>
#include <Adafruit_GPS.h>
#include <math.h>

//Create GPS instance
HardwareSerial mySerial = Serial3;
//HardwareSerial lcdSerial = Serial1;
Adafruit_GPS GPS(&Serial3);
#define GPSECHO  false

//Create instance for touch display
Genie genie;

//Pinnumbers
//int sensorPin = A0;  // the wiper of the trimmer potentiometer is connected to pin A0
int sensorPin1 = A1; // Pin voor optionele 2de voltmeter
#define RESETLINE 4  // Using Arduino Adaptor Shield Version 2.0 @Stijn, is dat ook voor 't display?? @David idd voor de display
#define Relay_1  22  // On Arduino Mega pin number to relay 1
#define Relay_2  23  // On Arduino Mega pin number to relay 2
#define Relay_3  24  // On Arduino Mega pin number to relay 3
#define Relay_4  25  // On Arduino Mega pin number to relay 4
#define Relay_5  26  // On Arduino Mega pin number to relay 5
#define Relay_6  27  // On Arduino Mega pin number to relay 6
#define Relay_7  28  // On Arduino Mega pin number to relay 7
#define Relay_8  29  // On Arduino Mega pin number to relay 8
int bilgesPin1 = 30;  // bilges pin D2
int bilgesPin2 = 31;  // bilges pin D2
int bilgesPin3 = 32;  // bilges pin D2

//Globals
int voltMeter;       // holds the digital voltage value to be sent to the angular meter
int voltMeter1;
int voltLED;         // holds the digital voltage value to be sent to the LED digits
int voltLED1;
int bilgesStatus1;   // Bilges status = read pin D2
int bilgesStatus2;   // Bilges status = read pin D2
int bilgesStatus3;   // Bilges status = read pin D2
int activity;
int Timezone = 2;

//Globals screensaver
boolean  newFormFlag = true;    // goes high when a new form has just been activated. Initially true.
boolean  ssActiveFlag = false;  // Screensaver status. Initially the screen is active.
long int ssTimeout = 30000;   // The screen goes off in 30 seconds. 
							  // Maximum is 32767 seconds (for the LED digits on the display)
long int ssTimeStart;
long int ssTimeElapsed;
int activeForm = 0;

//Globals voltmeter
int analogInput0 = 0;
int analogInput1 = 1;
float vout0 = 0.0;
float vout1 = 0.0;
float vin0 = 0.0;
float vin1 = 0.0;
float R1 = 10000.0; // resistance of R1 (10K) -see text!
float R2 = 1000.0; // resistance of R2 (1K) - see text!
int value0 = 0;
int value1 = 0;
//Globals Keyboard
char keyvalue[2]; // array to hold keyboard values
int n;
int temp;
int counter = 0;
float timezone = n;

void setup()
{
	Serial.begin(115200);  // Serial0 @ 9600 Baud @Stijn: wat wil je gebruiken? 9600 of 115200? :-)
	Serial1.begin(115200); // Serial1 voor comunicatie scherm, standaard is Serial0 maar geeft veel shit in serial monitor
	DisplayStart();
	GPSStart();
	BilgesStart();
	RelaysStart();
	VoltmeterStart();
}

void loop()
{
	CheckGPS();
	CheckBilges();
	CheckPower();
	CheckScreensaver();
}
