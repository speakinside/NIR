#pragma once
#include "datamodel.h"
#include "savemethods.h"

#ifdef USE_FAKE_DEVICE
#include "fakedevice.h"
#else
#include "deviceinterface.h"
#endif


struct Core
{
    DataModel *dataModel;
    DeviceInterface *deviceInterface;
    SaveMethods *saveMethods;
    Core()
    {
#ifdef USE_FAKE_DEVICE
        deviceInterface = new FakeDevice;
#else
        deviceInterface = new DeviceInterface;
#endif
        dataModel = new DataModel(deviceInterface);
        saveMethods = new SaveMethods;
    }
    ~Core()
    {
        delete deviceInterface;
        delete dataModel;
        delete saveMethods;
    }
};
