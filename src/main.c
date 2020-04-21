//************************************************************
//*Copyright (C), 1988-1999.
//*
//*FileName: main.c
//*
//*Author: ChangZheng Wei 
//*
//*Version : 1.0
//*
//*Date: 2019/08/17
//*
//*Description: 
//1.查找指定目录下的.jpg和.bmp格式文件，并且插入到双向链表中.
//2.将链表中的图片文件显示在LCD中，可手动切换图片和自动播放图片. 
//*
//*Function List: 
//1.find_pic()  //查找.jpg和.bmp格式文件，并且插到链表中.
//2.show_manual_or_automatic()  //手动切换图片和自动播放图片.
//3.main() //调用其他功能函数.
//*
//*History: 
//<author>       <time>     <version > <desc>
//ChangZheng Wei 2019/08/17  1.0       创建此框架
//*************************************************************
#include "common.h"
#include "link.h"
#include "display_bmp.h"
#include "lcd.h"
#include "display_jpeg.h"
#include "ts.h"



unsigned int  *lcd_fb_addr;
struct link_list_node *list_head,*new_node;


/*******************************************************************
Function: find_pic

Description: 查找指定目录下的.jpg和.bmp格式文件，并且插入到双向链表中.

Calls:  (1.request_link_list_head_node() 2.opendir() 3. readdir() 
4.new_link_list_node() 5.insert_node_to_link_list()
6. display_link_list_node())

Called By: 1.main()

Input: p为const char *类型的指针，指向输入的第二参数

Output: 输出链表里的内容
********************************************************************/
int find_pic(const char *p)
{

	char    path_and_name[4096];
	struct  dirent *dirp = NULL;
    struct  stat buf;
  
    list_head = request_link_list_head_node();
    if (list_head == NULL)
    {
        return -1;
    }

	/*获取指定目录指针*/
    DIR*dp = opendir(p);
    while (1)
    {

    	/*读取目录项指针*/
        dirp = readdir(dp);
        if (dirp == NULL)
        {
            break;
        }

    	 /*过滤掉 "." ".."*/
        if (*dirp->d_name == '.')         
        {
            continue;
        }

        if (dirp->d_type == DT_REG)
        {     

        	/*查找.jpg和.bmp文件*/                             
            if ((strstr(dirp->d_name, ".jpg")) || (strstr(dirp->d_name, ".bmp"))) 
            {

            	/*%s之间要加/，否则文件名会加到目录名后面*/
                sprintf(path_and_name, "%s/%s", p, dirp->d_name);              
                printf("The.jpg and .bmp files in this directory are: %s\n", path_and_name);

                stat(path_and_name, &buf);
                printf("file size:%ld \n", buf.st_size);
                printf("file format:%s \n", dirp->d_name + (strlen(dirp->d_name) - 4));

                printf("\n");

                /*把找到的文件的名称，大小和格式放入新结点*/
                new_node = new_link_list_node(dirp->d_name, buf.st_size, dirp->d_name + (strlen(dirp->d_name) - 4));

                /*新结点插到头结点后面*/
                insert_node_to_link_list(list_head, new_node);
            }
        }
    } 
    			/*遍历链表内容*/
                display_link_list_node(list_head);    
}


/*******************************************************************************
Function: show_manual_or_automatic

Description: 将链表中的图片文件显示在LCD中，可手动切换图片和自动播放图片. 

Calls: (1.display_format_jpeg() 2.display_format_bmp() 3.ts_slide() 4. sleep())

Called By: 1.main()

Input: p为struct link_list_node *类型的指针，指向链表头结点的上一个结点

Output: 输出链表里的内容到屏幕上
********************************************************************************/
void show_manual_or_automatic( struct link_list_node *p)
{
   
    int ret;

    if (strstr(p->pic_name, ".jpg"))
    {
        display_format_jpeg(p->pic_name, 0, 0, lcd_fb_addr);
    }

    else
    {
        display_format_bmp(0, 0, p->pic_name, lcd_fb_addr);
    }
          
    while (1)
    {
        ret = ts_slide();

        /*右滑切换到上一张图片*/
        if (ret == 0)           
        {
            p = p->next;
            if (p == list_head)
            {
                p = p->next;
            }
        }

        /*左滑切换到下一张图片*/
        if (ret == 1)        
        {
            p = p->prev;
            if (p == list_head)
            {
                p = p->prev;    
            }
        }

        /*下滑可以自动播放*/
        if (ret == 2)       
        {

            while (1)
            {

                if (strstr(p->pic_name, ".jpg"))
                {
                    display_format_jpeg(p->pic_name, 0, 0, lcd_fb_addr);
                }

                else
                {
                    display_format_bmp(0, 0, p->pic_name, lcd_fb_addr);
                }
       
                p = p->prev;

                if (p == list_head)
                {
                    break;
                } 

                sleep(2);
            }

        }

        if (strstr(p->pic_name, ".jpg"))
        {
            display_format_jpeg(p->pic_name, 0, 0, lcd_fb_addr);
        }

        else
        {
             display_format_bmp(0, 0, p->pic_name, lcd_fb_addr);
        }

    }

}


/**********************************************************************************
Function: main()

Description: 主函数，调用其他功能函数.

Calls: (1.find_pic() 2.open_lcd_device() 3.ts_open() 4.show_manual_or_automatic() 
5.ts_close() 6.remove_link_list() 7. close_lcd_device())

Called By: 无

Input:argc表示文件运行时输入的参数个数,argv[]数组中依次存放所有的参数

Output: 输出链表里的内容到屏幕上
***********************************************************************************/
int main(int argc, const char *argv[])
{
    if (argc != 2)
    {       
        printf("user:%s<dir path>\n", argv[0]);
        exit(1);
    }

    int lcd_fd;   

    find_pic(argv[1]);  

    struct link_list_node *tmp = list_head->prev;

    lcd_fd = open_lcd_device(&lcd_fb_addr);
    if (lcd_fd == -1)
    {
        return -1;
    }

    /*打开触摸屏*/
    ts_open("/dev/input/event0");

    show_manual_or_automatic(tmp);

    /*关闭触摸屏*/
	ts_close(); 

	/*销毁链表*/                               
    remove_link_list(list_head); 

    /*关闭LCD*/                          
    close_lcd_device(lcd_fd, lcd_fb_addr);       

    return 0;
    
}


