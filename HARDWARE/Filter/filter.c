/**
  ******************************************************************************
  * 文件名程: filter.c
  * 作    者: By Sw Young 
  * 版    本: V1.0
  * 功    能: 对传入数组进行排序，去最大值、最小值后求取平均值 
  * 编写日期: 2017.12.12
  ******************************************************************************
  * 说明：
  * 硬件平台： PC
  *   *****
  * 软件设计说明：
  *   *****
  * Github：
  ******************************************************************************
**/
#include <stdio.h>
/**
  * 函 数 名: swap
  * 函数功能: 交换两个值 
  * 输入参数: 需要交换的两个值 
  * 返 回 值: 无
  * 说    明: 无
  *   By Sw Young 
  *   2017.12.12
  */
void swap(float *left, float *right)
{
    float temp = *left;
    *left = *right;
    *right = temp;
}
/**
  * 函 数 名: SelectSort
  * 函数功能: 对数组进行选择排序 
  * 输入参数: 选哟排序的数组、数组长度 
  * 返 回 值: 无
  * 说    明: 无
  *   By Sw Young 
  *   2017.12.12
  */
void SelectSort(float arr[], int num)
{
    int i, j, Mindex;
    for (i = 0; i < num; i++)
    {
        Mindex = i;
        for (j = i + 1; j < num; j++)
        {
            if (arr[j] < arr[Mindex])
                Mindex = j;
        }

        swap(&arr[i], &arr[Mindex]);
    }
}
/**
  * 函 数 名: Filter
  * 函数功能: 对数组去最大值、最小值后求取平均值 
  * 输入参数: 需要操作的数组、数组长度 
  * 返 回 值: 无
  * 说    明: 无
  *   By Sw Young 
  *   2017.12.12
  */
float Filter(float arr[],int num)
{
	float sum = 0;
	int i = 0;
	SelectSort(arr,num);
	for(i=1;i<num-1;i++)
	{
		sum +=arr[i];
	}
	return sum/(num-2); 
}

//int main(void)
//{
//	double avg;
//	float array[10]={12.5,234.4,34.54,34.34,34.0,45.34,6.23,4.9,34.454,656.454};
//	avg = Filter(array,10);
//	printf("The avg is %.4f",avg);
//}
