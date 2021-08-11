#include <stdio.h>
// #include "lcd.h"

void showKeys()
{
    //第1行
    lcd_draw_jpg(600,0,"./btn/71.jpg");
    lcd_draw_jpg(700,0,"./btn/bj1.jpg");
    //第2行
    lcd_draw_jpg(600,96,"./btn/51.jpg");
    lcd_draw_jpg(700,96,"./btn/61.jpg");
    //第3行
    lcd_draw_jpg(600,192,"./btn/31.jpg");
    lcd_draw_jpg(700,192,"./btn/41.jpg");
    //第4行
    lcd_draw_jpg(600,288,"./btn/11.jpg");
    lcd_draw_jpg(700,288,"./btn/21.jpg");
    //第5行
    lcd_draw_jpg(600,384,"./btn/close1.jpg");
    lcd_draw_jpg(700,384,"./btn/open1.jpg");

}



void showAdv()
{
    lcd_draw_jpg(0,0,"./img/11.jpg");
    lcd_draw_jpg(0,240,"./img/12.jpg");
    lcd_draw_jpg(360,0,"./f/1.jpg");
    lcd_draw_jpg(360,240,"./btn/free.jpg");
}
