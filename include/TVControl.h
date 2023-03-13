#ifndef ESP32_BLE_GAMEPAD_H
#define ESP32_BLE_GAMEPAD_H

#include "sdkconfig.h"

#if defined(CONFIG_BT_ENABLED)
    #include "nimconfig.h"

    #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
        #include "TVConnectionStatus.h"
        #include "NimBLEHIDDevice.h"
        #include "NimBLECharacteristic.h"
        #include "TVControlConfiguration.h"

        typedef enum
        {
            /**
            * @brief Menu Pick/Enter (Middle button)
            */
            TV_BUTTON_MENU_PICK,
            /**
            * @brief Menu Up
            */
            TV_BUTTON_MENU_UP,
            /**
            * @brief Menu Down
            */
            TV_BUTTON_MENU_DOWN,
            /**
            * @brief Menu Left
            */
            TV_BUTTON_MENU_LEFT,
            /**
            * @brief Menu Right
            */
            TV_BUTTON_MENU_RIGHT,
            /**
            * @brief Power Button (Power On/Off the TV)
            */
            TV_BUTTON_POWER,
            /**
            * @brief Start Voice Command (NOT IN USE)
            */
            TV_BUTTON_VOICE_COMMAND,
            /**
            * @brief Increase the TV volume
            */
            TV_BUTTON_VOL_INCREMENT,
            /**
            * @brief Decrease the TV volume
            */
            TV_BUTTON_VOL_DECREMENT,
            /**
            * @brief Open App Launcher/Browser
            */
            TV_BUTTON_LAUNCH_SELECT,
            /**
            * @brief Open Movie Browser
            */
            TV_BUTTON_LAUNCH_MOVIE_BROWSER,
            /**
            * @brief Green button
            */
            TV_BUTTON_MENU_GREEN,
            /**
            * @brief Blue button
            */
            TV_BUTTON_MENU_BLUE,
            /**
            * @brief Yellow button
            */
            TV_BUTTON_MENU_YELLOW,
            /**
            * @brief Open Email reader
            */
            TV_BUTTON_LAUNCH_EMAIL_READER,
            /**
            * @brief Open calculator
            */
            TV_BUTTON_LAUNCH_CALCULATOR,
            /**
            * @brief Open local machine browser
            */
            TV_BUTTON_LAUNCH_LOCAL_BROWSER,
            /**
            * @brief Start Voice Command (NOT IN USE)
            */
            TV_BUTTON_VOICE_COMMAND_ALT,
            /**
            * @brief Home button
            */
            TV_BUTTON_APP_HOME,
            /**
            * @brief Back button
            */
            TV_BUTTON_APP_BACK,
            /**
            * @brief Forward button
            */
            TV_BUTTON_APP_FORWARD,
            /**
            * @brief Stop button
            */
            TV_BUTTON_APP_STOP,
            /**
            * @brief Refresh button
            */
            TV_BUTTON_APP_REFRESH,
            /**
            * @brief Bookmarks button
            */
            TV_BUTTON_APP_BOOKMARKS,
            /**
            * @brief Maximum count of Buttons
            */
            MAX_TV_BUTTONS,
        } TVControlButton;

        class TVControl
        {
        private:
            uint8_t _buttons[3]; // 8 bits x 3 --> 24 bits (buttons)

            TVControlConfiguration configuration;

            TVConnectionStatus *connectionStatus;

            NimBLEHIDDevice *hid;
            NimBLECharacteristic *inputGamepad;

            static void taskServer(void *pvParameter);

        public:
            TVControl(std::string deviceName = "ESP32 TV Control", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
            void begin(TVControlConfiguration *config = new TVControlConfiguration());
            void end(void);
            void press(TVControlButton b);
            void release(TVControlButton b);
            void setState(TVControlButton b, bool s);
            void sendReport();
            bool isPressed(TVControlButton b);
            bool isConnected(void);
            void resetButtons();
            void setBatteryLevel(uint8_t level);
            uint8_t batteryLevel;
            std::string deviceManufacturer;
            std::string deviceName;

        protected:
            virtual void onStarted(NimBLEServer *pServer){};
        };
    #endif // CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // CONFIG_BT_ENABLED

#endif // ESP32_BLE_GAMEPAD_H