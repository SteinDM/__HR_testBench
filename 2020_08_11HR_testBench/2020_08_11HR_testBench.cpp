/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.
 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"
/* The value for calibration_factor is obtained using the SparkFun_HX711_Calibration sketch hashed out at the bottom
 *
 *
 * ----------------
 * 10 kg element HR testbench
 * ----------------
 * calibration_factor 241.74
 * [reference weight( x9.81 = reference force ) -> measurement with cal. fact (*100 = readout)]
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - -
 * 2685gr (26.34N) -> 26.331 N measured;
 * 5076gr (49.766N)-> 49.78N measured ;
 * 38gr (3.728N)-> 3.7N measured;
 * 2149.4gr(21.0856N) -> 21.07N measured
 * 5165.6gr(50.67453N) -> 50.67N measured
 *
 *
 *
 * ----------------
 * 10 kg element TSDZ test bench
 * ----------------
 * calibration_factor 245.94
 * [reference weight( x9.81 = reference force ) -> measurement with cal. fact (*100 = readout)]
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - -
 * 2149.4gr(21.0856N) -> 21.098N measured
 * 5165.6gr(50.67453N) -> 50.665N measured
 *
 * --------------------
 * 5 kg element
 * --------------------
 * calibration_factor 447.74
 * 2149.4gr(21.0856N) -> 21.085N measured
 * 5165.6gr(50.67453N) -> 50.664N measured
 */

//TSDZ2 parameters
#define calibration_factor 245.94			// 10 kg element tsdz2 bench
#define TORQUE_ARM_LENGTH 730.425	// 730.425mm - see documentation/torquebench folder

//HR parameters
//#define calibration_factor 241.74			// 10 kg element Hongrunda bench
//#define TORQUE_ARM_LENGTH 1000	// 1000mm for rear traction motor



#define DOUT  3
#define CLK  2


HX711 scale;
float f_value=0;
int i_value=0;		// 10 kg = +-10 000 grams
byte start_byte=240;
byte stop_byte=44;


void setup() {
  Serial.begin(9600);
//  Serial.println("HX711 scale demo");

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  //Serial.println("Readings:");
}

void loop() {

	/*
	 * prep all the variables
	 */
  f_value=((scale.get_units())*TORQUE_ARM_LENGTH)/1000;
  if(f_value>0){
	  i_value=(int)(f_value+0.5);
  }
  else{
	  i_value=(int)(f_value-0.5);
  }
	// if only positive values are desired.
	//  if(i_value<0){
	//	  i_value=0
	//  }




 /*
  * sends [force x 100] as a int16 - MSB first, then LSB
  * max/min is +-320N~+-32kg
  */
  Serial.write(start_byte);
  Serial.write((byte)(i_value>>8));
  Serial.write((byte)i_value);
  Serial.write(stop_byte);
  // does this need a crc and start Byte?

  delay(95);


// unhash for debugging - sends it as ASCI Code, first int, then float.
//  Serial.println();
/*
  Serial.print(i_value);
  Serial.println();
  Serial.print(f_value, 1); //scale.get_units() returns a float
  Serial.print(" grams");
  Serial.println();
  */
}



















/*
 * /*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

/*

#include "HX711.h" //This library can be obtained here http://librarymanager/All#Avia_HX711

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

float calibration_factor = 241.74; // start point for 10kg element

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1);
  Serial.print(" N x 100"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += .1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= .1;
  }
}
/*
 */
