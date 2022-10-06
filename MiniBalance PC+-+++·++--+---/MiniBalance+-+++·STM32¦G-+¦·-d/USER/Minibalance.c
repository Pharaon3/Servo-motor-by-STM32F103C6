#include "sys.h"
#include "DataScope_DP.h"
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
static float a;
int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	uart_init(72,128000);           //初始化串口1
	while(1)
		{
			a+=0.1;
			if(a>3.14)  a=-3.14; 
			DataScope_Get_Channel_Data(500*sin(a), 1 );
			DataScope_Get_Channel_Data(500* tan(a), 2 );
			DataScope_Get_Channel_Data( 500*cos(a), 3 ); 
			DataScope_Get_Channel_Data( 100*a , 4 );   
			DataScope_Get_Channel_Data(0, 5 );
			DataScope_Get_Channel_Data(0 , 6 );
			DataScope_Get_Channel_Data(0, 7 );
			DataScope_Get_Channel_Data( 0, 8 ); 
			DataScope_Get_Channel_Data(0, 9 );  
			DataScope_Get_Channel_Data( 0 , 10);
			Send_Count = DataScope_Data_Generate(10);
			for( i = 0 ; i < Send_Count; i++) 
			{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer[i]; 
			}
			delay_ms(50); //20HZ
		} 
}
