
/*
=======

>>>>>>> 244f145fb07da5bbab27dcdffc125bd8f63cb858
#include "L1_Drivers/uart.hpp"

int main()
{
    char * receive;
    Uart object;
    
    // Set to use UART 2
    // Look at what pins are gonna be used
    if(object.Initialize(9600, 2) == 0)
    {
    	printf("Fail!");
    }
    
    char sending = "Hello World";
    object.Send(sending, 20);

    if(!object.Receive(receive, 30))
    {
    	printf("Fail!");
    }
    else
    	printf(receive);

    return 0;
<<<<<<< HEAD

*/

