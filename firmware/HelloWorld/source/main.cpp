#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/debug_print.hpp"
#include "L1_Drivers/uart2.hpp"

volatile uint32_t cycles = 500'000;


int main(void)
{
#if SJ2_ENABLE_ANSI_CODES
    // Clears the terminal
    fputs("\e[2J\e[H", stdout);
#endif
    fputs(SJ2_BACKGROUND_GREEN
         "================================== SJTwo Booted! "
         "==================================\n" SJ2_COLOR_RESET, stdout);
    DEBUG_PRINT("Initializing LEDs...");
    
    // -------------------------------------------------------------------
    char receive;    
    Uart test;
    char input = 'A';
    // Set to use UART 2
    // Look at what pins are gonna be used
    DEBUG_PRINT("Initializing UART");
    if(test.Init(9600) == 0)
    {
        printf("Fail!");
    }
    DEBUG_PRINT("Char Input");
    while(1)
    {
        //printf("Input a letter\n");
        //scanf("%c", &input);
        test.Send(input);

        if(!test.Reci())
        {
            printf("Fail!");
        }
        else
            printf("%c",receive);
        
        // Delay
        for(int i = 0; i < 1000; i++)
        {

        }
    }
    return 0;
}
