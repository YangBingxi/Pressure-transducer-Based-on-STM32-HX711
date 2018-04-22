/**
  ******************************************************************************
  * �ļ�����: filter.c
  * ��    ��: By Sw Young 
  * ��    ��: V1.0
  * ��    ��: �Դ��������������ȥ���ֵ����Сֵ����ȡƽ��ֵ 
  * ��д����: 2017.12.12
  ******************************************************************************
  * ˵����
  * Ӳ��ƽ̨�� PC
  *   *****
  * ������˵����
  *   *****
  * Github��
  ******************************************************************************
**/
#include <stdio.h>
/**
  * �� �� ��: swap
  * ��������: ��������ֵ 
  * �������: ��Ҫ����������ֵ 
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * �� �� ��: SelectSort
  * ��������: ���������ѡ������ 
  * �������: ѡӴ��������顢���鳤�� 
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * �� �� ��: Filter
  * ��������: ������ȥ���ֵ����Сֵ����ȡƽ��ֵ 
  * �������: ��Ҫ���������顢���鳤�� 
  * �� �� ֵ: ��
  * ˵    ��: ��
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
