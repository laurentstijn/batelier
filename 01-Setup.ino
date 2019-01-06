
SIGNAL(TIMER0_COMPA_vect) {
	char c = GPS.read();
	// if you want to debug, this is a good time to do it!
#ifdef UDR0
	if (GPSECHO)
		if (c) UDR0 = c;
	// writing direct to UDR0 is much much faster than Serial.print
	// but only one character can be written at a time.
#endif
}

//SETUP
//@Stijn, van waar komt deze code, is dat voor de gps of voor 't display?
//@David idd van de GPS
boolean usingInterrupt = false;
void useInterrupt(boolean);

void useInterrupt(boolean v) {
	if (v) {
		// Timer0 is already used for millis() - we'll just interrupt somewhere
		// in the middle and call the "Compare A" function above
		OCR0A = 0xAF;
		TIMSK0 |= _BV(OCIE0A);
		usingInterrupt = true;
	}
	else {
		// do not call the interrupt function COMPA anymore
		TIMSK0 &= ~_BV(OCIE0A);
		usingInterrupt = false;
	}
}

void GPSStart() {
	//Serial.println("Adafruit GPS library basic test!");
	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	useInterrupt(true);
	delay(1000);
	// Ask for firmware version
	mySerial.println(PMTK_Q_RELEASE);
}

void DisplayStart() {
	genie.Begin(Serial1);   // Use Serial0 for talking to the Genie Library, and to the 4D Systems display

	genie.AttachEventHandler(myGenieEventHandler); // Attach the user function Event Handler for processing events

	pinMode(RESETLINE, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
	digitalWrite(RESETLINE, 1);  // Reset the Display via D4
	delay(100);
	digitalWrite(RESETLINE, 0);  // unReset the Display via D4

	delay(3500); //let the display start up after the reset (This is important)

	//Turn the Display on (Contrast)
	genie.WriteContrast(15); //For uLCD43, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, where 0 = Display OFF, though to 15 = Max Brightness ON.
	//Timer voor screenSaver
	ssTimeStart = millis();
}

void BilgesStart() {
	//Bilges pinnen
	pinMode(bilgesPin1, INPUT_PULLUP);
	pinMode(bilgesPin2, INPUT_PULLUP);
	pinMode(bilgesPin3, INPUT_PULLUP);
}

void RelaysStart() {
	//Relay pinnen
	pinMode(Relay_1, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_1, HIGH);
	pinMode(Relay_2, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_2, HIGH);
	pinMode(Relay_3, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_3, HIGH);
	pinMode(Relay_4, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_4, HIGH);
	pinMode(Relay_5, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_5, HIGH);
	pinMode(Relay_6, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_6, HIGH);
	pinMode(Relay_7, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_7, HIGH);
	pinMode(Relay_8, OUTPUT);      // sets the digital pin as output
	digitalWrite(Relay_8, HIGH);
}

void VoltmeterStart() {
	pinMode(analogInput0, INPUT);
	pinMode(analogInput1, INPUT);
}
