#pragma once

#include <cstdint>

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/SystemFiles/system_LPC407x_8x_177x_8x.h"
#include "L1_Drivers/pin_configure.hpp"


class Uart_interface
{

    virtual bool Init (uint32_t baud) = 0;
    virtual void Send (char out) = 0;
    virtual char Reci () = 0;
};

class Uart : public Uart_interface
{
public:
    LPC_UART_TypeDef * UARTBaseReg;
    bool Init (uint32_t baud) override
    {
        UARTBaseReg = (LPC_UART_TypeDef *)LPC_UART2_BASE;
        
        // Set Power bit
        LPC_SC->PCONP &= ~(1 << 24);
        LPC_SC->PCONP |= (1 << 24);

        // Tx object created
        PinConfigure Tx = PinConfigure::CreatePinConfigure<2, 8>();
        //PinConfigure Tx(2,8);
        Tx.SetPinMode(PinConfigureInterface::PinMode::kPullUp);

        PinConfigure Rx = PinConfigure::CreatePinConfigure<2, 9>();
        Rx.SetPinMode(PinConfigureInterface::PinMode::kPullUp);

        // CHeck page 134 for function page
        Tx.SetPinFunction(0b010);
        Rx.SetPinFunction(0b010);


        float baudrate = static_cast<float>(baud);

        // Set baud rate
        UARTBaseReg -> LCR |= (1 << 7);
        uint32_t div = static_cast<uint32_t>(OSC_CLK / (16.0f * baudrate) + 0.5f);
        UARTBaseReg -> DLM = static_cast<uint8_t>(div >> 8);
        UARTBaseReg -> DLL = static_cast<uint8_t>(div >> 0);
        //UARTBaseReg -> LCR &= ~(1 << 7);
        UARTBaseReg -> LCR = 3; 
        // Then 8 bits data, 1 stop bit, no parity
        //UARTBaseReg->LCR |= (3 << 0);
        //UARTBaseReg->LCR &= ~(1 << 2);
        //UARTBaseReg->LCR &= ~(1 << 3);

        // Uart FIFO
        UARTBaseReg->FCR |= (1 << 0);
        UARTBaseReg->FCR |= (1 << 1);
        UARTBaseReg->FCR |= (1 << 2);

        return true;
    }

    void Send (char out) override
    {
        UARTBaseReg->THR = out;
        while(! (UARTBaseReg->LSR & (1 << 5)));
    }

    char Reci () override
    {
        char mychar;
        while(! (UARTBaseReg->LSR & (1 << 0)) );
        mychar = UARTBaseReg->RBR;
        return mychar;
    }
};