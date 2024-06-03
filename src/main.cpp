#include <Arduino.h>
#include <config.h>
#include <WiFi.h>
#include <TASK_read_temp.h>
#include <TASK_modbus_control.h>

String local_IP;

char ap_name[30];
uint8_t macAddr[6];

// HardwareSerial Serial(0);

void Task_modbus_control(void *pvParameters);
void Task_Thermo_get_data(void *pvParameters);

void setup()
{

    xThermoDataMutex = xSemaphoreCreateMutex();

    Serial.begin(BAUDRATE);

#if defined(DEBUG_MODE)
    Serial.printf("\nHOT AIR ROASTER STARTING...\n"); // 启动setup
#endif

    pwm.pause();
    pwm.write(pwm_fan_out, 0, frequency, resolution);
    pwm.write(pwm_heat_out, 0, frequency, resolution);
    pwm.resume();

#if defined(DEBUG_MODE)
    Serial.printf("\nStart PWM...\n"); // 初始化 pwm
    pwm.printDebug();
#endif

    // 初始化网络服务
    WiFi.macAddress(macAddr);
    WiFi.mode(WIFI_AP);
    sprintf(ap_name, "ROASTER_%02X%02X%02X", macAddr[3], macAddr[4], macAddr[5]);
    WiFi.softAP(ap_name, "12345678"); // defualt IP address :192.168.4.1 password min 8 digis

#if defined(DEBUG_MODE)
    Serial.printf("\nAP NAME:%s...\n", ap_name);
#endif

#if defined(DEBUG_MODE)
    Serial.printf("Start Task...\n");
#endif

    /*---------- Task Definition ---------------------*/
    // Setup tasks to run independently.
    xTaskCreate(
        Task_Thermo_get_data, "Thermo_get_data" // 获取温度数据的任务
        ,
        1024 * 6 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 1 // Priority, with 1 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL // Running Core decided by FreeRTOS,let core0 run wifi and BT
    );
#if defined(DEBUG_MODE)
    Serial.printf("TASK1:Task_Thermo_get_data...\n");
#endif

    xTaskCreate(
        Task_modbus_control, "modbus_control" // 获取监控artisan操控的任务
        ,
        1024 * 10 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 1 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL // Running Core decided by FreeRTOS,let core0 run wifi and BT
    );
#if defined(DEBUG_MODE)
    Serial.printf("TASK2:Task_modbus_control...\n");
#endif

#if defined(DEBUG_MODE)
    Serial.printf("Start Modbus-TCP   service...\n");// 初始化Modbus-TCP服务

#endif
    mb.server(502); // Start Modbus IP //default port :502
    mb.addHreg(BT_HREG);
    mb.addHreg(ET_HREG);
    mb.addHreg(HEAT_HREG);
    mb.addHreg(FAN_HREG);

    mb.Hreg(BT_HREG, 0);   // 初始化赋值
    mb.Hreg(ET_HREG, 0);   // 初始化赋值
    mb.Hreg(HEAT_HREG, 0); // 初始化赋值
    mb.Hreg(FAN_HREG, 0);  // 初始化赋值
}

void loop()
{
    mb.task();
}