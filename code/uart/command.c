#include "command.h"
#include "string.h"

u16 writeBinLen;
u16 writeAddr;

struct{
	u32 ms;
	u32 last;
}runtime;

void RuntimeClear(void)
{
	runtime.ms = 0;
	runtime.last = 0;
}

void RuntimeStart(void)
{
	runtime.last = millis();
}

void RuntimeEnd(void)
{
	if(runtime.last > millis())
	{
		RuntimeClear();
	}else{
		runtime.ms += millis() - runtime.last;
	}
	
}

void RunTimeGetCmd(char* str)
{
	printf(":%d ms:OK#",runtime.ms);
	//return runtime.ms;
}

typedef void (*cmd_cb)(char*);
typedef struct
{
	char* cmd;
	cmd_cb cb;
	char* info;
}DR_CMD;

void NULLCmd(char * str);
void help_proc(char* str);


DR_CMD cmd_list[] = //系统指令
{
	{"help",help_proc,"帮助"},					
	{"GETTIME",RunTimeGetCmd,"获取运行时间"},
};


char *skip(char *buf)//跳过过多空格
{
	while(*buf == ' ')
	{
		buf++;
	}
	return buf;
}

void cmd_proc(char* buf)//命令扫描
{
	u8 i;
	u8 size = sizeof(cmd_list)/sizeof(DR_CMD);
	for(i = 0; i < size; i++)
	{
		int len = strlen(cmd_list[i].cmd);
		
		if(strncmp(buf,cmd_list[i].cmd,len) == 0)
		{
			printf("%s",(char*)cmd_list[i].cmd);
			char *s = skip(buf + len);
			RuntimeStart();
			cmd_list[i].cb(s);		//执行命令
			RuntimeEnd();
			return;
		}
	}
	printf("%s:UnknownCommand:NG#",(char*)uart.rxbuf);
}

void CommandExecution(void)
{
	if(uart.Len>3)
	{
		uart.rxbuf[uart.Len] = '\0';                                        
		uart.Len = 0;   	
		cmd_proc((char*)uart.rxbuf);
	}else{
		uart.rxbuf[uart.Len] = '\0';  
		uart.Len = 0;  
		printf("%s:UnknownCommand:NG#",(char*)uart.rxbuf);
	}
}


void help_proc(char* str)
{
	u8 i,size;
	size = sizeof(cmd_list)/sizeof(DR_CMD);
	
	printf("this is helper~\r\n");
	for(i = 0; i < size; i++)
	{
		printf("%s# \t%s\r\n",cmd_list[i].cmd,cmd_list[i].info);
	}
	printf(":OK#");
}


void NULLCmd(char * str){
	printf(":NULLCmd:NG#\n");
}

