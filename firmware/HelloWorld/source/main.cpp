#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include <L0_LowLevel/LPC40xx.h>
#include <L0_LowLevel/delay.hpp>
#include <L2_Utilities/debug_print.hpp>
#include <L1_Drivers/uart.hpp>


void Initialize_LCD_Screen( char message[], uint32_t baud, uint32_t mode);

int main(void)
{

    char message[] = "Welcome to SJSU-Dev2 Expo!";
    Initialize_LCD_Screen(message,9600,4);
    Delay(500);

    return 0;
}

void Initialize_LCD_Screen(char message[], uint32_t baud, uint32_t mode)
{
    Uart test;

    if (test.Initialize(baud, mode) == 1)
    {
        printf("Fail!");
    }
    Delay(100);

    test.Send(0x15);
    Delay(100);

    test.Send(0x16);
    Delay(100);

    test.Send(0x11);
    Delay(100);

    // Clear LCD screen
    test.Send(0x0C);
    Delay(100);

    for ( int i=0; message[i] != '\0'; i++) test.Send(static_cast<uint32_t> (message[i]));

}
