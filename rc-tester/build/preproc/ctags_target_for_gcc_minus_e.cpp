# 1 "d:\\git\\tank\\rc-tester\\rc-tester.ino"
void setup()
{
 pinMode(2, 0x0);
 pinMode(3, 0x0);
 pinMode(4, 0x0);
 pinMode(7, 0x0);
 pinMode(8, 0x0);
 pinMode(9, 0x0);
 //pinMode(A4, INPUT);
 //pinMode(A5, INPUT);

 Serial.begin(115200);
 Serial.println("Ready");
}


void loop()
{
 Serial.print(pulseIn(2, 0x1));
 Serial.print(',');
 Serial.print(pulseIn(3, 0x1));
 Serial.print(',');
 Serial.print(pulseIn(4, 0x1));

 Serial.print(',');
 Serial.print(pulseIn(7, 0x1));
 Serial.print(',');
 Serial.print(pulseIn(8, 0x1));
 Serial.print(',');
 Serial.println(pulseIn(9, 0x1));
 return;

 Serial.print(',');
 Serial.print(analogRead(A4));
 Serial.print(',');
 Serial.println(analogRead(A5));
}
