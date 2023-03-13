#ifndef MAIN_H
#define MAIN_H

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <TVControl.h>

#define MENU_PICK_PIN   35
#define MENU_UP_PIN     32
#define MENU_DOWN_PIN   33
#define MENU_LEFT_PIN   25
#define MENU_RIGHT_PIN  26
#define POWER_PIN       34
#define VOL_INC_PIN     27
#define VOL_DEC_PIN     14
#define LAUNCHER_PIN    12
#define MOVIES_PIN      -1
#define MENU_GREEN_PIN  -1
#define MENU_BLUE_PIN   -1
#define MENU_YELLOW_PIN -1
#define EMAIL_PIN       -1
#define CALC_PIN        -1
#define LOCAL_PIN       -1
#define HOME_PIN        23
#define BACK_PIN        22
#define FORWARD_PIN     -1
#define STOP_PIN        -1
#define REFRESH_PIN     -1
#define BOOKMARKS_PIN   -1

#endif