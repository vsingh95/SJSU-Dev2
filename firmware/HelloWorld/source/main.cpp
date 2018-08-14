#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/delay.hpp"
#include "L2_Utilities/debug_print.hpp"
#include "L1_Drivers/uart.hpp"

int main(void)
{
    
    Uart test;
    char receive;
    // char input [] = {'H', 'e', 'l', 'l', 'o', '!','\0'};
    char input2 [] = "Varinder S Singh!";

    if(test.Init(9600, 4) == 0)
    {
        printf("Fail!");
    }

    while(1)
    {
        printf("Please type here; character length is set to 17:\n");
        scanf("%s", input2);

        for( int i=0; input2[i] != '\0'; i++) test.Send(input2[i]);
        for( int j=0; input2[j] != '\0'; j++) 
        {
            receive = test.Reci();
            printf("%c \n",receive);
        } 

    }

    return 0;
}
