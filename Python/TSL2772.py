# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# TSL2772
# This code is designed to work with the TSL2772_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# TSL2772 address, 0x39(57)
# Select enable register, 0x00(00) with command register, 0x80(128)
#		0x0B(11)	Set Power ON, ALS Enabled
bus.write_byte_data(0x39, 0x00 | 0x80, 0x0B)
# TSL2772 address, 0x39(57)
# Select ALS time register, 0x01(01) with command register, 0x80(128)
#		0xFF(255)	Atime = 2.73 ms, Max count = 1024
bus.write_byte_data(0x39, 0x01 | 0x80, 0xFF)
# TSL2772 address, 0x39(57)
# Select proximity time register, 0x02(02) with command register, 0x80(128)
#		0xFF(255)	Time = 2.73 ms
bus.write_byte_data(0x39, 0x02 | 0x80, 0xFF)
# TSL2772 address, 0x39(57)
# Select wait time register, 0x03(03) with command register, 0x80(128)
#		0xFF(255)	Wtime = 2.73 ms
bus.write_byte_data(0x39, 0x03 | 0x80, 0xFF)
# TSL2772 address, 0x39(57)
# Select control register, 0x0F(15) with command register, 0x80(128)
#		0x20(32)	Gain = 1x
bus.write_byte_data(0x39, 0x0F | 0x80, 0x20)

time.sleep(0.5)

# TSL2772 address, 0x39(57)
# Read data back from 0x14(20) with command register, 0x80(128), 6 bytes
# c0Data LSB, c0Data MSB, c1Data LSB, c1Data MSB, proximity LSB, proximity MSB
data = bus.read_i2c_block_data(0x39, 0x14 | 0x80, 6)

# Convert the data
c0Data = data[1] * 256 + data[0]
c1Data = data[3] * 256 + data[2]
CPL = (2.73 * 1.0) / 60.0;
luminance1 = (1 * c0Data - 1.87 * c1Data) / CPL
luminance2 = (0.63 * c0Data - 1.00 * c1Data) / CPL
luminance = 0.0
if luminance1 > 0 and luminance2 > 0 :
	if luminance1 > luminance2 :
		luminance = luminance1
	else :
		luminance = luminance2
proximity = data[5] * 256 + data[4]

# Output data to screen
print "Ambient Light luminance : %d" %luminance
print "Proximity of the device : %d" %proximity
