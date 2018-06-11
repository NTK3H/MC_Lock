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
	/* �˴���ʼ�ɹ����ļ�,ͨ��ָ��input */

	FILE *output = NULL;
	if((output = fopen(GetDir("Enter output file",DEF_OUTPUT),"wb"))==NULL)
	{
		fprintf(stderr,"Open output file fail");
		getch();
		exit(3);
	}
	/* �˴���ʼ�ɹ�������ļ�,ͨ��ָ��output */

#ifndef LOCKTEST
	char (*codepsw)[50] = readkey(DirOrPsw("Enter key file or password",DEF_PASSWORDFILE));	//�˴���ʼ��ȡ�ܳ�,�ܳ״����codepsw��
#endif

	fseek(input,0L,SEEK_END);
	int iSize = ftell(input);		//��ȡ�ļ���С
	rewind(input);
	iSize /= 6;
	int iMaxOrder = sqrt(iSize)+1;	//�����ļ���Сƥ����ѽ���
	if(iMaxOrder > MAX_ORDER)
		iMaxOrder = MAX_ORDER;	//�ļ���ѽ����������ֵ��ѡ��������Ϊ��ѽ���
	/* �˴��������õ�ħ������ */

	iSize = iMaxOrder*iMaxOrder;	//iSize�����������ֵ
	mc *m = (mc*)malloc(sizeof(mc));
	m -> order = iMaxOrder;
	/* �˴�����mc�ṹ�岢��������� */
	fputc(iMaxOrder,output);	//��������д���ļ�

	for(int i=0;i<6;i++)		//Ϊ���������ڴ�ռ�
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
	while(1)		//�˴�ָ�����һ��ѭ����������ı���
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
		/* �˴���ʼ�����������(i==0 && j==0) */
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

			/* �˴�ǰ��׼������,��ʼ���� */
			char *box_b = NULL;		//boxָ�뱸��
			puts("Locking...");
			while(!feof(input))
			{
#ifdef DEBUGMODE
				curnum++;
#endif
				/* Ϊ�����Ч�ʽ���ɾ��
				if(ferror(input))
				{
					fprintf(stderr,"Read file error!\n");
					getch();
					exit(3);
				}*/
				//���ѭ�������������
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
				MCLock(m,codepsw);//�˴����������е�����
#endif

				/* ���������������,�����������Ǳ����ܹ������� */
				for(register int i=0;i<6;i++)	//��ʼ�����ܵĲ���д������ļ�
					fwrite(m->box[i],sizeof(char),iSize,output);

				fread(&tmp,sizeof(char),1,input);
				if(feof(input))break;
				else fseek(input,-1L,SEEK_CUR);
			}
			for(int i=0;i<6;i++)	//���ѭ�������ͷ��ڴ�ռ�
				free(m->box[i]);

#ifdef DEBUGMODE
			printf("Bebug: Loop for %d times\n",curnum);
#endif

			/* ȫ������,�ر���������� */
			fclose(input);
			fclose(output);
			fflush(stdin);
			puts("Done..");
			getch();
}
