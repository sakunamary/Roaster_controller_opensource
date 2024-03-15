
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Wire.h>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define BAUDRATE 115200        // serial port baudrate

#define DEBUG_MODE

#define VERSION "1.0.0"


#define SPI_SCK D8
#define SPI_MISO D9
#define SPI_MOSI D10

#define SPI_CS_BT D2
#define SPI_CS_ET D1

#define PWM_FAN D3
#define PWM_HEAT D2

#define I2C_SCL D5
#define I2C_SDA D4

#define TXD_HMI D6
#define RXD_HMI D7

// pwm setting
#define PWM_FREQ 10000
#define PWM_RESOLUTION 10 // 0-1024

#endif