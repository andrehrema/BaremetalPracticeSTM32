#include "stm32f030x6.h"

#define BIT0 0x01
#define BIT1 0x02

int main(void)
{
    volatile int val;

    RCC->CR |= 0x1;
    //enabling clock in port A
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    //setting GPIOA0 in output mode
    GPIOA->MODER |= BIT0;
    // push-pull output
    GPIOA->OTYPER &= ~(BIT0);
    GPIOA->OSPEEDR &= ~BIT0;
    GPIOA->PUPDR |= BIT0;


    val = 0;
    while(1)
    {
        if(val == 1000000)
        {
            GPIOA->ODR ^= BIT0;
            val=0;
        }
        val++;
    }

}
