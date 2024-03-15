#ifndef __TASK_MODBUS_CONTROL_H__
#define __TASK_MODBUS_CONTROL_H__

#include <Arduino.h>
#include <Wire.h>
#include <ModbusIP_ESP8266.h>
#include <pwmWrite.h>

SemaphoreHandle_t xGetDataMutex = NULL; // Mutex for 读取温度数据

//  ModbusIP object
ModbusIP mb;

// pwm object
Pwm pwm_fan = Pwm();
Pwm pwm_heat = Pwm();
bool init_status = true;

// PWM Pins
const int HEAT_OUT_PIN = PWM_HEAT; // GPIO26
const int FAN_OUT_PIN = PWM_FAN;   // GPIO26

uint16_t last_FAN;
uint16_t last_PWR;

// Modbus Registers Offsets
const uint16_t HEAT_HREG = 3003;
const uint16_t FAN_HREG = 3004;

int heat_level_to_artisan = 0;
int fan_level_to_artisan = 0;

const uint32_t frequency = PWM_FREQ;
const byte resolution = PWM_RESOLUTION; // pwm -0-4096

void Task_modbus_control(void *pvParameters)
{ // function
    (void)pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xIntervel = 500 / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xIntervel);
        if (xSemaphoreTake(xGetDataMutex, xIntervel) == pdPASS)
        {
            if (init_status)
            {
                last_FAN = mb.Hreg(FAN_HREG);
                last_PWR = mb.Hreg(HEAT_HREG);
            }
            else
            {
                if (last_FAN != mb.Hreg(FAN_HREG)) // 发生变动
                {
                    fan_level_to_artisan = mb.Hreg(FAN_HREG);
                }
                if (last_PWR != mb.Hreg(HEAT_HREG)) // 发生变动
                {
                    heat_level_to_artisan = mb.Hreg(HEAT_HREG);
                }
            }

            xSemaphoreGive(xGetDataMutex); // end of lock mutex
        }
        pwm_heat.write(HEAT_OUT_PIN, map(heat_level_to_artisan, 0, 100, 100, 900), PWM_FREQ, resolution);  // 自动模式下，将heat数值转换后输出到pwm
        pwm_fan.write(FAN_OUT_PIN, map(fan_level_to_artisan, 0, 100, 100, 900), PWM_FREQ, resolution); // 自动模式下，将heat数值转换后输出到pwm
    }
}

#endif