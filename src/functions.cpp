/*
 *  @file   functions.cpp
 *  @brief  Functions used in the project 
 *
 *  @author Marti Dominguez
 *  @date   24/05/2024
 *  
 */

#include "includes.h"


/*!
 *  @brief  Connects to WiFi network
 */
void connectWifi(){
  Serial.println("Connecting to the WiFi network...");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connection established");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

/*!
 *  @brief  Reads file and saves content to a string variable
 *  @param  fileName: Name of the file to read
 *  @return String with file content
 */
String loadFile(String fileName) { 
  File file = SPIFFS.open(fileName);
  String content = "";

  if (!file) {
    Serial.println("Error al abrir el archivo");
    return content;
  }

  while (file.available()) {
    char c = file.read();
    content += c;
  }

  file.close();
  return content;
}

/*!
 *  @brief  Handles root from web server
 *  @param  request: AsyncWebServerRequest object
 */
void handle_root(AsyncWebServerRequest *request) {
  request->send(200, "text/html", webContent);
}

/*!
 *  @brief  Handles unlocking of the device server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_SetUnlocked(AsyncWebServerRequest *request) {
  setUnlocked();
  if(PRINT_MODES) Serial.println("Unlocked from server");
  request->send(200, "text/plain", "Unlocked");
}

/*!
 *  @brief  Handles locking of the device server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_SetLocked(AsyncWebServerRequest *request) {
  setLocked();
  if(PRINT_MODES) Serial.println("Locked from server");
  request->send(200, "text/plain", "Locked");
}

/*!
 *  @brief  Handles activation of the fan server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_ActivateFan(AsyncWebServerRequest *request) {
  activateFan();
  if(PRINT_MODES) Serial.println("Fan activated from server");
  request->send(200, "text/plain", "Fan ON");
}

/*!
 *  @brief  Handles deactivation of the fan server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_DeactivateFan(AsyncWebServerRequest *request) {
  deactivateFan();
  if(PRINT_MODES) Serial.println("Fan deactivated from server");
  request->send(200, "text/plain", "Fan OFF");
}

/*!
 *  @brief  Handles increase of fan speed server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_IncreaseSpeed(AsyncWebServerRequest *request) {
  incrementFanSpeed();
  if(PRINT_MODES) Serial.println("Speed increased from server");
  String response = "Speed: " + String(motor.speed);
  request->send(200, "text/plain", response);
}

/*!
 *  @brief  Handles decrease of fan speed server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_DecreaseSpeed(AsyncWebServerRequest *request) {
  decrementFanSpeed();
  if(PRINT_MODES) Serial.println("Speed dencreased from server");
  String response = "Speed: " + String(motor.speed);
  request->send(200, "text/plain", response);
}

/*!
 *  @brief  Handles activation of night mode server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_ActivateNightMode(AsyncWebServerRequest *request) {
  activateNightMode();
  if(PRINT_MODES) Serial.println("Night mode activated from server");
  request->send(200, "text/plain", "Night mode activated");
}

/*!
 *  @brief  Handles deactivation of night mode server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_DeactivateNightMode(AsyncWebServerRequest *request) {
  deactivateNightMode();
  if(PRINT_MODES) Serial.println("Night mode deactivated from server");
  request->send(200, "text/plain", "Night mode deactivated");
}

/*!
 *  @brief  Handles activation of auto mode server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_ActivateAutoMode(AsyncWebServerRequest *request) {
  activateAutoMode();
  if(PRINT_MODES) Serial.println("Auto mode activated from server");
  request->send(200, "text/plain", "Auto mode activated");
}

/*!
 *  @brief  Handles deactivation of auto mode server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_DeactivateAutoMode(AsyncWebServerRequest *request) {
  deactivateAutoMode();
  if(PRINT_MODES) Serial.println("Auto mode deactivated from server");
  request->send(200, "text/plain", "Auto mode deactivated");
}

/*!
 *  @brief  Handles activation of PIR sensor server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_ActivatePIRSensor(AsyncWebServerRequest *request) {
  activatePIRSensor();
  if(PRINT_MODES) Serial.println("PIR sensor activated from server");
  request->send(200, "text/plain", "PIR sensor activated");
}

/*!
 *  @brief  Handles deactivation of PIR sensor server request
 *  @param  request: AsyncWebServerRequest object
 */
void handle_DeactivatePIRSensor(AsyncWebServerRequest *request) {
  deactivatePIRSensor();
  if(PRINT_MODES) Serial.println("PIR sensor deactivated from server");
  request->send(200, "text/plain", "PIR sensor deactivated");
}

/*!
 *  @brief  Interrupt Service Routine for fan button
 */
void IRAM_ATTR ISR_FanButton() {
  static uint32_t lastInterruptTime = 0;
  uint32_t buttonTime = millis();
  if (buttonTime - lastInterruptTime > fanButton.debounceTime) {
    lastInterruptTime = buttonTime;
    if(motor.active) deactivateFan();
    else activateFan();
  }
}

/*!
 *  @brief  Interrupt Service Routine for lock button
 */
void IRAM_ATTR ISR_LockButton() {
  static uint32_t lastInterruptTime = 0;
  uint32_t buttonTime = millis();
  if (buttonTime - lastInterruptTime > lockButton.debounceTime) {
    lastInterruptTime = buttonTime;
    if (currentState == LOCKED) setUnlocked();
    else setLocked();
  }
}

/*!
 *  @brief  Interrupt Service Routine for auto mode button
 */
void IRAM_ATTR ISR_AutoButton() {
  static uint32_t lastInterruptTime = 0;
  uint32_t buttonTime = millis();
  if (buttonTime - lastInterruptTime > autoButton.debounceTime) {
    lastInterruptTime = buttonTime;
    if (autoMode) {
      deactivateAutoMode();
      motor.speed = 3;
    } else if (!autoMode) {
      deactivateNightMode();
      activateAutoMode();
    }
  }
}

/*!
 *  @brief  Interrupt Service Routine for night mode button
 */
void IRAM_ATTR ISR_NightButton() {
  static uint32_t lastInterruptTime = 0;
  uint32_t buttonTime = millis();
  if (buttonTime - lastInterruptTime > nightButton.debounceTime) {
    lastInterruptTime = buttonTime;
    if(nightMode) {
      deactivateNightMode();
      motor.speed = 3;
    } else if (!nightMode) {
      deactivateAutoMode();
      activateNightMode();
    }
  }
}


/*!
 *  @brief  Sets current state to Unlocked
 */
void setUnlocked(){
  currentState = UNLOCKED;
}

/*!
 *  @brief  Sets current state to Locked
 */
void setLocked(){
  currentState = LOCKED;
 }

/*!
 *  @brief  Callback function for fan timer
 *  @param  xTimer: Timer handle
 */
 void timerCallback(TimerHandle_t xTimer) {
  deactivateFan();
  Serial.println("Fan deactivated due to Timer");
}

/*!
 *  @brief  Callback function for OLED clear timer
 *  @param  xTimer: Timer handle
 */
void oledClearTimerCallback(TimerHandle_t xTimer) {
    display.clearDisplay();
    display.display();
}

/*!
 *  @brief  Deactivates PIR sensor
 */
void deactivatePIRSensor() {
  if (PIRTaskHandle != NULL) {
    vTaskSuspend(PIRTaskHandle);
    if(PRINT_MODES) Serial.println("PIR Sensor OFF");
  }

  OLEDMessage msg;
  msg.type = MESSAGE_TYPE_STRING;
  snprintf(msg.text, MAX_STRING_LENGTH, "PIR Sensor OFF");
  xQueueSend(screenQueue, &msg, portMAX_DELAY);
}

/*!
 *  @brief  Activates PIR sensor
 */
void activatePIRSensor() {
  if (currentState == UNLOCKED && motor.active) {
    if (PIRTaskHandle != NULL) {
      vTaskResume(PIRTaskHandle);
      if(PRINT_MODES) Serial.println("PIR Sensor ON");
    } 

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "PIR Sensor ON");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Deactivates the fan
 */
void deactivateFan() {
  if(motor.active) {
    motor.active = false;
    deactivateAutoMode();
    deactivateNightMode();
    deactivatePIRSensor();
    if(PRINT_MODES) Serial.println("Fan OFF");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Fan OFF");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Activates the fan
 */
void activateFan() {
  if (currentState == UNLOCKED && !motor.active) {
    motor.active = true;
    deactivateAutoMode();
    deactivateNightMode();
    if(PRINT_MODES) Serial.println("Fan ON");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Fan ON");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Increments fan speed
 */
void incrementFanSpeed() {
  if (motor.speed < 5) {
    motor.speed++;
    Serial.print("Fan speed incremented to: ");
    Serial.println(motor.speed);
  } else motor.speed = 5; 

  OLEDMessage msg;
  msg.type = MESSAGE_TYPE_STRING_INTEGER;
  snprintf(msg.text, MAX_STRING_LENGTH, "Speed: ");
  msg.integer = motor.speed;
  xQueueSend(screenQueue, &msg, portMAX_DELAY);
}

/*!
 *  @brief  Decrements fan speed
 */
void decrementFanSpeed() {
  if (motor.speed > 1) {
    motor.speed--;
    Serial.print("Fan speed decremented to: ");
    Serial.println(motor.speed);
  } else motor.speed = 1;

  OLEDMessage msg;
  msg.type = MESSAGE_TYPE_STRING_INTEGER;
  snprintf(msg.text, MAX_STRING_LENGTH, "Speed: ");
  msg.integer = motor.speed;
  xQueueSend(screenQueue, &msg, portMAX_DELAY);
}

/*!
 *  @brief  Sets fan timer
 */
void setFanTimer() {
  if (currentState == UNLOCKED && motor.active) {
    if (fanTimer != NULL) {
        xTimerStop(fanTimer, 0);
        xTimerDelete(fanTimer, 0);
    }
    fanTimer = xTimerCreate("FanTimer", pdMS_TO_TICKS(FanTimerDuration), pdFALSE, (void *)0, timerCallback);
    if (fanTimer != NULL) {
        xTimerStart(fanTimer, 0);
        Serial.print("Fan timer set for ");
        Serial.print(FanTimerDuration);
        Serial.println(" milliseconds");
    } else {
        Serial.println("Failed to create timer");
    }
    Serial.println("Fan timer enabled");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Timer ON");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Disables fan timer
 */
void disableFanTimer() {
  if (fanTimer != NULL && xTimerIsTimerActive(fanTimer)) {
    xTimerStop(fanTimer, 0);
    if (PRINT_MODES) Serial.println("Fan timer OFF");
    
    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Timer OFF");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Activates auto mode
 */
void activateAutoMode() {
  if (currentState == UNLOCKED && motor.active && !autoMode) {
    nightMode = false;
    autoMode = true;
    if(PRINT_MODES) Serial.println("Auto mode ON");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Auto mode ON");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Deactivates auto mode
 */
void deactivateAutoMode() {
  if (autoMode) {
    autoMode = false;
    motor.speed = 3;
    if(PRINT_MODES) Serial.println("Auto mode OFF");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Auto mode OFF");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Activates night mode
 */
void activateNightMode() {
  if (currentState == UNLOCKED && motor.active && !nightMode) {
    autoMode = false;
    nightMode = true;
    if(PRINT_MODES) Serial.println("Night mode ON");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Night mode ON");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Deactivates night mode
 */
void deactivateNightMode() {
  if (nightMode) {
    nightMode = false;
    motor.speed = 3;
    if(PRINT_MODES) Serial.println("Night mode OFF");

    OLEDMessage msg;
    msg.type = MESSAGE_TYPE_STRING;
    snprintf(msg.text, MAX_STRING_LENGTH, "Night mode OFF");
    xQueueSend(screenQueue, &msg, portMAX_DELAY);
  }
}

/*!
 *  @brief  Adjusts fan speed based on temperature
 *  @return Fan speed (From 0 to 5)
 */
uint8_t adjustFanSpeedBasedOnTemperature() {
  if      (currentTemperature > TEMP5) return 5;
  else if (currentTemperature > TEMP4) return 4;
  else if (currentTemperature > TEMP3) return 3;
  else if (currentTemperature > TEMP2) return 2;
  else if (currentTemperature > TEMP1) return 1;
  else return 0;
}

/*!
 *  @brief  Task that manages lock state changes
 */
void taskManageStates(void *pvParameters) {  
  for(;;) {
    if(currentState != lastState){
      if (currentState == LOCKED) {
        deactivateFan(); 
        Serial.println("Device locked");
        
        OLEDMessage msg;
        msg.type = MESSAGE_TYPE_STRING;
        snprintf(msg.text, MAX_STRING_LENGTH, "Device locked");
        xQueueSend(screenQueue, &msg, portMAX_DELAY);

      } else if (currentState == UNLOCKED) {           
        deactivateFan();
        Serial.println("Device unlocked");
        
        OLEDMessage msg;
        msg.type = MESSAGE_TYPE_STRING;
        snprintf(msg.text, MAX_STRING_LENGTH, "Device unlocked");
        xQueueSend(screenQueue, &msg, portMAX_DELAY);
      }
      lastState = currentState;
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    display.clearDisplay(); 
  }
}

/*!
 *  @brief  Task that controls LEDs
 */
void ledsTask(void *parameters) {
  for (;;) {
    digitalWrite(LED_AUTO_PIN, autoMode ? HIGH : LOW);
    digitalWrite(LED_NIGHT_PIN, nightMode ? HIGH : LOW);
    if (currentState == LOCKED) digitalWrite(LED_LOCK_PIN, HIGH);
      else digitalWrite(LED_LOCK_PIN, LOW); 
    digitalWrite(LED_FAN_PIN, motor.active ? HIGH : LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
  } 
}

/*!
 *  @brief  Task that controls the fan
 */
void fanTask(void *parameters) {
  DCMotor *motor = (DCMotor *)parameters;
  for(;;) {
    if(motor->active && currentState == UNLOCKED) {
      if (autoMode) motor->speed = adjustFanSpeedBasedOnTemperature();
      if (nightMode) motor->speed = NIGHT_MODE_SPEED;
      motor->dutyCycle = 255 * ((float)motor->speed / 5.0);
      if (PRINT_FAN_VALUES) {
        Serial.printf("Speed: %d", motor->speed);
        Serial.printf(" | DutyCycle: ", motor->dutyCycle);
        Serial.printf(" | Auto Mode = %d", autoMode);
        Serial.printf(" | Night Mode = %d", nightMode);
        Serial.println("");
      }
      digitalWrite(motor->pin1, HIGH);
      digitalWrite(motor->pin2, LOW);
      ledcWrite(motor->pwmChannel, motor->dutyCycle);
    } else {
      digitalWrite(motor->pin1, LOW);
      digitalWrite(motor->pin2, LOW);
      ledcWrite(motor->pwmChannel, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

/*!
 *  @brief  Task that handles PIR sensor
 */
void PIRTask(void *pvParameters) {
  for(;;) {
    pirSensor.detection = digitalRead(pirSensor.PIN);
    if (pirSensor.detection) {
      if (PRINT_PIR) Serial.println("Motion detected!");
      if (fanTimer != NULL) {
        if (xTimerReset(fanTimer, 0) != pdPASS) {
          if (PRINT_PIR) Serial.println("Failed to reset the fan timer");
        } else {
          if (PRINT_PIR) Serial.println("Timer restarted");
        }
      } else {
        if (PRINT_PIR) Serial.println("Fan timer is NULL");
      }
    }    
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}

/*!
 *  @brief  Task that manages OLED display
 */
void oledTask(void *pvParameters) {
  OLEDMessage msg;
  for(;;) {
    if (xQueueReceive(screenQueue, &msg, portMAX_DELAY)) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      switch (msg.type) {
        case MESSAGE_TYPE_INTEGER:
            display.println(msg.integer);
            break;

        case MESSAGE_TYPE_DECIMAL:
            display.println(msg.decimal);
            break;

        case MESSAGE_TYPE_STRING:
            display.println(msg.text);
            break;

        case MESSAGE_TYPE_STRING_INTEGER:
            display.print(msg.text);
            display.print(" ");
            display.println(msg.integer);
            break;

        case MESSAGE_TYPE_STRING_DECIMAL:
            display.print(msg.text);
            display.print(" ");
            display.println(msg.decimal);
            break;
      }
      display.display();

      if (xTimerIsTimerActive(oledClearTimer) == pdTRUE) {
        xTimerStop(oledClearTimer, 0); 
      }
      xTimerStart(oledClearTimer, 0);
    } else {
        msg.type = MESSAGE_TYPE_STRING;
        snprintf(msg.text, MAX_STRING_LENGTH, "Temp: %2.1f  Hum: %2.1f", currentTemperature, currentHumidity);
        xQueueSend(screenQueue, &msg, portMAX_DELAY);
    }
  }
}

/*!
 *  @brief  Task that reads temperature sensor data
 */
void tempSensorTask(void *pvParameters) {
  for(;;) {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      currentTemperature = event.temperature;
    }
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
        currentHumidity = event.relative_humidity;
    }
    vTaskDelay(pdMS_TO_TICKS(300000)); // 5 minutes
  }
}

/*!
 *  @brief  Lists files in a directory
 *  @param  dir: Directory path
 */
void listFiles(const char *dir) {
  Serial.print("Listing files in directory: ");
  Serial.println(String(dir));
  File root = SPIFFS.open(dir);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    Serial.println("File: " + String(file.name()) + ", Size: " + file.size());
    file = root.openNextFile();
  }
}




