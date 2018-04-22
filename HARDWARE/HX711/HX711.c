
#include "HX711.h"
#include "delay.h"
#include "usart.h"

u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu = 0;
u8 Flag_Error = 0;

void Init_HX711pin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    //HX711_SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);             //根据设定参数初始化GPIOC

    //HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //输入上拉
    GPIO_Init(GPIOC, & GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);   //初始化设置为0
}

u32 HX711_Read(void)                   //增益128
{
    u32 count;
    u8 i;
    HX711_DOUT = 1;
    delay_us(1);
    HX711_SCK = 0;
    count = 0;
    while (HX711_DOUT);
    for (i = 0; i < 24; i++)
    {
        HX711_SCK = 1;
        count = count << 1;
        delay_us(1);
        HX711_SCK = 0;
        if (HX711_DOUT)
            count++;
        delay_us(1);
    }
    HX711_SCK = 1;
    //count=count^0x800000;//第25个脉冲下降沿来时，转换数据
    delay_us(1);
    HX711_SCK = 0;
    return(count);
}

