#include "L0_LowLevel/40xx.h"
#include "L1_Drivers/uart.hpp"
#include "L5_Testing/testing_frameworks.hpp"

TEST_CASE("Testing Uart", "[uart]")
{
    LPC_UART2_Typdef UART2;
    Uart R;

    SECTION("Sending No Data but Waiting to Receive")
    {
        constexpr uint8_t EmptyData = 0;
        
        // UART2 will be used for the unit test

        char * a;

        CHECK(R.Initialize(9600,2) == TRUE);
        CHECK(R.Receive(a, 10) == TRUE);
        CHECK((UART2 -> THR) == EmptyData);
        CHECK(a == EmptyData);
    }
}