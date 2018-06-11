#include <unistd.h>
#include <conio.h>
#include <assert.h>

/* ��������ļ���ػ�����ļ�����س���������ĺ���
 *
 * ***������ļ����к���(����isExit)������ȷ����Ч��,���������Ӧ��Ӻ����������޸ģ�***
 *
 * �����Ҫ�������ڴ��ļ���궨��DEBUGMODE
 * �����Ҫ�������ļ����ļ���ȡ�����ܡ���д�빦���ڴ��ļ��ں궨��LOCKTEST
 * ���߿�ͬʱ���ֻ򵥶�����
 *
 * �����޸�ʱ��: 2018/5/25 11:10
 */

#define MAX_DIRC				100
#define MAX_ORDER				10

#define DEF_PASSWORDFILE		"output\\password.psw"
#define DEF_INPUT				"output\\source.txt"
#define DEF_OUTPUT				"output\\locked.mclock"
#define DEF_UNLOCKFILE			"output\\unlock.txt"

void isExit(void);
int readline(char *strDest,FILE *fp);
char (*readkey(char *strDir))[50];
char (*makekey(char *password))[50];
char *GetDir(const char* strMsg, const char* restrict strDef);
char *DirOrPsw(const char* strMsg, const char* restrict strDef);
void progressbar(int iMaxProgress, long iCurNum, long iCurProgress, const char* szMsg);

char *charmap = "/+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
char tmp;


void isExit()
{
	//TODO ��������˳���ʱ���ִ��,�����������β�õĴ���
	printf("\nProgram Exit\n");
}

/* ע��:
 * ������������̰߳�ȫ��,��ʹ����һ��ȫ�ֱ���curpb
 * �������ļ���ע�ⲻҪ����������Ƶ�ȫ�ֱ���
 */
int curpb = 0;
void progressbar(int iMaxProgress, long iCurNum, long iCurProgress, const char* szMsg)
{
	double dProgress = (double)iCurProgress/(double)iCurNum;
	int iProgress = (int)(dProgress*100);
	if(curpb == iProgress)return;
	printf("\r");
	printf("%s[",szMsg);
	int iNums = iMaxProgress*dProgress;
	for(int i=1;i<=iMaxProgress;i++)
	{
		if(0 != iNums)
		{
			printf("#");
			iNums--;
		}
		else
		{
			printf(" ");
		}
	}
	printf("] %d%%   ",(int)(dProgress*100));
	curpb = iProgress;
}

/* This Funcation can read one line from a file */
int readline(char *strDest,FILE *fp)
{
	while((*strDest++ = fgetc(fp))!='\n' && !feof(fp) && !ferror(fp));
	*--strDest = '\0';
	if(feof(fp))return EOF;
	if(ferror(fp))return 0;
	return 1;
}

char (*makekey(char *password))[50]
{
	assert(NULL != password);
	srand(*password);
	char (*codepsw)[50] = (char(*)[50])calloc(20,sizeof(char)*50);
	int iPswNum = 0;
	for(int i=0;i<19;i++)
	{
		for(int j=0;j<50;j++)
		{
			iPswNum++;
			codepsw[i][j] = charmap[(rand()%password[iPswNum%strlen(password)]) % strlen(charmap)];
		}
	}
	return codepsw;
}

/* This Funcation can read key in file*/
char (*readkey(char *strDir))[50]
{
	//atexit(isExit);
	if(*strDir == ':') return makekey(++strDir);	//�����ͷ��:���������һ������
	if(access(strDir,F_OK))	//����Ҳ����ļ����쳣��ֹ����
	{
		fprintf(stderr,"Cannot find file: %s\n",strDir);
		getch();
		exit(3);
	}
	char (*codepsw)[50] = (char(*)[50])calloc(20,sizeof(char)*50);
	if(NULL == codepsw)
	{
		fprintf(stderr,"malloc error in line:%d",__LINE__);
		getch();
		exit(3);
	}
	//��ʼ���Դ��ܳ��ļ�
	FILE *psw = NULL;
	if((psw = fopen(strDir,"r"))==NULL)
	{
		fprintf(stderr,"Cannot open file: %s",strDir);
		getch();
		exit(3);
	}
	//��ʼ��ȡ��Կ
	for(int i=0;i<19;i++)
		readline((char*)&codepsw[i],psw);
	fclose(psw);
	return codepsw;
}

/* This funcation can get directoy name */
char *GetDir(const char* strMsg, const char* restrict strDef)
{
	//atexit(isExit);
	printf("%s[%s]:",strMsg,strDef);
	char *strDir = (char*)malloc(MAX_DIRC);
	if(NULL == strDir)exit(3);	//���Ŀ¼���ƿռ����ʧ�����쳣��ֹ����
	char *_strDir = strDir;
	while((*_strDir++ = getchar())!='\n');
	if(*strDir == '\n')strcpy(strDir,strDef);
	else *--_strDir = '\0';			//���ղŶ�ȡ�Ļس��û�Ϊ\0
	if(strlen(strDir)>MAX_DIRC)		//��������Ŀ¼�������趨ֵ���쳣��ֹ����
	{
		fprintf(stderr,"Dir name can only be less than deafult character");
		getch();
		exit(3);
	}
	return strDir;
}

char *DirOrPsw(const char* strMsg, const char* restrict strDef)
{
	//atexit(isExit);
	printf("%s[%s]:",strMsg,strDef);
	char *strDir = (char*)malloc(100);
	if(NULL == strDir)exit(3);	//���Ŀ¼���ƿռ����ʧ�����쳣��ֹ����
	char *_strDir = strDir;
	while((*_strDir = getch())!='\r')
	{
		if(*_strDir == '\b')
		{
			if(_strDir == strDir)continue;
			_strDir = _strDir - 1;
			printf("\b \b");
		}
		else
		{
			if(*strDir == ':')
			{
				if(_strDir == strDir)
					printf(":");
				else
					printf("*");

			}
			else if(*_strDir != '\b')
				printf("%c",*_strDir);
			_strDir = _strDir + 1;

		}
	}
	printf("\n");
	if(*strDir == '\r')strcpy(strDir,strDef);
	else *_strDir = '\0';
	if(strlen(strDir)>MAX_DIRC)		//��������Ŀ¼������������趨ֵ���쳣��ֹ����
	{
		fprintf(stderr,"Dir name can only be less than deafult character");
		getch();
		exit(3);
	}
	return strDir;
}

