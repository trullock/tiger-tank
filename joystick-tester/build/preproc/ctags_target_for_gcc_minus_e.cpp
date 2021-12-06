# 1 "d:\\git\\tank\\joystick-tester\\tank.ino"

# 3 "d:\\git\\tank\\joystick-tester\\tank.ino" 2

void setup()
{
 pinMode(A0, 0x0);

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
