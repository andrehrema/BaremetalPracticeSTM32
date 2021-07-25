#include "stm32f042x6.h"
#include "core_cm0.h"

#define BUFFER_SIZE 10
#define BIT4 16
#define BIT5 32
#define BAUDRATE 9600
#define HSICLOCK 8000000
const uint8_t BYTE_SIZE = 8;

uint16_t BIT1 = 1;
const char OK_MESSAGE[] = {'O', 'K'};
const char ACK_MESSAGE[] = {'A', 'C', 'K'};

volatile char g_buffer_uart_rx[BUFFER_SIZE];

volatile uint8_t g_rx_position = 0;
volatile uint8_t g_tx_position = 0;


//pins A10 and A11 set to AF (UART rx and tx)
void ConfigPinAF()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    __DSB();

    GPIOA->MODER |= GPIO_MODER_MODER2_1 + GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= (BIT1 << GPIO_AFRL_AFSEL2_Pos) + (BIT1 << GPIO_AFRL_AFSEL3_Pos);
}


void SetUARTInterruption()
{
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 2);

    // enable receiver and transmitter interruptions
    USART2->CR1 |= USART_CR1_RXNEIE + USART_CR1_TXEIE;
}

void ConfigureUART()
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // enable receiver and transmitter
    USART2->CR1 |= USART_CR1_TE + USART_CR1_RE;

    /*
        - Word length, stop bits, hw control and parity maintained with the initial values:
        word length: 8 data bits;
        1 stop bit;
        no hardware control;
        no parity bit.

        - Baudrate setting:

        for oversampling == 16: USARTDIV = fclk/(baudrate)
        for oversampling == 8: USARTDIV = 2*fclk/(baudrate)

        USARTDIV = 8 000 000/9600
        USARTDIV = 833
    */
    USART2->BRR = (HSICLOCK / BAUDRATE);

    // Setting rx and tx interruption
    SetUARTInterruption();

    // enable uart
    USART2->CR1 |= USART_CR1_UE;
}

void SendAck()
{
    if(g_tx_position < sizeof(ACK_MESSAGE))
    {
        // automatically clear the transfer interruption flag
        USART2->TDR = ACK_MESSAGE[g_tx_position];
        g_tx_position++;
    }
    else
    {
        //cleaning tx interrupt flag
        USART2->ICR |= USART_ICR_TCCF;
        g_tx_position = 0;
    }
}

void USART2_IRQHandler()
{
    // rx interrupt
    if((USART2->ISR & USART_ISR_RXNE))
    {
        g_buffer_uart_rx[g_rx_position] = USART2->RDR;
        g_rx_position++;
        GPIOA->ODR |= BIT5;
    }
    // tx complete interrupt
    else if(USART2->ISR & USART_ISR_TXE)
    {
        GPIOA->ODR |= BIT4;
        SendAck();
    }
    else
    {
        //Do nothing
    }
}

uint8_t strcmp(const char *sentence1, const char *sentence2)
{
    uint8_t retval = 0;
    uint8_t smaller_size = sizeof(sentence1);
    if (sizeof(sentence2) < smaller_size)
    {
        smaller_size = sizeof(sentence2);
    }

    for(uint8_t index = 0; index < smaller_size; index++)
    {
        if(sentence1[index] != sentence2[index])
        {
            retval = 1;
            break;
        }
    }
    return retval;
}

void memset(char *sentence1, uint8_t content, uint8_t size_sentence)
{
    for(uint8_t index = 0; index < size_sentence; index++)
    {
        sentence1[index] = content;
    }
}

int main()
{
    ConfigPinAF();
    ConfigureUART();

    memset(g_buffer_uart_rx, 0, BUFFER_SIZE);

    // pins 4 and 5 in output mode
    GPIOA->MODER |= 256 + 1024;
    GPIOA->PUPDR |= 512 + 2048;

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
