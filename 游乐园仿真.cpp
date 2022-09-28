#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<iostream>

#define spot_number 5//������� 
#define walkspeed 10//�����ھ���֮���ƶ��ٶ�
 
#define visit -1
#define wait -2

int time=0;
int visited_people=0;
int wait_time=0;
int all_people=0;


class spot{//������ 
	public:
		spot(float x,float y,int maxpeople,int playtime)//����Ĺ��췽�� 
		{
			this->x=x;
			this->y=y;
			this->maxpeople=maxpeople;
			this->playtime=playtime;
			this->people=0;
			this->all_people=0;
		}
		
		void drawspot()//���ƾ��㣬�ô�ԲȦ��ʾ��ͬʱչʾ�������ڲι������������������ 
		{
			char ch[10];
			ege_fillellipse(this->x,this->y,35,35);
			sprintf(ch,"%d/%d",this->all_people,this->maxpeople);
			outtextxy(this->x,this->y,ch);
		}
		
		float Distance(spot b)//������������֮��ľ��� 
		{
			return sqrt((this->x-b.x)*(this->x-b.x)+(this->y-b.y)*(this->y-b.y));
		}
		
		float x,y;//����������� 
		int maxpeople,people,playtime,all_people;//����������������������������ڲι��������ι۸þ������ķѵ�ʱ��,�����е������� 
}s[spot_number]={spot(100,22,100,1),spot(3,50,20,5),spot(4,300,40,15),spot(200,250,10,3),spot(100,400,50,20)};

class people{//������ 
	public:
		people()//���췽�� 
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
		void drawpeople()//������������ص��ʾ����ɫ�����Ŷӣ���ɫ���ڲιۣ���ɫ���ڸ�·(����չ�ֳ��Ѿ���·�ľ���) 
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
					
					this->situation=this->location+1;//�ι۲��� 
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
	
		int situation;//��ʾ����״̬��һ�����֣�������0��ʾ����ǰ����������Ӧ�ľ��㣬-2�������ھ����Ŷӣ�-1��ʾ���ڲι�  
		int location;//��ʾ��ǰ�����ľ���λ�� 
		int lasttime;//��ʾִ�е�ǰ״̬��ʱ�䣬���Ѿ��Ŷӵ�ʱ�䣬�Ѿ��ι۵�ʱ�䣬�Ѿ���·��ʱ�� 
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
			if(num<=0)std::cerr<<"����������!";
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
			if(i>=this->length())std::cerr<<"�ڵ���ʴ���!";
			node *p;
			for(p=head;i;i--,p=p->next);
			return p;
		}
		
		people* getpeople(int i)
		{
			if(i>=this->length())std::cerr<<"�ڵ���Ϣ���ʴ���!";
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
			if(location>=this->length())std::cerr<<"�ڵ�ɾ������!";
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
	sprintf(ch,"ʱ�� : %d:%d",time/60+7,time%60);
	outtextxy(300,20,ch);
	
	sprintf(ch,"����԰������ : %d",list.length());
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
	sprintf(ch,"�ι��� : %d",v);
	outtextxy(300,60,ch);
	sprintf(ch,"�Ŷ��� : %d",w);
	outtextxy(300,80,ch);
	sprintf(ch,"��·�� : %d",m);
	outtextxy(300,100,ch);
	sprintf(ch,"�Ѳι� : %d",visited_people);
	outtextxy(300,120,ch);
	sprintf(ch,"������ : %d",all_people);
	outtextxy(300,140,ch);
	sprintf(ch,"ƽ���ȴ�ʱ�� : %d",wait_time/(all_people));
	outtextxy(280,160,ch);
	sprintf(ch,"����");
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

