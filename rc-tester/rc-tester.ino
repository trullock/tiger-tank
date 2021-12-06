void setup()
{
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(9, INPUT);
	//pinMode(A4, INPUT);
	//pinMode(A5, INPUT);

	Serial.begin(115200);
	Serial.println("Ready");
}


void loop()
{
	Serial.print(pulseIn(2, HIGH));
	Serial.print(',');
	Serial.print(pulseIn(3, HIGH));
	Serial.print(',');
	Serial.print(pulseIn(4, HIGH));
	
	Serial.print(',');
	Serial.print(pulseIn(7, HIGH));
	Serial.print(',');
	Serial.print(pulseIn(8, HIGH));
	Serial.print(',');
	Serial.println(pulseIn(9, HIGH));
	return;

	Serial.print(',');
	Serial.print(analogRead(A4));
	Serial.print(',');
	Serial.println(analogRead(A5));
}
