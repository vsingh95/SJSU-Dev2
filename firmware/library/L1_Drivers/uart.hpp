#pragma once

#include <cstdint>


#include "library/config.hpp"
#include "L1_Drivers/pin_configure.hpp"

class Uart_Interface
{
    virtual void SetBaudRate(uint32_t baudrate) = 0;
    virtual bool Initialize(uint32_t baud, uint32_t mode) = 0;
    virtual void Send(char out, uint32_t time_limit) = 0;
    virtual bool Receive(char * char_input, uint32_t time_limit) = 0;
};

class Uart : public Uart_Interface
{
 public:
    LPC_UART_TypeDef * UARTBaseReg;
    LPC_UART1_TypeDef * UARTBaseReg1;
    LPC_UART4_TypeDef * UARTBaseReg4;

    // Can only be used for UART 0, 2, and 3
    // Manually injected into UART1 and 4 cases
    void SetBaudRate(uint32_t baudrate) override
    {
        if(UARTBaseReg1 == (LPC_UART1_TypeDef *)LPC_UART1_BASE)
        {
            // LPC_SC -> PCLKSEL = 1;
            // Enable access to the divisor latches
            UARTBaseReg1 -> LCR |= (1 << 7);
            uint16_t div = (kSystemClockRate / (16 * baudrate)) + 0.5;
            UARTBaseReg1 -> DLM = (div >> 8);
            UARTBaseReg1 -> DLL = (div >> 0);
            // Close off access to divisor latches,
            // enable 2 stop bit, and 8 bit wide word length
            UARTBaseReg1 -> LCR |= (7 << 0);
        }
        else if(UARTBaseReg4 == (LPC_UART4_TypeDef *)LPC_UART4_BASE)
        {
            // LPC_SC -> PCLKSEL = 1;
            // Enable access to the divisor latches
            UARTBaseReg -> LCR |= (1 << 7);
            uint16_t div = (kSystemClockRate / (16 * baudrate)) + 0.5;
            UARTBaseReg -> DLM = (div >> 8);
            UARTBaseReg -> DLL = (div >> 0);
            // Close off access to divisor latches,
            // enable 2 stop bit, and 8 bit wide word length
            UARTBaseReg -> LCR |= (7 << 0);
        }
        else
        {
            // LPC_SC -> PCLKSEL = 1;
            // Enable access to the divisor latches
            UARTBaseReg -> LCR |= (1 << 7);
            uint16_t div = (kSystemClockRate / (16 * baudrate)) + 0.5;
            UARTBaseReg -> DLM = (div >> 8);
            UARTBaseReg -> DLL = (div >> 0);
            // Close off access to divisor latches,
            // enable 2 stop bit, and 8 bit wide word length
            UARTBaseReg -> LCR |= (7 << 0);
        }

    }
    bool Initialize(uint32_t baud, uint32_t mode) override
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
                UARTBaseReg = (LPC_UART_TypeDef *)LPC_UART0_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 3);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 3);
                // Reset bits 0, 1, 2, 6,and 7 in FCR register
                UARTBaseReg -> FCR &= ~(7 << 0 | 3 << 6);
                // Enable FIFO and set Rx trigger to have 1 char timeout
                UARTBaseReg -> FCR = (1 << 0) | (0 << 6);
                // Reset Rx and Tx FIFO
                SetBaudRate(baud);
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
                UARTBaseReg1 = (LPC_UART1_TypeDef *)LPC_UART1_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 4);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 4);
                // Reset bits 0, 1, 2, 6,and 7 in FCR register
                UARTBaseReg1 -> FCR &= ~(7 << 0 | 3 << 6);
                // Enable FIFO and set Rx trigger to have 1 char timeout
                UARTBaseReg1 -> FCR = (1 << 0) | (0 << 6);
                // Reset Rx and Tx FIFO
                // Enable access to the divisor latches
                UARTBaseReg1 -> LCR |= (1 << 7);
                uint16_t div = (kSystemClockRate / (16 * baudrate)) + 0.5;
                UARTBaseReg1 -> DLM = (div >> 8);
                UARTBaseReg1 -> DLL = (div >> 0);
                // Close off access to divisor latches,
                // enable 2 stop bit, and 8 bit wide word length
                UARTBaseReg1 -> LCR |= (7 << 0);
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
                UARTBaseReg = (LPC_UART_TypeDef *)LPC_UART2_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 24);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 24);
                // Reset bits 0, 1, 2, 6,and 7 in FCR register
                UARTBaseReg -> FCR &= ~(7 << 0 | 3 << 6);
                // Enable FIFO and set Rx trigger to have 1 char timeout
                UARTBaseReg -> FCR = (1 << 0) | (0 << 6);
                // Reset Rx and Tx FIFO
                SetBaudRate(baud);
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
                UARTBaseReg = (LPC_UART_TypeDef *)LPC_UART3_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 25);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 25);
                // Reset bits 0, 1, 2, 6,and 7 in FCR register
                UARTBaseReg -> FCR &= ~(7 << 0 | 3 << 6);
                // Enable FIFO and set Rx trigger to have 1 char timeout
                UARTBaseReg -> FCR = (1 << 0) | (0 << 6);
                // Reset Rx and Tx FIFO
                SetBaudRate(baud);
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
                // Set object to base address
                UARTBaseReg4 = (LPC_UART4_TypeDef *)LPC_UART4_BASE;
                // 0 out power bit
                LPC_SC -> PCONP &= ~(1 << 8);
                // Set power bit
                LPC_SC -> PCONP |= (1 << 8);
                // Reset bits 0, 1, 2, 6,and 7 in FCR register
                UARTBaseReg4 -> FCR &= ~(7 << 0 | 3 << 6);
                // Enable FIFO and set Rx trigger to have 1 char timeout
                UARTBaseReg4 -> FCR = (1 << 0) | (0 << 6);
                // Reset Rx and Tx FIFO
                // Enable access to the divisor latches
                UARTBaseReg4 -> LCR |= (1 << 7);
                uint16_t div = (kSystemClockRate / (16 * baudrate)) + 0.5;
                UARTBaseReg4 -> DLM = (div >> 8);
                UARTBaseReg4 -> DLL = (div >> 0);
                // Close off access to divisor latches,
                // enable 2 stop bit, and 8 bit wide word length
                UARTBaseReg4 -> LCR |= (7 << 0);
                break;
            default:
                return false;
        }
    }
    void Send(char out, uint32_t time_limit) override
    {
        uint32_t i = 0;
        if(UARTBaseReg1 == (LPC_UART1_TypeDef *)LPC_UART1_BASE)
        {
            // Load up the register
            UARTBaseReg1 -> THR = out;
            // Send out the data
            while (time_limit > i)
            {
                if (UARTBaseReg1 -> LSR & (1 << 5)) {break;}
                else
                {
                    i++;
                }
            }
        }
        else if(UARTBaseReg4 == (LPC_UART4_TypeDef *)LPC_UART4_BASE)
        {
            // Load up the register
            UARTBaseReg4 -> THR = out;
            // Send out the data
            while (time_limit > i)
            {
                if (UARTBaseReg4 -> LSR & (1 << 5)) {break;}
                else
                {
                    i++;
                }
            }
        }
        else
        {
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


    }
    bool Receive(char * char_input, uint32_t time_limit) override
    {
        if (!char_input){return false;}
        else
        {
            uint32_t i = 0;
            if(UARTBaseReg1 == (LPC_UART1_TypeDef *)LPC_UART1_BASE)
            {    
           // Checks if the input buffer was created
    

               while (time_limit > i)
               {
                   if (UARTBaseReg1 -> LSR & (1 << 0)) {break;}
                   else
                   {
                       i++;
                   }
               }
               char_input = (char *)UARTBaseReg1 -> RBR;
               return true;
            }
            else if(UARTBaseReg4 == (LPC_UART4_TypeDef *)LPC_UART4_BASE)
            {    
               // Checks if the input buffer was created
               while (time_limit > i)
               {
                   if (UARTBaseReg4 -> LSR & (1 << 0)) {break;}
                   else
                   {
                       i++;
                   }
               }
               char_input = (char *)UARTBaseReg4 -> RBR;
               return true;
            }
            else
            {    
               // Checks if the input buffer was created
               while (time_limit > i)
               {
                   if (UARTBaseReg -> LSR & (1 << 0)) {break;}
                   else
                   {
                       i++;
                   }
               }
               char_input = (char*)UARTBaseReg -> RBR;
               return true;
            }

       }
    }
    Uart()
    {
        // Empty
    }
    ~Uart()
    {

    }
};