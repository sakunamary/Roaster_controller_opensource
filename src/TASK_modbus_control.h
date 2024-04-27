#ifndef __TASK_MODBUS_CONTROL_H__
#define __TASK_MODBUS_CONTROL_H__

#include <Arduino.h>
#include <Wire.h>
#include <ModbusIP_ESP8266.h>

bool init_status = true;

uint16_t last_FAN;
uint16_t last_PWR;

// Modbus Registers Offsets
const uint16_t HEAT_HREG = 3003;
const uint16_t FAN_HREG = 3004;

int heat_level_to_artisan = 0;
int fan_level_to_artisan = 0;

// Arduino like analogWrite
// value has to be between 0 and valueMax
void PWMAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255)
{
    // calculate duty, 4095 from 2 ^ 12 - 1
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);

    // write duty to LEDC
    ledcWrite(channel, duty);
}

void Task_modbus_control(void *pvParameters)
{ // function
    (void)pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xIntervel = 500 / portTICK_PERIOD_MS;
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

            xSemaphoreGive(xThermoDataMutex); // end of lock mutex
        }
        PWMAnalogWrite(PWM_FAN_CHANNEL, fan_level_to_artisan);   // 自动模式下，将heat数值转换后输出到pwm
        PWMAnalogWrite(PWM_HEAT_CHANNEL, heat_level_to_artisan); // 自动模式下，将heat数值转换后输出到pwm
    }
}

#endif