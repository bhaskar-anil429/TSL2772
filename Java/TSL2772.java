// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL2772
// This code is designed to work with the TSL2772_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class TSL2772
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, TSL2772 I2C address is 0x39(57)
		I2CDevice device = Bus.getDevice(0x39);

		// Select enable register with command 0x80(128)
		// Set Power ON, ALS Enabled
		device.write(0x00 | 0x80, (byte)0x0F);
		// Select ALS Time Register 
		// Atime = 2.73 ms, Max count = 1024
		device.write(0x01 | 0x80, (byte)0xFF);
		// Select Proximity time control register
		// Time = 2.73 ms
		device.write(0x02 | 0x80, (byte)0xFF);
		// Select Wait Time Register
		// Wtime = 2.73 ms
		device.write(0x03 | 0x80, (byte)0xFF);
		// Select Control register
		// Gain = 1x
		device.write(0x0F | 0x80, (byte)0x20);
		Thread.sleep(500);

		// Read 6 bytes of data
		// c0Data LSB, c0Data MSB, c1Data LSB, c1Data MSB, proximity LSB, proximity MSB
		byte[] data=new byte[6];
		device.read(0x14 | 0x80, data, 0, 6);

		// Convert the data
		double ch0 = ((data[1] & 0xFF)* 256 + (data[0] & 0xFF));
		double ch1 = ((data[3] & 0xFF)* 256 + (data[2] & 0xFF));
		double proximity = ((data[5] & 0xFF)* 256 + (data[4] & 0xFF));
		double CPL = (2.73 * 1.0) / 60.0;
		double luminance1 = ((1 * ch0) - (1.87 * ch1)) / CPL;
		double luminance2 = ((0.63 * ch0) - (1 * ch1)) / CPL;
		double luminance = 0;
		if((luminance1 > 0) & (luminance2 > 0))
		{
			if(luminance1 > luminance2)
			{
				luminance = luminance1;
			}
			else if(luminance2 > luminance1)
			{
				luminance = luminance2;
			}
		}

		// Output data to screen
		System.out.printf("Ambient Light luminance : %.2f lux %n", luminance);
		System.out.printf("Proximity of the device : %.2f %n", proximity);
	}
}
