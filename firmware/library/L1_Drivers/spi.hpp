// Spi abstracts the process of changing the mode and attributes of a
// pin on the LPC40xx series of chips.
//   Usage:
//      Spi P0_0(0, 0);
//      P0_0.SetAsActiveLow();
//      P0_0.SetPinMode(SpiInterface::PinMode::kPullUp);
#pragma once

#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L1_Drivers/pin_configure.hpp"
#include "L2_Utilities/macros.hpp"

class SpiInterface
{
   public:
    virtual void Init() = 0;
};

class Spi : public SpiInterface
{
   public:
    static constexpr uint8_t kSpiFunction = 0b101;
    constexpr Spi(
        uint8_t mosi_port_number, uint8_t mosi_pin_number,
        uint8_t miso_port_number, uint8_t miso_pin_number,
        uint8_t sck_port_number, uint8_t sck_pin_number):
        miso_pin_(mosi_port_number, mosi_pin_number),
        mosi_pin_(miso_port_number, miso_pin_number),
        sck_pin_(sck_port_number, sck_pin_number),
        mosi_(mosi_pin_),
        miso_(miso_pin_),
        sck_(sck_pin_)
        { }
    // Construction using externally constructed mosi, miso, and sck pins.
    // This is a dependency injection point
    constexpr Spi(
        PinConfigureInterface & mosi_pin,
        PinConfigureInterface & miso_pin,
        PinConfigureInterface & sck_pin):
        mosi_(mosi_pin),
        miso_(miso_pin),
        sck_(sck_pin),
        mosi_pin_(PinConfigure::CreateInactivePin()),
        miso_pin_(PinConfigure::CreateInactivePin()),
        sck_pin_(PinConfigure::CreateInactivePin())
        { }
    // constexpr Spi():
    //     mosi_(PinConfigure::CreateInactivePin()),
    //     miso_(PinConfigure::CreateInactivePin()),
    //     sck_(PinConfigure::CreateInactivePin())
    //     { }
    void Init() final override
    {
        mosi_.EnableFastMode();
        mosi_.SetPinFunction(kSpiFunction);
        miso_.EnableFastMode();
        miso_.SetPinFunction(kSpiFunction);
        sck_.EnableFastMode();
        sck_.SetPinFunction(kSpiFunction);
    }
   private:
    PinConfigureInterface & mosi_;
    PinConfigureInterface & miso_;
    PinConfigureInterface & sck_;
    PinConfigure mosi_pin_;
    PinConfigure miso_pin_;
    PinConfigure sck_pin_;
};
