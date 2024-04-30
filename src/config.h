
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Wire.h>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define BAUDRATE 115200        // serial port baudrate

//#define DEBUG_MODE

#define VERSION "1.0.1"


#define SPI_SCK 8
#define SPI_MISO 9
#define SPI_MOSI 10

#define SPI_CS_BT 4
#define SPI_CS_ET 3

#define PWM_FAN 5
#define PWM_HEAT 2

#define I2C_SCL 7
#define I2C_SDA 6

#define TXD_HMI 21
#define RXD_HMI 20


// pwm setting
#define PWM_HEAT_CHANNEL    0
#define PWM_FAN_CHANNEL    1
#define PWM_FREQ 5000
#define PWM_RESOLUTION 10 // 0-1024

SemaphoreHandle_t xThermoDataMutex = NULL;


#endif