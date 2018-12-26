//LOOP

uint32_t timer = millis();

void CheckGPS() {
	if (GPS.newNMEAreceived()) {
		// a tricky thing here is if we print the NMEA sentence, or data
		// we end up not listening and catching other sentences!
		// so be very wary if using OUTPUT_ALLDATA and trytng to print out data
		//Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

		if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return;  // we can fail to parse a sentence in which case we should just wait for another
		Serial.print("\nTime: ");
		Serial.print(GPS.hour, DEC); Serial.print(':');
		Serial.print(GPS.minute, DEC); Serial.print(':');
		Serial.print(GPS.seconds, DEC); Serial.print('.');
		Serial.println(GPS.milliseconds);
		Serial.print("Date: ");
		Serial.print(GPS.day, DEC); Serial.print('/');
		Serial.print(GPS.month, DEC); Serial.print("/20");
		Serial.println(GPS.year, DEC);
		Serial.print("Fix: "); Serial.print((int)GPS.fix);
		Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
		if (GPS.fix) {
			Serial.print("Location: ");
			Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
			Serial.print(", ");
			Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

			Serial.print("Speed (km): "); Serial.println(GPS.speed*1.852);
			Serial.print("Speed (knots): "); Serial.println(GPS.speed);
			Serial.print("Angle: "); Serial.println(GPS.angle);
			Serial.print("Altitude: "); Serial.println(GPS.altitude);
			Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
		}
	}

	genie.DoEvents(); // This calls the library each loop to process the queued responses from the display
					  // Do your other application code in here as required

	  //@Stijn, waarom moet je die elke loop updaten? Zeker dag, maand en zo hoeft niet elke loop
	  //@David dit moet niet voor alles, moet enkel als er iets veranderd 
	genie.WriteStr(0, GPS.day);
	genie.WriteStr(1, GPS.month);
	genie.WriteStr(2, GPS.year);

	genie.WriteStr(3, (GPS.hour + n));
	genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x01, (GPS.hour + n));
	genie.WriteStr(4, GPS.minute);
	genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x02, GPS.minute);

	uint32_t speedAVG = average(GPS.speed, 1);
	uint32_t angleAVG = average(GPS.angle, 2);
	//genie.WriteStr(5, (GPS.speed*1,852));
	genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x04, (GPS.speed*1.852) * 10);
	//genie.WriteStr(5, round(speedAVG*10)/10);
	//genie.WriteStr(6, round(angleAVG*10)/10);
	genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, GPS.angle * 10); //*10 voor goede weergave op scherm
	//genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, round(angleAVG*10)/10);
	genie.WriteObject(GENIE_OBJ_GAUGE, 0x00, (int)GPS.satellites);
	genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x03, (int)GPS.satellites);
	delay(500);
}

void myGenieEventHandler(void) {
	genieFrame Event;
	genie.DequeueEvent(&Event);
	
	Serial.println("Event received:");
	Serial.println(String(Event.reportObject.cmd));
	Serial.println(String(Event.reportObject.object));
	Serial.println(String(Event.reportObject.index));

	//Relay stukje
	if (Event.reportObject.cmd == GENIE_REPORT_EVENT) {
		// If the Event is from a 4D Button object
		if (Event.reportObject.object == GENIE_OBJ_4DBUTTON)
		{
			if (Event.reportObject.index == 0) // If 4DButton0
			{
				if (genie.GetEventData(&Event) == 1)
				{ // if 4DButton0 has been Toggled On
					digitalWrite(Relay_1, LOW);  // set the Relay ON
				}
				else if (genie.GetEventData(&Event) == 0)
				{ // if 4Dbutton0 has been Toggled Off
					digitalWrite(Relay_1, HIGH);  // set the Relay OFF
				}
			}

			else if (Event.reportObject.index == 1) // If 4DButton1
			{
				if (genie.GetEventData(&Event) == 1)
				{ // if 4DButton1 has been Toggled On
					digitalWrite(Relay_2, LOW); // set the Relay ON
				}
				else if (genie.GetEventData(&Event) == 0)
				{ // if 4Dbutton1 has been Toggled Off
					digitalWrite(Relay_2, HIGH);  // set the Relay OFF
				}
			}

			else if (Event.reportObject.index == 2) // If 4DButton2
			{
				if (genie.GetEventData(&Event) == 1)
				{ // if 4DButton2 has been Toggled On
					digitalWrite(Relay_3, LOW); // set the Relay ON
				}
				else if (genie.GetEventData(&Event) == 0)
				{ // if 4Dbutton2 has been Toggled Off
					digitalWrite(Relay_3, HIGH);  // set the Relay OFF
				}
			}

			else if (Event.reportObject.index == 3) // If 4DButton3
			{
				if (genie.GetEventData(&Event) == 1)
				{ // if 4DButton3 has been Toggled On
					digitalWrite(Relay_4, LOW); // set the Relay ON
				}
				else if (genie.GetEventData(&Event) == 0)
				{ // if 4Dbutton3 has been Toggled Off
					digitalWrite(Relay_4, HIGH);  // set the Relay OFF
				}
			}

		}
		//Stop makenoise
		if (Event.reportObject.object == GENIE_OBJ_USERBUTTON) {
			if (Event.reportObject.index == 0) {
				Serial.println("Sound button pressed");
				AlarmSound = false;
			}
		}

		//ScreenSaver code
		ssTimeStart = millis();
		ssActiveFlag = false;
		//if (Event.reportObject.object == GENIE_OBJ_4DBUTTON) {}
		  // do something here
		if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
			newFormFlag = true;
		//else if(Event.reportObject.object == GENIE_OBJ_SLIDER){}
		  // do something here
		else if (Event.reportObject.object == GENIE_OBJ_FORM) {
			newFormFlag = true;
			if (Event.reportObject.index == 0)       // If Form0
				activeForm = 0;
			else if (Event.reportObject.index == 1)  // If Form1
				activeForm = 1;
			else if (Event.reportObject.index == 2)  // If Form2
				activeForm = 2;
			else if (Event.reportObject.index == 3)  // If Form2
				activeForm = 3;
			else if (Event.reportObject.index == 4)  // If Form2
				activeForm = 4;
			else if (Event.reportObject.index == 6)  // If Form2
				activeForm = 6;
		}

		//keyboard stukje

		if (Event.reportObject.object == GENIE_OBJ_KEYBOARD)
		{
			if (Event.reportObject.index == 0) // If keyboard0
			{
				temp = genie.GetEventData(&Event);
				if (temp >= 48 && temp <= 110 && counter <= 9) // Recognize only number 1-9 change this to 48-122 if using a QWERTY Keyboard
				{
					keyvalue[counter] = temp; // Receive the event data from the keyboard
					genie.WriteStr(7, keyvalue); //prints to String Object
				}
				else if (temp == 8) //check if backspace
				{
					counter--;
					keyvalue[counter] = 0;
					genie.WriteStr(7, keyvalue);
				}
				else if (temp == 13)
				{
					Serial.println(keyvalue); //so you can see the captured string
				}
				Serial.println("DONE");
				n = atof(keyvalue);
				Serial.println(n);
			}
		}
	}
}

int InitialAlarm = false;

void CheckBilges() {

	static long waitPeriod = millis();
	if (millis() >= waitPeriod) {

		bilgesStatus1 = digitalRead(bilgesPin1);
		bilgesStatus2 = digitalRead(bilgesPin2);
		bilgesStatus3 = digitalRead(bilgesPin3);

		// bilges 1
		if (bilgesStatus1 == HIGH) {
			if (activity == 0) {
				if (millis() >= waitPeriod) {
					genie.WriteObject(GENIE_OBJ_FORM, 2, 0);
					//genie.WriteObject(GENIE_OBJ_SOUND, 0, 0);
					genie.WriteObject(GENIE_OBJ_USER_LED, 0x00, 1);
					activity = 1;
					if (InitialAlarm == false) {
						InitialAlarm = true;
						AlarmSound = true;
					}
				}
			}
		}

		// bilges 2
		else if (bilgesStatus2 == HIGH) {
			if (activity == 0) {
				if (millis() >= waitPeriod) {
					genie.WriteObject(GENIE_OBJ_FORM, 2, 0);
					//genie.WriteObject(GENIE_OBJ_SOUND, 0, 0);
					genie.WriteObject(GENIE_OBJ_USER_LED, 0x01, 1);
					activity = 1;
					if (InitialAlarm == false) {
						InitialAlarm = true;
						AlarmSound = true;
					}
				}
			}
		}
		// bilges 3
		else if (bilgesStatus3 == HIGH) {
			if (activity == 0) {
				if (millis() >= waitPeriod) {
					genie.WriteObject(GENIE_OBJ_FORM, 2, 0);
					//genie.WriteObject(GENIE_OBJ_SOUND, 0, 0);
					genie.WriteObject(GENIE_OBJ_USER_LED, 0x02, 1);
					activity = 1;
					if (InitialAlarm == false) {
						InitialAlarm = true;
						AlarmSound = true;
					}
				}
			}
		}
		else {
			AlarmSound = false;
			InitialAlarm = false;
			genie.WriteObject(GENIE_OBJ_USER_LED, 0x00, 0);
			genie.WriteObject(GENIE_OBJ_USER_LED, 0x01, 0);
			genie.WriteObject(GENIE_OBJ_USER_LED, 0x02, 0);
			genie.WriteObject(GENIE_OBJ_SOUND, 0, 4);
			activity = 0;
		}
	}
}

void CheckPower() {
	value0 = analogRead(analogInput0); // read the value at analog input
	value1 = analogRead(analogInput1);
	vout0 = (value0 * 5) / 1023.0;
	vout1 = (value1 * 5) / 1023.0;
	vin0 = vout0 / (R2 / (R1 + R2));
	vin1 = vout1 / (R2 / (R1 + R2));
	if (vin0 < 0.09) {
		vin0 = 0.0;//statement to quash undesired reading !
	}
	if (vin1 < 0.09) {
		vin1 = 0.0;//statement to quash undesired reading !
	}
	genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 0x00, (vin0 * 10)); //write to Coolgauge0 the value of vin 
	genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 0x01, (vin1 * 10));//0x01 1 is de meter nummer
}

uint32_t SpeedAVG = 0;
uint32_t AngleAVG = 0;
int Interval = 100;

uint32_t average(uint32_t input, int variable) { //variable: 1 for Speed, 2 for Angle
	uint32_t AVG = 0;
	uint32_t PreviousData;

	switch (variable) {
	case 1:
		PreviousData = SpeedAVG;
		break;

	case 2:
		PreviousData = AngleAVG;
		break;
	}

	AVG = ((Interval - 1) * PreviousData + input) / Interval;

	switch (variable) {
	case 1:
		SpeedAVG = AVG;
		break;

	case 2:
		AngleAVG = AVG;
	}
	return AVG;
}

void CheckScreensaver() {
	static long waitPeriod = millis();
	if (millis() >= waitPeriod) {
		ssTimeElapsed = millis() - ssTimeStart;
		//Serial.print("time elapsed: ");   
		//Serial.println(ssTimeElapsed/1000);
		//Serial.print("active form: ");
		//Serial.println(activeForm);
		//Serial.print("new form flag: ");
		//Serial.println(newFormFlag);
		//Serial.print("\n");

		if (ssTimeElapsed > ssTimeout) {
			if (ssActiveFlag == false) {
				genie.WriteContrast(0);  //set the brightness to 0
				genie.WriteObject(GENIE_OBJ_FORM, 0x05, 0x00);  // make the display navigate to form 5                      
				ssActiveFlag = true;
			}
		}
		else {
			if (activeForm == 0) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x00, 0x00);
					newFormFlag = false;
				}
			}
			else if (activeForm == 1) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x01, 0x00);
					newFormFlag = false;
				}
			}
			else if (activeForm == 2) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x02, 0x00);
					newFormFlag = false;
				}
			}
			else if (activeForm == 3) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x03, 0x00);
					newFormFlag = false;
				}
			}
			else if (activeForm == 4) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x04, 0x00);
					newFormFlag = false;
				}
			}
			else if (activeForm == 6) {
				if (newFormFlag == true) {
					genie.WriteObject(GENIE_OBJ_FORM, 0x06, 0x00);
					newFormFlag = false;
				}
			}
			genie.WriteContrast(15);
		}
		waitPeriod = millis() + 100; // rerun this code in another 100 ms time.
	}
}

long timeout = 20000;
long timerstart = 0;

void MakeNoise() {
	if (millis() - timerstart >= timeout) {
		timerstart = millis();
		genie.WriteObject(GENIE_OBJ_SOUND, 0, 0);
	}
}
