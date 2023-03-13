#ifndef ESP32_BLE_CONNECTION_STATUS_H
#define ESP32_BLE_CONNECTION_STATUS_H

#include "sdkconfig.h"

#if defined(CONFIG_BT_ENABLED)
    #include "nimconfig.h"
    
    #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
        #include <NimBLEServer.h>
        #include "NimBLECharacteristic.h"

        class TVConnectionStatus : public NimBLEServerCallbacks
        {
        public:
            TVConnectionStatus(void);
            bool connected = false;
            void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo);
            void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason);
            NimBLECharacteristic *inputGamepad;
            NimBLECharacteristic *outputGamepad;
        };

    #endif // CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // CONFIG_BT_ENABLED

#endif // ESP32_BLE_CONNECTION_STATUS_H