#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<graphics.h>
#define N 5 //景点个数 
#define T0 50000//设置初始退火温度为50000 
#define r 0.98//退火系数
#define T_min 0.00000001
#define time 1000

int path[N]={0};
int new_path[N]={0};
float point[N][2]={
{100,22},{3,50},{4,300},{200,250},{100,400}
};


float distance(int a,int b);
float path_length(int path[]);
void init_path();
void generate_path();
void draw();

float distance(int a,int b)
{
	return(sqrt((point[a][0]-point[b][0])*(point[a][0]-point[b][0])+(point[a][1]-point[b][1])*(point[a][1]-point[b][1])));
}

float path_length(int path[])
{
	float sum=0;
	int i=0;
	for(i=0;i<N-1;i++)
	{
		sum+=distance(path[i],path[i+1]);
	}
	sum+=distance(path[N-1],path[0]);
	return sum;
}

void init_path()
{
	int i=0;
	for(i=0;i<N;i++)
	{
		path[i]=i;
	}
}

void generate_path()
{
	int a=0,b=0,i=0;
	while(a==b)
	{
		a=rand()%N;
		b=rand()%N;
	}
	for(i=0;i<N;i++)
	{
		if(i==a)
		{
			new_path[i]=path[b];
		}else if(i==b)
		{
			new_path[i]=path[a];
		}
		else new_path[i]=path[i];
	}
	
}


int main()
{
	
	srand(42);
	
	int i=0,t=0;
	float dl=0,T=0,v=0;
	
	init_path();
	
	for(T=T0;T>T_min;T*=r)
	{
		for(t=0;t<time;t++)
		{
			generate_path();
			dl=path_length(new_path)-path_length(path);
			v=((double)rand())/(RAND_MAX);
			if(exp(-dl/T)>v)
			{
				for(i=0;i<N;i++)
				{
					path[i]=new_path[i];
				}
			}
		}
	}
	
	
	for(i=0;i<N;i++)
	{
		printf("%d",path[i]);
	}
	printf("\n");
	
	printf("%f",path_length(path));
	draw();
}

void draw()
{
	int i;
	initgraph(400,480,0);
	setbkmode(TRANSPARENT);
	setfillcolor(EGEARGB(0x99,0x30,0x40,0x40));
	setcolor(EGEARGB(0xff,0x80,0x00,0xf0));
	setbkcolor(WHITE);
	
	for(i=0;i<N-1;i++)
	{
		ege_fillellipse(point[i][0],point[i][1],35,35);
		ege_line(point[path[i]][0],point[path[i]][1],point[path[i+1]][0],point[path[i+1]][1]);
	}
	
	ege_fillellipse(point[N-1][0],point[N-1][1],35,35);
	ege_line(point[path[N-1]][0],point[path[N-1]][1],point[path[0]][0],point[path[0]][1]);
	scanf("%d",&i);
	closegraph();
	
}
