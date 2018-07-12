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
    // Redirects mahttps://www.ebay.com/itm/70A-100V-48-72v-DC-motor-PWM-Speed-Controller-Reversible-H-bridge-RS232-Arduino-/172809743253on to the 'local_iocon'
    PinConfigure::phttps://www.ebay.com/itm/70A-100V-48-72v-DC-motor-PWM-Speed-Controller-Reversible-H-bridge-RS232-Arduino-/172809743253
        reinterpret_cast<PinConfigure::PinMap_t *>(&local_iocon);

    // PinConfigure a = PinConfigure::CreateInactivePin();
    // PinConfigure b = PinConfigure::CreateInactivePin();
    // PinConfigure c = PinConfigure::CreateInactivePin();
    // Instantiate a mock object.
    Mock<PinConfigureInterface> mock_mosi;
    Mock<PinConfigureInterface> mock_miso;
    Mock<PinConfigureInterface> mock_sck;

    // Setup mock behavior.
    Fake(Method(mock_mosi, EnableFastMode));
    Fake(Method(mock_mosi, SetPinFunction));
    Fake(Method(mock_miso, EnableFastMode));
    Fake(Method(mock_miso, SetPinFunction));
    Fake(Method(mock_sck, EnableFastMode));
    Fake(Method(mock_sck, SetPinFunction));

    // Fetch the mock instance.
    PinConfigureInterface &mosi = mock_mosi.get();
    PinConfigureInterface &miso = mock_miso.get();
    PinConfigureInterface &sck = mock_sck.get();

    Spi test_subject(mosi, miso, sck);

    SECTION("Test Pin Function")
    {
        test_subject.Init();

        Verify(Method(mock_mosi, EnableFastMode));
        Verify(Method(mock_miso, EnableFastMode));
        Verify(Method(mock_sck, EnableFastMode));
        Verify(Method(mock_mosi, SetPinFunction).Using(Spi::kSpiFunction));
        Verify(Method(mock_miso, SetPinFunction).Using(Spi::kSpiFunction));
        Verify(Method(mock_sck, SetPinFunction).Using(Spi::kSpiFunction));
    }
}
