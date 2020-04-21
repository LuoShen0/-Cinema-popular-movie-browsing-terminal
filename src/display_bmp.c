//************************************************************
//*Copyright (C), 1988-1999.
//*
//*FileName: display_bmp.c
//*
//*Author: ChangZheng Wei 
//*
//*Version : 1.0
//*
//*Date: 2019/08/20
//*
//*Description: 显示bmp格式图片
//*
//*Function List: 
//1.get_file_size() //获取文件大小
//2.display_format_bmp() //显示bmp格式文件
//*
//*History: 
//<author>       <time>     <version > <desc>
//ChangZheng Wei 2019/08/20  1.0       创建此文件
//*************************************************************

#include "common.h"
#include "lcd.h"


/**********************************************************************************
Function: get_file_size()

Description:获取bmp文件大小

Calls: stat

Called By: display_format_bmp()

Input:pathname 为文件路径名称
***********************************************************************************/
off_t get_file_size(const char *pathname)
{
	struct stat file_stat;
	int retval;

	retval = stat(pathname , &file_stat);
	if(retval == -1)
	{
		perror("get file size error\n");
		return -1;
	}

	return file_stat.st_size;
}



/**********************************************************************************
Function: display_format_bmp()

Description: 获取bmp格式文件的各种信息，然后显示到LCD上

Calls: 1.get_file_size() 2.lcd_display_point

Called By: show_manual_or_automatic()

Input: (1.x_s:想要显示在x的哪个位置开始 2.y_s:想要显示在y的哪个位置开始 
3.pathname：图片的路径名字 4.lcd_ptr：显存的地址)

Output: 输出bmp文件到屏幕上
***********************************************************************************/
int display_format_bmp(unsigned int x_s, unsigned int y_s, const char *pathname, unsigned int *lcd_ptr)
{
	off_t  file_size;
	FILE *pic_fp;
	ssize_t rd_ret;
	char *pic_buf;
	char file_head[54];
	unsigned int bmp_width, bmp_height;
	unsigned int x_end, y_end;
	unsigned int x, y;

	char *draw_ptr;
	unsigned int color;


	/* 获取文件的大小 */
	file_size = get_file_size(pathname);
	if(file_size == -1)
		return -1;

	printf("file_size=%ld\n", file_size);

	pic_buf = malloc(file_size-54);
	if(pic_buf == NULL)
	{
		perror("alloc pic_buf memory error\n");
		return -1;
	}


	pic_fp = fopen(pathname, "r");
	if(pic_fp == NULL)
	{
		perror("open picture error\n");
		return -1;
	}

	/*读取文件头部信息*/
	rd_ret = fread(file_head, 54, 1, pic_fp);
	if(ferror(pic_fp))
	{
		perror("read picture head error\n");
		return -1;
	}

	/*读取文件的颜色数据*/
	rd_ret = fread(pic_buf, file_size-54, 1, pic_fp);
	if(ferror(pic_fp))
	{
		perror("read picture color data error\n");
		return -1;
	}

	fclose(pic_fp);

	/* 获取文件宽度  */
	bmp_width =file_head[18];
	bmp_width|=file_head[19]<<8;
	printf("bmp_width=%d\r\n",bmp_width);
	
	/* 获取文件高度  */
	bmp_height =file_head[22];
	bmp_height|=file_head[23]<<8;
	printf("bmp_height=%d\r\n",bmp_height);	


	x_end = x_s + bmp_width;//登记x显示的结束坐标
	y_end = y_s + bmp_height;//等级y显示的结束坐标


	printf("x_end=%u, y_end=%u\n", x_end, y_end);

	/*实现将读取到的颜色数据在指定的位置显示出来*/
	for(y=y_s; y<y_end && y<480; y++)
	{
		/*计算当前是显示到第几行*/
		int cur_line = y-y_s;

		/*获取到当前行的相反行数据颜色的内存地址*/
		draw_ptr = pic_buf+((bmp_height-cur_line)*bmp_width*3);
	
		for( x=x_s; x<x_end && x<800; x++)
		{	
			//将内存中的3个8位数据组合成为RGB数据 B|G<<8|R<<16=RGB
			color = (*draw_ptr) | (*(draw_ptr+1)<<8) |(*(draw_ptr+2)<<16);
			lcd_display_point(x, y, color, lcd_ptr);
			
			draw_ptr += 3;
			
		}

	}


	free(pic_buf);

	return 0;
}
