/*************************************************
Copyright (C), 1988-1999
File name: ts.h
Author: ChangZheng Wei 
Version: 1.0
Date: 2019/08/21
Description: 包含了ts.c文件用到的所有头文件和其中的所有函数声明
Function List:
1.ts_open()  //打开触摸屏
2. get_x_y()  //获取手指离开触摸屏那一刻的坐标值
3. ts_slide() // 判断手指的滑动距离 
4. ts_close()   //关闭触摸屏
History: 
1. Date:2019/08/21
Author: ChangZheng Wei 
Modification: 创建了此文件
*************************************************/
#ifndef _TS_H_
#define _TS_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdlib.h>


int ts_open(char *pathname);
void get_x_y(int *x, int *y);
int ts_slide();
void ts_close();


#endif