#ifndef FUNCTIONS_H
#define FUNCTIONS_H

    #include "includes.h"

    void connectWifi();
    String loadFile(String fileName);
    void setUnlocked();
    void setLocked();
    void IRAM_ATTR ISR_LockButton();
    void IRAM_ATTR ISR_AutoButton();
    void IRAM_ATTR ISR_NightButton();
    void IRAM_ATTR ISR_FanButton();
    void handle_root(AsyncWebServerRequest *request);
    void handle_SetUnlocked(AsyncWebServerRequest *request);
    void handle_SetLocked(AsyncWebServerRequest *request);
    void handleSensorData(AsyncWebServerRequest *request);
    void handle_ActivateFan(AsyncWebServerRequest *request);
    void handle_DeactivateFan(AsyncWebServerRequest *request);
    void handle_IncreaseSpeed(AsyncWebServerRequest *request);
    void handle_DecreaseSpeed(AsyncWebServerRequest *request);
    void handle_ActivateNightMode(AsyncWebServerRequest *request);
    void handle_DeactivateNightMode(AsyncWebServerRequest *request);
    void handle_ActivateAutoMode(AsyncWebServerRequest *request);
    void handle_DeactivateAutoMode(AsyncWebServerRequest *request);
    void handle_ActivatePIRSensor(AsyncWebServerRequest *request);
    void handle_DeactivatePIRSensor(AsyncWebServerRequest *request);
    void doFunction(uint8_t cmdid);
    void taskManageStates(void *pvParameters);
    void oledTask(void *pvParameters);
    void tempSensorTask(void *pvParameters);
    void fanTask(void *parameters);
    void PIRTask(void *parameters);
    void ledsTask (void *parameters);
    void saveTemperatureData(float temperature, float humidity);
    void listFiles(const char *dir);
    void timerCallback(TimerHandle_t xTimer);
    void oledClearTimerCallback (TimerHandle_t xTimer);
    void deactivatePIRSensor();
    void activatePIRSensor();
    void deactivateFan();
    void activateFan();
    void incrementFanSpeed();
    void decrementFanSpeed();
    void setFanTimer();
    void disableFanTimer();
    void activateAutoMode();
    void deactivateAutoMode();
    void activateNightMode();
    void deactivateNightMode();

    extern AsyncWebServer server;
    extern Button lockButton;
    extern Button autoButton;
    extern Button nightButton;  
    extern Button fanButton;
    extern PIRSensor pirSensor;
    extern DCMotor motor;
    extern Adafruit_SSD1306 display;
    extern DeviceState currentState, lastState;
    extern float currentTemperature;
    extern float currentHumidity;
    extern String webContent;
    extern DHT_Unified dht;
    extern QueueHandle_t screenQueue;
    extern TaskHandle_t PIRTaskHandle;
    extern TimerHandle_t fanTimer;
    extern TimerHandle_t oledClearTimer;
    extern bool autoMode;
    extern bool nightMode;

#endif 
