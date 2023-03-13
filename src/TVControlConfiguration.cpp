#include "TVControlConfiguration.h"

TVControlConfiguration::TVControlConfiguration() :  _autoReport(true),
                                                    _hidReportId(1),
                                                    _vid(0x2717),
                                                    _pid(0x32b9),
                                                    _modelNumber("1.0.0"),
                                                    _softwareRevision("1.0.0"),
                                                    _serialNumber("0123456789"),
                                                    _firmwareRevision("1.0.0"),
                                                    _hardwareRevision("1.0.0")
{
}

bool TVControlConfiguration::getAutoReport() { return _autoReport; }
uint8_t TVControlConfiguration::getHidReportId() { return _hidReportId; }
uint16_t TVControlConfiguration::getVid(){ return _vid; }
uint16_t TVControlConfiguration::getPid(){ return _pid; }
char *TVControlConfiguration::getModelNumber(){ return _modelNumber; }
char *TVControlConfiguration::getSoftwareRevision(){ return _softwareRevision; }
char *TVControlConfiguration::getSerialNumber(){ return _serialNumber; }
char *TVControlConfiguration::getFirmwareRevision(){ return _firmwareRevision; }
char *TVControlConfiguration::getHardwareRevision(){ return _hardwareRevision; }


void TVControlConfiguration::setAutoReport(bool value) { _autoReport = value; }
void TVControlConfiguration::setHidReportId(uint8_t value) { _hidReportId = value; }
void TVControlConfiguration::setVid(uint16_t value) { _vid = value; }
void TVControlConfiguration::setPid(uint16_t value) { _pid = value; }
void TVControlConfiguration::setModelNumber(char *value) { _modelNumber = value; }
void TVControlConfiguration::setSoftwareRevision(char *value) { _softwareRevision = value; }
void TVControlConfiguration::setSerialNumber(char *value) { _serialNumber = value; }
void TVControlConfiguration::setFirmwareRevision(char *value) { _firmwareRevision = value; }
void TVControlConfiguration::setHardwareRevision(char *value) { _hardwareRevision = value; }