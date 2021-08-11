#include <stdio.h>
#include "jpeglib.h"
#include "yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>
#include <pthread.h>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>


#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "show.h"
#include "lcd.h"
#include "listenClient.h"

// 存放触摸坐标
int x;
int y;
int key_status;


// 定义按键
#define K1 1
#define K2 2
#define K3 3
#define K4 4
#define K5 5
#define K6 6
#define K7 7

#define K_OPEN 10
#define K_CLOSE 11
#define K_ALARM 12
//

//定义楼层参数
#define FLOOR_MAX 7
#define FLOOR_MIN 1

//定义锁状态
#define UNLOCKED 0
#define LOCKED 1

//定义按键状态
#define KEY_ACTIVATE 1
#define KEY_INACTIVE 0

//定义电梯运行状态

#define RUN_FREE 0
#define RUN_UP 1
#define RUN_UP_STOP 2
#define RUN_DOWN -1
#define RUN_DOWN_STOP -2

typedef struct RunTimeStatus 
{
    int btn_status[11];
    int btn_lock[11];
    int btn_up_client[8];
    int btn_down_client[8];
    int current_floor;
    int runStatus;
    int upMaxFloor;     //上升最大楼层
    int downMinFloor;   //下降最低楼层
    int clientUpMaxFloor;   //客户端最高楼层
    int clientDownMinFloor; //客户端最低楼层

    // int upMax;
    // int upMin;
    // int downMax;
    // int downMin;
} ERunTimeStatus,*PRunTimeStatus;

ERunTimeStatus runTimeStatus;

void initRunTimeStatus(PRunTimeStatus runTimeStatus)
{
    int i;
    for(i = 0; i < 8;i++)
    {
        runTimeStatus->btn_up_client[i] = 0;
        runTimeStatus->btn_down_client[i] = 0;
    }
    for(i = 0;i < 11;i++)
    {
        runTimeStatus->btn_status[i] = KEY_INACTIVE;
        runTimeStatus->btn_lock[i] = UNLOCKED;
    }
    runTimeStatus->current_floor = FLOOR_MIN;
    runTimeStatus->runStatus = RUN_FREE;
    runTimeStatus->upMaxFloor = 0;
    runTimeStatus->downMinFloor = 8;
    runTimeStatus->clientUpMaxFloor = 0;
    runTimeStatus->clientDownMinFloor = 8;

    for(i = 0;i < 11;i++)
    {
        printf("btn%d：%d\n",i,runTimeStatus->btn_status[i]);
        printf("lock%d：%d\n",i,runTimeStatus->btn_lock[i]);
        
    }
    printf("current_floor:%d\n",runTimeStatus->current_floor);
}

struct Point 
{
    int x;
    int y;
};

int getPoint(int *x,int *y,int *key_status)
{
   	// int x,y;
	
	
	//1、打开触摸屏驱动
	int ts_fd;
	ts_fd=open("/dev/input/event0",O_RDWR);
	if(ts_fd==-1)
	{
		perror("打开触摸屏失败\n");
		return -1;
	}
	
	//定义事件结构体
	struct input_event ts_event;
	
	while(1)
	{
		//2、读取触摸坐标
		read(ts_fd,&ts_event,sizeof(ts_event));
		
		//3、	处理坐标，判断信息 if
		if(ts_event.type==EV_ABS)//判断当前是否为触摸
		{
			if(ts_event.code==ABS_X)//判断当前触摸坐标是否是x轴
			{
				*x=ts_event.value;//输出x轴坐标
				//printf("x轴的坐标为：%d\n",x);
			}
			if(ts_event.code==ABS_Y)//判断当前触摸坐标是否是x轴
			{
				*y=ts_event.value;//输出x轴坐标
				//printf("y轴的坐标为：%d\n",y);
			}
			printf("当前的坐标为：(%d,%d)\n",*x,*y);
		}
		
		//判断是否按下   按下：1  松开0
		if(ts_event.type==EV_KEY &&ts_event.code==BTN_TOUCH && ts_event.value==0)
		{
			printf("当前的触摸值：%d\n",ts_event.value);
            printf("最后的坐标为：(%d,%d)\n",*x,*y);
			printf("you have leaving....\n");

            break;
		}
	}
	
	//4、关闭触摸
    *key_status = 0;
	close(ts_fd);
	
    return 0;
}

int getKey(int x,int y)
{
    if (x >= 600 && x <= 700 && y >= 0 && y <= 96)
    {
        printf("按下了7\n");
        return K7;
    }
    else if (x >= 700 && x <= 800 && y >= 0 && y<= 96)
    {
        printf("按下了报警\n");
        return K_ALARM;
    }
    else if (x >= 600 && x <= 700 && y > 96 && y <= 192)
    {
        printf("按下了5\n");
        return K5;
    }
    else if (x >= 700 && x <= 800 && y > 96 && y <= 192)
    {
        printf("按下了6\n");
        return K6;
    }
    else if (x >= 600 && x <= 700 && y > 192 && y <= 288)
    {
        printf("按下了3\n");
        return K3;
    }
    else if (x >= 700 && x <= 800 && y > 192 && y <= 288)
    {
        printf("按下了4\n");
        return K4;
    }
    else if (x >= 600 && x <= 700 && y > 288 && y <= 384)
    {
        printf("按下了1\n");
        return K1;
    }
    else if (x >= 700 && x <= 800 && y > 288 && y <= 384)
    {
        printf("按下了2\n");
        return K2;
    }
    else if (x >= 600 && x <= 700 && y > 384 && y <= 480)
    {
        printf("按下了关闭\n");
        return K_CLOSE;
    }
    else if (x >= 700 && x <= 800 && y > 384 && y <= 480)
    {
        printf("按下了打开\n");
        return K_OPEN;
    }
    return -1;
}


void cancelBtn(int key)
{
    
    switch(key)
    {
        case K1:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(600,288,"./btn/11.jpg");
            break;
        }
        case K2:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(700,288,"./btn/21.jpg");
            break;
        }
        case K3:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(600,192,"./btn/31.jpg");
            break;
        }
        case K4:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(700,192,"./btn/41.jpg");
            break;
        }
        case K5:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(600,96,"./btn/51.jpg");
            break;
        }
        case K6:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(700,96,"./btn/61.jpg");
            break;
        }
        case K7:
        {
            runTimeStatus.btn_status[key] = 0;
            lcd_draw_jpg(600,0,"./btn/71.jpg");
            break;
        }
    }
        
    
}

int count_activate_key()
{
    int count = 0;
    int i;
    for(i = 1; i <= FLOOR_MAX;i++)
    {
        if(runTimeStatus.btn_status[i] == KEY_ACTIVATE)
        {
            count++;
        }
    }
    return count;
}






void *cancelClientKey(void* message)
{
    int fd;  //file discription
    struct hostent *he;
    struct sockaddr_in server, client;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error!\n");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t len;
    len = sizeof(struct sockaddr_in);

    printf("%s\n",message);
    sendto(fd, message, strlen(message), 0, (struct sockaddr *)&server, len);
    close(fd);

}

void *bj()
{
    int fd;  //file discription
    struct hostent *he;
    struct sockaddr_in server, client;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error!\n");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(BJ_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t len;
    len = sizeof(struct sockaddr_in);
    char* message = "bj";
    printf("开始报警:%s\n",message);
    sendto(fd, message, strlen(message), 0, (struct sockaddr *)&server, len);
    close(fd);

}


void createMessage(int run,int floor)
{
    // char* message;
    if(run == RUN_UP)
    {
        switch(floor)
        {
            case 1:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP1);
                printf("关闭1楼上行按钮\n");
                break;
            }
            case 2:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP2);
                printf("关闭2楼上行按钮\n");
                break;
            }
            case 3:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP3);
                printf("关闭3楼上行按钮\n");
                break;
            }
            case 4:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP4);
                printf("关闭4楼上行按钮\n");
                break;
            }
            case 5:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP5);
                printf("关闭5楼上行按钮\n");
                break;
            }
            case 6:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP6);
                printf("关闭6楼上行按钮\n");
                break;
            }
        }
    }
    else if(run == RUN_DOWN)
    {
        switch(floor)
        {
            case 2:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN2);
                printf("关闭2楼下行按钮\n");
                break;
            }
            case 3:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN3);
                printf("关闭3楼下行按钮\n");
                break;
            }
            case 4:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN4);
                printf("关闭4楼下行按钮\n");
                break;
            }
            case 5:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN5);
                printf("关闭5楼下行按钮\n");
                break;
            }
            case 6:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN6);
                printf("关闭6楼下行按钮\n");
                break;
            }
            case 7:
            {
                pthread_t pid_cancel_client_key;
	            pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN7);
                printf("关闭7楼下行按钮\n");
                break;
            }
        }
    }
}



void changeUpMaxFloor(int floor)
{
    if(floor > runTimeStatus.upMaxFloor)
    {
        runTimeStatus.upMaxFloor = floor;
        runTimeStatus.runStatus = RUN_UP;
    }
}

void changeDownMinFloor(int floor)
{
    if(floor < runTimeStatus.downMinFloor)
    {
        runTimeStatus.downMinFloor = floor;
        runTimeStatus.runStatus = RUN_DOWN;
    }
}

int getBtnMin()
{
    int i;
    for(i = 1;i < 8;i++)
    {
        if(runTimeStatus.btn_status[i] == 1)
        {
            return i;
        }

    }
    return 8;
}
int getBtnMax()
{
    int i;
    for(i = 7;i > 0;i--)
    {
        if(runTimeStatus.btn_status[i] == 1)
        {
            return i;
        }

    }
    return 0;
}

int getUpMin()
{
    int i;
    for(i = 1;i < 8;i++)
    {
        if(runTimeStatus.btn_up_client[i] == 1)
        {
            return i;
        }

    }
    return 8;
}
int getUpMax()
{
    int i;
    for(i = 7;i > 0;i--)
    {
        if(runTimeStatus.btn_up_client[i] == 1)
        {
            return i;
        }

    }
    return 0;
}
int getDownMax()
{
    int i;
    for(i = 7;i > 0;i--)
    {
        if(runTimeStatus.btn_down_client[i] == 1)
        {
            return i;
        }

    }
    return 0;
}
int getDownMin()
{
    int i;
    for(i = 1;i < 8;i++)
    {
        if(runTimeStatus.btn_down_client[i] == 1)
        {
            return i;
        }

    }
    return 8;
}

int getRunMaxFloor()
{
    int btn = getBtnMax();
    int up = getUpMax();
    int down = getDownMax();
    int temp;
    if(btn > up)
    {
        temp = btn;
    }
    else
    {
        temp = up;
    }

    if(temp < down)
    {
        return down;
    }
    return temp;

}

int getRunMinFloor()
{
    int btn = getBtnMin();
    int up = getUpMin();
    int down = getDownMin();
    int temp;
    if(btn < up)
    {
        temp = btn;
    }
    else
    {
        temp = up;
    }
    if(temp > down)
    {
        return down;
    }
    return temp;
}



void *run()
{
    while(1)
    {
        if(runTimeStatus.runStatus == RUN_FREE)
        {
            continue;
        }
        else if(runTimeStatus.runStatus == RUN_UP)
        {
            int i = runTimeStatus.current_floor;

            for(;i < getRunMaxFloor();i++)
            {
                printf("目标最高楼层：%d\n",getRunMaxFloor());
                char pathname[20] = {0};
                sprintf(pathname, "./f/%d.jpg", i + 1);
                sleep(1);
                lcd_draw_jpg(360,240,"./btn/up1.jpg");
                
                sleep(1);
                lcd_draw_jpg(360,0,pathname);
                int count = 0;
                while(count < 3)
                {
                    printf("正在上升:%d\n",count);
                    // lcd_draw_jpg(360,240,"./btn/up1.jpg");
                    sleep(1);
                    // lcd_draw_jpg(360,240,"./btn/white.jpg");
                    count++;
                }
                runTimeStatus.current_floor++;

                // if(runTimeStatus.btn_status[runTimeStatus.current_floor] == 1 && runTimeStatus.current_floor == getRunMaxFloor())
                // {

                // }

                if(runTimeStatus.btn_status[runTimeStatus.current_floor] == 1 || runTimeStatus.btn_up_client[runTimeStatus.current_floor] == 1)
                {
                    lcd_draw_jpg(360,240,"./btn/up2.jpg");
                    if(runTimeStatus.btn_status[runTimeStatus.current_floor] == 1)
                    {
                        cancelBtn(runTimeStatus.current_floor);
                        
                        // printf("已到达%d楼\n",runTimeStatus.current_floor);
                        // printf("已打开%d楼电梯门\n",runTimeStatus.current_floor);
                        // lcd_draw_jpg(700,384,"./btn/open2.jpg");


                        // int count = 0;
                        // while(count < 3)
                        // {
                        //     sleep(1);
                        //     count++;
                        // }
                        // printf("已关闭%d楼电梯门\n",runTimeStatus.current_floor);
                        // lcd_draw_jpg(700,384,"./btn/open1.jpg");

                        // if(runTimeStatus.current_floor == getRunMaxFloor() && getRunMinFloor() == 8)
                        // {
                        //     lcd_draw_jpg(360,240,"./btn/free.jpg");
                        //     runTimeStatus.runStatus = RUN_FREE;
                        // }
                        // if(runTimeStatus.current_floor == getRunMaxFloor() && getRunMinFloor() != 8)
                        // {
                        //     lcd_draw_jpg(360,240,"./btn/free.jpg");
                        //     runTimeStatus.runStatus = RUN_FREE;
                        // }

                    }
                    if(runTimeStatus.btn_up_client[runTimeStatus.current_floor] == 1)
                    {
                        runTimeStatus.btn_up_client[runTimeStatus.current_floor] = 0;
                        createMessage(RUN_UP,runTimeStatus.current_floor);
                        
                    }


                    printf("已到达%d楼\n",runTimeStatus.current_floor);
                    printf("已打开%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open2.jpg");


                    int count = 0;
                    while(count < 3)
                    {
                        sleep(1);
                        count++;
                    }
                    printf("已关闭%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open1.jpg");

                }

                if(runTimeStatus.current_floor == getRunMaxFloor() && runTimeStatus.btn_down_client[runTimeStatus.current_floor] == 1)
                {
                    lcd_draw_jpg(360,240,"./btn/down2.jpg");
                    printf("上行转下行\n");
                    printf("已到达%d楼\n",runTimeStatus.current_floor);
                    printf("已打开%d楼电梯门\n",runTimeStatus.current_floor);




                    lcd_draw_jpg(700,384,"./btn/open2.jpg");

                    int count = 0;
                    while(count < 3)
                    {
                        sleep(1);
                        count++;
                    }
                    printf("已关闭%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open1.jpg");

                    runTimeStatus.btn_down_client[runTimeStatus.current_floor] = 0;
                    createMessage(RUN_DOWN,runTimeStatus.current_floor);
                    runTimeStatus.runStatus = RUN_DOWN;
                    break;
                }

                if(runTimeStatus.current_floor >= getRunMaxFloor())
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                    break;
                }

                printf("getRunMinFloor----->%d",getRunMinFloor());
            }

            // if(getRunMaxFloor() == runTimeStatus.current_floor)
            // {
            //     printf("getRunMinFloor----->%d",getRunMinFloor());
            //     if(getRunMinFloor() != 8)
            //     {
            //         lcd_draw_jpg(360,240,"./btn/down1.jpg");
            //         runTimeStatus.runStatus = RUN_DOWN;
            //     }
            //     lcd_draw_jpg(360,240,"./btn/free.jpg");
            //     // runTimeStatus.upMaxFloor = 0;
            //     runTimeStatus.runStatus = RUN_FREE;
            // }
            
            if(getRunMaxFloor() == 0 && getRunMinFloor() == 8)
            {
                lcd_draw_jpg(360,240,"./btn/free.jpg");
                runTimeStatus.runStatus = RUN_FREE;
            }

        }
        else if(runTimeStatus.runStatus == RUN_DOWN)
        {
            int i = runTimeStatus.current_floor;
            for(;i > getRunMinFloor();i--)
            {
                printf("i---->%d\n",i);
                printf("目标最低楼层：%d\n",getRunMinFloor());
                char pathname[20] = {0};
                sprintf(pathname, "./f/%d.jpg", i - 1);
                sleep(1);
                lcd_draw_jpg(360,240,"./btn/down1.jpg");
                sleep(1);                
                lcd_draw_jpg(360,0,pathname);
                int count = 0;
                while(count < 3)
                {
                    printf("正在下降:%d\n",count);
                    // lcd_draw_jpg(360,240,"./btn/up1.jpg");
                    sleep(1);
                    // lcd_draw_jpg(360,240,"./btn/white.jpg");
                    count++;
                }
                runTimeStatus.current_floor--;
                if(runTimeStatus.btn_status[runTimeStatus.current_floor] == 1 || runTimeStatus.btn_down_client[runTimeStatus.current_floor] == 1)
                {
                    lcd_draw_jpg(360,240,"./btn/down2.jpg");

                    if(runTimeStatus.btn_status[runTimeStatus.current_floor] == 1)
                    {
                        cancelBtn(runTimeStatus.current_floor);
                    }
                    if(runTimeStatus.btn_down_client[runTimeStatus.current_floor] == 1)
                    {
                        runTimeStatus.btn_down_client[runTimeStatus.current_floor] = 0;
                        createMessage(RUN_DOWN,runTimeStatus.current_floor);
                        
                    }


                    printf("已到达%d楼\n",runTimeStatus.current_floor);
                    printf("已打开%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open2.jpg");

                    int count = 0;
                    while(count < 3)
                    {
                        sleep(1);
                        count++;
                    }
                    printf("已关闭%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open1.jpg");
                    // runTimeStatus.btn_status[runTimeStatus.current_floor] = 0;
                }


                if(runTimeStatus.current_floor == getRunMinFloor() && runTimeStatus.btn_up_client[runTimeStatus.current_floor] == 1)
                {
                    lcd_draw_jpg(360,240,"./btn/up2.jpg");
                    printf("下行转上行\n");

                    printf("已到达%d楼\n",runTimeStatus.current_floor);
                    printf("已打开%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open2.jpg");

                    int count = 0;
                    while(count < 3)
                    {
                        sleep(1);
                        count++;
                    }
                    printf("已关闭%d楼电梯门\n",runTimeStatus.current_floor);
                    lcd_draw_jpg(700,384,"./btn/open1.jpg");

                    runTimeStatus.btn_up_client[runTimeStatus.current_floor] = 0;
                    createMessage(RUN_UP,runTimeStatus.current_floor);
                    runTimeStatus.runStatus = RUN_UP;
                    break;
                }

                if(runTimeStatus.current_floor <= getRunMinFloor())
                {
                    runTimeStatus.runStatus = RUN_UP;
                    break;
                }
                printf("getRunMaxFloor----->%d",getRunMaxFloor());
            }
            // if(getRunMinFloor() == runTimeStatus.current_floor )
            // {
            //     if(getRunMaxFloor() != 0)
            //     {
            //         lcd_draw_jpg(360,240,"./btn/free.jpg");
            //         runTimeStatus.runStatus = RUN_UP;
            //     }
            //     lcd_draw_jpg(360,240,"./btn/free.jpg");
            //     // runTimeStatus.downMinFloor = 8;
            //     runTimeStatus.runStatus = RUN_FREE;
            // }
            if(getRunMaxFloor() == 0 && getRunMinFloor() == 8)
            {
                lcd_draw_jpg(360,240,"./btn/free.jpg");
                runTimeStatus.runStatus = RUN_FREE;
            }


        }

    }
}

void *listenClient()
{
    int server_fd, ret;
    struct sockaddr_in ser_addr;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return ;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return ;
    }

    // handle_udp_msg(server_fd);   //处理接收到的数据



    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息
    while(1)
    {
        memset(buf, 0, BUFF_LEN);
        len = sizeof(clent_addr);
        count = recvfrom(server_fd, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            return;
        }
        printf("client:%s\n",buf);  //打印client发过来的信息

        if(strcmp(buf,M_ADD_UP1) == 0)
        {
            runTimeStatus.btn_up_client[1] = 1;
            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 1)
                {
                    printf("打开1楼门\n");
                    runTimeStatus.btn_up_client[1] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP1);
                    printf("关闭1楼上行按钮\n");
                }
                if(runTimeStatus.current_floor > 1)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
            }
            printf("1楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_UP2) == 0)
        {
            runTimeStatus.btn_up_client[2] = 1;
            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 2)
                {
                    printf("打开3楼门\n");
                    runTimeStatus.btn_up_client[2] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP2);
                    printf("关闭2楼上行按钮\n");
                }
                else if(runTimeStatus.current_floor > 2)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 2)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("2楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_UP3) == 0)
        {
            runTimeStatus.btn_up_client[3] = 1;
            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 3)
                {
                    printf("打开3楼门\n");
                    runTimeStatus.btn_up_client[3] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP3);
                    printf("关闭3楼上行按钮\n");
                }
                else if(runTimeStatus.current_floor > 3)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 3)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("3楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_UP4) == 0)
        {
            runTimeStatus.btn_up_client[4] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 4)
                {
                    printf("打开4楼门\n");
                    runTimeStatus.btn_up_client[4] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP4);
                    printf("关闭4楼上行按钮\n");
                }
                else if(runTimeStatus.current_floor > 4)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 4)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }

            printf("4楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_UP5) == 0)
        {
            runTimeStatus.btn_up_client[5] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 5)
                {
                    printf("打开5楼门\n");
                    runTimeStatus.btn_up_client[5] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP5);
                    printf("关闭5楼上行按钮\n");
                }
                else if(runTimeStatus.current_floor > 5)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 5)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }

            printf("5楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_UP6) == 0)
        {
            runTimeStatus.btn_up_client[6] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 6)
                {
                    printf("打开6楼门\n");
                    runTimeStatus.btn_up_client[6] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_UP6);
                    printf("关闭6楼上行按钮\n");
                }
                else if(runTimeStatus.current_floor > 6)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 6)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }

            printf("6楼上行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN2) == 0)
        {
            runTimeStatus.btn_down_client[2] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 2)
                {
                    printf("打开2楼门\n");
                    runTimeStatus.btn_down_client[2] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN2);
                    printf("关闭2楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor > 2)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 2)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("2楼下行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN3) == 0)
        {

            runTimeStatus.btn_down_client[3] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 3)
                {
                    printf("打开3楼门\n");
                    runTimeStatus.btn_down_client[3] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN3);
                    printf("关闭3楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor > 3)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 3)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }

            printf("3楼下行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN4) == 0)
        {
            runTimeStatus.btn_down_client[4] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 4)
                {
                    printf("打开4楼门\n");
                    runTimeStatus.btn_down_client[4] = 0;

                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN4);
                    printf("关闭4楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor > 4)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 4)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("4楼下行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN5) == 0)
        {
            runTimeStatus.btn_down_client[5] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 5)
                {
                    printf("打开5楼门\n");
                    runTimeStatus.btn_down_client[5] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN5);
                    printf("关闭5楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor > 5)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 5)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("5楼下行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN6) == 0)
        {
            runTimeStatus.btn_down_client[6] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 6)
                {
                    printf("打开6楼门\n");
                    runTimeStatus.btn_down_client[6] = 0;
                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN6);
                    printf("关闭6楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor > 6)
                {
                    runTimeStatus.runStatus = RUN_DOWN;
                }
                else if(runTimeStatus.current_floor < 6)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }

            printf("6楼下行按钮按下\n");
        }
        else if(strcmp(buf,M_ADD_DOWN7) == 0)
        {
            runTimeStatus.btn_down_client[7] = 1;

            if(runTimeStatus.runStatus == RUN_FREE)
            {
                if(runTimeStatus.current_floor == 7)
                {
                    printf("打开7楼门\n");
                    runTimeStatus.btn_down_client[7] = 0;

                    pthread_t pid_cancel_client_key;
	                pthread_create(&pid_cancel_client_key, NULL, cancelClientKey, M_DELETE_DOWN7);
                    printf("关闭7楼下行按钮\n");
                }
                else if(runTimeStatus.current_floor < 7)
                {
                    runTimeStatus.runStatus = RUN_UP;
                }
            }
            printf("7楼下行按钮按下\n");
        }
    }



    close(server_fd);
    return;
}





int setKey(int floor)
{
    if(runTimeStatus.runStatus == RUN_FREE)
    {
        if(floor > runTimeStatus.current_floor)
        {           
            runTimeStatus.runStatus = RUN_UP;
            runTimeStatus.btn_status[floor] = KEY_ACTIVATE;
            return 1;
        }
        else if(floor < runTimeStatus.current_floor)
        {
            printf("current_floor:%d\n",runTimeStatus.current_floor);
            printf("downMinFloor:%d\n",runTimeStatus.downMinFloor);
            runTimeStatus.runStatus = RUN_DOWN;
            runTimeStatus.btn_status[floor] = KEY_ACTIVATE;
            printf("current_floor:%d\n",runTimeStatus.current_floor);
            printf("downMinFloor:%d\n",runTimeStatus.downMinFloor);
            return 1;
        }
        return 0;
    }
    else if(runTimeStatus.runStatus == RUN_UP)
    {
        if(floor > runTimeStatus.current_floor)
        {
            runTimeStatus.btn_status[floor] = KEY_ACTIVATE;
            changeUpMaxFloor(floor);
            return 1;
        }
        return 0;
    }
    else if(runTimeStatus.runStatus == RUN_DOWN)
    {
        if(floor < runTimeStatus.current_floor)
        {
            runTimeStatus.btn_status[floor] = KEY_ACTIVATE;
            changeDownMinFloor(floor);
            return 1;
        }
        return 0;
    }

    return 0;
}

int cancelKey(int floor)
{
    if(runTimeStatus.runStatus == RUN_UP)
    {
        if(floor > runTimeStatus.current_floor + 1)
        {
            // if(floor == runTimeStatus.upMaxFloor)
            // {
            //     runTimeStatus.btn_status[floor] = KEY_INACTIVE;
            //     printf("upMaxFloor:%d\n",runTimeStatus.upMaxFloor);
            //     updateUpMaxFloor(floor);
            //     printf("current_floor:%d\n",runTimeStatus.current_floor);
            //     printf("upMaxFloor:%d\n",runTimeStatus.upMaxFloor);
            //     return 1;
            // }

            runTimeStatus.btn_status[floor] = KEY_INACTIVE;
            return 1;
        }
        return 0;
    }
    else if(runTimeStatus.runStatus == RUN_DOWN)
    {
        if(floor < runTimeStatus.current_floor)
        {
            // if(floor == runTimeStatus.downMinFloor)
            // {
            //     printf("current_floor:%d\n",runTimeStatus.current_floor);
            //     printf("downMinFloor:%d\n",runTimeStatus.downMinFloor);
            //     runTimeStatus.btn_status[floor] = KEY_INACTIVE;
            //     updateDownMinFloor(floor);
            //     printf("current_floor:%d\n",runTimeStatus.current_floor);
            //     printf("downMinFloor:%d\n",runTimeStatus.downMinFloor);
            //     return 1;
            // }
            runTimeStatus.btn_status[floor] = KEY_INACTIVE;
            return 1;
        }
        return 0;
    }
    return 0;
}





int main()
{
    lcd_open();
    mmap_lcd();
    showAdv();
    showKeys();
    initRunTimeStatus(&runTimeStatus);


    pthread_t pid_run;
	pthread_create(&pid_run, NULL, run, NULL);

    pthread_t pid_listen_client;
	pthread_create(&pid_listen_client, NULL, listenClient, NULL);

    while(1)
    {
        getPoint(&x,&y,&key_status);
        if(key_status == 0)
        {
            // getKey(x,y);

            switch(getKey(x,y))
            {
                case K1:
                {
                    if(runTimeStatus.btn_status[1] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(1) == 1)
                            {
                                lcd_draw_jpg(600,288,"./btn/11.jpg");
                            }
                        }
                        
                        
                        break;                                                                     
                    }
                    if(runTimeStatus.btn_status[1] == KEY_INACTIVE)
                    {
                        if(setKey(1) == 1)
                        {
                            lcd_draw_jpg(600,288,"./btn/12.jpg"); 
                        }
                                             
                        break;
                    }                                       
                }
                case K2:
                {
                    if(runTimeStatus.btn_status[2] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(2) == 1)
                            {
                                lcd_draw_jpg(700,288,"./btn/21.jpg");
                            }
                        }

                        
                        break;                                    
                    }
                    if(runTimeStatus.btn_status[2] == KEY_INACTIVE)
                    {
                        if(setKey(2) == 1)
                        {
                            lcd_draw_jpg(700,288,"./btn/22.jpg");
                        }
                        
                        break;
                    } 
                }

                case K3:
                {
                    if(runTimeStatus.btn_status[3] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(3) == 1)
                            {
                                lcd_draw_jpg(600,192,"./btn/31.jpg");
                            }
                        }
                        
                        break;
                        
                    }
                    if(runTimeStatus.btn_status[3] == KEY_INACTIVE)
                    {
                        if(setKey(3) == 1)
                        {
                            lcd_draw_jpg(600,192,"./btn/32.jpg"); 
                        }
                                              
                        break;
                    } 
                }                   
                case K4:
                {
                    if(runTimeStatus.btn_status[4] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(4) == 1)
                            {
                                lcd_draw_jpg(700,192,"./btn/41.jpg");
                            }
                        }
                        
                        break;
                    }
                    if(runTimeStatus.btn_status[4] == KEY_INACTIVE)
                    {
                        if(setKey(4) == 1)
                        {
                            lcd_draw_jpg(700,192,"./btn/42.jpg"); 
                        }
                                             
                        break;
                    } 
                }
                case K5:
                {
                    if(runTimeStatus.btn_status[5] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(5) == 1)
                            {
                                lcd_draw_jpg(600,96,"./btn/51.jpg");
                            }
                        }
                                              
                        break;
                    }
                    if(runTimeStatus.btn_status[5] == KEY_INACTIVE)
                    {
                        if(setKey(5) == 1)
                        {
                            lcd_draw_jpg(600,96,"./btn/52.jpg");
                        }
                                               
                        break;
                    } 
                }                    
                case K6:
                {
                    if(runTimeStatus.btn_status[6] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(6) == 1)
                            {
                                lcd_draw_jpg(700,96,"./btn/61.jpg");
                            }
                        }
                                            
                        break;
                    }
                    if(runTimeStatus.btn_status[6] == KEY_INACTIVE)
                    {
                        if(setKey(6) == 1)
                        {
                            lcd_draw_jpg(700,96,"./btn/62.jpg");
                        }
                                             
                        break;
                    } 
                }                   
                case K7:
                {
                    if(runTimeStatus.btn_status[7] == KEY_ACTIVATE)
                    {
                        if(count_activate_key() > 1)
                        {
                            if(cancelKey(7) == 1)
                            {
                                lcd_draw_jpg(600,0,"./btn/71.jpg"); 
                            }
                        }
                                               
                        break;
                    }
                    if(runTimeStatus.btn_status[7] == KEY_INACTIVE)
                    {
                        if(setKey(7) == 1)
                        {
                            lcd_draw_jpg(600,0,"./btn/72.jpg");
                        }                                            
                        break;
                    } 
                }                   
                case K_OPEN:
                {
                    // if(runTimeStatus.btn_status[8] == KEY_ACTIVATE)
                    // {
                    //     runTimeStatus.btn_status[8] = KEY_INACTIVE;
                    //     lcd_draw_jpg(700,384,"./btn/open1.jpg");
                    //     break;
                    // }
                    // if(runTimeStatus.btn_status[8] == KEY_INACTIVE)
                    // {
                    //     runTimeStatus.btn_status[8] = KEY_ACTIVATE;
                    //     lcd_draw_jpg(700,384,"./btn/open2.jpg");
                    //     break;
                    // }
                }                   
                case K_CLOSE:
                {
                    // createMessage(RUN_UP,6);

                    // if(runTimeStatus.btn_status[9] == KEY_ACTIVATE)
                    // {
                    //     runTimeStatus.btn_status[9] = KEY_INACTIVE;
                    //     lcd_draw_jpg(600,384,"./btn/close1.jpg");
                    //     break;
                    // }
                    // if(runTimeStatus.btn_status[9] == KEY_INACTIVE)
                    // {
                    //     runTimeStatus.btn_status[9] = KEY_ACTIVATE;
                    //     lcd_draw_jpg(600,384,"./btn/close2.jpg");
                    //     break;
                    // }
                }  
                case K_ALARM:
                {
                    // int i;
                    // for(i = 0;i < 8;i++)
                    // {
                    //     printf("btn_up_client-->%d: %d\n",i,runTimeStatus.btn_up_client[i]);
                    //     printf("btn_down_client-->%d: %d\n",i,runTimeStatus.btn_down_client[i]);

                    // }
                    if(runTimeStatus.btn_status[10] == KEY_ACTIVATE)
                    {
                        runTimeStatus.btn_status[10] = KEY_INACTIVE;
                        lcd_draw_jpg(700,0,"./btn/bj1.jpg");
                        break;
                    }
                    if(runTimeStatus.btn_status[10] == KEY_INACTIVE)
                    {
                        runTimeStatus.btn_status[10] = KEY_ACTIVATE;
                        pthread_t pid_bj;
	                    pthread_create(&pid_run, NULL, bj, NULL);
                        lcd_draw_jpg(700,0,"./btn/bj2.jpg");
                        break;
                    }
                }
                default:
                    break;
            }

        }
    }
    return 0;
}