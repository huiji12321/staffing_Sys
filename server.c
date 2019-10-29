/*************************************************************************
 *	 FileName	: server.c
 *	 Author		: jishuaifei 
 *	 Email		: 751998164@qq.com 
 *	 Created	: 2019年10月25日 星期五 11时46分59秒
 ************************************************************************/

#include "ope.h"

#define DATABASE "staff.db"

void *connectfun(void *arg);   //多线程函数
int main(int argc, const char *argv[])
{
	//用户主动传参
	if(argc<2)
	{
		printf("Usrmsg:%s <Port>\n",argv[0]);
		return -1;
	}
	//打开数据库
	CONNECT con; //多线程传参用的连接结构体
	if(sqlite3_open(DATABASE, &(con.db)) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(con.db));
		return -1;
	}
	printf("open DATABASE success.\n");
	char *errmsg;
	if(sqlite3_exec(con.db,"create table usrinfo(no integer,,name text,pwd text,type char,level integer,age integer,addr text,phone text,salary REAL);",NULL,NULL,&errmsg)!= SQLITE_OK){
		printf("%s.\n",errmsg);
	}else{
		printf("create usrinfo table success.\n");
	}
	//创建log文件
	if((con.fdd = open("log.txt",O_RDWR|O_CREAT|O_APPEND,0666 ))== -1 )
	{
		perror("opendes");
		return -1;
	}
	
	
	//1.创建套接字文件，用于链接
	int socket_fd;
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd == -1)
	{
		perror("socket failed!\n");
		return -2;
	}
	printf("socket successed!\n");

	//定义填充结构体
	int bind_fd;
	struct sockaddr_in serveraddr; //定义填充的结构体
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET; //IPV4协议
	serveraddr.sin_port = htons(atoi(argv[1])); //本地端口号转化位网络字节码
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地IP转化位网络字节码,任意IP
	//2.绑定socket文件描述符、端口、IP
	bind_fd = bind(socket_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(bind_fd == -1)
	{
		perror("bind failed!\n");
		return -3;
	}
	printf("bind successed\n");

	//3.监听，将套接字文件属性该主动为被动模式
	int listen_fd;
	listen_fd = listen(socket_fd,10);
	if(listen_fd == -1)
	{
		perror("listen failed\n");
		return -4;
	}
	printf("listen successed\n");

	//循环等待连接
	int addrsize = sizeof(serveraddr);
	
	while(1)
	{
		con.accept_fd = accept(socket_fd,(struct sockaddr*)&(con.connectaddr),&addrsize);
		if(con.accept_fd == -1)
		{
			perror("accept");
			return -5;
		}
		printf("connect successed\n");

		pthread_t pid ;
		pthread_create(&pid, NULL,connectfun,(void*)&con);
	}

	close(socket_fd);
	return 0;

}

void *connectfun(void *arg){
	pthread_detach(pthread_self());
	USER user;
	MSG msg;
	int recvbyte;
	while(1)
	{
		memset(&msg,0,sizeof(MSG));
		recvbyte = recv(*(int *)arg, &msg,sizeof(MSG),0);
		if(recvbyte <= 0)
		{
			printf("client failed connect\n");
			return NULL;
		}
		switch(msg.ope)
		{
			case LOGIN:
				printf("LOGIN\n");
				login_handle(arg,&msg);
				break;				//登录
			case INC:
				printf("INC\n");
				inc_handle(arg,&msg);
				break;                   //增
			case DEL:
				printf("DEL\n");
				del_handle(arg,&msg);
				break;                    //删除
			//case UPGRA_NAME:printf("UPGRA_NAME\n");break;			//修改
			case UPGRA_PWD:
				printf("UPGRA_PWD\n");
				upgra_pwd_handle(arg,&msg);
				break;
			//case UPGRA_TYPE:break;
			//case UPGRA_LEVEL:break;
			//case UPGRA_AGE:break;
			case UPGRA_ADDR:
				printf("UPGRA_ADDR\n");
				upgra_addr_handle(arg,&msg);
				break;
			//case UPGRA_PHONE:break;
			case UPGRA_SALARY:
				printf("UPGRA_SALARY\n");
				upgra_salary_handle(arg,&msg);
				break;
			case SEARCH:
				printf("SEARCH\n");
				search_handle(arg,&msg);
				break;                 //查询个人
			case SEARCHALL:
				printf("SEARCHALL\n");
				search_all_handle(arg,&msg);
				break;             //查询所有
			case HISTORY:
				printf("HISTORY\n");
				history_handle(arg,&msg);
				break; 
								
		}
	}
}










