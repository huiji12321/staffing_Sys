/*************************************************************************
 *	 FileName	: ope.c
 *	 Author		: jishuaifei 
 *	 Email		: 751998164@qq.com 
 *	 Created	: 2019年10月25日 星期五 11时47分07秒
 ************************************************************************/
#include "ope.h"


void staff_ope(int socket_fd,MSG *msg,USER *user)
{
	int choice;
	printf("\n***亲爱的员工，欢迎您登陆员工管理系统!***\n\n");
	
	while(1){
		printf("********************************************\n");
		printf("********************************************\n");
		printf("**     1.查询   2.修改   3.退出           **\n");
		printf("********************************************\n");
		printf("********************************************\n");
		printf("请输入您的选择（数字）>>");

		scanf("%d",&choice);
		getchar();
		switch(choice)
		{
			case 1:
				staff_search(user);
				break;
			case 2:
				staff_upgra(socket_fd,msg,user);
				break;
			case 3:
				printf("亲爱的员工，再见！\n");
				sleep(3);
				return;
			default:
				printf("输入错误，请重新选择!\n");
		}
	}
}

void staff_search(USER *user)
{
	printf("\n*********************************************************************\n");
	printf("工号\t姓名\t密码\t类型\t级别\t年龄\t住址\t电话\t工资\n");
	printf("%d\t%s\t%s\t",user->no,user->name,user->pwd);
	printf("%c\t%d\t%d\t",user->type,user->level,user->age);
	printf("%s\t%s\t%.1lf\n",user->addr,user->phone,user->salary);
	printf("*********************************************************************\n\n");
	
}
void staff_upgra(int socket_fd,MSG *msg,USER *user)
{
	int choice;
	USER_UNION user_union;
	while(1){
		printf("********************************************\n");
		printf("********************************************\n");
		printf("**     1.住址     2.密码     3.退出       **\n");
		printf("********************************************\n");
		printf("********************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&choice);
		getchar();
		switch(choice)
		{
			case 1:
				printf("请输入新地址 >>");
				scanf("%s",(char *)&user_union);
				msg->ope = UPGRA_ADDR;
				msg->Usemsg.no = user->no;
				strcpy(msg->Usemsg.addr,(char *)&user_union);
				send(socket_fd, msg, sizeof(MSG),0);
				memset(user,0,sizeof(USER));
				recv(socket_fd,user,sizeof(USER),0);
				printf("修改后的员工信息请确认：\n");
				staff_search(user);
				break;
			
			case 2:
				printf("请输入新密码 >>");
				scanf("%s",(char *)&user_union);
				msg->ope = UPGRA_PWD;
				msg->Usemsg.no = user->no;
				strcpy(msg->Usemsg.pwd,(char *)&user_union);
				send(socket_fd, msg, sizeof(MSG),0);
				memset(user,0,sizeof(USER));
				recv(socket_fd,user,sizeof(USER),0);
				printf("修改后的员工信息请确认：\n");
				staff_search(user);
				break;				
			case 3:
				printf("退出修改\n");
				sleep(1);
				return;
			default:
				printf("输入错误，请重新选择!\n");
		}
	}
}

void admin_ope(int socket_fd,MSG *msg,USER *user)
{
	int choice;
	printf("\n***亲爱的管理员，欢迎您登陆员工管理系统!***\n");
	while(1){
		printf("********************************************\n");
		printf("********************************************\n");
		printf("**     1.添加   2.删除   3.修改           **\n");
		printf("**     4.查询   5.历史   6.退出           **\n");
		printf("********************************************\n");
		printf("********************************************\n");
		printf("请输入您的选择（数字）>>");

		scanf("%d",&choice);
		getchar();
		switch(choice)
		{
			case 1:
				admin_add(socket_fd,msg,user);
				break;
			case 2:
				admin_del(socket_fd,msg,user);
				break;
			case 3:
				admin_upgra(socket_fd,msg,user);
				break;
			case 4:
				admin_search(socket_fd,msg,user);
				break;
			case 5:
				admin_history(socket_fd,msg,user);
				printf("查询历史完成！\n");
				break;
			case 6:
				printf("亲爱的管理员，再见！\n");
				sleep(3);
				return;
			default:
				printf("输入错误，请重新选择!\n");
		}
	}
	
}

void admin_add(int socket_fd,MSG *msg,USER *user)
{
	printf("admin_add test\n");
	printf("******热烈欢迎新员工******\n");
	printf("请输入工号（唯一）：");
	scanf("%d",&(msg->Usemsg.no));
	getchar();
	
	printf("\n请输入姓名：");
	scanf("%s",msg->Usemsg.name);
	getchar();
	
	printf("\n请输入密码）：");
	scanf("%s",msg->Usemsg.pwd);
	getchar();
	
	printf("\n请输入类型：");
	scanf("%c",&(msg->Usemsg.type));
	getchar();
	
	printf("\n请输入等级：");
	scanf("%d",&(msg->Usemsg.level));
	getchar();
	
	printf("\n请输入年龄：");
	scanf("%d",&(msg->Usemsg.age));
	getchar();
	
	printf("\n请输入地址：");
	scanf("%s",msg->Usemsg.addr);
	getchar();
	
	printf("\n请输入电话：");
	scanf("%s",msg->Usemsg.phone);
	getchar();
	
	printf("\n请输入工资：");
	scanf("%lf",&(msg->Usemsg.salary));
	getchar();
	msg->ope = INC;
	printf("新增的员工信息请确认：\n");
	staff_search(&(msg->Usemsg)); 
	send(socket_fd, msg, sizeof(MSG),0);
	printf("新增的员工成功！\n");
	memset(user,0,sizeof(USER));
	recv(socket_fd,user,sizeof(USER),0);
	
	staff_search(user);
}
void admin_del(int socket_fd,MSG *msg,USER *user)
{
	printf("admin_del test\n");
	printf("请输入删除的工号（唯一）：");
	scanf("%d",&(msg->Usemsg.no));
	msg->ope = DEL;
	send(socket_fd, msg, sizeof(MSG),0);
	memset(user,0,sizeof(USER));
	recv(socket_fd,user,sizeof(USER),0);
	printf("删除员工信息请确认：\n");
	staff_search(user);
	printf("删除成功\n");
}
void admin_upgra(int socket_fd,MSG *msg,USER *user)
{
	printf("admin_upgra test\n");
	printf("请输入修改的工号（唯一）：");
	scanf("%d",&(msg->Usemsg.no));
	printf("\n请输入修改工资）：");
	scanf("%lf",&(msg->Usemsg.salary));
	msg->ope = UPGRA_SALARY;
	send(socket_fd, msg, sizeof(MSG),0);
	memset(user,0,sizeof(USER));
	recv(socket_fd,user,sizeof(USER),0);
	printf("\n修改的员工信息请确认：\n");
	staff_search(user);
	printf("修改成功\n");

}
void admin_search(int socket_fd,MSG *msg,USER *user)
{
	int choice;
	int rec;
	while(1)
	{
		printf("********************************************\n");
		printf("**    1.查询个人    2.查询所有   3.退出   **\n");
		printf("********************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&choice);
		getchar();
		switch(choice)
		{
			case 1:
				msg->ope = SEARCH;
				printf("请输入查找的工号（唯一）：");
				scanf("%d",&(msg->Usemsg.no));
				send(socket_fd, msg, sizeof(MSG),0);
				memset(user,0,sizeof(USER));
				recv(socket_fd,user,sizeof(USER),0);
				if(user->type=='3')
				{
					printf("未找到\n");
					break;
				}
				staff_search(user);
				break;
			case 2:
				msg->ope = SEARCHALL;
				send(socket_fd, msg, sizeof(MSG),0);
				printf("\n*********************************************************************\n");
				printf("工号\t姓名\t密码\t类型\t级别\t年龄\t住址\t电话\t工资\n");
				while(1)
				{
					recv(socket_fd,user,sizeof(USER),0);
					if(user->type == 0)
						break;
					printf("%d\t%s\t%s\t",user->no,user->name,user->pwd);
					printf("%c\t%d\t%d\t",user->type,user->level,user->age);
					printf("%s\t%s\t%.1lf\n",user->addr,user->phone,user->salary);
					memset(user,0,sizeof(USER));
				}
				printf("*********************************************************************\n\n");
				break;
			case 3:
				printf("退出查询\n");
				sleep(1);
				return;
			default:
				printf("输入错误，请重新选择!\n");
		}
	}
}



void login_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;
	time(&((CONNECT *)arg)->t);
	((CONNECT *)arg)->lt = localtime(&((CONNECT *)arg)->t);
	
	sprintf(sql,"select * from usrinfo where no=%d and pwd='%s';",msg->Usemsg.no,msg->Usemsg.pwd);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"name or passwd failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
		else
		{
			user.no = msg->Usemsg.no;
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
			sprintf(sql,"%d-%02d-%02d %02d-%02d-%02d  %d %s login\n",
				((CONNECT *)arg)->lt->tm_year+1900,((CONNECT *)arg)->lt->tm_mon+1,
				((CONNECT *)arg)->lt->tm_mday,((CONNECT *)arg)->lt->tm_hour,
				((CONNECT *)arg)->lt->tm_min,((CONNECT *)arg)->lt->tm_sec,
				user.no,user.name);
			write(((CONNECT *)arg)->fdd,sql,strlen(sql));
			sleep(1);
		}
	}
	
}

void inc_handle(void *arg,MSG *msg)
{
	printf("***inc_handle*****\n");
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;
	
	sprintf(sql,"insert into usrinfo values(%d,\"%s\",\"%s\",\"%c\",%d,%d,\"%s\",\"%s\",%lf);",msg->Usemsg.no,msg->Usemsg.name,msg->Usemsg.pwd,msg->Usemsg.type,msg->Usemsg.level,msg->Usemsg.age,msg->Usemsg.addr,msg->Usemsg.phone,msg->Usemsg.salary);
	printf("%s\n",sql);
	
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0){
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else{
			user.no = msg->Usemsg.no;
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
}

void del_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			user.no = atoi(result[9]);
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
	sprintf(sql,"delete from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
}

void search_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			user.no = atoi(result[9]);
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
	
}

void search_all_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;
	int i=0;
	strcpy(sql,"select * from usrinfo;");
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			for(i=0;i<nrow;i++)
			{			
				user.no = atoi(result[9+i*9]);
				strcpy(user.name,result[10+i*9]);
				strcpy(user.pwd,result[11+i*9]);
				user.type = result[12+i*9][0];
				user.level = atoi(result[13+i*9]);
				user.age = atoi(result[14+i*9]);
				strcpy(user.addr,result[15+i*9]);
				strcpy(user.phone,result[16+i*9]);
				user.salary = atoi(result[17+i*9]);
				send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
			}
			user.type = 0;
			strcpy(user.des,"over\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
	return;
}
void upgra_pwd_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;

	sprintf(sql,"update usrinfo set pwd=%s where no=%d;",msg->Usemsg.pwd,msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			printf("---****----%s.\n",result[6]);
			user.no = atoi(result[9]);
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
	
}
void upgra_addr_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;

	sprintf(sql,"update usrinfo set addr='%s' where no=%d;",msg->Usemsg.addr,msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("11---****----%s.\n",errmsg);		
	}
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("22---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			printf("---****----%s.\n",result[6]);
			user.no = atoi(result[9]);
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
}

void upgra_salary_handle(void *arg,MSG *msg)
{
	char sql[128];
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	USER user;

	sprintf(sql,"update usrinfo set salary=%lf where no=%d;",msg->Usemsg.salary,msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("11---****----%s.\n",errmsg);		
	}
	sprintf(sql,"select * from usrinfo where no=%d;",msg->Usemsg.no);
	if(sqlite3_get_table(((CONNECT *)arg)->db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
	{
		printf("22---****----%s.\n",errmsg);		
	}
	else
	{		
		if(nrow == 0)
		{
			user.type = '3';
			strcpy(user.des,"failed.\n");
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}else
		{
			printf("---****----%s.\n",result[6]);
			user.no = atoi(result[9]);
			strcpy(user.name,result[10]);
			strcpy(user.pwd,result[11]);
			user.type = result[12][0];
			user.level = atoi(result[13]);
			user.age = atoi(result[14]);
			strcpy(user.addr,result[15]);
			strcpy(user.phone,result[16]);
			user.salary = atoi(result[17]);
			send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		}
	}
}
void history_handle(void *arg,MSG *msg)
{
	int readbyte;
	USER user;
	printf("history_handle\n");
	lseek(((CONNECT *)arg)->fdd, 0, SEEK_SET);
	while((readbyte = read(((CONNECT *)arg)->fdd,user.des,64))>0)
	{
		user.des[readbyte] = '\0';
		send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
		usleep(50);
	}
	user.type = 0;
	strcpy(user.des,"over");
	send(((CONNECT *)arg)->accept_fd,&user,sizeof(USER),0);
	usleep(50);
}

void admin_history(int socket_fd,MSG *msg,USER *user)
{
	printf("admin_history test\n");
	msg->ope = HISTORY;
	send(socket_fd, msg, sizeof(MSG),0);
	memset(user,0,sizeof(USER));
	printf("\n**********************************************\n");
	while(1)
	{	recv(socket_fd,user,sizeof(USER),0);
		usleep(50);
		if(user->type==0)
			break;
		printf("%s",user->des);
		memset(user,0,sizeof(USER));
	}
	printf("\n**********************************************\n\n");
}


