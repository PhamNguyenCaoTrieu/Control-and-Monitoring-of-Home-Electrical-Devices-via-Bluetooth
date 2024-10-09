#include "stm32f10x.h"
 
void delay_ms(uint16_t t);
void usart1_sendByte(unsigned char c);
 
int main()
{
  RCC->APB2ENR |= 0xFC | (1<<14); //enable GPIO & USART1 clocks
		
  //USART1 init.
  GPIOA->ODR |= (1<<10);  //pull-up PA10
  GPIOA->CRH = 0x444448B4; /* RX1=input with pull-up, TX1=alt. func. output */
  USART1->CR1 = 0x200C;  // UE = 1, TE = 1, RE = 1
  USART1->BRR = 7500;	 // 72MHz/9600bps = 7500
	
  while(1)
  {		
    usart1_sendByte('H');
    usart1_sendByte('i');
    usart1_sendByte('\n'); //go to new line
    usart1_sendByte('\r'); //carrier return
		
    delay_ms(1000);	//wait 1 second
  }
}
 
void usart1_sendByte(unsigned char c)
{  
   while((USART1->SR&(1<<6)) == 0);  //wait until the TC flag is set = 1, in other words, “while” loop proceeds when TC flag = 0
   USART1->DR = c;
}
 
void delay_ms(uint16_t t)
{
  volatile unsigned long l = 0;
  for(uint16_t i = 0; i < t; i++)
    for(l = 0; l < 6000; l++)
    {
    }
}
//======================================//
#include "stm32f10x.h"
 
void usart2_sendByte(unsigned char c);
uint8_t usart2_recByte(void);
 
int main()
{
  RCC->APB1ENR |= (1<<17); //enable usart2 clock
  RCC->APB2ENR |= 0xFC; //enable GPIO clocks
	
  //USART2 init.
  GPIOA->CRL = 0x44448B44; //RX2=in, TX2=alt. func. Output
  GPIOA->ODR |= (1<<3); // pull-up PA3(RX2)
  USART2->CR1 = 0x200C; // UE = 1, TE = 1, RE = 1

  USART2->BRR = 3750; //36MHz/9600 = 3750	
	
  while(1)
  {	
    uint8_t receivedByte = usart2_recByte();
    usart2_sendByte(receivedByte);
  }
}
 
uint8_t usart2_recByte()
{
  while((USART2->SR&(1<<5)) == 0);  //wait until the RXNE flag is set
  return USART2->DR;	
}
 
void usart2_sendByte(unsigned char c)
{
  while((USART2->SR&(1<<6)) == 0);	//wait until the TC flag is set in other words, “while” loop proceeds when TC flag = 0 
  USART2->DR = c;
}
//======================================//
