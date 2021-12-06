#include <Arduino.h>
#include <SPI.h>
#include "MCP4261.h"
#include "input.h"

// Wire up the SPI Interface common lines:
// #define SPI_CLOCK            13 //arduino   <->   SPI Slave Clock Input     -> SCK (Pin 02 on MCP4261 DIP)
// #define SPI_MOSI             11 //arduino   <->   SPI Master Out Slave In   -> SDI (Pin 03 on MCP4261 DIP)
// #define SPI_MISO             12 //arduino   <->   SPI Master In Slave Out   -> SDO (Pin 13 on MCP4261 DIP)

#define MCP4261_SLAVE_SELECT_PIN 5 //arduino   <->   Chip Select               -> CS  (Pin 01 on MCP4261 DIP)

// Its recommended to measure the rated end-end resistance (terminal A to terminal B)
// Because this can vary by a large margin, up to -+ 20%. And temperature variations.
float rAB_ohms = 945.00; // 1k Ohm

// Instantiate Mcp4261 object, with default rW (=117.5 ohm, its typical resistance)
MCP4261 Mcp4261 = MCP4261( MCP4261_SLAVE_SELECT_PIN, rAB_ohms );

// rW - Wiper resistance. This is a small additional constant. To measure it
// use the example, setup(). Required for accurate calculations (to nearest ohm)
// Datasheet Page 5, gives typical values MIN=75ohm, MAX @5v=160ohm,@2.7v=300ohm
// Usually rW should be somewhere between 100 and 150 ohms.
// Instantiate Mcp4261 object, after measuring the real rW wiper resistance
// MCP4261 Mcp4261 = MCP4261( MCP4261_SLAVE_SELECT_PIN, rAB_ohms, rW_ohms );

#define FR_PWM_PIN 2
#define LR_PWM_PIN 3
#define TURRET_PWM_PIN 4

#define JOY_FR_PIN A4
#define JOY_LR_PIN A5

#define JOY_FR_MAX 790
#define JOY_FR_MIN 235
#define JOY_FR_MID 513
#define JOY_FR_MID_TOLERANCE 10

#define JOY_LR_MAX 790
#define JOY_LR_MIN 235
#define JOY_LR_MID 517
#define JOY_LR_MID_TOLERANCE 10

#define FR_PWM_MAX 1800
#define FR_PWM_MIN 950
#define FR_PWM_MID 1375
#define FR_PWM_MID_TOLERANCE 50
#define FR_OUT_MIN 0.45
#define FR_OUT_MAX 1.42

#define LR_PWM_MAX 1800
#define LR_PWM_MIN 950
#define LR_PWM_MID 1375
#define LR_PWM_MID_TOLERANCE 50
#define LR_OUT_MIN 0.45
#define LR_OUT_MAX 1.42

#define TURRET_PWM_MAX 1850
#define TURRET_PWM_MIN 950
#define TURRET_PWM_MID 1400
#define TURRET_PWM_MID_TOLERANCE 50

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
	pinMode(FR_PWM_PIN, INPUT);
	pinMode(LR_PWM_PIN, INPUT);
	pinMode(TURRET_PWM_PIN, INPUT);

	pinMode(JOY_FR_PIN, INPUT);
	pinMode(JOY_LR_PIN, INPUT);

	Serial.begin(115200);
	Serial.println("Ready");
}

Input read()
{
	Input inp;

	float fwdRevPwm = pulseIn(FR_PWM_PIN, HIGH);
	// cap min and max, and add noise buffer around midpoint
	fwdRevPwm = max(min(fwdRevPwm, FR_PWM_MAX), FR_PWM_MIN);
	if (fwdRevPwm >= FR_PWM_MID - FR_PWM_MID_TOLERANCE && fwdRevPwm <= FR_PWM_MID + FR_PWM_MID_TOLERANCE)
		fwdRevPwm = FR_PWM_MID;
	inp.fwdRev = (fwdRevPwm - FR_PWM_MIN) / (FR_PWM_MAX - FR_PWM_MIN);

	float leftRightPwm = pulseIn(LR_PWM_PIN, HIGH);
	// cap min and max, and add noise buffer around midpoint
	leftRightPwm = max(min(leftRightPwm, LR_PWM_MAX), LR_PWM_MIN);
	if (leftRightPwm >= LR_PWM_MID - LR_PWM_MID_TOLERANCE && leftRightPwm <= LR_PWM_MID + LR_PWM_MID_TOLERANCE)
		leftRightPwm = LR_PWM_MID;
	inp.leftRight = (leftRightPwm - LR_PWM_MIN) / (LR_PWM_MAX - LR_PWM_MIN);

	// read and stabilise throttle PWM
	float turretPwm = pulseIn(TURRET_PWM_PIN, HIGH);
	turretPwm = max(min(turretPwm, TURRET_PWM_MAX), TURRET_PWM_MIN);
	if (turretPwm >= TURRET_PWM_MID - TURRET_PWM_MID_TOLERANCE && turretPwm <= TURRET_PWM_MID + TURRET_PWM_MID_TOLERANCE)
		turretPwm = TURRET_PWM_MID;
	inp.turret = (turretPwm - TURRET_PWM_MIN) / (TURRET_PWM_MAX - TURRET_PWM_MIN);

	// Read and stabilise Joystick F/R axis
	float joyFwdRev = analogRead(JOY_FR_PIN);
	joyFwdRev = min(max(joyFwdRev, JOY_FR_MIN), JOY_FR_MAX);
	if (joyFwdRev >= JOY_FR_MID - JOY_FR_MID_TOLERANCE && joyFwdRev <= JOY_FR_MID + JOY_FR_MID_TOLERANCE)
		joyFwdRev = JOY_FR_MID;
	joyFwdRev = (joyFwdRev - JOY_FR_MIN) / (JOY_FR_MAX - JOY_FR_MIN);
	inp.joyFwdRed = joyFwdRev;

	// Read and stabilise Joystick L/R axis
	float joyLeftRight = analogRead(JOY_LR_PIN);
	joyLeftRight = min(max(joyLeftRight, JOY_LR_MIN), JOY_LR_MAX);
	if (joyLeftRight >= JOY_LR_MID - JOY_LR_MID_TOLERANCE && joyLeftRight <= JOY_LR_MID + JOY_LR_MID_TOLERANCE)
		joyLeftRight = JOY_LR_MID;
	joyLeftRight = (joyLeftRight - JOY_LR_MIN) / (JOY_LR_MAX - JOY_LR_MIN);
	inp.joyLeftRight = joyLeftRight;

	return inp;
}

void write(Input inp) {

	// map range % to output value
	float fwdRev = (FR_OUT_MAX - FR_OUT_MIN) * inp.fwdRev + FR_OUT_MIN;
	Mcp4261.wiper0_pos(fwdRev);

	// map range % to output value
	float leftRight = (LR_OUT_MAX - LR_OUT_MIN) * inp.leftRight + LR_OUT_MIN;
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
