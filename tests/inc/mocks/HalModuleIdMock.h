#ifndef ARTRONICS_HALMODULEIDMOCK_H
#define ARTRONICS_HALMODULEIDMOCK_H

#include "gmock/gmock.h"
#include "hal_module_id.h"

class HalModuleIdMock {
public:
    virtual ~HalModuleIdMock()= default;;
    MOCK_METHOD(int, HalModuleId_init, ());
    MOCK_METHOD(uint8_t *, HalModuleId_get64BitId, ());
};

#endif //ARTRONICS_HALMODULEIDMOCK_H
