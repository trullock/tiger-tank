
#include <Arduino.h>

void setup()
{
	pinMode(A1, INPUT);

	Serial.begin(115200);
	Serial.println("Ready");
}

void loop()
{
	int val = analogRead(A1)

	Serial.println(val);
}
