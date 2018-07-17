#pragma once

#include "L0_LowLevel/LPC40xx.h"
#include "L1_Drivers/pin_configure.hpp"
#include "queue.h" // Change this when FreeRTOS changes are put in

class Uart_Interface
{
	virtual void SetBaud(unsigned int baudrate) = 0;
    virtual bool Initialize(unsigned int baud, uint32_t queue_size = 16) = 0;
    virtual void SendData(char out, unsigned int time_limit) = 0;
    virtual bool ReceiveData(char* char_input, unsigned int time_limit) = 0;
};
class Uart_Base : public Uart_Interface
{
	public:
		static LPC_UART_TypeDef* UARTBaseReg;		
        virtual void InterruptHandler() = 0;//Need to rethink how I am going to implement this class
		//Class Methods
		void SetBaud(unsigned int baudrate) override
		{

		}
		bool Initialize(unsigned int baud, uint32_t queue_size = 16, uint32_t mode) override
		{	
			//Power up and enable the appropriate UART, configure pins, and NVIC Interrupt
			switch(mode){
				case 0:
					//Configure Pins
					PinConfigure Tx(0,2);
					Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					PinConfigure Rx(0,3);
					Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					//Check pages starting at 133 for documentation on pin functions
					Tx.SetPinFunction(4);
					Rx.SetPinFunction(4);
					//Set object to base address of UART0
					UARTBaseReg = (unsigned int)LPC_UART0_BASE;
					//0 out power bit
					LPC_SC -> PCONP &= ~(1 << 3);
					//Set power bit
					LPC_SC -> PCONP |= (1 << 3);
					//Enable interrupt request for UART0
					isr_register(UART0_IRQn, InterruptHandler);
					NVIC_EnableIRQ(UART0_IRQn);
					break;
				case 1:
					//Configure Pins
					PinConfigure Tx(2,0);
					Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					PinConfigure Rx(2,1);
					Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					//Check pages starting at 133 for documentation on pin functions
					Tx.SetPinFunction(2);
					Rx.SetPinFunction(2);
					//Set object to base address of UART1
					UARTBaseReg = (unsigned int)LPC_UART1_BASE;
					//0 out power bit
					LPC_SC -> PCONP &= ~(1 << 4);
					//Set power bit
					LPC_SC -> PCONP |= (1 << 4);
					//Enable interrupt request for UART1
					isr_register(UART1_IRQn, InterruptHandler);
					NVIC_EnableIRQ(UART1_IRQn);
					break;
				case 2:
					//Configure Pins
					PinConfigure Tx(2,8);
					Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					PinConfigure Rx(2,9);
					Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					//Check pages starting at 133 for documentation on pin functions
					Tx.SetPinFunction(2);
					Rx.SetPinFunction(2);
					//Set object to base address of UART2
					UARTBaseReg = (unsigned int)LPC_UART2_BASE;
					//0 out power bit
					LPC_SC -> PCONP &= ~(1 << 24);
					//Set power bit
					LPC_SC -> PCONP |= (1 << 24);
					//Enable interrupt request for UART2
					isr_register(UART2_IRQn, InterruptHandler);
					NVIC_EnableIRQ(UART2_IRQn);
					break;
				case 3:
					//Configure Pins
					PinConfigure Tx(4,28);
					Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					PinConfigure Rx(4,29);
					Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					//Check pages starting at 133 for documentation on pin functions
					Tx.SetPinFunction(2);
					Rx.SetPinFunction(2);
					//Set object to base address of UART3
					UARTBaseReg = (unsigned int)LPC_UART3_BASE;
					//0 out power bit
					LPC_SC -> PCONP &= ~(1 << 25);
					//Set power bit
					LPC_SC -> PCONP |= (1 << 25);
					//Enable interrupt request for UART3
					isr_register(UART3_IRQn, InterruptHandler);
					NVIC_EnableIRQ(UART3_IRQn);
					break;
				case 4:
					//Configure Pins
					PinConfigure Tx(1,29);
					Tx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					PinConfigure Rx(2,9);
					Rx.SetPinMode(PinConfigureInterface::PinMode::kInactive);
					//Check pages starting at 133 for documentation on pin functions
					Tx.SetPinFunction(5);
					Rx.SetPinFunction(3);
					//Set object to base address of UART4
					UARTBaseReg = (unsigned int)LPC_UART4_BASE;
					//0 out power bit
					LPC_SC -> PCONP &= ~(1 << 8);
					//Set power bit
					LPC_SC -> PCONP |= (1 << 8);
					//Enable interrupt request for UART4
					isr_register(UART4_IRQn, InterruptHandler);
					NVIC_EnableIRQ(UART4_IRQn);
					break;
				default:
					return false;
			}
			setBaud(baud);
			//Reset bits 0, 1, 2, 6,and 7 in FCR register
			UARTBaseReg -> FCR &= ~(7 << 0 | 3 << 6);
			//Enable FIFO and set Rx trigger to have 1 char timeout
			UARTBaseReg -> FCR = (1 << 0) | (0 << 6);
			//Reset Rx and Tx FIFO
		}
		void InterruptHandler(void) override
		{
			char data;
			while(!UARTBaseReg -> LSR & (1 << 0));
			data = UARTBaseReg -> RBR;
			QueueSendFromISR(Rx, &data, NULL);//Done on purpose. If the person is not grabbing the data from the queue
											  //then they will lose their data. They will either have to set a larger
											  //or make time to grab the data.
		}
		void SendData(char out) override
		{
			//Load up the register
			UARTBaseReg -> THR = out;
			//Send out the data
			while(UARTBaseReg -> LSR & (1 << 5));
		}
		bool ReceiveData(char* char_input, unsigned int time_limit) override
		{
			//Checks if the input buffer and the receiving queue was created
			if(!char_input || !Rx)
				return false;
			else{
				while(!QueueReceive(Rx, &char_input, time_limit));
				return true;
			}	
		}
	private:
		//Objects
		//Queue Handle Object
		QueueHandle_t Rx;
		//Pin COnfiguration Objects
		PinConfigure Tx;
		PinConfigure Rx;
};