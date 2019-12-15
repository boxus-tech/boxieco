/******************************************************************************
  Read CCS811 and BME280 sensors data and display it on the LCD5110 screen.
  This code uses BME280 temperature data to compensate the CCS811 readings.

  Author: Alexey Kondratov <kondratov.aleksey@gmail.com>, 2019

  ### Hardware Connections ###

  CCS811:
  WAK -> GND
  VCC -> 5V / 3V
  SDA -> A4
  SCL -> A5
  GND -> GND

  BME280:
  VCC -> 3V
  SDA -> A4
  SCL -> A5
  GND -> GND

  LCD5110:
  VCC -> 3V
  CLK -> D13
  DIN -> D11
  DC  -> D10
  CE  -> D9
  RST -> D8
  BL  -> 3V
  GND -> GND

  ### Resources ###
  Uses Wire.h for I2C operation
  Uses SPI.h for LCD operation

  ### Development environment specifics ###
  Arduino IDE 1.8.10

  Based on the example provided by
  Marshall Taylor @ SparkFun Electronics, April 4, 2017
  https://github.com/sparkfun/Qwiic_BME280_CCS811_Combo

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
  Distributed as-is; no warranty is given.
******************************************************************************/

#include <avr/power.h> // Experimental and for power management only
// #include "LowPower.h"  // Download LowPower.h here: https://github.com/rocketscream/Low-Power

#include <Wire.h>
#include <SparkFunBME280.h> // Click here to get the library: http://librarymanager/All#SparkFun_BME280
#include <SparkFunCCS811.h> // Click here to get the library: http://librarymanager/All#SparkFun_CCS811
                            // https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library
                            // https://github.com/sparkfun/CCS811_Air_Quality_Breakout

#include <SPI.h>
#include <Adafruit_GFX.h>     // Install https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_PCD8544.h> // Install https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library

// #define CCS811_ADDR 0x5B // Default I2C Address
#define CCS811_ADDR 0x5A // Alternate I2C Address

// Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 10, 9, 8);

//
// Device setup routine
//
void setup()
{
  // ****************************************************************
  // ********** Some attempts to reduce power consumption ***********
  // Disable ADC
  ADCSRA = 0;
  power_adc_disable();

  noInterrupts();
  CLKPR = _BV(CLKPCE);  // enable change of the clock prescaler
  CLKPR = _BV(CLKPS0);  // divide frequency by 2
  CLKPR = _BV(CLKPS0);
  interrupts();
  // ****************** Power management end ************************
  // ****************************************************************

  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting BME280 and CCS811 based air monitor");

  Wire.begin();

  //
  // Initialize CCS811
  //
  // This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  myCCS811.setDriveMode(2); // Read every 10 seconds
  Serial.print("CCS811 begin exited with: ");
  // Pass the error code to a function to print the results
  printDriverError( returnCode );
  Serial.println();

  //
  // Initialize BME280
  //
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x76;
  myBME280.settings.runMode = 3; // Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  // Calling .begin() causes the settings to be loaded
  myBME280.begin();
  delay(10); // Make sure sensor had enough time to turn on.
             // BME280 requires 2ms to start up.

  //
  // Initialize and clear display
  //
  display.begin();
  display.clearDisplay();
  display.display();
  
  display.setContrast(70);
  display.setRotation(2);

  display.setTextSize(1);  // set font size
  // display.setTextColor(WHITE, BLACK); // enable to use white on black
  display.setTextColor(BLACK);
}

//
// Main device loop
//
void loop()
{
  // Check for availability of data from CCS811
  if (myCCS811.dataAvailable())
  {
    // Calling this function updates the global tVOC and eCO2 variables
    myCCS811.readAlgorithmResults();

    // Fetch and print values of tVOC and eCO2
    // printInfoSerial();
    printInfoDisplay();

    float BMEtempC = myBME280.readTempC();
    float BMEhumid = myBME280.readFloatHumidity();

    Serial.print("Applying new values (deg C, %): ");
    Serial.print(BMEtempC);
    Serial.print(",");
    Serial.println(BMEhumid);
    Serial.println();

    // This sends the temperature data to the CCS811
    myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
  }
  else if (myCCS811.checkForStatusError())
  {
    // If some internal CCS811 error occured, print it
    printSensorError();
  }

  // ********** Another attempt to reduce power consumption *********
  // Triggers a 8000 ms sleep
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  // LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
  //               SPI_OFF, USART0_OFF, TWI_OFF);
  // ****************************************************************

  delay(2000); // Delay between readings
}

//
// Empirical corrections for in-box tempreature in order to predict
// a temperature outside of the box.
//
float tempCorrection(int inTemp)
{
  if (inTemp < 19)
    return inTemp - 3;
  else if (inTemp >= 19 and inTemp < 22)
    return inTemp - 2.2;
  else if (inTemp >= 22)
    return inTemp - 2.8;
}

//
// Print sensors data on the display
//
void printInfoDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print(" Temp: ");
  display.print(tempCorrection(myBME280.readTempC()), 2);
  display.println("C");

  display.print(" Hum.: ");
  display.print(myBME280.readFloatHumidity(), 2);
  display.println("%");

  // getCO2() gets the previously read data from the library
  display.print(" CO2: ");
  display.print(myCCS811.getCO2());
  display.println(" ppm");

  // getTVOC() gets the previously read data from the library
  display.print(" TVOC: ");
  display.print(myCCS811.getTVOC());
  display.println(" ppb");

  display.print(" P: ");
  display.print(myBME280.readFloatPressure() / 100, 0);
  display.println(" hPa");

  display.print(" Alt.: ");
  display.print(myBME280.readFloatAltitudeMeters(), 0);
  display.println(" m");

  display.display();
}

//
// Debugging serial port output
//
void printInfoSerial()
{
  // Print CCS811 data
  // getCO2() gets the previously read data from the library
  Serial.println("CCS811 data:");
  Serial.print(" CO2 concentration: ");
  Serial.print(myCCS811.getCO2());
  Serial.println(" ppm");

  // getTVOC() gets the previously read data from the library
  Serial.print("TVOC concentration: ");
  Serial.print(myCCS811.getTVOC());
  Serial.println(" ppb");

  // Print BME280 data
  Serial.println("BME280 data:");
  Serial.print("Temperature: ");
  Serial.print(myBME280.readTempC(), 2);
  Serial.println(" degrees C");

  Serial.print("Temperature: ");
  Serial.print(myBME280.readTempF(), 2);
  Serial.println(" degrees F");

  Serial.print("Pressure: ");
  Serial.print(myBME280.readFloatPressure() / 100, 2);
  Serial.println(" hPa");

  Serial.print("Pressure: ");
  Serial.print((myBME280.readFloatPressure() * 0.0002953), 2);
  Serial.println(" InHg");

  Serial.print("Altitude: ");
  Serial.print(myBME280.readFloatAltitudeMeters(), 2);
  Serial.println("m");

  Serial.print("Altitude: ");
  Serial.print(myBME280.readFloatAltitudeFeet(), 2);
  Serial.println("ft");

  Serial.print("%RH: ");
  Serial.print(myBME280.readFloatHumidity(), 2);
  Serial.println(" %");

  Serial.println();
}

//
// printSensorError gets, clears, then prints the errors
// saved within the error register.
//
void printSensorError()
{
  uint8_t error = myCCS811.getErrorRegister();

  if ( error == 0xFF ) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5) Serial.print("HeaterSupply");
    if (error & 1 << 4) Serial.print("HeaterFault");
    if (error & 1 << 3) Serial.print("MaxResistance");
    if (error & 1 << 2) Serial.print("MeasModeInvalid");
    if (error & 1 << 1) Serial.print("ReadRegInvalid");
    if (error & 1 << 0) Serial.print("MsgInvalid");
    Serial.println();
  }
}

//
// printDriverError decodes the CCS811Core::status type and prints the
// type of error to the serial terminal.
//
// Save the return value of any function of type CCS811Core::status, then pass
// to this function to see what the output was.
//
void printDriverError( CCS811Core::status errorCode )
{
  switch ( errorCode )
  {
    case CCS811Core::SENSOR_SUCCESS:
      Serial.print("SUCCESS");
      break;
    case CCS811Core::SENSOR_ID_ERROR:
      Serial.print("ID_ERROR");
      break;
    case CCS811Core::SENSOR_I2C_ERROR:
      Serial.print("I2C_ERROR");
      break;
    case CCS811Core::SENSOR_INTERNAL_ERROR:
      Serial.print("INTERNAL_ERROR");
      break;
    case CCS811Core::SENSOR_GENERIC_ERROR:
      Serial.print("GENERIC_ERROR");
      break;
    default:
      Serial.print("Unspecified error.");
  }
}
