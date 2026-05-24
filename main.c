#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    __enable_interrupt();


    while(1);
    return 0;

}


