#pragma once

#include <cstdint>

#include "pin_configure.hpp"

class Uart_Interface
{
    virtual void SetBaudRate(uint32_t baudrate) = 0;
    virtual bool Initialize(uint32_t baud, uint32_t mode) = 0;
    virtual void SendData(char out, uint32_t time_limit) = 0;
    virtual bool ReceiveData(char* char_input, uint32_t time_limit) = 0;
};

class Uart : public : Uart_Interface
{
 public:
    void SetBaudRate(uint32_t baudrate) override
    {
        uint32_t PCLK = 96000000;
        // Setting the PCLK for now until we know
        // what function is used to access it
        // LPC_SC -> PCLKSEL = 1;
        // Enable access to the divisor latches
        UARTBaseReg -> LCR |= (1 << 7);
        uint16_t div = (PCLK / (16 * baudrate)) + 0.5;
        UARTBaseReg -> DLM = (div >> 8);
        UARTBaseReg -> DLL = (div >> 0);
        // Close off access to divisor latches,
        // enable 2 stop bit, and 8 bit wide word length
        UARTBaseReg -> LCR |= (7 << 0);
    }
    bool Initialize(uint32_t baud, uint32_t mode)
    {
        // Power up and enable the appropriate UART,
        // configure pins, and NVIC Interrupt
        switch (mode)
        {
            case 0:
                // Configure Pins
                PinConfigure Tx(0, 2);
                Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                PinConfigure Rx(0, 3);
                Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                // Check pages starting at 133
                // for documentation on pin functions
                Tx.SetPinFunction(4);
                Rx.SetPinFunction(4);
                // Set object to base address of UART0
                UARTBaseReg = (unsigned int)LPC_UART0_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 3);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 3);
                break;
            case 1:
                // Configure Pins
                PinConfigure Tx(2, 0);
                Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                PinConfigure Rx(2, 1);
                Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                // Check pages starting at 133
                // for documentation on pin functions
                Tx.SetPinFunction(2);
                Rx.SetPinFunction(2);
                // Set object to base address of UART1
                UARTBaseReg = (unsigned int)LPC_UART1_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 4);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 4);
                break;
            case 2:
                // Configure Pins
                PinConfigure Tx(2, 8);
                Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                PinConfigure Rx(2, 9);
                Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                // Check pages starting at 133
                // for documentation on pin functions
                Tx.SetPinFunction(2);
                Rx.SetPinFunction(2);
                // Set object to base address of UART2
                UARTBaseReg = (unsigned int)LPC_UART2_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 24);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 24);
                break;
            case 3:
                // Configure Pins
                PinConfigure Tx(4, 28);
                Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                PinConfigure Rx(4, 29);
                Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                // Check pages starting at 133
                // for documentation on pin functions
                Tx.SetPinFunction(2);
                Rx.SetPinFunction(2);
                // Set object to base address of UART3
                UARTBaseReg = (unsigned int)LPC_UART3_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 25);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 25);
                break;
            case 4:
                // Configure Pins
                PinConfigure Tx(1, 29);
                Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                PinConfigure Rx(2, 9);
                Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
                // Check pages starting at 133
                // for documentation on pin functions
                Tx.SetPinFunction(5);
                Rx.SetPinFunction(3);
                // Set object to base address of UART4
                UARTBaseReg = (unsigned int)LPC_UART4_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 8);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 8);
                break;
            default:
                return false;
        }
        SetBaudRate(baud);
        // Reset bits 0, 1, 2, 6,and 7 in FCR register
        UARTBaseReg -> FCR &= ~(7 << 0 | 3 << 6);
        // Enable FIFO and set Rx trigger to have 1 char timeout
        UARTBaseReg -> FCR = (1 << 0) | (0 << 6);
        // Reset Rx and Tx FIFO
    }
    void Send(char out, uint32_t time_limit)
    {
        uint32_t i = 0;
        // Load up the register
        UARTBaseReg -> THR = out;
        // Send out the data
        while (time_limit > i)
        {
            if (UARTBaseReg -> LSR & (1 << 5)) {break;}
            else
            {
                i++;
            }
        }
    }
    bool Receive(char * char_input, uint32_t time_limit)
    {
       uint32_t i = 0;
       // Checks if the input buffer was created
       if (!char_input){return false;}
       else
       {
           while (time_limit > i)
           {
               if (UARTBaseReg -> LSR & (1 << 0)) {break;}
               else
               {
                   i++;
               }
           }
           char_input = UARTBaseReg -> RBR;
           return true;
       }
    }
};
