
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


// Just for reference at the very top
// The circuit:
// * analog sensors on analog ins 0, 1, and 2
// ** Current pin is A0
// ** Voltage pin is A1
// ** Speed sensor is A2
// ** Push Button is 9
// ** Diode is 10

////////////////////////////////////////////////////////////
//Voltage divider
const PROGMEM byte voltPin = A1;
float voltage = 0;
float VoltRead = 0;


/////////////////////////////////////////////////////////////


#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.0;// supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)
float current = 0 ; 


float cutOffLimit = 1.00;// reading cutt off current. 1.00 is 1 Amper

/*
          "ACS758LCB-050B",// for model use 0
          "ACS758LCB-050U",// for model use 1
          "ACS758LCB-100B",// for model use 2
          "ACS758LCB-100U",// for model use 3
          "ACS758KCB-150B",// for model use 4
          "ACS758KCB-150U",// for model use 5
          "ACS758ECB-200B",// for model use 6
          "ACS758ECB-200U"// for model use  7   
sensitivity array is holding the sensitivy of the  ACS758
current sensors. Do not change.          
*/
float sensitivity[] ={
          40.0,// for ACS758LCB-050B
          60.0,// for ACS758LCB-050U
          20.0,// for ACS758LCB-100B
          40.0,// for ACS758LCB-100U
          13.3,// for ACS758KCB-150B
          16.7,// for ACS758KCB-150U
          10.0,// for ACS758ECB-200B
          20.0,// for ACS758ECB-200U     
         }; 

/*         
 *   quiescent Output voltage is factor for VCC that appears at output       
 *   when the current is zero. 
 *   for Bidirectional sensor it is 0.5 x VCC
 *   for Unidirectional sensor it is 0.12 x VCC
 *   for model ACS758LCB-050B, the B at the end represents Bidirectional (polarity doesn't matter)
 *   for model ACS758LCB-100U, the U at the end represents Unidirectional (polarity must match)
 *    Do not change.
 */
float quiescent_Output_voltage [] ={
          0.5,// for ACS758LCB-050B
          0.12,// for ACS758LCB-050U
          0.5,// for ACS758LCB-100B
          0.12,// for ACS758LCB-100U
          0.5,// for ACS758KCB-150B
          0.12,// for ACS758KCB-150U
          0.5,// for ACS758ECB-200B
          0.12,// for ACS758ECB-200U            
          };
const float FACTOR = sensitivity[model]/1000;// set sensitivity for selected model
const float QOV =   quiescent_Output_voltage [model] * VCC;// set quiescent Output voltage for selected model
float volt;// internal variable for voltage
float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV

//////////////////////////////////////////////////////////////




void setup() {
   Serial.begin(9600);

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
 //display.display();
 // Clear the buffer.
 //delay(2000); 
 display.clearDisplay();
  
  
}

void loop() {

///////////////////////////////////////////////////////////
  //Obtain RAW voltage data
  float voltRead = analogRead(voltPin);
  //Convert to actual voltage (0 - 5 Vdc)
  voltRead = (voltRead / 1023) * 5.0;
  //voltage = map(voltRead, 0, 5, 33, 55) - 3.5;
  VoltRead = map(voltRead, 0, 5, 48, 53) ;

  // Serial.print("voltage = ");
   //Serial.print(voltRead);
  // Serial.print(voltage);
  // Serial.print("\n");

//////////////////////////////////////////////////////

  //Robojax.com ACS758 Current Sensor 
    float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.007 ;// 0.007 is a value to make voltage zero when there is no current
  //float current = voltage / FACTOR;
   current = voltage / FACTOR;
  if(abs(voltage) > cutOff ){
   // Serial.print("V: ");
   // Serial.print(voltage,3);// print voltage with 3 decimal places
   Serial.print("the loop current I: ");
   Serial.println(current,2); // print the current with 2 decimal places
//    Serial.println("A");
  }//else{
   // Serial.print("I:0A ");
  //}

  display_data();
  delay(500);
  
///////////////////////Display//////////////////////////////////////

 
}


void display_data(){
// Clear the buffer.
 //display.clearDisplay();


  // text display tests
  
  display.setTextColor(WHITE);
  display.setTextSize(1);

  //current
  display.setCursor(0,0);
  display.print("Current: ");
 // display.setCursor(80,0);
  display.println(current);
 //display.println(300.141592);
    //Serial.print("I: ");
  // Serial.println(current,2); // print the current with 2 decimal places
   // Serial.println("A");

//    Serial.print("I: ");
//    Serial.print(current,2); // print the current with 2 decimal places
//    Serial.println("A");

Serial.print("    loop current I: ");
   Serial.println(current,2); // print the current with 2 decimal places


//voltage
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  //display.setTextColor(WHITE);
  //display.setCursor(2, 0);
  //display.println(300.141592);
 
  //display.setCursor(0,8);
  display.print("voltage:");
 //display.setCursor(80,8 );
  display.println(VoltRead);
// display.print(400.141592);

 // Serial.println(voltage);
   Serial.println(VoltRead);

  
  //display.println(voltage);
   //Serial.print(voltRead);
 //  Serial.print(voltage);
 //  Serial.print("\n");

  //display.setCursor(0, 65);
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.print("0x"); display.println(0xDEADBEEF, HEX);
  

  
  display.display();

 
 
  //delay(2000);
  display.clearDisplay();  
//  counter++;
 }


