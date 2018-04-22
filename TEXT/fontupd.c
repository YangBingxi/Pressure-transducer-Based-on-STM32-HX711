#include "fontupd.h"
#include "w25qxx.h"   
#include "lcd.h"  
#include "string.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//字库更新 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/20
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
#define FONTINFOADDR 	1024*1024*12 					//WarShip STM32F103 V3是从12M地址以后开始存放字库
														//前面12M被fatfs占用了.
														//12M以后紧跟3个字库+UNIGBK.BIN,总大小3.09M,被字库占用了,不能动!
														//15.10M以后,用户可以自由使用.建议用最后的100K字节比较好.
														
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;

 
//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{		
	u8 t=0;
	W25QXX_Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





























