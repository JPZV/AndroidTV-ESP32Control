#include <NimBLEDevice.h>
#include <NimBLEUtils.h>
#include <NimBLEServer.h>
#include "NimBLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

#include "TVConnectionStatus.h"
#include "TVControl.h"
#include "TVControlConfiguration.h"

#include "esp_log.h"
static const char *LOG_TAG = "TVControl";

#define SERVICE_UUID_DEVICE_INFORMATION        "180A"      // Service - Device information

#define CHARACTERISTIC_UUID_MODEL_NUMBER       "2A24"      // Characteristic - Model Number String - 0x2A24
#define CHARACTERISTIC_UUID_SOFTWARE_REVISION  "2A28"      // Characteristic - Software Revision String - 0x2A28
#define CHARACTERISTIC_UUID_SERIAL_NUMBER      "2A25"      // Characteristic - Serial Number String - 0x2A25
#define CHARACTERISTIC_UUID_FIRMWARE_REVISION  "2A26"      // Characteristic - Firmware Revision String - 0x2A26
#define CHARACTERISTIC_UUID_HARDWARE_REVISION  "2A27"      // Characteristic - Hardware Revision String - 0x2A27


uint8_t lastReportButtons[BUTTONS_ARRAY_LENGTH];
uint8_t tempHidReportDescriptor[145];
int hidReportDescriptorSize = 0;
uint16_t vid;
uint16_t pid;
std::string modelNumber;
std::string softwareRevision;
std::string serialNumber;
std::string firmwareRevision;
std::string hardwareRevision;

TVControl::TVControl(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) : _buttons()
{
    this->resetButtons();
    this->deviceName = deviceName;
    this->deviceManufacturer = deviceManufacturer;
    this->batteryLevel = batteryLevel;
    this->connectionStatus = new TVConnectionStatus();
}

uint8_t lowByte(uint16_t input)
{
    return input & 0xff;
}
uint8_t highByte(uint16_t input)
{
    return (input>>8) & 0xff;
}

void TVControl::resetButtons()
{
    memset(&_buttons, 0, sizeof(_buttons));
}

void TVControl::begin(TVControlConfiguration *config)
{
    configuration = *config; // we make a copy, so the user can't change actual values midway through operation, without calling the begin function again

    modelNumber = configuration.getModelNumber();
    softwareRevision = configuration.getSoftwareRevision();
    serialNumber = configuration.getSerialNumber();
    firmwareRevision = configuration.getFirmwareRevision();
    hardwareRevision = configuration.getHardwareRevision();

	vid = configuration.getVid();
	pid = configuration.getPid();

	uint8_t high = highByte(vid);
	uint8_t low = lowByte(vid);

	vid = low << 8 | high;

	high = highByte(pid);
	low = lowByte(pid);

	pid = low << 8 | high;

    //----------REMOTE CONTROL----------

    // USAGE_PAGE (Consumer Device)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0C;

    // USAGE (Consumer Control)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (Default: 1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getHidReportId();

    // LOGICAL_MINIMUM (0)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x14;

    // LOGICAL_MAXIMUM (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (24)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x24;

    // USAGE (Menu Pick)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x41;

    // USAGE (Menu Up)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x42;

    // USAGE (Menu Down)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x43;

    // USAGE (Menu Left)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x44;

    // USAGE (Menu Right)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x45;

    // USAGE (Power)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x30;

    // USAGE (Voice Command)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xCF;

    // USAGE (Volume Increment)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xE9;

    // USAGE (Volume Decrement)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xEA;
    
    // USAGE (AL Select Task/Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA2;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (AL Movie Browser)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xB8;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Green Menu Button)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x6A;

    // USAGE (Blue Menu Button)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x6B;

    // USAGE (Yellow Menu Button)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x6C;
    
    // USAGE (AL Email Reader)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x8A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
    
    // USAGE (AL Calculator)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x92;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
    
    // USAGE (Local Machine Browser)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x94;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
    
    // USAGE (Voice Command)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xCF;
    
    // USAGE (AC Home)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x23;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
    
    // USAGE (AC Back)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x24;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
    
    // USAGE (AC Forward)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
    
    // USAGE (AC Stop)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
    
    // USAGE (AC Refresh)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x27;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
    
    // USAGE (AC Bookmarks)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x2A;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

    // INPUT (Variable)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

    // END_COLLECTION (Main Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC0;

    //----------KEYBOARD----------

    // USAGE_PAGE (Generic Desktop)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Keyboard)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x06;

    // COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (Default: 2)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getHidReportId() + 1;

    // USAGE_PAGE (Keyboard/Keypad Page)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x07;

    // USAGE_MINIMUM (Keyboard Left Control)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xE0;

    // USAGE_MAXIMUM (Keyboard Right GUI)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xE7;

    // LOGICAL_MINIMUM (0)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x14;

    // LOGICAL_MAXIMUM (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (8)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

    // INPUT (Variable)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

    // REPORT_SIZE (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

    // REPORT_COUNT (8)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // INPUT (Constant)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (5)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;

    // USAGE_PAGE (LED Indicator)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

    // USAGE_MINIMUM (Num Lock)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE_MAXIMUM (Kana)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;

    // OUTPUT (Variable)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x91;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

    // REPORT_SIZE (3)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;

    // REPORT_COUNT (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // OUTPUT (Constant)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x91;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (8)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

    // REPORT_COUNT (6)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x06;

    // LOGICAL_MINIMUM (40)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x28;

    // LOGICAL_MAXIMUM (-2)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFE;

    // USAGE_PAGE (Keyboard/Keypad)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x07;

    // USAGE_MINIMUM (Keyboard Return)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x28;

    // USAGE_MAXIMUM (Reserved)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFE;

    // INPUT (0)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // END_COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC0;

    xTaskCreate(this->taskServer, "server", 20000, (void *)this, 10, NULL);
}

void TVControl::end(void)
{
}

void TVControl::sendReport(void)
{
    if (this->isConnected())
    {
        bool shouldReport = false;
        for (int i = 0; i < BUTTONS_ARRAY_LENGTH; i++)
        {
            if (_buttons[i] != lastReportButtons[i])
            {
                shouldReport = true;
            }
            lastReportButtons[i] = _buttons[i];
        }
        if (shouldReport)
        {
            this->inputGamepad->setValue(_buttons, sizeof(_buttons));
            this->inputGamepad->notify();
        }
    }
}

void TVControl::press(TVControlButton b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    uint8_t result = _buttons[index] | bitmask;

    if (result != _buttons[index])
    {
        _buttons[index] = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void TVControl::release(TVControlButton b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    uint64_t result = _buttons[index] & ~bitmask;

    if (result != _buttons[index])
    {
        _buttons[index] = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void TVControl::setState(TVControlButton b, bool s)
{
    if (b)
    {
        press(b);
    }
    else
    {
        release(b);
    }
}

bool TVControl::isPressed(TVControlButton b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    if ((bitmask & _buttons[index]) > 0)
        return true;
    return false;
}

bool TVControl::isConnected(void)
{
    return this->connectionStatus->connected;
}

void TVControl::setBatteryLevel(uint8_t level)
{
    this->batteryLevel = level;
    if (hid != 0)
    {
        this->hid->setBatteryLevel(this->batteryLevel);

        if (this->isConnected())
        {
            this->hid->batteryLevel()->notify();
        }
		
        if (configuration.getAutoReport())
        {
            sendReport();
        }
    }
}

void TVControl::taskServer(void *pvParameter)
{
    TVControl *controlInstance = (TVControl *)pvParameter;

    NimBLEDevice::init(controlInstance->deviceName);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(controlInstance->connectionStatus);

    controlInstance->hid = new NimBLEHIDDevice(pServer);

    controlInstance->inputGamepad = controlInstance->hid->inputReport(controlInstance->configuration.getHidReportId()); // <-- input REPORTID from report map

    controlInstance->connectionStatus->inputGamepad = controlInstance->inputGamepad;

    controlInstance->hid->manufacturer()->setValue(controlInstance->deviceManufacturer);

    NimBLEService *pService = pServer->getServiceByUUID(SERVICE_UUID_DEVICE_INFORMATION);
	
	BLECharacteristic* pCharacteristic_Model_Number = pService->createCharacteristic(
      CHARACTERISTIC_UUID_MODEL_NUMBER,
      NIMBLE_PROPERTY::READ
    );
    pCharacteristic_Model_Number->setValue(modelNumber);
	
	BLECharacteristic* pCharacteristic_Software_Revision = pService->createCharacteristic(
      CHARACTERISTIC_UUID_SOFTWARE_REVISION,
      NIMBLE_PROPERTY::READ
    );
    pCharacteristic_Software_Revision->setValue(softwareRevision);
	
	BLECharacteristic* pCharacteristic_Serial_Number = pService->createCharacteristic(
      CHARACTERISTIC_UUID_SERIAL_NUMBER,
      NIMBLE_PROPERTY::READ
    );
    pCharacteristic_Serial_Number->setValue(serialNumber);
	
	BLECharacteristic* pCharacteristic_Firmware_Revision = pService->createCharacteristic(
      CHARACTERISTIC_UUID_FIRMWARE_REVISION,
      NIMBLE_PROPERTY::READ
    );
    pCharacteristic_Firmware_Revision->setValue(firmwareRevision);
	
	BLECharacteristic* pCharacteristic_Hardware_Revision = pService->createCharacteristic(
      CHARACTERISTIC_UUID_HARDWARE_REVISION,
      NIMBLE_PROPERTY::READ
    );
    pCharacteristic_Hardware_Revision->setValue(hardwareRevision);

    controlInstance->hid->pnp(0x01, vid, pid, 0x0110);
    controlInstance->hid->hidInfo(0x00, 0x01);

    NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND);

    uint8_t *customHidReportDescriptor = new uint8_t[hidReportDescriptorSize];
    memcpy(customHidReportDescriptor, tempHidReportDescriptor, hidReportDescriptorSize);

    controlInstance->hid->reportMap((uint8_t *)customHidReportDescriptor, hidReportDescriptorSize);
    controlInstance->hid->startServices();

    controlInstance->onStarted(pServer);

    NimBLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_KEYBOARD);
    pAdvertising->addServiceUUID(controlInstance->hid->hidService()->getUUID());
    pAdvertising->start();
    controlInstance->hid->setBatteryLevel(controlInstance->batteryLevel);

    ESP_LOGD(LOG_TAG, "Advertising started!");
    vTaskDelay(portMAX_DELAY);
}
