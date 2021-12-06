# 1 "d:\\git\\tank\\firmware\\firmware.ino"
# 2 "d:\\git\\tank\\firmware\\firmware.ino" 2
# 3 "d:\\git\\tank\\firmware\\firmware.ino" 2
# 4 "d:\\git\\tank\\firmware\\firmware.ino" 2
# 5 "d:\\git\\tank\\firmware\\firmware.ino" 2

// Wire up the SPI Interface common lines:
// #define SPI_CLOCK            13 //arduino   <->   SPI Slave Clock Input     -> SCK (Pin 02 on MCP4261 DIP)
// #define SPI_MOSI             11 //arduino   <->   SPI Master Out Slave In   -> SDI (Pin 03 on MCP4261 DIP)
// #define SPI_MISO             12 //arduino   <->   SPI Master In Slave Out   -> SDO (Pin 13 on MCP4261 DIP)



// Its recommended to measure the rated end-end resistance (terminal A to terminal B)
// Because this can vary by a large margin, up to -+ 20%. And temperature variations.
float rAB_ohms = 945.00; // 1k Ohm

// Instantiate Mcp4261 object, with default rW (=117.5 ohm, its typical resistance)
MCP4261 Mcp4261 = MCP4261( 5 /*arduino   <->   Chip Select               -> CS  (Pin 01 on MCP4261 DIP)*/, rAB_ohms );

// rW - Wiper resistance. This is a small additional constant. To measure it
// use the example, setup(). Required for accurate calculations (to nearest ohm)
// Datasheet Page 5, gives typical values MIN=75ohm, MAX @5v=160ohm,@2.7v=300ohm
// Usually rW should be somewhere between 100 and 150 ohms.
// Instantiate Mcp4261 object, after measuring the real rW wiper resistance
// MCP4261 Mcp4261 = MCP4261( MCP4261_SLAVE_SELECT_PIN, rAB_ohms, rW_ohms );
# 63 "d:\\git\\tank\\firmware\\firmware.ino"
void setup()
{
 // First measure the the wiper resistance, called rW
 //Mcp4261.wiper0_pos(0); // rAW = rW_ohms
 //Mcp4261.wiper1_pos(0); // rAW = rW_ohms
 //delay(5000);

 // (optional)
 // Scale to 100.0 for a percentage, or 1.0 for a fraction
 // Eg if scale=100, then wiper(100) = max rAW resistance
 // Eg    scale=1.0, then wiper(1.0) = max rAW resistance
 Mcp4261.scale = 1.0;

 //Mcp4261.scale = 100.0; // For the timeout example, below

 // Go back to using ohms
 // Mcp4261.scale = Mcp4261.rAB_ohms;

 // SETUP PWM Input pins
 pinMode(2, 0x0);
 pinMode(3, 0x0);
 pinMode(4, 0x0);

 pinMode(A4, 0x0);
 pinMode(A5, 0x0);

 Serial.begin(115200);
 Serial.println("Ready");
}

Input read()
{
 Input inp;

 float fwdRevPwm = pulseIn(2, 0x1);
 // cap min and max, and add noise buffer around midpoint
 fwdRevPwm = ((((fwdRevPwm)<(1800)?(fwdRevPwm):(1800)))>(950)?(((fwdRevPwm)<(1800)?(fwdRevPwm):(1800))):(950));
 if (fwdRevPwm >= 1375 - 50 && fwdRevPwm <= 1375 + 50)
  fwdRevPwm = 1375;
 inp.fwdRev = (fwdRevPwm - 950) / (1800 - 950);

 float leftRightPwm = pulseIn(3, 0x1);
 // cap min and max, and add noise buffer around midpoint
 leftRightPwm = ((((leftRightPwm)<(1800)?(leftRightPwm):(1800)))>(950)?(((leftRightPwm)<(1800)?(leftRightPwm):(1800))):(950));
 if (leftRightPwm >= 1375 - 50 && leftRightPwm <= 1375 + 50)
  leftRightPwm = 1375;
 inp.leftRight = (leftRightPwm - 950) / (1800 - 950);

 // read and stabilise throttle PWM
 float turretPwm = pulseIn(4, 0x1);
 turretPwm = ((((turretPwm)<(1850)?(turretPwm):(1850)))>(950)?(((turretPwm)<(1850)?(turretPwm):(1850))):(950));
 if (turretPwm >= 1400 - 50 && turretPwm <= 1400 + 50)
  turretPwm = 1400;
 inp.turret = (turretPwm - 950) / (1850 - 950);

 // Read and stabilise Joystick F/R axis
 float joyFwdRev = analogRead(A4);
 joyFwdRev = ((((joyFwdRev)>(235)?(joyFwdRev):(235)))<(790)?(((joyFwdRev)>(235)?(joyFwdRev):(235))):(790));
 if (joyFwdRev >= 513 - 10 && joyFwdRev <= 513 + 10)
  joyFwdRev = 513;
 joyFwdRev = (joyFwdRev - 235) / (790 - 235);
 inp.joyFwdRed = joyFwdRev;

 // Read and stabilise Joystick L/R axis
 float joyLeftRight = analogRead(A5);
 joyLeftRight = ((((joyLeftRight)>(235)?(joyLeftRight):(235)))<(790)?(((joyLeftRight)>(235)?(joyLeftRight):(235))):(790));
 if (joyLeftRight >= 517 - 10 && joyLeftRight <= 517 + 10)
  joyLeftRight = 517;
 joyLeftRight = (joyLeftRight - 235) / (790 - 235);
 inp.joyLeftRight = joyLeftRight;

 return inp;
}

void write(Input inp) {

 // map range % to output value
 float fwdRev = (1.42 - 0.45) * inp.fwdRev + 0.45;
 Mcp4261.wiper0_pos(fwdRev);

 // map range % to output value
 float leftRight = (1.42 - 0.45) * inp.leftRight + 0.45;
 Mcp4261.wiper1_pos(leftRight);
}

void loop()
{
 Input inp = read();

 Serial.print(inp.fwdRev);
 Serial.print(',');
 Serial.print(inp.leftRight);
 Serial.print(',');
 Serial.print(inp.turret);
 Serial.print(',');
 Serial.print(inp.joyFwdRed);
 Serial.print(',');
 Serial.println(inp.joyLeftRight);

 //write(inp);
}
