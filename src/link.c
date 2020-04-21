//************************************************************************
//*Copyright (C), 1988-1999
//*
//*FileName: link.c
//*
//*Author: ChangZheng Wei 
//*
//*Version : 1.0
//*
//*Date: 2019/08/18
//*
//*Description: 创建一个双向链表，用来存放查找到的图片文件
//*
//*Function List: 
//1.request_link_list_head_node() //申请链表头结点
//2.malloc //申请堆内存
//3.new_link_list_node()  //创建一个新结点
//4.is_empty()  //判断链表是否为空
//5.insert_node_to_link_list() //插入新结点到链表头结点后面 
//6.display_link_list_node() //遍历链表内容
//7.remove_link_list()  //销毁链表
//8.free() //释放内存
//*
//*History: 
//<author>          <time>   <version >     <desc>
//ChangZheng Wei 2019/08/18   1.0       修改display_link_list_node()函数，
//                                     使链表从链表头结点的下一个结点开始遍历
//**************************************************************************
#include "common.h"
#include "link.h"
#include "lcd.h"
#include "display_bmp.h"


/*******************************************************************
Function: request_link_list_head_node()

Description: 申请链表头结点

Calls:  (1.malloc() 2.sizeof() 3.fprintf())

Called By: main()

Input: 无

Output: 无

Return:返回头结点 
********************************************************************/
struct link_list_node *request_link_list_head_node(void)
{
    struct link_list_node *list_head;
    
    list_head = malloc(sizeof(struct link_list_node));
    if(list_head == NULL)
    {
        fprintf(stderr, "request list head error:%s\n", strerror(errno));
        return NULL;
    }

   
    list_head->next = list_head;
    list_head->prev = list_head;

    return list_head;
}


/*************************************************************************
Function: new_link_list_node()

Description: 申请一个新结点

Calls:  (1.malloc() 2.sizeof() 3.fprintf() 4.strcpy())

Called By: find_pic()

Input: 1.pic_name为图片文件名称 2.pic_size为图片文件大小 3.format图片文件格式

Output: 无

Return:返回新结点 
**************************************************************************/
struct link_list_node *new_link_list_node(char *pic_name, size_t pic_size,  char *format)
{

    struct link_list_node *new_node;
    
    new_node = malloc(sizeof(struct link_list_node));
    if(new_node == NULL)
    {
        fprintf(stderr, "request list node error:%s\n", strerror(errno));
        return NULL;
    }

    strcpy(new_node->pic_name,pic_name);
    strcpy( new_node->format,format);
    new_node->pic_size = pic_size;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}


/*******************************************************************
Function: insert_node_to_link_list()

Description: 新结点插入到头结点后面

Calls:  无

Called By: find_pic()

Input: 1.list_head为头结点  2.new_node为新结点

Output: 无

Return:无
********************************************************************/
void insert_node_to_link_list(struct link_list_node *list_head, struct link_list_node *new_node)
{
    
    new_node->next = list_head->next;
    new_node->prev = list_head;
    list_head->next->prev = new_node;
    list_head->next = new_node;
 
}


/*******************************************************************
Function: is_empty()

Description: 判断链表是否为空

Calls:  无

Called By: display_link_list_node()

Input: list_head为头结点 

Output: 无

Return:返回头结点的下一个为空
********************************************************************/
bool is_empty(struct link_list_node *list_head)
{
    return list_head->next == NULL;
}


/*******************************************************************
Function:  display_link_list_node()

Description: 遍历链表的内容
Calls:  is_empty

Called By: find_pic()

Input: list_head为头结点 

Output: 输出链表里的内容
********************************************************************/
void display_link_list_node(struct link_list_node *list_head)
{
    struct link_list_node *ptr;

    if(is_empty(list_head))
    {
        fprintf(stderr, "list empty\n");
        return;
    }   

    printf("the file of link list:\n");

    for(ptr = list_head->prev; ptr != list_head; ptr = ptr->prev)
    {
        printf("file name: %s\t", ptr->pic_name);
        printf("file size: %ld\t", ptr->pic_size);
        printf("file format: %s\t", ptr->format);
        printf("\n");
  
    }

    printf("\n");

}


/*******************************************************************
Function: remove_link_list

Description: 销毁链表

Calls: free()

Called By: main()

Input: list_head为头结点 

Output: 释放链表
********************************************************************/
void remove_link_list(struct link_list_node * list_head)
{
  struct link_list_node *ptr, *n;

    for(ptr=list_head->next, n=ptr->next; ptr != list_head; ptr=n, n=n->next)
    {
        printf("free %s\n", ptr->pic_name);
        free(ptr);
    }

    free(list_head);
}
