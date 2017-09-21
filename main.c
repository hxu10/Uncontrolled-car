#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define M 30
#define N 36
#define maxstep 100
static double location[2];
static double velocity[2];
static double acceleration[2];
static double control[100][2];
static int path[100][2];
static int map[M+1][N+1];

//random generator
int random(int seed)
{
int  num;
num=16807*(seed%127773)-2836*(seed/127773);
if (num<0) num=num+2147483647;
return num;
}






// function of making map
void makemap()
{
    int i,j,seed;
	double x;


	for(i=0;i<M;i++)
	{	
		for(j=0;j<N;j++)
		{
			map[i][j]=0;
		}
	}
	
	for(j=0;j<N;j++)
	{
	   map[0][j]=1;
	   map[M-1][j]=1;
	}
	for(i=0;i<M;i++)
	{
	   map[i][0]=1;
	   map[i][N-1]=1;
	}
    for(i=0;i<M-6;i++)  map[i][6]=1;
	for(j=8;j<N-5;j++) map[M-5][j]=1;
	for(i=2;i<M-5;i++) map[i][21]=1;
	for(j=26;j<N;j++) map[M-10][j]=1;
	for(i=M-12;i<M-8;i++) map[i][25]=1;
	for(j=21;j<N-5;j++) map[M-16][j]=1;
	for(i=4;i<M-12;i++) map[i][N-6]=1;
	for(i=1;i<11;i++) map[i][26]=1;


	seed = 21;
	for(i=1;i<M-6;i++)
	{
		for(j=7;j<21;j++)
		{
		   seed = random(seed);
	       x= seed *1.0/2147483647;	
		   if(x<0.3) map[i][j]=1;
		}
	}
	map[4][7]=0;
	map[5][7]=0;
	map[5][10]=0;
	map[5][15]=1;
	map[7][7]=0;
	map[7][19]=1;
	map[10][12]=1;
	map[13][16]=1;
	map[15][15]=1;
	map[15][20]=1;
	map[16][7]=0;
	map[19][14]=1;
	map[8][20]=1;








	map[1][21]=2;




}

// function of printing map
void printmap(int i_step)
{
    int i,j,xblock,yblock;

    for(i=0;i<i_step;i++)
	{
	    xblock = path[i][1];
	    yblock = path[i][0];
		map[xblock][yblock] = 4;
	}


	xblock = int(location[1]);
	yblock = int(location[0]);
    map[yblock][xblock] = 3;



//	printf("-----------------------\n");
	for(i=M-1;i>=0;i--)
	{
	    for(j=0;j<N;j++)
		{
			if(map[i][j]==0) printf("  ");
		    if(map[i][j]==1) printf("▇");  // 1 stands for walls or blocks
            if(map[i][j]==2) printf("%c ",6);  //2 stands for final point	
			if(map[i][j]==3) printf("%c ",1);
			if(map[i][j]==4) printf("* ");
		
		}
		printf("\n");
	
	}
	printf("------------------------\n");


}


// the function of moving car and judge if the car hit the block
// 1 represent forward, -1 represent backward
int move(double ax,double ay, int direc)
{
    double dt=0.1;
    int t,xblock,yblock,totalstep;
    int collision = 0;

	if(direc!=1 && direc!=-1)
	{
	   printf("something is wrong with the direction");
	}
    totalstep = int(1/dt);
    for(t=0;t<totalstep;t++)
	{
        if(direc==1)
		{
	        location[0] += velocity[0]*dt + ay*dt*dt*0.5 ;
	    	location[1] += velocity[1]*dt + ax*dt*dt*0.5 ;
		    velocity[0] += ay*dt ;
		    velocity[1] += ax*dt ;
		}
		else
		{
		    velocity[0] -= ay*dt ;
		    velocity[1] -= ax*dt ;	
	        location[0] -= velocity[0]*dt + ay*dt*dt*0.5 ;
	    	location[1] -= velocity[1]*dt + ax*dt*dt*0.5 ;	
		}
	//    printf("location is %lf %lf\n",location[1],location[0]);
        xblock = int(location[1]);
		yblock = int(location[0]);
	//	printf("xblock is %d, yblock is %d, %d\n",xblock,yblock,map[yblock][xblock]);
        if(map[yblock][xblock] == 1) collision = 1;
	//	printf("%d\n",collision);
	}
//	if(direc == -1) 
    return collision;
}

int welcome()
{
	int choice,step,i_step,tem,judge,xblock,yblock;
	double ax,ay;
	FILE *fp;
    printf("欢迎来到“失控的汽车”游戏\n");
	printf("本游戏是由B站：魔法小分队队长 制作。\n");
	printf("其在B站上UID是：10783631\n");
	printf("---------------------------\n");
	printf("游戏的规则如下：\n");
    printf("你要操控一辆刹车不太好的汽车驶过终点\n");
	printf("汽车不能撞到墙\n");
    printf("而且必须在%d时间内完成\n",maxstep);
	printf("汽车的水平和竖直方向的加速度都不能超过1\n");
    printf("当你到达终点并且速度降为0的时候你就胜利了。\n");
	printf("---------------------------\n");
	printf("图例：汽车 %c ,  终点 %c ,  障碍物 ▇ ,  汽车轨迹 *\n",1,6,15);
    printf("输入1代表新游戏，输入2代表读取路径文件control.txt\n");
	scanf("%d",&choice);
	if(choice==2)
	{
	    fp = fopen("control.txt","r");
		if(fp==NULL)
		{
		    printf("找不到文件control.txt!\n");
			exit(0);
		}
        fscanf(fp,"%d",&step);
		for(i_step=1;i_step<=step;i_step++)
		{
		    fscanf(fp,"%d  %lf %lf",&tem,&ax,&ay);
			judge = move(ax,ay,1);
			if(judge==1)
			{
			    printf("汽车在第%d步撞墙了！\n",i_step);
				system("pause");
				exit(0);
			}
			control[i_step][0]=ax;
			control[i_step][1]=ay;
	        xblock = int(location[1]);
         	yblock = int(location[0]);
			path[i_step][0] = xblock;
			path[i_step][1] = yblock;

            
		}
		printf("数据读取成功！下面您将直接从第%d步开始\n",step+1);
	    printf("游戏开始，请把窗口放大到全屏\n");
		system("pause");
		system("cls");
		return step;
	}
	else
	{
	    printf("游戏开始，请把窗口放大到全屏\n");
		system("pause");
		system("cls");
		return 0;
	
	}






}

// the subroutine of saving the path to i_step 

void save(int i_step)
{
	int k;
	FILE * fq;
	fq = fopen("control.txt","w");
    fprintf(fq,"%d\n",i_step);
	for(k=0;k<i_step;k++)
	{
	    fprintf(fq,"%d\t%lf\t%lf\n",k+1,control[k][0],control[k][1]);
	}
}






// main function

int main()
{
	int i_step,step,judge,success;
	int xblock,yblock;
	double ax,ay;
	char cc;
	FILE *fq;




    makemap();   

    location[0]=1;
	location[1]=3;
	velocity[0]=0;
	velocity[1]=0;
    

    step=welcome();
	fq = fopen("control.txt","rw");



    // the process of game
    for(i_step=step;i_step<maxstep;i_step++)
	{
	    printf("第%d步，您的剩余时间%d秒\n",i_step+1,maxstep-i_step);
		printmap(i_step);
		printf("当前位置：（ %5.3f， %5.3f）\n",location[1],location[0]);
		printf("终点位置：（21.000， 1.000）\n");
		printf("您目前的速度：x方向：%lf，y方向：%lf\n",velocity[1],velocity[0]);
		printf("请您输入下一秒的x轴和y轴加速度，两个-1到1之间的数字\n");
		printf("正数代表上方和右方，负数代表下方和左方\n");
		printf("输入两个2代表退出并且保存路径\n");
		scanf("%lf %lf",&ax,&ay);

		if(ax==2 && ay==2) 
		{
            save(i_step);
            printf("路径保存成功！输出到control.txt文件中\n");
			printf("请将该文件备份到别的文件夹，以防被覆盖\n");
			i_step--;
			system("pause");
			exit(0);
            
		}
		else if(fabs(ax)>1.0 || fabs(ay)>1.0)
		{
		    printf("注意！x和y方向的加速度必须在-1与1之间！\n");
		    i_step --;
		}
		else
		{
		    judge = move(ax,ay,1);
			if(judge==1)
			{
				printf("\n");
			    printf("您撞到墙上了，游戏结束！\n");
				if(i_step>2)
				{
				   printf("系统自动保存您的路径到前%d步，并输出到control.txt中\n",i_step-2);
				   save(i_step-2);
				}
				scanf("%c",&cc);
                system("pause");
				exit(0);
			}
			else
			{
			    printf("输入成功！");
				control[i_step][0]=ax;
				control[i_step][1]=ay;
	        	xblock = int(location[1]);
         		yblock = int(location[0]);
				path[i_step][0] = xblock;
				path[i_step][1] = yblock;	
			}

		
		
		
		}
		
		xblock = int(location[1]);
		yblock = int(location[0]);
		if(map[yblock][xblock]==2)
		{
		    if(velocity[0]==0 && velocity[1]==0) success = 1;
		}
    system("pause");
	system("cls");
	
	
	
	
	
	    if(success==1) break;
	}

    if(success==0) printf("游戏失败，您未能在规定时间到达终点!\n");
    if(success==1) 
	{ 
		printf("游戏成功！您的用时是%d\n",i_step);
		printf("路径已经保存到conrol.txt，快去炫耀一下吧！\n");
		save(i_step);

	
	}






    return 0;
}
