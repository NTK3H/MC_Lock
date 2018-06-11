#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "include/magic_cube_lock.h"
#include "include/ctrlfile.h"

int main()
{
	atexit(isExit);
	mc *m;
	FILE *fp = fopen(GetDir("Enter input file",DEF_OUTPUT),"rb");
	if(NULL == fp)
	{
		fprintf(stderr,"Cannot open input file\n");
		getch();
		exit(3);
	}
	FILE *ofp = fopen(GetDir("Enter output file",DEF_UNLOCKFILE),"wb+");
	if(NULL == ofp)
	{
		fprintf(stderr,"Cannot open output file\n");
		getch();
		exit(3);
	}

#ifndef LOCKTEST
	char (*codepsw)[50] = readkey(DirOrPsw("Enter key file or password",DEF_PASSWORDFILE));	//此处开始读取密匙,密匙存放在codepsw里
#endif

	m = (mc*)malloc(sizeof(mc));
	m->order = fgetc(fp);
	/*fseek(fp,0L,SEEK_END);
	long fSize = ftell(fp);		//获取文件大小
	rewind(fp);*/
	//开始获取文件结束前容器容积
	int EOF_iFlag = fgetc(fp);
	int EOF_jFlag = fgetc(fp);

#ifdef DEBUGMODE
	printf("\nDebug: i=%d j=%d\n",EOF_iFlag,EOF_jFlag);
#endif

	int iSize = m->order * m->order;

	for(int i=0;i<6;i++)		//为容器分配内存空间
	{
		m->box[i] = (char*)malloc(iSize);
		if(NULL == m->box[i])
		{
			fprintf(stderr,"malloc error in line: %d",__LINE__);
			getch();
			exit(3);
		}
	}

#ifdef DEBUGMODE
	int curnum = 0;
#endif

	char *box_b = NULL;
	puts("Unlocking...");
	int ef = 0;
	while(!feof(fp))
	{
#ifdef DEBUGMODE
		curnum++;
#endif

		for(register int i=0;i<6;i++)
		{
			memset(m->box[i],-1,iSize);
			box_b = m->box[i];
			fread(box_b,sizeof(char),iSize,fp);
		}

#ifndef LOCKTEST
		MCUnlock(m,codepsw);
#endif

		fread(&tmp,sizeof(char),1,fp);
		if(feof(fp))ef = 1;
		else fseek(fp,-1L,SEEK_CUR);

		for(register int i=0;i<6;i++)
		{
			//fwrite(m->box[i],sizeof(char),iSize,ofp);
			for(int j=0;j<iSize;j++)
			{
				fputc(m->box[i][j],ofp);
				if(ef && EOF_iFlag == i && EOF_jFlag == j)
					goto endflag;
			}
		}
	}

endflag:

#ifdef DEBUGMODE
	printf("Debug: Loop for %d times\n",curnum);
#endif

	for(int i=0;i<6;i++)	//这个循环用于释放内存空间
		free(m->box[i]);

	fclose(fp);				//关闭输入输出流
	fclose(ofp);

	fflush(stdin);
	puts("Done..");
	getch();
}

