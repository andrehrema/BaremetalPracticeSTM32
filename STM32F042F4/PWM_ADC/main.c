#include "stm32f042x6.h"
#include "core_cm0.h"

#define PIN_ALTERNATE_FUNCTION 0x4


void EnableADCInterrupt()
{
    NVIC_EnableIRQ(ADC1_COMP_IRQn);
    NVIC_SetPriority(ADC1_COMP_IRQn,2);
}


void ConfigureADC()
{
    //Enabling ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN_Msk;
    EnableADCInterrupt();
    __DSB();

    // enabling ADC calibration
    ADC1->CR |= ADC_CR_ADCAL_Msk;
    while((ADC1->CR & ADC_CR_ADCAL_Msk) != 0);


    ADC1->ISR &= ~(ADC_ISR_ADRDY_Msk); // cleaning ready flag on ADC
    ADC1->CR |= ADC_CR_ADEN_Msk; //enabling the adc
    while((ADC1->ISR & ADC_ISR_ADRDY_Msk) != 1);

    ADC1->CHSELR |= ADC_CHSELR_CHSEL1_Msk; // selecting channel 1
    ADC1->SMPR |= (ADC_SMPR_SMP_0 + ADC_SMPR_SMP_1); // 28.5 ADC clock cycles

    ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0; // trigger detection on rising edge
    ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_1; // TIMER2 update event as trigger

    ADC1->IER |= ADC_IER_EOCIE_Msk; // Interruption set to the end of conversion
    ADC1->CR |= ADC_CR_ADSTART_Msk; // start conversion
}


void ConfigurePA4asAF4()
{
    // enabling clock of GPIOA

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;
    __DSB();

    GPIOA->MODER |= GPIO_MODER_MODER4_1; // alternate fuction
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4; // push-pull
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR4_1; // low speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1; // pull-down

    GPIOA->AFR[0] |= (PIN_ALTERNATE_FUNCTION << GPIO_AFRL_AFSEL4_Pos); //Alternate function 4, TIM14_CH1
}


void ConfigureTIM2()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    __DSB();

    TIM2->PSC = 7; // Prescaler_value = TIM2->PSC + 1;
    TIM2->ARR = 1000 - 1;  //Auto reload register value
    TIM2->CR2 |= TIM_CR2_MMS_1; // TRGO = update event
    TIM2->CR1 |= TIM_CR1_CEN;
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
    TIM14->ARR = 4096-1; // auto reload value
    TIM14->CCR1 = 2-1; // capture compare register
    TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 + TIM_CCMR1_OC1M_1 + TIM_CCMR1_OC1PE;
    TIM14->CCER |= TIM_CCER_CC1E; //capture compare enable
    // Timer 14 has only one channel, so it is not necessary to specify
    TIM14->BDTR |= TIM_BDTR_MOE;
    TIM14->EGR |= TIM_EGR_UG;
    TIM14->CR1 |= TIM_CR1_CEN;
}


void ConfigurePA1asADCIn()
{
    // enabling clock on GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;
    __DSB();

    // analog mode
    GPIOA->MODER |= (GPIO_MODER_MODER3_1 + GPIO_MODER_MODER3_0);
}

void ADC1_IRQHandler()
{
    GPIOA->ODR ^= 0x20;
    if(ADC1->ISR & ADC_ISR_EOC_Msk)
    {
        TIM14->CCR1 = ADC1->DR;
    }
    return;
}


int main(void)
{
    ConfigurePA4asAF4();
    ConfigurePA1asADCIn();

    ConfigureADC();

    ConfigureTIM14();
    ConfigureTIM2();

    //setting GPIOA5 in output mode, pull-down
    GPIOA->MODER |= 0x400;
    GPIOA->PUPDR |= 0x400;


    while(1);

    return 0;
}