
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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);             //�����趨������ʼ��GPIOC

    //HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //��������
    GPIO_Init(GPIOC, & GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);   //��ʼ������Ϊ0
}

u32 HX711_Read(void)                   //����128
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
    //count=count^0x800000;//��25�������½�����ʱ��ת������
    delay_us(1);
    HX711_SCK = 0;
    return(count);
}

