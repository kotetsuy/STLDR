/*
 * debug.c
 *
 *  Created on: Jan 27, 2020
 *      Author: Kotetsu Yamamoto
 *      Copyright [Kotetsu Yamamoto]
 *
MIT License

Copyright (c) 2020 Kotetsu Yamamoto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 *
 */

#include "main.h"
#include "debug.h"

#ifdef KYDEBUG

extern void MX_USART3_UART_Init(void);

void Debug_Init(void)
{
    LL_USART_InitTypeDef USARTInit = {0};

    MX_USART3_UART_Init();
    
    USARTInit.BaudRate = 115200;
    USARTInit.DataWidth = 8;
    USARTInit.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USARTInit.OverSampling = LL_USART_OVERSAMPLING_16;
    USARTInit.Parity = LL_USART_PARITY_NONE;
    USARTInit.StopBits = LL_USART_STOPBITS_1;
    USARTInit.TransferDirection = LL_USART_DIRECTION_TX;
    
    LL_USART_Init(USART3, &USARTInit);
    LL_USART_ConfigAsyncMode(USART3);
    LL_USART_Enable(USART3);
}
#else
void Debug_Init(void)
{
}
#endif

#ifdef KYDEBUG
void Debug_Print(uint8_t *s)
{
    while (*s != '\0') {
        while (LL_USART_IsActiveFlag_TXE(USART3) == 0)
            ;
        LL_USART_TransmitData8(USART3, *s);
        s++;
    }
}
#else
void Debug_Print(uint8_t *s)
{
}
#endif

#ifdef KYDEBUG
void Debug_sprintf(uint8_t *d, uint8_t *s, int16_t v)
{
    uint8_t stack[5];
    while (*s != '\0') {
        *d++ = *s++;
    }
    for (int32_t i = 4; i >= 0; i--) {
        stack[i] = '0' + v % 10;
        v /= 10;
    }
    for (int32_t i = 0; i < 5; i++) {
        *d++ = stack[i];
    }
    *d++ = '\0';
}
#else
void Debug_sprintf(uint8_t *d, uint8_t *s, int16_t v)
{
}
#endif

#ifdef KYDEBUG
void Debug_DeInit(void)
{
    LL_USART_Disable(USART3);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);    
}
#else
void Debug_DeInit(void)
{
}
#endif
