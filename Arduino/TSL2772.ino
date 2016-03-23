// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL2772
// This code is designed to work with the TSL2772_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<Wire.h>

// TSL2772 I2C address is 0x39(57)
#define Addr 0x39

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x00 | 0x80);
  // Set power on, ALS enabled
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select ALS time register
  Wire.write(0x01 | 0x80);
  // Atime = 2.73 ms, max count = 1024
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select proximity time register
  Wire.write(0x02 | 0x80);
  // Atime = 2.73 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Wait time register
  Wire.write(0x03 | 0x80);
  // Wtime = 2.73 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x0F | 0x80);
  // Gain = 1x
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);

void loop()
{
  unsigned int data[4];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x14 | 0x80);
  // Stop I2C Transmission
  Wire.endTransmission();
   
  // Request 4 bytes of data
  Wire.requestFrom(Addr, 4);
    
  // Read 4 bytes of data
  // c0Data lsb, c0Data msb, c1Data lsb, c1Data msb, proximity lsb, proximity msb
  if(Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }

  // Convert the data
  int c0Data = (data[1] * 256) + data[0];
  int c1Data = (data[3] * 256) + data[2];
  int proximity = (data[5] * 256) + data[3];
  float CPL = (2.73 * 1.0) / 60.0;
  float luminance1 = (1 * c0Data - 1.87 * c1Data) / CPL;
  float luminance2 = (0.63 * c0Data - 1.00 * c1Data) / CPL;
  float luminance = 0.0;
  
  if((luminance1 > 0) && (luminance1 > luminance2))
  {
    luminance = luminance1;
  }
  else if((luminance2 > 0) && (luminance2 > luminance1))
  {
    luminance = luminance2;
  }

  // Output data to serial monitor
  Serial.print("Ambient Light luminance :");
  Serial.println(luminance);
  Serial.print("Proximity of the device:");
  Serial.println(proximity);
 
}


