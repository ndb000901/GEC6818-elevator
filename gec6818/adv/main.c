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





int main()
{

	lcd_open();
	mmap_lcd();
	int i = 1;

	system("mplayer -slave -quiet -geometry 0:240 -zoom -x 360 -y 240 -loop 0 1.avi &");
	while(1)
	{
		if(i == 15)
		{
			i = 1;
		}
		char namepath[20];
		sprintf(namepath,"./img/%d.jpg",i);
		lcd_draw_jpg(0,0,namepath);
		i++;
		sleep(5);
	}
	lcd_close();

	return 0;
}


