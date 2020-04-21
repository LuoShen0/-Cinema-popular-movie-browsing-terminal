//************************************************************
//*Copyright (C), 1988-1999.
//*
//*FileName: ts.c
//*
//*Author: ChangZheng Wei 
//*
//*Version : 1.0
//*
//*Date: 2019/08/21
//*
//*Description: 打开触摸屏，获取手指触摸屏幕的坐标  
//*
//*Function List: 
//1. ts_open()  //打开触摸屏
//2. get_x_y()  //获取手指离开屏幕时的坐标
//3. ts_slide() //判断手指滑动距离
//4.ts_close()  //关闭触摸屏
//*
//*History: 
//<author>       <time>     <version > <desc>
//ChangZheng Wei 2019/08/21  1.0      创建此文件
//*************************************************************
#include "ts.h"

int ts_fd;


/*******************************************************************************
Function: ts_open

Description: 打开触摸屏. 

Calls: open 

Called By: main()

Input: pathname为图片文件路径名称

********************************************************************************/
int ts_open(char *pathname)
{
	ts_fd = open(pathname, O_RDONLY);
	if(ts_fd == -1)
	{
		printf("open ts failed\n");
		return -1;
	}

	return 0;
}


/*******************************************************************************
Function: get_x_y()

Description: 获取手指离开触摸屏那一刻的坐标值

Calls: read()

Called By: 无

Input: x和y分别为坐标值
********************************************************************************/
void get_x_y(int *x, int *y)
{
	struct input_event ev;
	while(1)
	{
		read(ts_fd, &ev, sizeof(ev));

		if(ev.type == EV_ABS && ev.code == ABS_X)
		{
			*x = ev.value;
		}

		if(ev.type == EV_ABS && ev.code == ABS_Y)
		{
			*y = ev.value;
		}

		if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
		{
			break;
		}
	}

}


/*******************************************************************************
Function: ts_slide()

Description: 判断手指的滑动距离 

Calls: 1.read() 2.sizeof() 3.abs()

Called By:  show_manual_or_automatic()

Input:无

Output: 坐标值
********************************************************************************/
int ts_slide()
{

	/*read函数第一次返回的x和第一次返回的y就是begin_x和begin_y*/
	int begin_x, begin_y;
	int end_x, end_y;
	
	/*0代表还未还未进行第一次读取 1代表已经进行了第一次读取*/
	int flag_x = 0;
	int flag_y = 0;

	struct input_event ev;
	while (1)
	{
		read(ts_fd, &ev, sizeof(ev));

		if(ev.type == EV_ABS && ev.code == ABS_X)
		{
			if(flag_x == 0)
			{
				begin_x = ev.value;
				flag_x = 1;
			}

			end_x = ev.value;
		}

		if(ev.type == EV_ABS && ev.code == ABS_Y)
		{
			if(flag_y == 0)
			{
				begin_y = ev.value;
				flag_y = 1;
			}

			end_y = ev.value;
		}

		if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
		{
			break;
		}
	}


		/*左滑*/
		if(begin_x - end_x >= 150)
		{
			return 0;
		}

		/*右滑*/
		if(end_x - begin_x >= 150)
		{
			return 1;
		}
	
		/*下滑*/
		if(end_y - begin_y >= 150)
		{
			return 2;
		}

		return -1;
	}


/*******************************************************************************
Function: ts_close()

Description:关闭触摸屏

Calls: close()

Called By: main()

Input: 无

Output: 无
********************************************************************************/
void ts_close()
{
	close(ts_fd);
}