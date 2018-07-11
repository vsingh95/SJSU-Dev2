#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <cstdlib>
#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/debug_print.hpp"
#include "L1_Drivers/spi.hpp"
#include "config.hpp"

int main(void)
{
    // This application assumes that all pins are set to function 0 (GPIO)
    DEBUG_PRINT("Pin Configure Application Starting...\n");
    // Using constructor directly to constuct PinConfigure object
    // This is discouraged, since this constructor does not perform any compile
    // time checks on the port or pin value
    Spi spi(0, 7, 0, 2, 0, 3);
    spi.Init();
    DEBUG_PRINT("Halting any action.\n");
    while (1) { continue; }
    return 0;
}
