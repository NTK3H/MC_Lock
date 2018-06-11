#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "include/ctrlfile.h"

#define MAX_PASSWORD 1000
#define CATCH_TIME 0

int main()
{
	char *strDir = GetDir("Enter key file",DEF_PASSWORDFILE);
	FILE *fp;
	if((fp = fopen(strDir,"w"))==NULL)
	{
		printf("Cannot open %s\n",strDir);
		getch();
		exit(3);
	}
	POINT p,p2;
	char password[MAX_PASSWORD] = {0};
	int loop = 0;

	puts("Move mouse to genkey...");
	while(TRUE)
	{
		GetCursorPos(&p);
		if((p.x==p2.x)&&(p.y==p2.y))continue;
		p2.x=p.x;
		p2.y=p.y;
		password[loop-1] = (p.x%10)*10+p.y%10;
		progressbar(50,MAX_PASSWORD,loop,"Progress:");
		loop++;
		Sleep(CATCH_TIME);
		if(MAX_PASSWORD+1 == loop)break;
	}
	printf("\nKey:\n");
	int len = strlen(charmap);
	for(int i=1;i<MAX_PASSWORD+1;i++)
	{
		password[i]%=len;
		printf("%c",charmap[(int)password[i-1]]);
		fputc(charmap[(int)password[i-1]],fp);
		if(i%50 == 0)
		{
			printf("\n");
			fputc('\n',fp);
		}
	}
	printf("\nKey is already saved in %s\n",strDir);
	fclose(fp);
	fflush(stdin);
	printf("Hit any key to quit...");
	getch();
}
