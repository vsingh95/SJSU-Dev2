#include <cstdint>

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/startup.hpp"
#include "L1_Drivers/pin_configure.hpp"
#include "L2_Utilities/debug_print.hpp"

PinConfigure led3(PinConfigure::CreatePinConfigure<1, 15>());

void DemoSystemIsr()
{
    LPC_GPIO1->PIN ^= (1 << 15);
}

void LowLevelInit() // override
{
    constexpr uint8_t kGpioFunction = 0b000;
    led3.SetPinFunction(kGpioFunction);
    LPC_GPIO1->DIR |= (1 << 15);
    LPC_GPIO1->PIN |= (1 << 15);

    system_timer.SetIsrFunction(DemoSystemIsr);
    system_timer.SetTickFrequency(10);
    if(system_timer.StartTimer())
    {
        DEBUG_PRINT("System Timer has begun.");
    }
    else
    {
        DEBUG_PRINT("System Timer has FAILED!!");
    }
}
