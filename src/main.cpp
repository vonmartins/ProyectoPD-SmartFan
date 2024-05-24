/*
 *  @file   main.cpp
 *  @brief  Main file to set up parameters and setup 
 *
 *  @author Marti Dominguez
 *  @date   24/05/2024
 *  
 */

#include "includes.h"

// ------------------- TIMER ---------------------
TimerHandle_t fanTimer = NULL;
TimerHandle_t oledClearTimer = NULL;

// ------- PIR Sensor ----------
PIRSensor pirSensor = {PIR_PIN, false};

// ---------- DC MOTOR ----------
DCMotor motor = {MOTOR_PIN1, MOTOR_PIN2, MOTOR_ENABLE_PIN, PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION};

// ------------------- WEB SERVER -------------------
AsyncWebServer server(80);

// ------------------- Buttons ----------------------
Button lockButton  = {LOCK_BUTTON_PIN, BUTTON_DEBOUNCE_TIME};
Button autoButton  = {AUTO_BUTTON_PIN, BUTTON_DEBOUNCE_TIME};
Button nightButton = {NIGHT_BUTTON_PIN, BUTTON_DEBOUNCE_TIME};
Button fanButton   = {FAN_BUTTON_PIN, BUTTON_DEBOUNCE_TIME};

// ---------------- FreeRTOS --------------------
QueueHandle_t screenQueue;
TaskHandle_t PIRTaskHandle = NULL;

// ---------------- OLED SCREEN -------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ----- TEMP SENSOR -----
DHT_Unified dht(TEMPERATURE_SENSOR_PIN, DHTTYPE);

// --------------------- VARIABLES GENERALES --------------

DeviceState currentState, lastState;

float currentTemperature;
float currentHumidity;

String webContent;

bool autoMode;
bool nightMode;

// -----------------------------------------------------------

void setup() {

// ----- Init comms -----
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

// ----- Init pins -----
  pinMode(LED_FAN_PIN, OUTPUT);
  pinMode(LED_LOCK_PIN, OUTPUT);
  pinMode(LED_AUTO_PIN, OUTPUT);
  pinMode(LED_NIGHT_PIN, OUTPUT);
  pinMode(lockButton.PIN, INPUT_PULLUP);
  pinMode(autoButton.PIN, INPUT_PULLUP);
  pinMode(nightButton.PIN, INPUT_PULLUP);
  pinMode(fanButton.PIN, INPUT_PULLUP);
  pinMode(motor.pin1, OUTPUT);
  pinMode(motor.pin2, OUTPUT);
  pinMode(motor.enablePin, OUTPUT);
  pinMode(pirSensor.PIN, INPUT);

// ----- Init Interrupts -----
  attachInterrupt(digitalPinToInterrupt(lockButton.PIN), &ISR_LockButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(autoButton.PIN), &ISR_AutoButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(nightButton.PIN), &ISR_NightButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(fanButton.PIN), &ISR_FanButton, FALLING);

// Configurar la funcionalidad PWM
  ledcSetup(motor.pwmChannel, motor.frequency, motor.resolution);
  ledcAttachPin(motor.enablePin, motor.pwmChannel);
 
// ----- Init SPIFFS -----
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully");
    if (LIST_FILES) listFiles("/");
  } else {
    Serial.println("SPIFFS mount failed");
  }
  server.serveStatic("/", SPIFFS, "/");
  webContent = loadFile("/index.html");
  delay(1000);

// ----- Init OLED Screen -----

  if(display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED Screen enabled!");
    display.clearDisplay();
    } else {
        Serial.println(F("¡Error allocating memory for SSD1306!"));
        delay(3000); 
    }
  

// ----- Init Temperature sensor -----
dht.begin();

// ----- Init FreeRTOS -----

  screenQueue = xQueueCreate(10, sizeof(OLEDMessage));
  if (screenQueue == NULL) {
      Serial.println("Error creating the queue");
      for (;;);
  }
  fanTimer = xTimerCreate("FanTimer", pdMS_TO_TICKS(FanTimerDuration), pdFALSE, (void *)0, timerCallback);
  oledClearTimer = xTimerCreate("OLED Clear Timer", pdMS_TO_TICKS(OLEDTimerDuration), pdFALSE, (void *)0, oledClearTimerCallback); 

  xTaskCreate(taskManageStates, "ManageStates", 4096, NULL, 2, NULL);
  xTaskCreate(oledTask, "OLEDTask", 2048, NULL, 5, NULL);
  xTaskCreate(tempSensorTask, "TempSensorTask", 4096, NULL, 3, NULL);
  xTaskCreate(fanTask, "Fan Task", 2048, &motor, 3, NULL);
  xTaskCreate(PIRTask, "PIR Sensor Task", 2048, NULL, 4, &PIRTaskHandle);
  xTaskCreate(ledsTask, "LED Control Task", 2048, NULL, 5, NULL);

// ----- Init WiFi -----
  connectWifi();

// ----- Init Server -----
  server.on("/", HTTP_GET, handle_root);
  server.on("/setUnlocked", HTTP_GET, handle_SetUnlocked);
  server.on("/setLocked", HTTP_GET, handle_SetLocked);
  server.on("/turnOnFan", HTTP_GET, handle_ActivateFan);
  server.on("/turnOffFan", HTTP_GET, handle_DeactivateFan);
  server.on("/increaseSpeed", HTTP_GET, handle_IncreaseSpeed);
  server.on("/decreaseSpeed", HTTP_GET, handle_DecreaseSpeed);
  server.on("/activateNightMode", HTTP_GET, handle_ActivateNightMode);
  server.on("/deactivateNightMode", HTTP_GET, handle_DeactivateNightMode);
  server.on("/activateAutoMode", HTTP_GET, handle_ActivateAutoMode);
  server.on("/deactivateAutoMode", HTTP_GET, handle_DeactivateAutoMode);
  server.on("/activatePIRSensor", HTTP_GET, handle_ActivatePIRSensor);
  server.on("/deactivatePIRSensor", HTTP_GET, handle_DeactivatePIRSensor);
  server.begin();

// ----- Init states -----
  currentState = UNLOCKED;
  lastState = UNLOCKED;
  motor.active = false;
  motor.speed = 3;
  autoMode = false;
  nightMode = false;

// ----- Init completed -----
  OLEDMessage msg;
  msg.type = MESSAGE_TYPE_STRING;
  snprintf(msg.text, MAX_STRING_LENGTH, "Smart Fan Started");
  xQueueSend(screenQueue, &msg, portMAX_DELAY);
  Serial.println("\n-------------- SMART FAN STARTED --------------");

// vTaskStartScheduler(); // Ya se ejecuta en la esp32 !!!!
}

void loop() {
// Nothing...
}


