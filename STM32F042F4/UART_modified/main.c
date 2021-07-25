#include "stm32f042x6.h"
#include "core_cm0.h"
#include <string.h>


#define BUFFER_SIZE 10

uint16_t BIT1 = 1;
const char OK_MESSAGE[] = {'O', 'K'};
const char ACK_MESSAGE[] = {'A', 'C', 'K'};

volatile char g_buffer_uart_rx[BUFFER_SIZE];

uint8_t g_rx_position = 0;
uint8_t g_tx_position = 0;



//pins A10 and A11 set to AF (UART rx and tx)
void ConfigPinAF()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODER10_1 + GPIO_MODER_MODER9_1;
    GPIOA->AFR[1] |= (BIT1 << GPIO_AFRH_AFSEL9_Msk) + (BIT1 << GPIO_AFRH_AFSEL10_Msk);
}


void SetUARTInterruption()
{
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 2);

    // transmission complete and  reception interruptions enabled.
    USART1->CR1 |= USART_CR1_TXEIE + USART_CR1_RXNEIE;
}

void ConfigureUART()
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Word length set to default value: 8 data bits, n stop bits

    /*
        for oversampling == 16: USARTDIV = fclk/(baudrate)
        for oversampling == 8: USARTDIV = 2*fclk/(baudrate)

        USARTDIV = 8 000 000/9600
        USARTDIV = 833
    */
    USART1->BRR = 833;

    // stop bits set to default value: 1 stop bit.

    // enable receiver and transmitter
    USART1->CR1 |= USART_CR1_TE + USART_CR1_RE;

    SetUARTInterruption();

    //Enable UART
    USART1->CR1 |= USART_CR1_UE;
}

void SendAck()
{
    if(g_tx_position < sizeof(ACK_MESSAGE))
    {
        // automatically clear the transfer interruption flag
        for(uint8_t index = 0; index < sizeof(uint8_t); index++)
        {
            USART1->TDR = 0;
            USART1->TDR |= ACK_MESSAGE[g_tx_position] & (BIT1 << index);
        }
        g_tx_position++;
    }
    else
    {
        g_tx_position = 0;
    }
}

void USART1_IRQHandler()
{
    // rx interrupt
    if(USART1->ISR & USART_ISR_RXNE)
    {
        g_buffer_uart_rx[g_rx_position] = USART1->RDR;
        g_rx_position++;
    }
    // tx complete interrupt
    else if(USART1->ISR & USART_ISR_TXE)
    {
        SendAck();
    }
    else
    {
        //Do nothing
    }
}

int main()
{
    ConfigPinAF();
    ConfigureUART();

    memset((void *) g_buffer_uart_rx, 0, BUFFER_SIZE);

    while(1)
    {
        if(0 == strcmp((const char *) g_buffer_uart_rx, OK_MESSAGE))
        {
            g_rx_position = 0;
            SendAck();
        }
    }

    return 0;
}