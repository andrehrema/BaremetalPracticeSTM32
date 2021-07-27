#include "stm32f042x6.h"
//#include "core_cm0.h"

#define PIN_ALTERNATE_FUNCTION 0x4

volatile uint16_t adc_value = 0;


/*
void ConfigureADC()
{
    //Enabling ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN_Msk;

    //Setting PA0 as input/output analog
    //  It is recommended that the pin read by the ADC should be
    //    configured as analog function

}
*/

void ConfigurePA4asAF2()
{
    // enabling clock of GPIOA

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;
    __DSB();

    GPIOA->MODER |= GPIO_MODER_MODER4_1; // alternate fuction
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4; // push-pull
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR4_1; // low speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1; // pull-down

    GPIOA->AFR[0] |= (PIN_ALTERNATE_FUNCTION << GPIO_AFRL_AFSEL4_Pos); //Alternate function 4, TIM14_CH1

   /*
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    __DSB();
    GPIOA->AFR[0] |= (4u << GPIO_AFRL_AFSEL4_Pos); // 4u means AF4
    GPIOA->MODER |= (0b10u << GPIO_MODER_MODER4_Pos); // 0b10u means AF
    */
}

void ConfigureTIM14()
{
    /*
        Glossary
        UG: Update event bit on TIMx_EGR
        UDIS: enable(0)/disable(1) events, located in TIMx_CR1
    */

    //Enabling TIM14 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    __DSB();
    /*  18.3.2
        The auto reload value set in the auto reload register (TIMx_ARR)
        set the upper limit of the counter */

    /*  The timer countint apparatus is handled mainly by three registers:
        - Counter register (TIMx_CNT) (store the current value);
        - Prescaler register (TIMx_PSC) (divide the clock);
        - Auto-Reload register (TIMx_ARR) (set an upper/bottom limit).
    */
    /*
        PWM is a compare mode.

        PMW frequency is determined by the value of the TIMx_ARR;
        Duty cycle is determined by the value of the TIMx_CCRx;
        TIM_CCMRx set the capture compare mode, OCxPE
    */
    /* Frequency of the peripheral devices is 8MHz
        The value is divided by eight to get 1MHz
        1MHz counting up to 1000 return 1KHz.
    */

    TIM14->PSC = 7; // Pre scaler
    TIM14->ARR = 1000-1; // auto reload value
    TIM14->CCR1 = 700-1; // capture compare register
    TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 + TIM_CCMR1_OC1M_1 + TIM_CCMR1_OC1PE;
    TIM14->CCER |= TIM_CCER_CC1E; //capture compare enable
    // Timer 14 has only one channel, so it is not necessary to specify
    TIM14->BDTR |= TIM_BDTR_MOE;
    TIM14->EGR |= TIM_EGR_UG;
    TIM14->CR1 |= TIM_CR1_CEN;

    /*
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    __DSB();
    TIM14->PSC = 7;
    TIM14->ARR = 1000 - 1;
    TIM14->CCR1 = 1000 - 1;
    TIM14->CCMR1 |= (0b110u << TIM_CCMR1_OC1M_Pos) // 0b110u means PWM mode 1
                | TIM_CCMR1_OC1PE;
    TIM14->CCER |= TIM_CCER_CC1E;
    TIM14->EGR |= TIM_EGR_UG;
    TIM14->CR1 |= TIM_CR1_CEN;
    */
}

/*
void ConfigurePA3asOutput()
{
    // enabling clock of GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;

    GPIOA->MODER |= GPIO_MODER_MODER4_0;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR4_1;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
}
*/

int main(void)
{

    ConfigurePA4asAF2();
    ConfigureTIM14();

    while(1);

    return 0;
}
