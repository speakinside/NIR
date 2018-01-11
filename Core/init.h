#pragma once
#include "datamodel.h"
#include "saveproxy.h"
#include <plog/Log.h>
#ifdef USE_FAKE_DEVICE
#include "fakedevice.h"
#else
#include "deviceinterface.h"
#endif

struct Core
{
    DataModel *dataModel;
    DeviceInterface *deviceInterface;
    Core()
    {
#ifdef USE_FAKE_DEVICE
        deviceInterface = new FakeDevice;
        LOG_DEBUG << "USEFAKE";
#else
        deviceInterface = new DeviceInterface;
#endif
        dataModel = new DataModel(deviceInterface);
    }
    ~Core()
    {
        delete deviceInterface;
        delete dataModel;
    }
};
