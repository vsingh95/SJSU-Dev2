#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/delay.hpp"
#include "L2_Utilities/debug_print.hpp"
#include "L1_Drivers/uart2.hpp"

int main(void)
{
    
    Uart test;
    char receive, input = 'A';

    if(test.Init(38400) == 0)
    {
        printf("Fail!");
    }

    while(1)
    {
        test.Send(input);
        receive = test.Reci();
        printf("Sent: %c Recieved: %c \n", input, receive);
        Delay(500);    
    }
    return 0;
}
