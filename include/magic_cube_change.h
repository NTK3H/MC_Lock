#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ����ļ����ڹ���ħ��ģ�Ͳ����л�������ת
 * ��ӿ�Ϊ��������,�ֱ���EdgeChange��MidChange
 * ���Ƿֱ�����������ת���м���ת
 * ���ļ��������ĺ������ɵ���ʹ��,�����޷��ﵽЧ��
 *
 * ***�����ļ������к�������ȷ����Ч,��Ӧ���޸��κζ�����***
 *
 * �޸���־:
 * 1.����swap,anticlockwise��clockwise����������ṹ��mc
 * 2.����UpOrDown��LeftOrRight����
 * 3.����ContraryArr������MCRound����
 * 4.�޸�UpOrDown��LeftOrRight������BUG
 * 5.�޸�MCRound��BUG�������������ת����
 * 6.���EdgeChange����
 * 7.���MidChange����
 * 8.ɾ��LeftOrRight����
 * 
 * �����޸�ʱ��: 2018/15/17 12:32
 */

typedef struct Magic_Cube{
	int order;
	char *box[6];
}mc;

static void swap(char *x, char *y);
static void anticlockwise(char *cArr,int n);
static void clockwise(char *cArr, int n);
static void UpOrDown(int mode,int pos,mc *m);
static void ContraryArr(char *cArr, int len);
static void MCRound(int mode, int pos, mc *m);
void EdgeChange(int index, int mode, mc *m);
void MidChange(int index, int dirmode, int mode, int pos, mc *m);

/* This funcation can exchange x and y */
static void swap(char *x, char *y)
{
	char z = *x;
	*x = *y;
	*y = z;
}

//��ʱ��
static void anticlockwise(char *cArr,int n)
{
	for(int i=0; i<n; ++i)
		for(int j=0; j<n-i; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-j)+(n-1-i)]);

	for(int i=0; i<n/2; ++i)
		for(int j=0; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-i)+j]);
}

//˳ʱ��
static void clockwise(char *cArr, int n)
{
	for(int i=0; i<n; ++i)
		for(int j=i+1; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*j+i]);
	for(int i=0; i<n/2; ++i)
		for(int j=0; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-i)+j]);
}

//��������
//mode = 0 : ��ת
//mode = 1 : ��ת
static void UpOrDown(int mode,int pos,mc *m)
{
	assert(NULL != m);
	char *strTrv[4] = {NULL};
	int BoxMap[4] = {0,2,4,5};

	if(0 == mode)
	{
		int j=0;
		for(int i=0;i<=3;i++)
		{
			j = i==3?0:j+1;
			strTrv[i] = (char*)malloc(m->order+1);
			for(int k=0;k<m->order;k++)
				strTrv[i][k] = m->box[BoxMap[j]][m->order*k+pos];
		}
		for(int i=0;i<=3;i++)
			for(int j=0;j<m->order;j++)
				m->box[BoxMap[i]][m->order*j+pos] = strTrv[i][j];
	}

	if(1 == mode)
		for(int i=0; i<3; i++)
			UpOrDown(0,pos,m);
}

//��������
//mode = 0 : ��ת
//mode = 1 : ��ת
/* ����������ò�������ɾ��
void LeftOrRight(int mode, int pos, mc *m)
{
	assert(NULL != m);
	char *strTrv[3] = {NULL};

	if(0 == mode)
	{
		int j=1;
		for(int i=0;i<3;i++)
		{
			j = i==2?1:j+1;
			strTrv[i] = (char*)malloc(m->order+1);
			for(int k=0;k<m->order;k++)
				strTrv[i][k] = m->box[j][m->order*pos+k];
		}
		for(int i=1;i<=3;i++)
			for(int j=0;j<m->order;j++)
			{
				int k = i-1;
				m->box[i][m->order*pos+j] = strTrv[k][j];
			}
	}

	if(1 == mode)
		for(int i=0; i<3; i++)
			UpOrDown(0,pos,m);
}
*/

//��������
static void ContraryArr(char *cArr, int len)
{
	for(int i=0;i<len/2;i++)
	{
		char z = cArr[i];
		cArr[i] = cArr[len-i-1];
		cArr[len-i-1] = z;
	}
}

//�����Ա�
static void MCRound(int mode, int pos, mc *m)
{
	assert(NULL != m);

	char RBox[m->order+2][m->order+2];
	memset(RBox,0,sizeof RBox);

	/* ����һ���������,R����ת��Ҫʹ�ÿ�λ�� */
	if(3 == mode || 4 == mode)
	{
		//��ȡ
		for(int i=0;i<m->order;i++)
		{
			for(int j=0;j<3;j++)
				RBox[j][i] = m->box[j+1][m->order*(m->order-pos)+i];
			RBox[3][i] = m->box[5][m->order*(pos-1)+i];
		}

		char cRBox[m->order][m->order];
		memset(cRBox,0,sizeof cRBox);

		//λ��
		for(int n=0;n<3;n++)
		{
			for(int i=0;i<3;i++)
				for(int j=0;j<m->order;j++)
					cRBox[i][j] = RBox[i+1][j];
			for(int j=0;j<m->order;j++)
				cRBox[3][j] = RBox[0][j];
			ContraryArr(cRBox[3],m->order);
			ContraryArr(cRBox[2],m->order);
			if(3 == mode)break;			//���mode��3��ô�������R�п���ֱ���˳�ѭ��

			//����R��
			for(int i=0;i<4;i++)
				for(int j=0;j<m->order;j++)
					RBox[i][j] = cRBox[i][j];
		}

		//д��
		for(int i=0;i<m->order;i++)
		{
			for(int j=0;j<3;j++)
				m->box[j+1][m->order*(m->order-pos)+i] = cRBox[j][i];
			m->box[5][m->order*(pos-1)+i] = cRBox[3][i];
		}
		return;		//�����������
	}

	/* ������һ�����,R����ת����ֱ������˳��ʱ */

	void (*round)(char*, int);
	if(0 == mode)round = clockwise;
	if(1 == mode)round = anticlockwise;

	//��ȡ
	for(int i=0;i<m->order;i++)
	{
		RBox[0][i+1] = m->box[0][m->order*(m->order-pos)+i];			//��
		RBox[m->order+1][i+1] = m->box[4][(m->order)*(pos-1)+i];		//��
		RBox[i+1][0] = m->box[1][i*m->order+m->order-pos];				//��
		RBox[i+1][m->order+1] = m->box[3][i*m->order+pos-1];			//��
	}

	//��ת
	round((char*)RBox,m->order+2);

	//д��
	for(int i=0;i<m->order;i++)
	{
		m->box[0][m->order*(m->order-pos)+i] = RBox[0][i+1];			//��
		m->box[4][(m->order)*(pos-1)+i] = RBox[m->order+1][i+1];		//��
		m->box[1][i*m->order+m->order-pos] = RBox[i+1][0];				//��
		m->box[3][i*m->order+pos-1] = RBox[i+1][m->order+1];			//��
	}
}

//mode = 0: Left
//mode = 1: Right
//
//index = 1-6
void EdgeChange(int index, int mode, mc *m)
{
	if(0 == mode)
		anticlockwise(m->box[index-1],m->order);
	else if(1 == mode)
		clockwise(m->box[index-1],m->order);
	switch(index)
	{
		case 1:
			MCRound(3+mode,m->order,m);
			break;
		case 2:
			UpOrDown(1-mode,0,m);
			break;
		case 3:
			MCRound(1-mode,1,m);
			break;
		case 4:
			UpOrDown(mode,m->order,m);
			break;
		case 5:
			MCRound(4-mode,1,m);
			break;
		case 6:
			MCRound(mode,m->order,m);
			break;
	}
}

//index ��ʾѡ�е���
//
//dirmode = 0: ��ʾ������ת
//dirmode = 1: ��ʾ������ת
//
//mode = 0: ��ʾ���ϻ�������ת
//mode = 1: ��ʾ���»�������ת
//
//pos ��ʾ�����Ͻǿ�ʼ�������������
//
//m ��ʾ׼�����ĵĽṹ��
//
//ȡֵ��Χ:
//index: 1-6
//dirmode: 0-1
//mode: 0-1
//pos: 2-(m->order-1)
void MidChange(int index, int dirmode, int mode, int pos, mc *m)
{
	assert(NULL != m && pos>1 && pos<m->order && m->order>2);

	switch(index)
	{
		case 1:
		case 5:	//��1����5ִ������Ч��һ��
			if(0 == dirmode) MCRound(1-mode, pos, m);
			if(1 == dirmode) UpOrDown(mode, pos-1, m);
			break;
		case 2:
		case 4:	//��2����4ִ������Ч��һ��
			if(0 == dirmode) MCRound(4-mode, pos, m);
			if(1 == dirmode) MCRound(1-mode, pos, m);
			break;
		case 3:
		case 6:	//��3����6ִ������Ч��һ��
			if(0 == dirmode) MCRound(4-mode, pos, m);
			if(1 == dirmode) UpOrDown(mode, pos-1, m);
			break;
	}
}
