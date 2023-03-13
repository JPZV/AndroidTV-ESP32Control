#ifndef ESP32_BLE_GAMEPAD_CONFIG_H
#define ESP32_BLE_GAMEPAD_CONFIG_H

#include "esp_system.h"

/**
* @brief Configuration class for the TV Control
*/
class TVControlConfiguration
{
private:
    bool _autoReport;
    uint8_t _hidReportId;
    uint16_t _vid;
    uint16_t _pid;
    char *_modelNumber;
    char *_softwareRevision;
    char *_serialNumber;
    char *_firmwareRevision;
    char *_hardwareRevision;

public:
    TVControlConfiguration();

    /**
    * @brief Get the current value for Auto Report
    */
    bool getAutoReport();
    /**
    * @brief Get the current value for the HID Report ID
    */
    uint8_t getHidReportId();
    /**
    * @brief Get the current value for Vendor ID
    */
    uint16_t getVid();
    /**
    * @brief Get the current value for Product ID
    */
    uint16_t getPid();
    /**
    * @brief Get the current value for Model Number
    */
    char *getModelNumber();
    /**
    * @brief Get the current value for Software Revision
    */
    char *getSoftwareRevision();
    /**
    * @brief Get the current value for Serial Number
    */
    char *getSerialNumber();
    /**
    * @brief Get the current value for Firmware Revision
    */
    char *getFirmwareRevision();
    /**
    * @brief Get the current value for Hardware Revision
    */
    char *getHardwareRevision();

    /**
    * @brief Set the Auto Report. If it's set on False, then TVControl::sendReport() must be called after a button update
    */
    void setAutoReport(bool value);
    /**
    * @brief Set the HID Report ID
    */
    void setHidReportId(uint8_t value);
    /**
    * @brief Set the Vendor ID
    */
    void setVid(uint16_t value);
    /**
    * @brief Set the Product ID
    */
    void setPid(uint16_t value);
    /**
    * @brief Set the Model Number
    */
    void setModelNumber(char *value);
    /**
    * @brief Set the Software Revision
    */
    void setSoftwareRevision(char *value);
    /**
    * @brief Set the Serial Number
    */
    void setSerialNumber(char *value);
    /**
    * @brief Set the Firmware Revision
    */
    void setFirmwareRevision(char *value);
    /**
    * @brief Set the Hardware Revision
    */
    void setHardwareRevision(char *value);
};

#endif
