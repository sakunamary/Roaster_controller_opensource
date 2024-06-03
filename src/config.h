
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Wire.h>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define BAUDRATE 115200        // serial port baudrate

// #define DEBUG_MODE  //是否输出debug信息

#define VERSION "1.0.3"

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
#define PWM_FREQ 50       // 最低频率不能低于50Hz，一般驱动都在1K到5K之间。
#define PWM_RESOLUTION 10 // 0-1024档 pwm分辨为2的10次方。如果需要降低就调成9，分辨率就成了2的9次方（0-512级）
// SSR控制芯片的pwm输入频率最低位50Hz
// 航模电调的pwm频率为50Hz

SemaphoreHandle_t xThermoDataMutex = NULL;

#endif