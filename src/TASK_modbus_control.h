#ifndef __TASK_MODBUS_CONTROL_H__
#define __TASK_MODBUS_CONTROL_H__

#include <Arduino.h>
#include <config.h>
#include <Wire.h>
#include <ModbusIP_ESP8266.h>
#include <pwmWrite.h>

bool init_status = true;

uint16_t last_FAN;
uint16_t last_PWR;

const uint32_t frequency = PWM_FREQ;

const byte resolution = PWM_RESOLUTION;
const byte pwm_fan_out = PWM_FAN;
const byte pwm_heat_out = PWM_HEAT;

Pwm pwm = Pwm();

// Modbus Registers Offsets
const uint16_t HEAT_HREG = 3003;
const uint16_t FAN_HREG = 3004;

int heat_level_to_artisan = 0;
int fan_level_to_artisan = 0;

void Task_modbus_control(void *pvParameters)
{ // function
    (void)pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xIntervel = 200 / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xIntervel);
        if (xSemaphoreTake(xThermoDataMutex, xIntervel) == pdPASS)
        {
            if (init_status)
            {
                last_FAN = mb.Hreg(FAN_HREG);
                last_PWR = mb.Hreg(HEAT_HREG);
                fan_level_to_artisan = last_FAN;
                heat_level_to_artisan = last_PWR;
            }
            else
            {
                if (last_FAN != mb.Hreg(FAN_HREG)) // 发生变动
                {
                    last_FAN = mb.Hreg(FAN_HREG);
                    fan_level_to_artisan = last_FAN;
                }
                if (last_PWR != mb.Hreg(HEAT_HREG)) // 发生变动
                {
                    last_PWR = mb.Hreg(HEAT_HREG);
                    heat_level_to_artisan = last_PWR;
                }
            }

            xSemaphoreGive(xThermoDataMutex); // end of lock mutex
        }
        // 数值转换后输出到pwm
        pwm.write(pwm_heat_out, map(heat_level_to_artisan, 0, 100, 230, 950), frequency, resolution); 
        pwm.write(pwm_fan_out, map(fan_level_to_artisan, 0, 100, 285, 350), frequency, resolution);
    }
}

#endif