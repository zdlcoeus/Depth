//ʵ���Ĳ�������

#include"stdio.h"
#include <stdlib.h>

void Qutree(int arysize, int level, float curary[])//arysize ��ʾ���󳤶ȣ�level��ʾ�ȼ���curary[]��ʾ��ǰ����
{

	float fi = curary[0];
	int i;
	//������ǰ���飬�Ƿ�ͬ��
	for (i = 0; i <= arysize*arysize - 1; i++)
	{
		if (fi != curary[i])
		{
			break;
		}

	}
	if (i == arysize*arysize)//�����ݹ������
	{
		printf("%d,%f", level, fi);
		printf("\n");
		return;
	}

	else
	{
		arysize /= 2;
		float *ary1 = new float[arysize*arysize];
		float *ary2 = new float[arysize*arysize];
		float *ary3 = new float[arysize*arysize];
		float *ary4 = new float[arysize*arysize];
		for (i = 0; i < arysize; i++)
		{
			for (int j = 0; j < arysize; j++)
			{
				//����
				ary1[i*arysize + j] = curary[i*(arysize * 2) + j];
				//����
				ary2[i*arysize + j] = curary[i*(arysize * 2) + (arysize + j)];
				//����
				ary3[i*arysize + j] = curary[(arysize + i)*(arysize * 2) + j];
				//����
				ary4[i*arysize + j] = curary[(arysize + i)*(arysize * 2) + (arysize + j)];
			}
		}

		level++;
		Qutree(arysize, level, ary1);
		Qutree(arysize, level, ary2);
		Qutree(arysize, level, ary3);
		Qutree(arysize, level, ary4);

	}

}
int main()
{
	//float aa[64] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	//Qutree(8, 0, aa);



	system("pause");
	return 0;

}