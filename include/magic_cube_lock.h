#include "magic_cube_change.h"

/* ����ļ�������ܺͽ��ܺ���
 * ���ִ�������Ĺ��ܽ�����һ���汾����
 */

void MCLock(mc *m, char (*cpw)[50]);
void MCUnlock(mc *m, char (*cpw)[50]);

/* ����: ����cpw�����ݼ���m->box������
 *
 * �˺������ֹ����ѱ�֤ʵ����Ǳ��Σ�ջ����
 * ��ע�͵Ĳ��־�����Щ����
 * δ���޸Ĳ�Ҫʹ��,����������������ʧ�Ը�
 *
 * �����޸�ʱ��: 2018/5/17 17:41
 */
void MCLock(mc *m, char (*cpw)[50])
{
	for(int i=0;i<19;i++)
	{
		for(int j=0;j<49;j++)
		{
			if(cpw[i][j]&2 || m->order<=2)
				EdgeChange(cpw[i][j]%6+1,
						(cpw[i][j+1]%cpw[i][j])&1,m);
			else
				MidChange(cpw[i][j]%6+1,
						(cpw[i][j+1]%cpw[i][j])&1,
						(cpw[i][j]%cpw[i][j+1])&1,
						(cpw[i][j+1]%(m->order-2))+2,m);
		}
	}
	char bku;
	for(int i=0;i<6;i++)
		for(int j=0;j<m->order*m->order;j++)
		{
			if(cpw[i%19][j%49]&1 && j!=m->order*m->order-1)
			{
				bku = m->box[i][j];
				m->box[i][j] = (m->box[i][j]&0xf0)+(m->box[i][j+1]&0x0f);
				m->box[i][j+1] = (m->box[i][j+1]&0xf0)+(bku&0x0f);
			}
			else if(j!=m->order*m->order-1)
			{
				bku = m->box[i][j];
				m->box[i][j] = (m->box[i][j]&0x0f)+(m->box[i][j+1]&0xf0);
				m->box[i][j+1] = (m->box[i][j+1]&0x0f)+(bku&0xf0);
			}
			else m->box[i][j] = ~m->box[i][j];
			m->box[i][j] += cpw[i%19][j%49]%m->order;
			m->box[i][j] ^= cpw[i%19][j%49]&0xff;

			/* TODO Fix it in next version
			float iCPW = (float)i/(float)5;
			float jCPW = (float)j/(float)(m->order*m->order-1);
			if(cpw[i][j]&1 || m->order<=2)
				EdgeChange(cpw[(int)(19*iCPW)][(int)(49*jCPW)]%5+1,
						(cpw[(int)(18*iCPW)][48%(j+1)]%cpw[(int)(17*iCPW)][(int)(47*jCPW)]&1),m);
			else
				MidChange(cpw[(int)(16*iCPW)][(int)(46*jCPW)]%5+1,
						(cpw[(int)(15*iCPW)][45%(j+1)]%cpw[(int)(14*iCPW)][(int)(44*jCPW)])&1,
						(cpw[(int)(13*iCPW)][(int)(43*jCPW)]%cpw[(int)(12*iCPW)][42%(j+1)])&1,
						(cpw[(int)(11*iCPW)][11%(j+1)]%(m->order-2))+2,m);
						*/
		}
}

/* ����: ����cpw�����ݽ���m->box������
 *
 * �˺������ֹ����ѱ�֤ʵ����Ǳ��Σ�ջ����
 * ��ע�͵Ĳ��־�����Щ����
 * δ���޸Ĳ�Ҫʹ��,����������������ʧ�Ը�
 *
 * �����޸�ʱ��: 2018/5/17 17:10
 */
void MCUnlock(mc *m, char (*cpw)[50])
{
	char bku;
	for(int i=5;i>=0;i--)
		for(int j=m->order*m->order-1;j>=0;j--)
		{
			/* TODO Fix it in next version
			float iCPW = (float)i/(float)5;
			float jCPW = (float)j/(float)(m->order*m->order-1);
			if(cpw[i][j]&1 || m->order<=2)
				EdgeChange(cpw[(int)(19*iCPW)][(int)(49*jCPW)]%5+1,
						!((cpw[(int)(18*iCPW)][48%(j+1)]%cpw[(int)(17*iCPW)][(int)(47*jCPW)]&1)),m);
			else
				MidChange(cpw[(int)(16*iCPW)][(int)(46*jCPW)]%5+1,
						(cpw[(int)(15*iCPW)][45%(j+1)]%cpw[(int)(14*iCPW)][(int)(44*jCPW)])&1,
						!((cpw[(int)(13*iCPW)][(int)(43*jCPW)]%cpw[(int)(12*iCPW)][42%(j+1)])&1),
						(cpw[(int)(11*iCPW)][11%(j+1)]%(m->order-2))+2,m);*/
			m->box[i][j] ^= cpw[i%19][j%49]&0xff;
			m->box[i][j] -= cpw[i%19][j%49]%m->order;

			if(cpw[i%19][j%49]&1 && j!=m->order*m->order-1)
			{
				bku = m->box[i][j];
				m->box[i][j] = (m->box[i][j]&0xf0)+(m->box[i][j+1]&0x0f);
				m->box[i][j+1] = (m->box[i][j+1]&0xf0)+(bku&0x0f);
			}
			else if(j!=m->order*m->order-1)
			{
				bku = m->box[i][j];
				m->box[i][j] = (m->box[i][j]&0x0f)+(m->box[i][j+1]&0xf0);
				m->box[i][j+1] = (m->box[i][j+1]&0x0f)+(bku&0xf0);
			}
			else m->box[i][j] = ~m->box[i][j];
		}
	for(int i=18;i>=0;i--)
	{
		for(int j=48;j>=0;j--)
		{
			if(cpw[i][j]&2 || m->order<=2)
				EdgeChange(cpw[i][j]%6+1,
						!((cpw[i][j+1]%cpw[i][j])&1),m);
			else
				MidChange(cpw[i][j]%6+1,
						(cpw[i][j+1]%cpw[i][j])&1,
						!((cpw[i][j]%cpw[i][j+1])&1),
						(cpw[i][j+1]%(m->order-2))+2,m);
		}
	}
}
