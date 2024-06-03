#ifndef __TASK_READ_TEMP_H__
#define __TASK_READ_TEMP_H__

#include <Arduino.h>
#include <config.h>
#include <Wire.h>
#include "max6675.h"
#include <ModbusIP_ESP8266.h>

//  ModbusIP object
ModbusIP mb;

double BT_TEMP;
double ET_TEMP;

double bt_temp[5] = {0};
int i, j;
double temp_;

// MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MAX6675 thermo_BT(SPI_SCK, SPI_CS_BT, SPI_MISO); // CH1  豆温
MAX6675 thermo_ET(SPI_SCK, SPI_CS_ET, SPI_MISO); // CH2  风温

// Modbus Registers Offsets
const uint16_t BT_HREG = 3001; // 对应的 modbus 寄存器偏移量
const uint16_t ET_HREG = 3002; // 对应的 modbus 寄存器偏移量

void Task_Thermo_get_data(void *pvParameters)
{ // function

    /* Variable Definition */
    (void)pvParameters;
    TickType_t xLastWakeTime;

    const TickType_t xIntervel = 1500 / portTICK_PERIOD_MS; // 1.5s完成一次数据采集

    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xIntervel);
        if (xSemaphoreTake(xThermoDataMutex, xIntervel) == pdPASS)
        {
            for (i = 0; i < 5; i++)
            {
                if (i == 0)
                {
                    bt_temp[5] = {0};
                }
                vTaskDelay(50);
                bt_temp[i] = round((thermo_BT.readCelsius() * 10)) / 10; // 只取到小数点后1位
                for (j = i + 1; j < 5; j++)
                {
                    if (bt_temp[i] > bt_temp[j])
                    {
                        temp_ = bt_temp[i];
                        bt_temp[i] = bt_temp[j];
                        bt_temp[j] = temp_;
                    }
                }
            }

            BT_TEMP = bt_temp[2]; // 取5次采集数据的中间值作为输出

            ET_TEMP = round((thermo_ET.readCelsius() * 10)) / 10; // 只取到小数点后1位
            vTaskDelay(20);

            xSemaphoreGive(xThermoDataMutex); // end of lock mutex
            // update  Hreg data
            mb.Hreg(BT_HREG, int(round(BT_TEMP * 10))); // 更新modbus寄存器数据
            mb.Hreg(ET_HREG, int(round(ET_TEMP * 10))); // 更新modbus寄存器数据
#if defined(DEBUG_MODE)
            Serial.printf("\nBT RAW :%4.2f BT out: %4.2f", thermo_BT.readCelsius(), BT_TEMP); //用作debug监控数据采集情况
#endif
        }
    }

} // function

#endif