#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<iostream>

#define spot_number 5//景点个数 
#define walkspeed 10//人流在景区之间移动速度
 
#define visit -1
#define wait -2

int time=0;
int visited_people=0;
int wait_time=0;
int all_people=0;


class spot{//景点类 
	public:
		spot(float x,float y,int maxpeople,int playtime)//景点的构造方法 
		{
			this->x=x;
			this->y=y;
			this->maxpeople=maxpeople;
			this->playtime=playtime;
			this->people=0;
			this->all_people=0;
		}
		
		void drawspot()//绘制景点，用大圆圈表示，同时展示景点正在参观人数与最大容纳人数 
		{
			char ch[10];
			ege_fillellipse(this->x,this->y,35,35);
			sprintf(ch,"%d/%d",this->all_people,this->maxpeople);
			outtextxy(this->x,this->y,ch);
		}
		
		float Distance(spot b)//返回两个景点之间的距离 
		{
			return sqrt((this->x-b.x)*(this->x-b.x)+(this->y-b.y)*(this->y-b.y));
		}
		
		float x,y;//景点横纵坐标 
		int maxpeople,people,playtime,all_people;//景点所能容纳最大人数，景点正在参观人数，参观该景点所耗费的时间,景点中的总人数 
}s[spot_number]={spot(100,22,100,1),spot(3,50,20,5),spot(4,300,40,15),spot(200,250,10,3),spot(100,400,50,20)};

class people{//人物类 
	public:
		people()//构造方法 
		{
			all_people++;
			int i;
			this->situation=1;
			this->location=0;
			this->lasttime=0;
			for(i=0;i<spot_number;i++)
			this->visited[i]=0;
			this->visited[0]=1;
		}
		void drawpeople()//绘制人物，用像素点表示，黑色正在排队，红色正在参观，绿色正在赶路(可以展现出已经赶路的距离) 
		{
			switch (this->situation)
			{
				case wait:
				{
					putpixel(s[location].x+5+rand()%20,s[location].y+15+rand()%10,BLACK);
					break;
				}
				case visit:
				{
					putpixel(s[location].x+5+rand()%20,s[location].y+15+rand()%10,LIGHTRED);
					break;
				}
				default:
				{
					float per=lasttime*walkspeed/s[location].Distance(s[situation]);
					putpixel(s[location].x*(1-per)+s[situation].x*per+rand()%20+5,s[location].y*(1-per)+s[situation].y*per+rand()%20+10,GREEN);
				}
			}
		}
		void decision()
		{
			lasttime++;
			if(this->situation>=0)
			{
				if(lasttime*walkspeed/s[location].Distance(s[situation])>1)
				{
					this->location=this->situation;
					this->situation=wait;
					s[this->location].all_people++;
					lasttime=0;
				}
			}
			if(this->situation==wait)
			{
				if(s[this->location].people<s[this->location].maxpeople)
				{
					this->situation=visit;
					s[this->location].people++;
					lasttime=0;
				}
			}
			if(this->situation==visit)
			{
				if(this->lasttime>=s[this->location].playtime)
				{
					
					this->situation=this->location+1;//参观策略 
					s[this->location].all_people--;
					s[this->location].people--;
					this->lasttime=0;
					this->visited[this->location]=1;
				}
			}
		}
		int visited_all()
		{
			int ans=1,i;
			for(i=0;i<spot_number;i++)ans*=this->visited[i];
			return ans;
		}
	
		int situation;//表示人物状态，一共三种，正数或0表示正在前往正数所对应的景点，-2代表正在景点排队，-1表示正在参观  
		int location;//表示当前所处的景区位置 
		int lasttime;//表示执行当前状态的时间，如已经排队的时间，已经参观的时间，已经赶路的时间 
		int visited[spot_number]={0};
};

struct node{
	people *People;
	node *next=NULL;
};

class LinkList{
	public:
		
		LinkList(int num)
		{
			if(num<=0)std::cerr<<"链表创建错误!";
			int i;
			node *p=NULL,*q=NULL;
			for(i=0;i<num;i++)
			{
				p=(node*)malloc(sizeof(node));
				if(head==NULL)
				{
					head=p;
				}
				else
				{
					q->next=p;
				}
				q=p;
				p->People=new people();
			}
			p->next=NULL;
			tail=p;
		}
		
		node* getnode(int i)
		{
			if(i>=this->length())std::cerr<<"节点访问错误!";
			node *p;
			for(p=head;i;i--,p=p->next);
			return p;
		}
		
		people* getpeople(int i)
		{
			if(i>=this->length())std::cerr<<"节点信息访问错误!";
			node *p;
			for(p=head;i;i--,p=p->next);
			return p->People;
		}
		
		int length()
		{
			node *p=NULL;
			int l;
			for(l=0,p=head;p;p=p->next,l++);
			return l;
		}
		
		void push(people* in)
		{
			node *p=(node*)malloc(sizeof(node));
			p->People=in;
			tail->next=p;
			tail=p;
			tail->next=NULL;
		}
		
		void pull(int location)
		{
			if(location>=this->length())std::cerr<<"节点删除错误!";
			int i;
			node *p,*q;
			if(location==0){
				p=head;
				head=head->next;
			}else{
				for(i=1,q=head,p=head->next;i<location;i++,q=q->next,p=p->next);
				q->next=p->next;
			}
			free(p->People);
			free(p);
			visited_people++;
		}
		
		void people_generate()
		{
			int num;
			for(num=3;num;num--)
			{
				this->push(new people());
			}
			
		}
		
		private:
			node *head=NULL,*tail=NULL;
		
}list(1);

void drawstatic()
{
	int v=0,w=0,m=0,i;
	char ch[30];
	sprintf(ch,"时间 : %d:%d",time/60+7,time%60);
	outtextxy(300,20,ch);
	
	sprintf(ch,"游乐园中人数 : %d",list.length());
	outtextxy(280,40,ch);
	for(i=0;i<list.length();i++)
	{
		switch(list.getpeople(i)->situation)
		{
			case -1:{
				v++;
				break;
			}
			case -2:{
				wait_time++;
				w++;
				break;
			}
			default:{
				m++;
				break;
			}
		}
	}
	sprintf(ch,"参观中 : %d",v);
	outtextxy(300,60,ch);
	sprintf(ch,"排队中 : %d",w);
	outtextxy(300,80,ch);
	sprintf(ch,"赶路中 : %d",m);
	outtextxy(300,100,ch);
	sprintf(ch,"已参观 : %d",visited_people);
	outtextxy(300,120,ch);
	sprintf(ch,"总人数 : %d",all_people);
	outtextxy(300,140,ch);
	sprintf(ch,"平均等待时间 : %d",wait_time/(all_people));
	outtextxy(280,160,ch);
	sprintf(ch,"大门");
	outtextxy(s[0].x,s[0].y-20,ch);
}

int main()
{
	
	
	int i;
	initgraph(400,480,0);
	setbkmode(TRANSPARENT);
	setfillcolor(EGEARGB(0x99,0x30,0x40,0x40));
	setcolor(EGEARGB(0xff,0x80,0x00,0xf0));
	setbkcolor(WHITE);
	
	
	for(;;Sleep(100),cleardevice())
	{
	list.people_generate();
	for(i=0;i<spot_number;i++)
	s[i].drawspot();
	drawstatic();
	for(i=0;i<list.length();i++){
	list.getpeople(i)->drawpeople();
	list.getpeople(i)->decision();
	if(list.getpeople(i)->visited_all())
	list.pull(i);
	}
	time++;
	}
	scanf("%d",&i);
	closegraph();
	
	
	
}

