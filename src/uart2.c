/*
 * Soubor:  uart.c
 * Datum:   15.01.2021 13:24
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL
 * Úloha:
 * Popis:
 ****************************************************/

#include "uart2.h"

void init_uart2(void)
{
    UART2_DeInit();
    /* UART2 configuration ------------------------------------------------------ */
    UART2_Init((uint32_t)9600,               // - BaudRate = 9600 baud
               UART2_WORDLENGTH_8D,          // - Word Length = 8 Bits
               UART2_STOPBITS_1,             // - One Stop Bit
               UART2_PARITY_NO,              // - No parity
               UART2_SYNCMODE_CLOCK_DISABLE, // - UART2 Clock disabled
               UART2_MODE_TXRX_ENABLE        //  - Receive and transmit enabled
    );
    UART2_Cmd(ENABLE);
}

PUTCHAR_PROTOTYPE
{
    /* Write a character to the UART2 */
    UART2_SendData8(c);
    /* Loop until the end of transmission */
    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET)
    {
        ;
    }
    return (c);
}

/**
 * @brief Retargets the C library scanf function to the USART.
 * @param None
 * @retval char Character to Read
 */
GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
    char c = 0;
#else
    int c = 0;
#endif
    /* Loop until the Read data register flag is SET */
    while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET)
    {
        ;
    }
    c = UART2_ReceiveData8();
    return (c);
}
