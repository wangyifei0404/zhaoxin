#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
    if (BitValue) {
        GPIOB->BSRR = (1 << 10);    //设置为高电平
    } else {
        GPIOB->BRR = (1 << 10);		//设置位低电平
    }
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    if (BitValue) { 
        GPIOB->BSRR = (1 << 11);  // 设置为高电平
    } else {
        GPIOB->BRR = (1 << 11);   // 设置PB11为低电平
    }
    Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    
    BitValue = (GPIOB->IDR & (1 << 11)) ? 1 : 0; // 读取PB11的输入电平
    
    Delay_us(10);
    
    return BitValue;
}

void MyI2C_Init(void)
{
	RCC->APB2ENR |=(1<<3) ;   //开启GPIOB的时钟
	
	GPIOB->CRH &= ~(0x0F<<(4*2) | 0x0F<<(4*3));	//清除10和11位的现有配置
	GPIOB->CRH |= (7<<(4*2));  //0x00000E00  GPIOB10 开漏输出 50hz
	GPIOB->CRH |= (7<<(4*3));  //0x0000E000  GPIOB11 开漏输出 50hz
	
	GPIOB->BSRR =(1<<10) | (1<<11);  //设置为高电平

}


void MyI2C_Start(void)
{
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);  //兼容起始条件和重复起始条件
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i<8;i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));   //(BitAction)BitValue 有非0即1的特点  >>右移
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
	
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	MyI2C_W_SDA(1);
	for(i=0;i<8;i++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i );}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);   
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);

	return AckBit;
}
