#ifndef FUNCTIONS_H
#define FUNCTIONS_H

    #include "includes.h"

    void connectWifi();
    String loadFile(String fileName);
    void setUnlocked();
    void setLocked();
    void handle_root(AsyncWebServerRequest *request);
    void handle_SetUnlocked(AsyncWebServerRequest *request);
    void handle_SetLocked(AsyncWebServerRequest *request);
    void handle_ActivateFan(AsyncWebServerRequest *request);
    void handle_DeactivateFan(AsyncWebServerRequest *request);
    void handle_IncreaseSpeed(AsyncWebServerRequest *request);
    void handle_DecreaseSpeed(AsyncWebServerRequest *request);
    void handle_ActivateNightMode(AsyncWebServerRequest *request);
    void handle_DeactivateNightMode(AsyncWebServerRequest *request);
    void handle_ActivateAutoMode(AsyncWebServerRequest *request);
    void handle_DeactivateAutoMode(AsyncWebServerRequest *request);
    void handle_ActivateTimer(AsyncWebServerRequest *request);
    void handle_DeactivateTimer(AsyncWebServerRequest *request);
    void handle_GetSensorData(AsyncWebServerRequest *request);
    void doFunction(uint8_t cmdid);
    void taskManageStates(void *pvParameters);
    void oledTask(void *pvParameters);
    void tempSensorTask(void *pvParameters);
    void fanTask(void *parameters);
    void ledsTask (void *parameters);
    void listFiles(const char *dir);
    void timerCallback(TimerHandle_t xTimer);
    void oledClearTimerCallback (TimerHandle_t xTimer);
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
    extern DCMotor motor;
    extern Adafruit_SSD1306 display;
    extern DeviceState currentState, lastState;
    extern float currentTemperature;
    extern float currentHumidity;
    extern String webContent;
    extern DHT_Unified dht;
    extern QueueHandle_t screenQueue;
    extern TimerHandle_t fanTimer;
    extern TimerHandle_t oledClearTimer;
    extern volatile bool autoMode;
    extern volatile bool nightMode;

#endif 
