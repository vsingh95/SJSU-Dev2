#pragma once

#include <cstdint>

#include "LPC40xx.h"
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
        PinConfigure Tx(2,8);
        Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);

        PinConfigure Rx(2,9);
        Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);

        // CHeck page 134 for function page
        Tx.SetPinFunction(2);
        Rx.SetPinFunction(2);

        // Set baud rate
        UARTBaseReg -> LCR |= (1 << 7);
        uint16_t div = (kSystemClockRate / (16 * baud)) + 0.5;
        UARTBaseReg -> DLM = (div >> 8);
        UARTBaseReg -> DLL = (div >> 0);
        UARTBaseReg -> LCR &= ~(1 << 7);

        // Then 8 bits data, 1 stop bit, no parity
        UARTBaseReg->LCR |= (3 << 0);
        UARTBaseReg->LCR |= (1 << 2);
        UARTBaseReg->LCR &= ~(1 << 3);

        // Uart FIFO
        UARTBaseReg->FCR |= (1 << 0);
        UARTBaseReg->FCR |= (1 << 1);
        UARTBaseReg->FCR |= (1 << 2);

        return true;
    }

    void Send (char out) override
    {
        while(! (UARTBaseReg->LSR & (1 << 5)));
        UARTBaseReg->THR = out;
        printf("Sent \n");
    }

    char Reci () override
    {
        char mychar;
        while(! (UARTBaseReg->LSR & (1 << 0)) );
        mychar = UARTBaseReg->RBR;
        return mychar;
    }
};