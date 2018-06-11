#include <stdio.h>
#include <string.h>
#include <assert.h>

/* 这个文件用于构建魔方模型并进行基础的旋转
 * 其接口为两个函数,分别是EdgeChange和MidChange
 * 它们分别用做表面旋转和中间旋转
 * 此文件内其它的函数不可单独使用,否则将无法达到效果
 *
 * ***！此文件内所有函数均正确且有效,不应再修改任何东西！***
 *
 * 修改日志:
 * 1.增加swap,anticlockwise和clockwise函数并定义结构体mc
 * 2.增加UpOrDown和LeftOrRight函数
 * 3.增加ContraryArr函数和MCRound函数
 * 4.修复UpOrDown和LeftOrRight函数的BUG
 * 5.修复MCRound的BUG并添加了特殊旋转功能
 * 6.添加EdgeChange函数
 * 7.添加MidChange函数
 * 8.删除LeftOrRight函数
 * 
 * 近期修改时间: 2018/15/17 12:32
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

//逆时针
static void anticlockwise(char *cArr,int n)
{
	for(int i=0; i<n; ++i)
		for(int j=0; j<n-i; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-j)+(n-1-i)]);

	for(int i=0; i<n/2; ++i)
		for(int j=0; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-i)+j]);
}

//顺时针
static void clockwise(char *cArr, int n)
{
	for(int i=0; i<n; ++i)
		for(int j=i+1; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*j+i]);
	for(int i=0; i<n/2; ++i)
		for(int j=0; j<n; ++j)
			swap(&cArr[n*i+j], &cArr[n*(n-1-i)+j]);
}

//控制上下
//mode = 0 : 下转
//mode = 1 : 上转
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

//控制左右
//mode = 0 : 左转
//mode = 1 : 右转
/* 这个函数因用不到而被删除
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

//倒置数组
static void ContraryArr(char *cArr, int len)
{
	for(int i=0;i<len/2;i++)
	{
		char z = cArr[i];
		cArr[i] = cArr[len-i-1];
		cArr[len-i-1] = z;
	}
}

//控制旁边
static void MCRound(int mode, int pos, mc *m)
{
	assert(NULL != m);

	char RBox[m->order+2][m->order+2];
	memset(RBox,0,sizeof RBox);

	/* 这是一个特殊情况,R盒旋转需要使用块位移 */
	if(3 == mode || 4 == mode)
	{
		//读取
		for(int i=0;i<m->order;i++)
		{
			for(int j=0;j<3;j++)
				RBox[j][i] = m->box[j+1][m->order*(m->order-pos)+i];
			RBox[3][i] = m->box[5][m->order*(pos-1)+i];
		}

		char cRBox[m->order][m->order];
		memset(cRBox,0,sizeof cRBox);

		//位移
		for(int n=0;n<3;n++)
		{
			for(int i=0;i<3;i++)
				for(int j=0;j<m->order;j++)
					cRBox[i][j] = RBox[i+1][j];
			for(int j=0;j<m->order;j++)
				cRBox[3][j] = RBox[0][j];
			ContraryArr(cRBox[3],m->order);
			ContraryArr(cRBox[2],m->order);
			if(3 == mode)break;			//如果mode是3那么无需更新R盒可以直接退出循环

			//更新R盒
			for(int i=0;i<4;i++)
				for(int j=0;j<m->order;j++)
					RBox[i][j] = cRBox[i][j];
		}

		//写入
		for(int i=0;i<m->order;i++)
		{
			for(int j=0;j<3;j++)
				m->box[j+1][m->order*(m->order-pos)+i] = cRBox[j][i];
			m->box[5][m->order*(pos-1)+i] = cRBox[3][i];
		}
		return;		//特殊情况结束
	}

	/* 以下是一般情况,R盒旋转可以直接套用顺逆时 */

	void (*round)(char*, int);
	if(0 == mode)round = clockwise;
	if(1 == mode)round = anticlockwise;

	//读取
	for(int i=0;i<m->order;i++)
	{
		RBox[0][i+1] = m->box[0][m->order*(m->order-pos)+i];			//上
		RBox[m->order+1][i+1] = m->box[4][(m->order)*(pos-1)+i];		//下
		RBox[i+1][0] = m->box[1][i*m->order+m->order-pos];				//左
		RBox[i+1][m->order+1] = m->box[3][i*m->order+pos-1];			//右
	}

	//旋转
	round((char*)RBox,m->order+2);

	//写入
	for(int i=0;i<m->order;i++)
	{
		m->box[0][m->order*(m->order-pos)+i] = RBox[0][i+1];			//上
		m->box[4][(m->order)*(pos-1)+i] = RBox[m->order+1][i+1];		//下
		m->box[1][i*m->order+m->order-pos] = RBox[i+1][0];				//左
		m->box[3][i*m->order+pos-1] = RBox[i+1][m->order+1];			//右
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

//index 表示选中的面
//
//dirmode = 0: 表示横向旋转
//dirmode = 1: 表示竖向旋转
//
//mode = 0: 表示向上或向右旋转
//mode = 1: 表示向下或向左旋转
//
//pos 表示从左上角开始向左或右数的数
//
//m 表示准备更改的结构体
//
//取值范围:
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
		case 5:	//面1和面5执行起来效果一致
			if(0 == dirmode) MCRound(1-mode, pos, m);
			if(1 == dirmode) UpOrDown(mode, pos-1, m);
			break;
		case 2:
		case 4:	//面2和面4执行起来效果一致
			if(0 == dirmode) MCRound(4-mode, pos, m);
			if(1 == dirmode) MCRound(1-mode, pos, m);
			break;
		case 3:
		case 6:	//面3和面6执行起来效果一致
			if(0 == dirmode) MCRound(4-mode, pos, m);
			if(1 == dirmode) UpOrDown(mode, pos-1, m);
			break;
	}
}
