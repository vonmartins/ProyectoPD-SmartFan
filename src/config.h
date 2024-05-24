#ifndef CONFIG_H
#define CONFIG_H

    #define MAX_STRING_LENGTH 50

    #define LIST_FILES 0        // Put to 1 if want to list files in directory data suring the setup
    #define PRINT_FAN_VALUES 1  // Put 1o 1 to print Fan Values on the serial monitor
    #define PRINT_MODES 0       // Put to 1 to print mode changes on the serial monitor. There will be errors with interrupts
    #define PRINT_PIR 0         // Put to 1 to print PIR Sensor comments

    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 32
    #define OLED_RESET -1
    #define SCREEN_ADDRESS 0x3C
    #define OLEDTimerDuration 5000

    #define BUTTON_DEBOUNCE_TIME 80

    #define FanTimerDuration 1800000 // 30 minutes
    #define NIGHT_MODE_SPEED 1
    #define TEMP0 13
    #define TEMP1 15
    #define TEMP2 17
    #define TEMP3 19
    #define TEMP4 22
    #define TEMP5 25

    #define PWM_CHANNEL 0
    #define PWM_FREQUENCY 5000      // Frecuencia del PWM
    #define PWM_RESOLUTION 8        // Resolución de 8 bits

    #define DHTTYPE DHT11

    #define WIFI_SSID "MOVISTAR_D220"
    #define WIFI_PASSWORD "cHowxiTG2otK8kNaoRQF"

    // #define WIFI_SSID "MIWIFI_RH6t"
    // #define WIFI_PASSWORD "xXEdQFxc"

#endif // CONFIG_H
