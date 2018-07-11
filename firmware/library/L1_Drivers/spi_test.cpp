// Test for PinConfigure class.
// Using a test by side effect on the LPC_IOCON register
#include <assert.h>
#include <cstdio>
#include "L0_LowLevel/LPC40xx.h"
#include "L5_Testing/testing_frameworks.hpp"
#include "L1_Drivers/pin_configure.hpp"
#include "L1_Drivers/spi.hpp"

TEST_CASE("Testing Spi Dependency Injection", "[spi]")
{
    // Simulated local version of LPC_IOCON register to verify register
    // manipulation by side effect of PinConfigure method calls
    LPC_IOCON_TypeDef local_iocon;

    // Substitute the memory mapped LPC_IOCON with the local_iocon test struture
    // Redirects manipulation to the 'local_iocon'
    PinConfigure::pin_map =
        reinterpret_cast<PinConfigure::PinMap_t *>(&local_iocon);

    PinConfigure a = PinConfigure::CreateInactivePin();
    PinConfigure b = PinConfigure::CreateInactivePin();
    PinConfigure c = PinConfigure::CreateInactivePin();

    Spi test_subject(a, b, c);

    SECTION("Test Pin Function")
    {
        // Source: "UM10562 LPC408x/407x User manual" table 84 page 133
        constexpr uint8_t kP0_0_U3_TXD = 0b010;
        constexpr uint8_t kP2_5_PWM1_6 = 0b001;

        test_subject.Init();
        // CHECK((local_iocon.P2_5 & 0b111) == kP2_5_PWM1_6);
    }
}
