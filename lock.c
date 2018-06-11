#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "include/ctrlfile.h"
#include "include/magic_cube_lock.h"

int main()
{
	atexit(isExit);
	FILE *input = NULL;
	if((input = fopen(GetDir("Enter input file",DEF_INPUT),"rb"))==NULL)
	{
		fprintf(stderr,"Open input file fail");
		getch();
		exit(3);
	}
	/* 此处开始成功打开文件,通过指针input */

	FILE *output = NULL;
	if((output = fopen(GetDir("Enter output file",DEF_OUTPUT),"wb"))==NULL)
	{
		fprintf(stderr,"Open output file fail");
		getch();
		exit(3);
	}
	/* 此处开始成功打开输出文件,通过指针output */

#ifndef LOCKTEST
	char (*codepsw)[50] = readkey(DirOrPsw("Enter key file or password",DEF_PASSWORDFILE));	//此处开始读取密匙,密匙存放在codepsw里
#endif

	fseek(input,0L,SEEK_END);
	int iSize = ftell(input);		//获取文件大小
	rewind(input);
	iSize /= 6;
	int iMaxOrder = sqrt(iSize)+1;	//根据文件大小匹配最佳阶数
	if(iMaxOrder > MAX_ORDER)
		iMaxOrder = MAX_ORDER;	//文件最佳阶数大于最大值则选最大阶数作为最佳阶数
	/* 此处决定采用的魔方阶数 */

	iSize = iMaxOrder*iMaxOrder;	//iSize决定单面最大值
	mc *m = (mc*)malloc(sizeof(mc));
	m -> order = iMaxOrder;
	/* 此处声明mc结构体并填充最大阶数 */
	fputc(iMaxOrder,output);	//将最大阶数写入文件

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

	char *cpoint = (char*)malloc(sizeof(char));
	while(1)		//此处指定最后一次循环填充容器的比例
	{
		for(int i=0;i<6;i++)
		{
			for(int j=0;j<iSize;j++)
			{
				fread(cpoint,sizeof(char),1,input);
				if(feof(input))
				{
					int _i = i;
					int _j = j-1;
					if(_j<0)
					{
						_i = i-1;
						_j = iSize - 1;
					}
					fputc(_i,output);
					fputc(_j,output);
#ifdef DEBUGMODE
					printf("\nBebug: i=%d j=%d\n",_i,_j);
#endif
					goto eofendflag;
				}
			}
		}
		/* 此处开始处理特殊情况(i==0 && j==0) */
		fgetc(input);
		if(feof(input))
		{
			fputc(6,output);
			fputc(iSize,output);
#ifdef DEBUGMODE
					printf("\nBebug: **Special case: i=6 j=%d\n",iSize);
#endif
			goto eofendflag;
		}
		else fseek(input,-1L,SEEK_CUR);
	}
eofendflag: rewind(input);
			free(cpoint);
			cpoint = NULL;

#ifdef DEBUGMODE
			int curnum = 0;
#endif

			/* 此处前期准备就绪,开始加密 */
			char *box_b = NULL;		//box指针备份
			puts("Locking...");
			while(!feof(input))
			{
#ifdef DEBUGMODE
				curnum++;
#endif
				/* 为了提高效率将此删除
				if(ferror(input))
				{
					fprintf(stderr,"Read file error!\n");
					getch();
					exit(3);
				}*/
				//这个循环用于填充容器
				for(register int i=0;i<6;i++)
				{
					memset(m->box[i],0,iSize);
					box_b = m->box[i];
					fread(box_b,sizeof(char),iSize,input);
					/*for(register int j=0;j<iSize;j++)
					 *box_b++ = fgetc(input);
					 *box_b = '\0';*/
				}

#ifndef LOCKTEST
				MCLock(m,codepsw);//此处加密容器中的内容
#endif

				/* 单个容器打乱完毕,现在容器内是被加密过的内容 */
				for(register int i=0;i<6;i++)	//开始将加密的产物写入输出文件
					fwrite(m->box[i],sizeof(char),iSize,output);

				fread(&tmp,sizeof(char),1,input);
				if(feof(input))break;
				else fseek(input,-1L,SEEK_CUR);
			}
			for(int i=0;i<6;i++)	//这个循环用于释放内存空间
				free(m->box[i]);

#ifdef DEBUGMODE
			printf("Bebug: Loop for %d times\n",curnum);
#endif

			/* 全部结束,关闭输入输出流 */
			fclose(input);
			fclose(output);
			fflush(stdin);
			puts("Done..");
			getch();
}
