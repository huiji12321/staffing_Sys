/*************************************************************************
 *	 FileName	: client.c
 *	 Author		: jishuaifei 
 *	 Email		: 751998164@qq.com 
 *	 Created	: 2019年10月25日 星期五 11时47分07秒
 ************************************************************************/
 
#include "ope.h"

void login(int socket_fd);   //登录函数
int main(int argc, const char *argv[])
{
	//判断用户输入格式，需求./client ip Port
	if(argc <3)
	{
		printf("Usemsg:%s <IP> <Port>\n",argv[0]);
		return -1;
	}

	//创建用于链接的套接字
	int socket_fd;
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd < 0)
	{
		perror("socket failed!");
		return -2;
	}
	printf("socket successed!\n");

	//定义填充结构体
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	//连接
	int connect_fd;
	connect_fd = connect(socket_fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(connect_fd < 0)
	{
		perror("connect failed!\n");
		return -3;
	}
	printf("connect successed!\n");

	login(socket_fd);

	close(socket_fd);
	return 0;
}


void login(int socket_fd)
{
	//显示界面
	printf("********************************************\n");
	printf("********************************************\n");
	printf("**          欢迎使用员工管理系统          **\n");
	printf("********************************************\n");
	printf("********************************************\n");
	//接收登录信息
	int no;
	char pwd[20];
	USER user;
	printf("请输入员工号：");
	scanf("%d",&no);
	printf("请输入登录密码：");
	scanf("%s",pwd);
	//填充发送消息体
	MSG msg;
	msg.ope = LOGIN;
	msg.Usemsg.no = no;
	strcpy(msg.Usemsg.pwd,pwd);
	//发送消息
	send(socket_fd, &msg, sizeof(MSG),0);

	//接收返回消息，判断用户类型进入不同界面
	memset(&user,0,sizeof(USER));
	recv(socket_fd,&user,sizeof(user),0);
	
	if(user.type=='1')
	{
		admin_ope(socket_fd,&msg,&user);
		printf("admin_ope\n");
	}
	else if(user.type=='0')
	{
		staff_ope(socket_fd,&msg,&user);
		printf("admin_ope\n");
	}
	else
	{
		printf("%s\n",user.des);

	}

}

