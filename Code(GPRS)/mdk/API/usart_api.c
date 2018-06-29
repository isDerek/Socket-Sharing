#include<lpc54608_api.h>
#include<fsl_usart.h>
#include "fsl_debug_console.h"
#define DEMO_RING_BUFFER_SIZE 20
extern volatile bool BufferFullFlag;
extern char usart2Buffer[DEMO_RING_BUFFER_SIZE];
extern char regvalue[DEMO_RING_BUFFER_SIZE];
extern volatile uint16_t txIndex; /* Index of the data to send out. */
extern volatile uint16_t i; /* Index of the memory to save new arrived data. */
extern uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
extern uint8_t g_tipString[256];
void usart_writebyByte(USART_Type *base)
{

	        while ((kUSART_TxFifoNotFullFlag & USART_GetStatusFlags(base)) && (i != txIndex))
        {
            USART_WriteByte(base, regvalue[txIndex]);
            txIndex++;
            txIndex %= DEMO_RING_BUFFER_SIZE;
        }
	
}
void usart2_readbyByte(USART_Type *base)
{
    uint8_t data;
    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(base))
    {
        data = USART_ReadByte(base);
        /* If ring buffer is not full, add data to ring buffer. */
        if (((i + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
	
            usart2Buffer[i] = data;
//						PRINTF("READ DATA = %c\n",usart2Buffer[i]);
            i++;          

					if(i == 19)
					{
						BufferFullFlag = true;						
					}  
					i %= DEMO_RING_BUFFER_SIZE-1;
					
        }	
}
}


void usart_readbyByte(USART_Type *base)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(base))
    {
        data = USART_ReadByte(base);
        /* If ring buffer is not full, add data to ring buffer. */
        if (((i + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
	
            regvalue[i] = data;
						PRINTF("READ DATA = %c\n",regvalue[i]);
            i++;          

					if(i == 19)
					{
						BufferFullFlag = true;						
					}  
					i %= DEMO_RING_BUFFER_SIZE-1;
					
        }	
}
}

int usart_readable(USART_Type *base)
{
	 if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(base))
	 {
		 return 1;
	 }
	 else
		 return 0;
}



