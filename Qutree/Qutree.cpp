//实现四叉树编码

#include"stdio.h"
#include <stdlib.h>

void Qutree(int arysize, int level, float curary[])//arysize 表示矩阵长度，level表示等级，curary[]表示当前矩阵
{

	float fi = curary[0];
	int i;
	//遍历当前数组，是否同构
	for (i = 0; i <= arysize*arysize - 1; i++)
	{
		if (fi != curary[i])
		{
			break;
		}

	}
	if (i == arysize*arysize)//结束递归的条件
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
				//左上
				ary1[i*arysize + j] = curary[i*(arysize * 2) + j];
				//右上
				ary2[i*arysize + j] = curary[i*(arysize * 2) + (arysize + j)];
				//左下
				ary3[i*arysize + j] = curary[(arysize + i)*(arysize * 2) + j];
				//右下
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