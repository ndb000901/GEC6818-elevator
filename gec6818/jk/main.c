#include <stdio.h>
#include "jpeglib.h"
#include "yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main()
{
	//定义结构体变量
	struct jpg_data video_buf;
	
	// lcd_open();
	// mmap_lcd();
	
	//初始化摄像头
	linux_v4l2_yuyv_init("/dev/video7");
	//开启摄像头捕捉
	linux_v4l2_start_yuyv_capturing();
	
	// int jpg_num=0;
	// char pic_name[40];
	// int fd;


	int fd_jk;  //file discription
    struct hostent *he;
    struct sockaddr_in server, client;

    if ((fd_jk = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error!\n");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(30000);
    server.sin_addr.s_addr = inet_addr("192.168.0.1");

    socklen_t len;
    len = sizeof(struct sockaddr_in);
    char* message = "bj";
    printf("开始报警:%s\n",message);
    
    


	
	while(1)
	{
     	//获取摄像头捕捉的画面
		linux_v4l2_get_yuyv_data (&video_buf);
	
		//显示摄像头捕捉的画面
		// show_video_data(0,0,video_buf.jpg_data,video_buf.jpg_size);				
		// sendto(fd_jk,video_buf.jpg_data , strlen(video_buf.jpg_data), 0, (struct sockaddr *)&server, len);
		sendto(fd_jk,video_buf.jpg_data , video_buf.jpg_size, 0, (struct sockaddr *)&server, len);
		// printf("开始报警:%s\n",video_buf.jpg_data);
		// printf("开始报警:%s\n",message);
	}
	
	close(fd_jk);
	//摄像头解除初始化
	linux_v4l2_yuyv_quit();
	
	lcd_close();

	return 0;
}

