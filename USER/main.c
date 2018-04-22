#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "w25qxx.h"
#include "text.h"

#include "HX711.h"
#include "usart.h"
#include "filter.h"
/******************************************************************
У׼������
��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ��������
�����������ʹ����ֵ��׼ȷ�������ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
������Գ���������ƫСʱ����С����ֵ����ֵ����ΪС����
******************************************************************/

int main(void)
{
    //ADCon_InitVal��HX711��AD������ʼֵ
    //ADCon_CurrentVal�����غ��HX711��AD��ǰ����ֵ
    u32 ADCon_InitVal, ADCon_CurrentVal;
    float Weight, GapValue;
    float Weight_Array[10];
    u8 i = 0;
    delay_init();                  //��ʱ������ʼ��

    //LCD��ʼ��
    LCD_Init();                //��ʼ��LCD
    W25QXX_Init();             //��ʼ��W25Q128
    font_init();               //�ֿ���Ϣ��ʼ��
    LCD_Clear(BLACK);          //��ʾ����ɫ
    BACK_COLOR = BLACK;
    POINT_COLOR=GREEN;         //��ʾ��ɫ
    LCD_Display_Dir(0);        //Ĭ��Ϊ����

    /***********************************************************
     ����ʵ��ʹ��USART1ͨ�ſڣ���ͨ�ſ�ʹ��USART1��������жϣ�
     ���䲨����Ϊ115200
    ***********************************************************/
    //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);             //���ڳ�ʼ��Ϊ115200
    Init_HX711pin();               //HX711��ʼ��

    /***********************************************************
     ��ȡѹ����������HX711ת����õ���ADת����ʼֵADCon_InitVal
     �������ǰ��0����ʹADCon_InitVal = ADCon_CurrentVal�� ��ʮ
     ����AD����ֵ�����3λ��0����Ҫ����Ϊ�����λ����������ʱ��
     Ȼ���ϲ���
    ***********************************************************/
    ADCon_InitVal = (HX711_Read() / 1000) * 1000;
    ADCon_CurrentVal = (HX711_Read() / 1000) * 1000;
    while (ADCon_InitVal != ADCon_CurrentVal) {
        ADCon_InitVal = (HX711_Read() / 1000) * 1000;
        ADCon_CurrentVal = (HX711_Read() / 1000) * 1000;
        printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
        printf("ADCon_InitVal = %d \r\n", ADCon_InitVal);
    }
    printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
    printf("ADCon_InitVal = %d \r\n", ADCon_InitVal);

    /***********************************************************
     ��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ����
     �����GapValue��������������ֲ��Գ���������ƫ��ʱ�����Ӹ�
     ��ֵ��������Գ���������ƫСʱ����С����ֵ������ֵ�ܵ���Ӧ
     ��Ƭ��Ӧ���Ӱ�죬��Ӧ����������Թ�ϵ�����ҵ����ص�������
     ͬʱ������Ӧ��Ƭ��Ӧ���������ͬ�����ԣ����Խ����̻������䣬
     �������GapValueֵ��ͬ���Ի�ñȽϾ�ȷ�Ĳ���ֵ
    ***********************************************************/
    GapValue = 520;               //��ֵ���ݲ���ֵ���е�����
    while (1)
    {
        //��ȡѹ����������HX711ת����õ���ADת����ǰֵADCon_CurrentVal
        ADCon_CurrentVal = (HX711_Read() / 1000) * 1000;
        printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
        printf("ADCon_InitVal = %d \r\n", ADCon_InitVal);

        if (ADCon_CurrentVal >= ADCon_InitVal)
        {
            //��ȡʵ���AD������ֵ��������û���κ�����������£�
            //ת��ֵ�����λΪ��0�����Խ�ת��ֵ�����3λ���Բ���
            ADCon_CurrentVal = ADCon_CurrentVal - ADCon_InitVal;

            //����ʵ���ʵ������������ʵ��������Ըı�GapValue��
            //ֵ�����Weight�ټӻ������һ��С��
            Weight_Array[i] = (float)ADCon_CurrentVal / GapValue;
            i++;
            //�����ֶ����
            if(Weight_Array[i]<15)
              GapValue = 502;
            else if(Weight_Array[i]<30)
              GapValue = 501;
            else if(Weight_Array[i]<45)
              GapValue = 501;
            else if(Weight_Array[i]<60)
              GapValue = 515;
            else if(Weight_Array[i]<75)
              GapValue = 516;
            else if(Weight_Array[i]<90)
              GapValue = 516;
            else
              GapValue = 520;
        }
        if(i>9)
        { 
            i = 0;
            Weight = Filter(Weight_Array,10);
            //����ͨ�����ڵ���������ʾADCon_CurrentVal
            printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
            //����ͨ�����ڵ���������ʾ�õ�������ֵ����λΪg
            printf("Weight = %f g \r\n", Weight);
            //Weight = (float)Weight*0.9745+0.6432; //�ع������Է���
            Weight = (float)Weight; //�ع������Է���
            //����ͨ�����ڵ���������ʾ�õ��ĵ�������ֵ����λΪg
            printf("Adjust_Weight = %f g \r\n", Weight);
            POINT_COLOR = RED;
            LCD_ShowString(30,100,300,16,24,"Pressure sensor experiment");
            POINT_COLOR = GREEN;
            LCD_ShowString(30,150,100,16,24,"Weight:");
            LCD_ShowString(74+84+65,150,12,16,24,"g");
            LCD_ShowNum(30+84,150,(int)Weight,3,24);
            LCD_ShowString(66+84,150,16,16,24,".");
            LCD_ShowNum(74+84,150,(Weight-(int)Weight)*100000,5,24);
            LCD_ShowString(250,200,200,16,24,"Sw Young");

        }
        delay_ms(80);
    }
}

