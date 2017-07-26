#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

/*长宽大小*/
#define MAX 15
#define BoardLEN 5 

/*踏板类型宏定义数值*/
#define Sinpe 0
#define Hard 1
#define Fragile 2
#define Move 3
#define HP_PLUS 4 

void gotoxy(int,int);
bool Menu(void); 					       	/*选择菜单*/
void Begin(void);						      /*初始化场景*/
void RandBoard(int);					    /*随机生成踏板*/
void Play(void);						      /*玩家操作*/
void Display(void); 				    	/*显示*/
unsigned int FindMax(void);				/*找出最下面的踏板的y坐标*/
bool ManMoveDetectionA(void);			/*是否进行移动的检测*/
bool ManMoveDetectionD(void);
bool CoordinateComparisonOfMoving(void);/*碰撞踏板的检测*/
int CollisionDetection(void);			  /*踩踏碰撞检测*/
void GenerationDetection(void);			/*生成函数*/
void SomethingWillHappen(int);			/*踩踏板事件生成*/
bool IsDieDetection(void); 			  	/*死亡检测*/
 
int HP = 100;        /*生命*/
int Score = 0;		   /*分数*/
int Times = 0;
int Times_ = 0; 

/*人物位置信息*/
struct MAN
{
	unsigned int x;
	unsigned int y;
	bool Stop;
}Man;

/*游戏场景中的踏板*/
struct BOARD
{
	unsigned int leftx;
	unsigned int rightx;
	unsigned int y;
	int style; 
}Board[4]; 

int main(void)
{
	THE_ORIGIN_SAGA:
	srand((unsigned)(time(0)));
	Begin();
	Play();
	if(Menu())
	{
		goto  THE_ORIGIN_SAGA;
	}
	return 0;
}

bool Menu(void)
{
	char ch;
	Score = Times = Times_ = 0;
	system("cls");
	printf("游戏结束！\n得分:%d\n",Score);
	printf("累计踩踏石板次数:%d\n",Times);
	printf("累计石板生成次数:%d\n",Times_);
	printf("\n\n继续游戏？(Y键继续,其余任意键退出):");
	scanf("%c",&ch);
	return (ch == 'Y' || ch == 'y'); 
}

void Begin(void)
{
	Man.x = 5;
	Man.y = 5;
	Man.Stop = false;
	RandBoard(0);
	Board[0].y = 1;
	RandBoard(1);
	Board[1].y = 5;
	RandBoard(2);
	Board[2].y = 9;
	RandBoard(3);
	Board[3].y = 13;
	Times_++;
}

void RandBoard(int N)
{
	do{
		Board[N].leftx = rand() % MAX;
		Board[N].rightx = Board[N].leftx + (BoardLEN-1);
	}while(Board[N].rightx>MAX); 
	Board[N].style = rand()%5;
	Board[N].y = FindMax() + 4;
}

void Play(void)
{
	char ch;
	int num; 					/*获取玩家正在站立的木板的位置*/
	unsigned int count = 0;
	while(1)
	{
		while(kbhit())
		{
			ch = getch();
			switch(ch)
			{
				case 'a':
				case 'A':
				{
					if(!ManMoveDetectionA())
					{
						Man.x--;
					}
					break; 
				}
				case 'd':
				case 'D':
				{
					if(!ManMoveDetectionD())
					{
						Man.x++;
					}
					break; 
				}
			}
		}
		system("cls");
		GenerationDetection();
		num = CollisionDetection();
		SomethingWillHappen(num); 
		for(count = 0;count < 4;count++)
		{
			Board[count].y--;
		} 
		if(!Man.Stop)
		{
			(Man.y)++;
		}
		else 
		{
			Man.y = Board[num].y-1;
			
		}
		
		if(IsDieDetection())
		{
			return;
		}
		Display();
		Score += 10;
		Sleep(1000);
	}
	return;
}

int CollisionDetection(void)
{
	int x,y,num;
	for(x = 0;x<MAX;x++)
	{
		for(y = 0;y<MAX;y++)
		{
			for(num = 0;num < 4;num++)
			{
				if(Man.y+2 == Board[num].y || Man.y+1 == Board[num].y)
				{
					if(Man.x+2 >= Board[num].leftx && Man.x <= Board[num].rightx)
					{
						Man.Stop = true;
						Times++;
						return num;
					}
				}
			}
		}
	}
	Man.Stop = false;
	return -1;
}

void Display(void)
{
	int N;
	gotoxy(Man.x+10,Man.y+1);
	printf("QAQ");
	gotoxy(20+10,0+1);
	printf("HP:%d",HP);
	gotoxy(20+10,1+1);
	printf("Score:%d",Score);
	gotoxy(10,MAX+1);
	printf("---------------");
	gotoxy(10,0);
	printf("^^^^^^^^^^^^^^^"); 
	for(N = 0;N<4;N++)
	{ 
		if(Board[N].y > MAX)
		{
			continue; 
		} 
		gotoxy(Board[N].leftx+10,Board[N].y+1);
		switch(Board[N].style)
		{
			case Sinpe:
			{	
				printf("^^^^^^");
				break;
			}
			case Hard:
			{
				printf("------");
				break;
			}
			case Fragile:
			{
				printf("~~~~~~");
				break;
			}
			case Move:
			{
				printf("******");
				break;
			}
			case HP_PLUS:
			{
				printf("++++++");
				break; 
			} 
		} 
	}
}

bool ManMoveDetectionA(void)
{
	return (Man.x > 0 && CoordinateComparisonOfMoving());
}

bool ManMoveDetectionD(void)
{
	return (Man.x > 0 && CoordinateComparisonOfMoving());
}

bool CoordinateComparisonOfMoving(void)
{
	int x,y,num;
	for(x = 0;x<MAX;x++)
	{
		for(y = 0;y<MAX;y++)
		{
			for(num = 0;num < 4;num++)
			{
				if(Man.x+1+2 == Board[num].leftx && Man.y == Board[num].y)
				{
					return true;
				}
				else if(Man.x-1 == Board[num].rightx && Man.y == Board[num].y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GenerationDetection(void)
{
	int count;
	for(count = 0;count < 4;count++)
	{
		if(Board[count].y == 0)
		{
			RandBoard(count);
			return;
		}
	}
	return;
}

unsigned int FindMax(void)
{
	unsigned int Tmp,Tmp2; 
	Tmp = max(Board[0].y,Board[1].y);
	Tmp2 = max(Board[2].y,Board[3].y);
	return max(Tmp,Tmp2);
}

void SomethingWillHappen(int num)
{
	/*#define Sinpe 0
	#define Hard 1
	#define Fragile 2
	#define Move 3
	#define HP_PLUS 4 */
	if(num >= 0)
	{ 
		switch(Board[num].style)
		{
			case Sinpe:
			{
				HP-=20;
				break;
			}
			case Fragile:
			{
				RandBoard(num);
				Man.Stop = false;
				break;
			}
			case Move:
			{
				Man.x++;
				break;
			}
			case HP_PLUS:
			{
				HP+=20;
				break;
			} 
		}
	}
}

bool IsDieDetection(void)
{
	if(HP <=0 || Man.y <= 0 ||Man.y >= MAX)
	{
		return true;
	}
	return false; 
}

void gotoxy(int x, int y)							 
{
    COORD pos = {x,y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
    return;
}
