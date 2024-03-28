#include <Arduino.h>
#include <config.h>
#include <WiFi.h>
#include <TASK_read_temp.h>
#include <TASK_modbus_control.h>

String local_IP;

char ap_name[30];
uint8_t macAddr[6];

//HardwareSerial Serial_debug(0);

void Task_modbus_control(void *pvParameters);
void Task_Thermo_get_data(void *pvParameters);

void setup()
{

    xThermoDataMutex = xSemaphoreCreateMutex();

    pinMode(PWM_HEAT, OUTPUT);
    pinMode(PWM_FAN, OUTPUT);
    // PWM Pins
    ledcSetup(PWM_FAN_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_FAN, PWM_FAN_CHANNEL);

    ledcSetup(PWM_HEAT_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_HEAT, PWM_HEAT_CHANNEL);

    Serial.begin(BAUDRATE);

#if defined(DEBUG_MODE)
    Serial.printf("\nHOT AIR ROASTER STARTING...\n");
#endif

    // 初始化网络服务

    WiFi.macAddress(macAddr);
    WiFi.mode(WIFI_AP);
    sprintf(ap_name, "ROASTER_%02X%02X%02X", macAddr[3], macAddr[4], macAddr[5]);
    WiFi.softAP(ap_name, "12345678"); // defualt IP address :192.168.4.1 password min 8 digis

#if defined(DEBUG_MODE)
    Serial.printf("Start Task...\n");
#endif

    /*---------- Task Definition ---------------------*/
    // Setup tasks to run independently.
    xTaskCreate(
        Task_Thermo_get_data, "Thermo_get_data" // 获取HB数据
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
        Task_modbus_control, "modbus_control" // 获取HB数据
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

// Init Modbus-TCP
#if defined(DEBUG_MODE)
    Serial.printf("Start Modbus-TCP   service...\n");
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