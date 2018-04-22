#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "w25qxx.h"
#include "text.h"

#include "HX711.h"
#include "usart.h"
#include "filter.h"
/******************************************************************
校准参数：
因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里
这个参数才能使测量值很准确。当发现测试出来的重量偏大时，增加该数值；
如果测试出来的重量偏小时，减小改数值。该值可以为小数。
******************************************************************/

int main(void)
{
    //ADCon_InitVal：HX711的AD采样初始值
    //ADCon_CurrentVal：加载后的HX711的AD当前采样值
    u32 ADCon_InitVal, ADCon_CurrentVal;
    float Weight, GapValue;
    float Weight_Array[10];
    u8 i = 0;
    delay_init();                  //延时函数初始化

    //LCD初始化
    LCD_Init();                //初始化LCD
    W25QXX_Init();             //初始化W25Q128
    font_init();               //字库信息初始化
    LCD_Clear(BLACK);          //显示屏底色
    BACK_COLOR = BLACK;
    POINT_COLOR=GREEN;         //显示颜色
    LCD_Display_Dir(0);        //默认为竖屏

    /***********************************************************
     本次实验使用USART1通信口，该通信口使用USART1接收完成中断，
     传输波特率为115200
    ***********************************************************/
    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);             //串口初始化为115200
    Init_HX711pin();               //HX711初始化

    /***********************************************************
     读取压力传感器经HX711转换后得到的AD转换初始值ADCon_InitVal
     进入称重前清0，即使ADCon_InitVal = ADCon_CurrentVal， 将十
     进制AD采样值的最后3位清0，主要是因为这低三位在重量不变时仍
     然不断波动
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
     因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正
     这里的GapValue这个除数。当发现测试出来的重量偏大时，增加该
     数值。如果测试出来的重量偏小时，减小该数值。该数值受电阻应
     变片的应变的影响，与应变基本呈线性关系，而且当加载的重量不
     同时，电阻应变片的应变会有所不同，所以，可以将量程划分区间，
     各区间的GapValue值不同，以获得比较精确的测量值
    ***********************************************************/
    GapValue = 520;               //该值根据测量值进行调整，
    while (1)
    {
        //读取压力传感器经HX711转换后得到的AD转换当前值ADCon_CurrentVal
        ADCon_CurrentVal = (HX711_Read() / 1000) * 1000;
        printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
        printf("ADCon_InitVal = %d \r\n", ADCon_InitVal);

        if (ADCon_CurrentVal >= ADCon_InitVal)
        {
            //获取实物的AD采样数值。由于在没有任何重量的情况下，
            //转换值最后三位为非0，所以将转换值的最后3位忽略不计
            ADCon_CurrentVal = ADCon_CurrentVal - ADCon_InitVal;

            //计算实物的实际重量，根据实际情况可以改变GapValue的
            //值，或对Weight再加或减补偿一个小数
            Weight_Array[i] = (float)ADCon_CurrentVal / GapValue;
            i++;
            //参数分段拟合
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
            //电脑通过串口调试助手显示ADCon_CurrentVal
            printf("ADCon_CurrentVal = %d \r\n", ADCon_CurrentVal);
            //电脑通过串口调试助手显示得到的重量值，单位为g
            printf("Weight = %f g \r\n", Weight);
            //Weight = (float)Weight*0.9745+0.6432; //回归后的线性方程
            Weight = (float)Weight; //回归后的线性方程
            //电脑通过串口调试助手显示得到的调整重量值，单位为g
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

