#include "inc/DBOPS.h"
#include <string.h>
#include <iostream>
using namespace std;
void logFile(char* file,const char*txt){
	cout << txt << "\n";
}

DBOPS::DBOPS(const char* host,const char* user,const char*pass,const char*db)
{
	strcpy(this->host,host);
	strcpy(this->user,user);
	strcpy(this->pass,pass);
	strcpy(this->db,db);
	char txt[1000];
	sqlConn = mysql_init(NULL);
	if(mysql_real_connect(sqlConn,host,user,pass,NULL,0,NULL,0) == NULL){
		logFile(SQL_LOG,mysql_error(sqlConn));
		checkConnection(&sqlConn,host,user,pass,db);

	}
	if(mysql_select_db(sqlConn,db)){
		sprintf(txt,"SELECT db %s\nError:%s",db,mysql_error(sqlConn));		
		logFile(SQL_LOG,txt);
		checkConnection(&sqlConn,host,user,pass,db);

	}
	sem_init(&sqlSem,0,1);
	sem_init(&sqlSem2,0,1);
}


DBOPS::~DBOPS(void)
{
}
int DBOPS::checkConnection(MYSQL** conn,const char *host,const char *user,const char *pass,const char *db){
	char txt[1000];
	int state = -1;
	int error = 0;
	do{
		error = mysql_errno(*conn);
		if(error >= 2000 && error <= 2059){
			if(mysql_real_connect(*conn,host,user,pass,db,0,NULL,0) == NULL){		
				sprintf(txt,"Error connectig to %s %s\n",host,mysql_error(*conn));
				logFile(SQL_LOG,txt);
				//sendEmail("igindac@iristel.ro","MYSQL ERROR",txt);
				//logFile(pstat->file,txt);
				//sleep(5);
				error = mysql_errno(*conn);
				if(error == SERVER_GONE || error == COMMAND_OUT_OF_SYNC)
					continue;
			}
			sprintf(txt,"CONNECTION state %d %d %s\n",state,mysql_errno(*conn),mysql_error(*conn));
			logFile(SQL_LOG,txt);
			sleep(1);
		}
		else{
			break;
			return 1;
		}
	}while(error != 0);
	return 0;
}

int DBOPS::executeSql(MYSQL* sqlConn,char *sqlCmd){
	sem_wait(&sqlSem);
	if(sqlConn == NULL){
		sem_post(&sqlSem);
		return -2;
	}
	int reloaded = 0;
	if(sqlCmd == NULL || strlen(sqlCmd) == 0){
		sem_post(&sqlSem);
		return -3;
	}
	//puts(sqlCmd);
	logFile(SQL_LOG,sqlCmd);
	if(mysql_query(sqlConn,sqlCmd)){
		//TODO
		//checkConnection(&sqlConn,
		logFile(SQL_LOG,mysql_error(sqlConn));
		if(checkConnection(&sqlConn,host,user,pass,db) == 1){
			sem_post(&sqlSem);
			return -1;
		}
		else
		{
			reloaded = 1;
		}
	}
	if(reloaded){
		if(mysql_query(sqlConn,sqlCmd)){
			//TODO
			//checkConnection(&sqlConn,
			logFile(SQL_LOG,mysql_error(sqlConn));
			if (checkConnection(&sqlConn, host, user, pass, db) == 1){
				sem_post(&sqlSem);
				return -1;
			}
			else
			{
				reloaded = 1;
			}
		}	
	}
	sem_post(&sqlSem);
	return 0;
}

MYSQL_RES * DBOPS::executeSqlWithRes(MYSQL* sqlConn,char *sqlCmd){
	sem_wait(&sqlSem2);

	MYSQL_RES *res = NULL;
	int rc = executeSql(sqlConn,sqlCmd);
	if(!rc){
		res = (MYSQL_RES*)mysql_store_result(sqlConn);
		sem_post(&sqlSem2);
		return res;
	}
	sem_post(&sqlSem2);
	return NULL;
}
