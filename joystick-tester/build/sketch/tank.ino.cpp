#line 1 "d:\\git\\tank\\joystick-tester\\tank.ino"

#include <Arduino.h>

#line 4 "d:\\git\\tank\\joystick-tester\\tank.ino"
void setup();
#line 12 "d:\\git\\tank\\joystick-tester\\tank.ino"
void loop();
#line 4 "d:\\git\\tank\\joystick-tester\\tank.ino"
void setup()
{
	pinMode(A0, INPUT);

	Serial.begin(115200);
	Serial.println("Ready");
}

void loop()
{
	Input inp = read();

	Serial.print(inp.fwdRev);
	Serial.print(',');
	Serial.print(inp.leftRight);
	Serial.print(',');
	Serial.println(inp.turret);
}

