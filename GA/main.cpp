#include<stdio.h>
#include<stdlib.h> 
#include<math.h>
#include<time.h> 
#include<iostream>
#include <vector>
#include <algorithm> // std::move_backward
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock


#define M 100				//种群数量
#define N 2					//变量个数
#define PC 0.8				//交叉概率
#define PM 0.04				//变异概率
#define PA 0.4				//交叉因子

#define COPYNUM 4


using namespace std;

struct Node
{
	double X[N];
	double Myfitness;		//Myfitness是适应度
	double Myfitpercent;		//Myfitsum是适应度占总体适应度的百分比，然后从第一个个体往后累加，主要用于选择操作
}Nownode[M], Nextnode[M], TempNode[COPYNUM];	//本代群体和下一代群体,TempNode存放每一代最优的COPYNUM个个体


double XMIN[N] = { -100,-100 };//每一个参数的最小值
double XMAX[N] = { 100,100 };//每一个参数的最大值



double fx(double *x);
int sortnode();
int save_best(int n);
int replace_worst(int n);
int calfitpercent();
int calfitness();
double randn(int index);
int init();
int assignment(struct Node *node1, struct Node *node2);
int evolution();
int crossover();
int mutation();


double fx(double *x)			//根据x计算fx
{
	double y;
	int i = 0, px[N] = { 0 };
	y = 4.5 - (pow(sin(sqrt(pow(x[0], 2) + pow(x[1], 2))), 2) - 0.5) / pow((1 + 0.001*(pow(x[0], 2) + pow(x[1], 2))), 2);//schaffer函数
	return y;
}

int sortnode()
{
	int i, j;
	struct Node temp;
	for (i = 0; i < M; i++)
	{
		for (j = i; j < M; j++)
		{
			if (Nownode[i].Myfitness < Nownode[j].Myfitness)
			{
				assignment(&temp, &Nownode[i]);
				assignment(&Nownode[i], &Nownode[j]);
				assignment(&Nownode[j], &temp);
			}
		}
	}
	calfitpercent();
	return 0;
}

int replace_worst(int n)
{
	int i;
	sortnode();
	for (i = 0; i < n; i++)
	{
		assignment(&Nownode[M - 1 - i], &TempNode[i]);
	}
	calfitpercent();
	return 0;
}

int save_best(int n)
{
	int i;
	sortnode();
	for (i = 0; i < n; i++)
	{
		assignment(&TempNode[i], &Nownode[i]);
	}
	return 0;
}

int calfitpercent()
{
	int i;
	Nownode[0].Myfitpercent = Nownode[0].Myfitness;
	for (i = 1; i < M; i++)
	{
		Nownode[i].Myfitpercent = Nownode[i].Myfitness + Nownode[i - 1].Myfitpercent;//每一个Myfitsum都是自己的适应度加上前一个的Myfitsum
	}


	for (i = 0; i < M; i++)
	{
		Nownode[i].Myfitpercent = Nownode[i].Myfitpercent / Nownode[M - 1].Myfitpercent;//每一个Myfitsum除以所有适应度之和，使Myfitsum为0~1之间
	}
	return 0;
}

int calfitness()			//计算适应度值
{
	int i;
	double minfitness, maxfitness, avefitness = 0;
	double temp;
	minfitness = Nownode[0].Myfitness = fx(Nownode[0].X);
	maxfitness = minfitness;
	avefitness = maxfitness;
	for (i = 1; i < M; i++)
	{
		Nownode[i].Myfitness = fx(Nownode[i].X);
		avefitness += Nownode[i].Myfitness;
		if (minfitness > Nownode[i].Myfitness)
		{
			minfitness = Nownode[i].Myfitness;
		}
		if (maxfitness < Nownode[i].Myfitness)
		{
			maxfitness = Nownode[i].Myfitness;
		}
	}

	if (minfitness < 0)//如果有负的适应度值，就把所以的适应度都加上一个数，使适应度全都为正数
	{
		temp = minfitness;
		Nownode[0].Myfitness += -temp;
		avefitness = Nownode[0].Myfitness;
		maxfitness = Nownode[0].Myfitness;
		minfitness = Nownode[0].Myfitness;
		for (i = 1; i < M; i++)
		{
			Nownode[i].Myfitness += -temp;
			avefitness += Nownode[i].Myfitness;
			if (minfitness > Nownode[i].Myfitness)
			{
				minfitness = Nownode[i].Myfitness;
			}
			if (maxfitness < Nownode[i].Myfitness)
			{
				maxfitness = Nownode[i].Myfitness;
			}
		}
	}

	calfitpercent();
	return 0;
}

double randn(int index)		//产生XMIN到XMAX之间的随机数
{
	return XMIN[index] + 1.0*rand() / RAND_MAX * (XMAX[index] - XMIN[index]);
}

int init()	//初始化种群
{
	int i, j;
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			Nownode[i].X[j] = randn(j);
		}
	}
	calfitness();		//计算适应度
	return 0;
}

int assignment(struct Node *node1, struct Node *node2)//把node2的值赋值给node1
{
	int j;
	for (j = 0; j < N; j++)
	{
		node1->X[j] = node2->X[j];
	}
	node1->Myfitness = node2->Myfitness;
	node1->Myfitpercent = node2->Myfitpercent;
	return 0;
}

int evolution()		//轮盘法复制
{
	int i, num = 0;
	double temp;
	while (num < M)
	{
		temp = 1.0*rand() / RAND_MAX;
		for (i = 1; i < M; i++)
		{
			if (temp <= Nownode[0].Myfitpercent)
			{
				assignment(&Nextnode[num++], &Nownode[0]);
				break;
			}
			if (temp >= Nownode[i - 1].Myfitpercent&&temp <= Nownode[i].Myfitpercent)
			{
				assignment(&Nextnode[num++], &Nownode[i]);
				break;
			}
		}
	}
	for (i = 0; i < M; i++)
	{
		assignment(&Nownode[i], &Nextnode[i]);	//更新本代个体
	}
	calfitpercent();
	return 0;
}

int crossover()
{
	int i, j, temp;
	double temp_pc;

	

	for (j = 0; j < N; j++)
	{
		vector<int> v;

		for (int i = 0; i < M; i++) {
			v.push_back(i);
		}

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(v.begin(), v.end(), std::default_random_engine(seed));

		for (i = 0; i < M; i = i + 2)
		{
			temp_pc = 1.0*rand() / RAND_MAX;	//如果满足交叉的条件，就开始交叉
			if (temp_pc <= PC)
			{
				double c1= PA * Nownode[v[i + 1]].X[j] + (1 - PA)*Nownode[v[i]].X[j];
				double c2= PA * Nownode[v[i]].X[j] + (1 - PA)*Nownode[v[i + 1]].X[j];
				double test[N];
				for (int k = 0; k < N; k++) {
					if (k == j)test[k] = c1;
					else test[k] = Nownode[v[i]].X[k];
				}

				if(fx(test)> Nownode[v[i]].Myfitness)
				Nownode[v[i]].X[j] = c1;


				for (int k = 0; k < N; k++) {
					if (k == j)test[k] = c2;
					else test[k] = Nownode[v[i+1]].X[k];
				}

				if (fx(test) > Nownode[v[i+1]].Myfitness)
				Nownode[v[i + 1]].X[j] = c2;

			}
		}
	}
	calfitness();		//计算适应度
	return 0;
}

int mutation()			//变异操作
{
	int i, j, temp;
	double k = 0.3, temp_pm;
	for (j = 0; j < N; j++)
	{
		for (i = 0; i < M; i++)
		{
			temp_pm = 1.0*rand() / RAND_MAX;
			if (temp_pm <= PM)	//如果满足变异条件，就开始变异
			{
				temp = rand() % 2;
				if (temp == 0)
				{
					Nownode[i].X[j] = Nownode[i].X[j] + k * (XMAX[j] - Nownode[i].X[j])*1.0*rand() / RAND_MAX;
				}
				else
				{
					Nownode[i].X[j] = Nownode[i].X[j] - k * (Nownode[i].X[j] - XMIN[j])*1.0*rand() / RAND_MAX;
				}
			}
		}
	}
	calfitness();	//计算适应度
	return 0;
}

int main()
{

	clock_t startTime, endTime;
	startTime = clock();
	int T = 0;
	srand(time(NULL));

	init();
	while (T++ < 2000)
	{
		save_best(COPYNUM);//保留前COPYNUM好的个体
		evolution();
		crossover();
		mutation();
		replace_worst(COPYNUM);//替换最差的COPYNUM的个体
	}

	

	sortnode();
	for (int j = 0; j < N; j++)
	{
		cout <<"x"<<j<<": "<< Nownode[0].X[j] << endl;
	}
	cout <<"函数值： "<< fx(Nownode[0].X) << endl;

	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	system("pause");
	return 0;
}