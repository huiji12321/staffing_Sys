#ifndef __OPE_H__
#define __OPE_H__

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct{				//员工结构体
	int no;					//员工号，唯一标识
	char name[20];			//名字
	char pwd[20];			//密码
	unsigned char type;		//用户类型  管理员为1，普通用户为0
	int level;				//员工等级
	int age;				//年龄
	char addr[50];			//员工住址
	char phone[12];			//电话
	double salary;			//工资
	char des[128];
}USER;

enum OPE{					//操作枚举
	LOGIN=0,				//登录
	INC,                    //增
	DEL,                    //删除
	UPGRA_NAME,				//修改
	UPGRA_PWD,
	UPGRA_TYPE,
	UPGRA_LEVEL,
	UPGRA_AGE,
	UPGRA_ADDR,
	UPGRA_PHONE,
	UPGRA_SALARY,
	SEARCH,                 //查询个人
	SEARCHALL,              //查询所有
	HISTORY                 //历史
};
typedef union{				//员工联合体
	int no;					//员工号，唯一标识
	char name[20];			//名字
	char pwd[20];			//密码
	unsigned char type;		//用户类型  管理员为1，普通用户为0
	int level;				//员工等级
	int age;				//年龄
	char addr[50];			//员工住址
	char phone[12];			//电话
	double salary;			//工资
}USER_UNION;
typedef struct{             //发送消息结构体
	enum OPE ope;
	USER Usemsg;
}MSG;

typedef struct{             //多线程传参结构体
	int  accept_fd;
	struct sockaddr_in connectaddr;
	sqlite3 *db;
	int fdd;
	time_t t;
	struct tm *lt;
}CONNECT;


//客户端操作函数
//员工操作函数 
void staff_ope(int socket_fd,MSG *msg,USER *user); 
//员工信息查询
void staff_search(USER *user); 
//员工信息更新
void staff_upgra(int socket_fd,MSG *msg,USER *user); 
//管理员操作函数
void admin_ope(int socket_fd,MSG *msg,USER *user);
//增删改查、历史   操作需求提交函数
void admin_add(int socket_fd,MSG *msg,USER *user);
void admin_del(int socket_fd,MSG *msg,USER *user);
void admin_upgra(int socket_fd,MSG *msg,USER *user);
void admin_search(int socket_fd,MSG *msg,USER *user);
void admin_history(int socket_fd,MSG *msg,USER *user);

//服务器端处理函数
//登录
void login_handle(void *arg,MSG *msg);
//增删改查 历史
void inc_handle(void *arg,MSG *msg);
void del_handle(void *arg,MSG *msg);
void search_handle(void *arg,MSG *msg);
void search_all_handle(void *arg,MSG *msg);
void upgra_pwd_handle(void *arg,MSG *msg);
void upgra_addr_handle(void *arg,MSG *msg);
void upgra_salary_handle(void *arg,MSG *msg);
void history_handle(void *arg,MSG *msg);
#endif
