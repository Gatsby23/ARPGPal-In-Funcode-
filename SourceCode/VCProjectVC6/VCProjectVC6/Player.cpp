#include <Stdio.h>
#include "CommonAPI.h"

#include <Mmsystem.h>  
#pragma comment ( lib, "Winmm.lib" )  
///******************************���ܺ���***************************************/

//������Ϣ
void sendMessage(char* message);

///********************************����*****************************************/

//���䣨����szSrcNameΪNULL��ʾ������ʵ����
void addBox(const char *szSrcName, const char *szMyName, short value);
//����
void cutBox(const char *name);

///******************************�������***************************************/

static struct player
{
	short hp;              //�������
	short mag;             //�������
	short exp;             //��Ҿ���
	short lv;              //��ҵȼ�
	short gold;            //��ҽ�Ǯ
	short weapon;          //��������
	
	//����ٶȷ���-1,0,1
	short dirX;
	short dirY;
	//����ϴγ���, -1,0,1
	short lastDirX;
	short lastDirY;

	//��Ҹ�������
	float resusX;
	float resusY;

	//������ȴʱ��
	float magRest;
	//��ұ���ʱ��
	float proTime;

}player[2];

short getPlHp(short pl_id){return player[pl_id].hp;}
short getPlMag(short pl_id){return player[pl_id].mag;}
short getPlExp(short pl_id){return player[pl_id].exp;}
short getPlLv(short pl_id){return player[pl_id].lv;}
short getPlGold(short pl_id){return player[pl_id].gold;}
short getPlWeapon(short pl_id){return player[pl_id].weapon;}

short getPlDirX(short pl_id){return player[pl_id].dirX;}
short getPlDirY(short pl_id){return player[pl_id].dirY;}
short getLastDirX(short pl_id){return player[pl_id].lastDirX;}
short getLastDirY(short pl_id){return player[pl_id].lastDirY;}

float getResusX(short pl_id){return player[pl_id].resusX;}
float getResusY(short pl_id){return player[pl_id].resusY;}

float getProTime(short pl_id){return player[pl_id].proTime;}


void setPlHp(short pl_id, short value){player[pl_id].hp = value;}
void setPlMag(short pl_id, short value){player[pl_id].mag = value;}
void setPlExp(short pl_id, short value){player[pl_id].exp = value;}
void setPlLv(short pl_id, short value){player[pl_id].lv = value;}
void setPlWeapon(short pl_id, short value){player[pl_id].weapon = value;}

void setPlDir(short pl_id, short dirX, short dirY)
{player[pl_id].dirX = dirX;player[pl_id].dirY = dirY;}
void setLastDir(short pl_id, short lastDirX, short lastDirY)
{player[pl_id].lastDirX = lastDirX; player[pl_id].lastDirY = lastDirY;}

void setResusPos(short pl_id, float resusX, float resusY)
{player[pl_id].resusX = resusX;player[pl_id].resusY = resusY;}

void setMagRest(short pl_id, short value){player[pl_id].magRest = value;}
void setProTime(short pl_id, short value){player[pl_id].proTime = value;}

///******************************��ҹ���***************************************/

//��������
bool addPlHp(short pl_id, short value)
{
	if (player[pl_id].hp>=8 + 2 * player[pl_id].lv) return false;
	player[pl_id].hp+=value;
	//���޼��
	if (player[pl_id].hp > 8 + 2 * player[pl_id].lv)
		player[pl_id].hp = 8 + 2 * player[pl_id].lv;
	return true;
}
//��������
bool addPlMag(short pl_id, short value)
{
	if (player[pl_id].mag>=9 + 3 * player[pl_id].lv) return false;
	player[pl_id].mag+=value;
	//���޼��
	if (player[pl_id].mag > 9 + 3 * player[pl_id].lv)
		player[pl_id].mag = 9 + 3 * player[pl_id].lv;
	return true;
}
//�õ�ͭǮ
bool addPlGold(short pl_id, short value)
{
	if (player[pl_id].gold>=9999) return false;
	player[pl_id].gold+=value;
	if (player[pl_id].gold > 9999)
		player[pl_id].gold = 9999;
	return true;
}
//ʧȥͭǮ
bool losePlGold(short pl_id, short value)
{
	if (player[pl_id].gold < value) return false;
	player[pl_id].gold-=value;
	return true;
}

//ˢ������
void rebuildPl()
{
	player[0].hp = 8 + 2 * player[0].lv;
	player[1].hp = 8 + 2 * player[1].lv;
	player[0].exp = player[1].exp = 0;
	player[0].mag = 9 + 3 * player[0].lv;
	player[1].mag = 9 + 3 * player[0].lv;
	player[0].gold = player[1].gold = 0;
	player[0].weapon = player[1].weapon = 0;


	player[0].dirX = player[1].dirX = player[0].dirY = player[1].dirY = 0;
	player[0].lastDirX = player[1].lastDirX = 0;
	player[0].lastDirY = player[1].lastDirY = 1;

	player[0].magRest = player[1].magRest = 2;
	player[0].proTime = player[1].proTime = 3;
}

//������Ҷ�������¼��ҳ��򣬲����������ᣬ���ID
void playerWork(long axis, short pl_id)
{
	if (!player[pl_id].hp) return; 
	char *pl_name = dMakeSpriteName("player", pl_id);
	short pl_frame = axis%4;
	short pl_dir = player[pl_id].dirX + 3 * player[pl_id].dirY;
	switch (pl_dir)
	{
	case 3: 
		{
			player[pl_id].lastDirX = 0;
			player[pl_id].lastDirY = 1;
		}break;
	case -1:
		{
			player[pl_id].lastDirX = -1;
			player[pl_id].lastDirY = 0;
			pl_frame+=4;
		}break;
	case 1:
		{
			player[pl_id].lastDirX = 1;
			player[pl_id].lastDirY = 0;
			pl_frame+=8;
		}break;
	case -3:
		{
			player[pl_id].lastDirX = 0;
			player[pl_id].lastDirY = -1;
			pl_frame+=12;
		}break;
	case 2:
		{
			player[pl_id].lastDirX = -1;
			player[pl_id].lastDirY = 1;
			pl_frame+=16;
		}break;
	case -4:
		{
			player[pl_id].lastDirX = -1;
			player[pl_id].lastDirY = -1;
			pl_frame+=20;
		}break;
	case 4:
		{
			player[pl_id].lastDirX = 1;
			player[pl_id].lastDirY = 1;
			pl_frame+=24;
		}break;
	case -2:
		{
			player[pl_id].lastDirX = 1;
			player[pl_id].lastDirY = -1;
			pl_frame+=28;
		}break;
	default:
		pl_frame = 4 * (dGetStaticSpriteFrame( pl_name )/4);
		break;
	}
	dSetStaticSpriteFrame( pl_name, pl_frame);
}

//���¼�����ȴʱ��
void magRestChange(short pl_id)
{
	if (player[pl_id].magRest == 1)
	{
		//ɾ������
		cutBox(dMakeSpriteName("ob_magic", pl_id));
		//����ͼ�����
		dSetSpriteColorAlpha(dMakeSpriteName("magIcon", pl_id), 255);
	}
	player[pl_id].magRest+=0.2f;
}
//������ұ���ʱ��
void proTimeChange(short pl_id)
{
	if (player[pl_id].proTime > 0)
	{
		player[pl_id].proTime-=0.2f;
		const char *pl_name = dMakeSpriteName("player", pl_id);
		if (player[pl_id].proTime <= 0) player[pl_id].proTime = 0;
		dSetSpriteColorAlpha( pl_name, (int)(255 - 50 * player[pl_id].proTime) );
	}
}

//Ѫ�����ټ��������
void hpLose(short pl_id, short hurt)
{
	player[pl_id].hp-=hurt;

	//���˶���
	char *pl_name = dMakeSpriteName("player", pl_id);
	dCloneSprite("playerHurt", "ob_playerHurt");
	dSetSpritePosition("ob_playerHurt", dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name));
	dSetSpriteLifeTime("ob_playerHurt", 0.5f);

	//�������
	if (player[pl_id].hp<=0)
	{
		dDeleteSprite(pl_name);
		player[pl_id].hp = 0;
		if (!strcmp(dGetStaticSpriteImage(pl_name), "xiaoyaoImageMap"))
		{
			if (player[0].hp||player[1].hp) sendMessage("������ң��磡���Ҫ����ң��籨��");
			else
			{
				player[0].resusX = player[1].resusX = dGetSpritePositionX("player0");
				player[0].resusY = player[1].resusY = dGetSpritePositionY("player0");
				sendMessage("�������Ȫ·��������������");
			}
			//��Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/lxy_died.wav", NULL, SND_ASYNC);
		}
		else if (!strcmp(dGetStaticSpriteImage(pl_name), "lingerImageMap"))
		{
			if (player[0].hp||player[1].hp) sendMessage("������ɶ���ɱ�����ǣ�");
			else
			{
				player[0].resusX = player[1].resusX = dGetSpritePositionX("player1");
				player[0].resusY = player[1].resusY = dGetSpritePositionY("player1");
				sendMessage("��ң���...���...��...�ˡ�����");
			}
			//��Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/zle_died.wav", NULL, SND_ASYNC);
		}
	}
}

//�������Ӽ��������
void expAdd(short pl_id, short value)
{
	player[pl_id].exp+=value;

	//�������
	if (player[pl_id].exp>=20 + 20 * player[pl_id].lv)
	{
		//����ﵽ�ȼ�����
		if (player[pl_id].lv>=3)
		{
			player[pl_id].exp = 0;
			sendMessage("��������");
			return;
		}
		player[pl_id].lv++;
		player[pl_id].exp = 0;
		player[pl_id].hp = 8 + 2 * player[pl_id].lv;
		player[pl_id].mag =9 + 3 * player[pl_id].lv;

		//������������
		char *pl_name = dMakeSpriteName("player", pl_id);
		char messageStr[20];
		if (!strcmp(dGetStaticSpriteImage(pl_name), "xiaoyaoImageMap"))
			sprintf(messageStr, "����ң����%d����", player[pl_name[6]-48].lv);
		else if (!strcmp(dGetStaticSpriteImage(pl_name), "lingerImageMap"))
			sprintf(messageStr, "���������%d����", player[pl_name[6]-48].lv);
		sendMessage(messageStr);
		//��Ч
		dCloneSprite("levelUp", "ob_levelUp");
		dSetSpritePosition("ob_levelUp", dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name));
		dSetSpriteLifeTime("ob_levelUp", 1);
		//��Ч
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound("game/data/audio/level_up.wav", NULL, SND_ASYNC);
	}
}

///******************************��Ҷ���***************************************/

//������
void plPhyAct(short pl_id)
{
	if (!player[pl_id].hp) return;
	//����ͼ��Ϩ��
	dSetSpriteColorAlpha(dMakeSpriteName("phyIcon", pl_id), 100);

	//�����
	char *pl_name = dMakeSpriteName("player", pl_id);
	//ģ��
	char phy_name[7] = "attack";
	//ʵ��
	char *thisPhy = dMakeSpriteName("ob_attack", pl_id);
	//����ʵ��
	dCloneSprite(phy_name, thisPhy);
	//����
	float fPosX = (float)3 * player[pl_id].lastDirX+ dGetSpritePositionX(pl_name);
	float fPosY = (float)3 * player[pl_id].lastDirY + dGetSpritePositionY(pl_name);
	//����
	dSetSpritePosition(thisPhy, fPosX, fPosY);
	//����ʱ��
	dSetSpriteLifeTime(thisPhy, 0.25f);
	//��Ч
	PlaySound(NULL, NULL, SND_PURGE);
	PlaySound("game/data/audio/phy_attack.wav", NULL, SND_ASYNC);
}
//��������
void plMagAct(short pl_id)
{
	//��������ȴʱ�䲻������
	if (!player[pl_id].hp||player[pl_id].magRest<=1) return;
	//����ͼ��Ϩ��
	dSetSpriteColorAlpha(dMakeSpriteName("magIcon", pl_id), 100);

	//�����
	char *pl_name = dMakeSpriteName("player", pl_id);

	//��ң��ս
	if (!strcmp(dGetStaticSpriteImage(pl_name), "xiaoyaoImageMap"))
	{
		//������ȴ
		player[pl_id].magRest = 0;
		//����������
		if (player[pl_id].mag < 3)
		{
			sendMessage("�ͷ�ʧ�ܣ���������");
			return;
		}
		//����
		player[pl_id].mag-=3;

		//����ģ����
		char *mag_name = dMakeSpriteName("magic", 10 * player[pl_id].lv + pl_id);
		//����ʵ����
		char *thisMag = dMakeSpriteName("ob_magic", pl_id);
		//��������
		short power = dRandomRange(0, 3) + 3 * player[pl_id].lv;
		//��������
		addBox(mag_name, thisMag, power);

		//�����ͷŴ����꣬����*ʩ������+ʩ��������
		float fPosX = (float)5 * player[pl_id].lastDirX + dGetSpritePositionX(pl_name);
		float fPosY = (float)5 * player[pl_id].lastDirY + dGetSpritePositionY(pl_name);
		//�ͷż���
		dSetSpritePosition(thisMag, fPosX, fPosY);
		//��Ч
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound("game/data/audio/lxy_magic.wav", NULL, SND_ASYNC);
	}

	//���Զ��
	else if (!strcmp(dGetStaticSpriteImage(pl_name), "lingerImageMap"))
	{
		//������ȴ
		player[pl_id].magRest = 0;
		//����������
		if (player[pl_id].mag < 2)
		{
			sendMessage("�ͷ�ʧ�ܣ���������");
			return;
		}
		//����
		player[pl_id].mag-=2;


		//����ģ����
		char *mag_name = dMakeSpriteName("magic", 10 * player[pl_id].lv + pl_id);
		//����ʵ����
		char *thisMag = dMakeSpriteName("ob_magic", pl_id);
		//��������
		short power = dRandomRange(0, 2) + 2* player[pl_id].lv;
		//��������
		addBox(mag_name, thisMag, power);

		//�����ͷŴ����꣬����*ʩ������+ʩ��������
		float fPosX = (float)3 * player[pl_id].lastDirX + dGetSpritePositionX(pl_name);
		float fPosY = (float)3 * player[pl_id].lastDirY + dGetSpritePositionY(pl_name);
		//�ͷż���
		dSetSpritePosition(thisMag, fPosX, fPosY);

		/*����ת����ҳ���*/
		//����Ƕ�
		float fRot = dCalLineRotation( dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name), fPosX, fPosY );
		//������ת
		dSetSpriteRotation(thisMag, fRot);
		//���ܷ���
		dSetSpriteLinearVelocityPolar( thisMag, (float)(5 + 4 * player[pl_id].lv), fRot );
		//��Ч
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound("game/data/audio/zle_magic.wav", NULL, SND_ASYNC);
	}
}

//�ƶ�����������ֵ��״̬������1��̧��-1��
void plMove(const int iKey, short upOrDown)
{
	switch (iKey)
	{
	//��������ƶ�����
	case KEY_W:
		player[0].dirY-=upOrDown;break;
	case KEY_S:
		player[0].dirY+=upOrDown;break;
	case KEY_A:
		player[0].dirX-=upOrDown;break;
	case KEY_D:
		player[0].dirX+=upOrDown;break;
	case KEY_UP:
		player[1].dirY-=upOrDown;break;
	case KEY_DOWN:
		player[1].dirY+=upOrDown;break;
	case KEY_LEFT:
		player[1].dirX-=upOrDown;break;
	case KEY_RIGHT:
		player[1].dirX+=upOrDown;break;
	}
	//�ƶ����
	if (player[0].hp)
		dSetSpriteLinearVelocity("player0", (float)9 * player[0].dirX, (float)9 * player[0].dirY);
	if (player[1].hp)
		dSetSpriteLinearVelocity("player1", (float)9 * player[1].dirX, (float)9 * player[1].dirY);
}
