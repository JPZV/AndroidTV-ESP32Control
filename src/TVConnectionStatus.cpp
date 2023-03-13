#include "TVConnectionStatus.h"

TVConnectionStatus::TVConnectionStatus(void)
{
}

void TVConnectionStatus::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo)
{
    pServer->updateConnParams(connInfo.getConnHandle(), 6, 7, 0, 600);
    this->connected = true;
}

void TVConnectionStatus::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo& connInfo, int reason)
{
    this->connected = false;
}
