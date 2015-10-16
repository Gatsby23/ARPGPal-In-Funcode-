#include "CommonAPI.h"

#include <Mmsystem.h>  
#pragma comment ( lib, "Winmm.lib" )  
///******************************���ܺ���***************************************/

//������루δ���㹴�ɣ�
float getDistance(const char *startName, const char *endName);

///********************************����*****************************************/

#define Box struct box
extern Box
{
	char *name;
	short value;
	Box *next;
} *obList;

///*******************************AI�趨****************************************/

//����
void randMove(const char *name)
{
	//����Ǿ�ֹ�ͣ������˶�����
	if (dGetSpriteIsMounted( name )) return;
	//����ƶ�
	short ranDirX = dRandomRange( -1, 1 );
	short ranDirY = dRandomRange( -1, 1 );
	dSetSpriteLinearVelocity(name, (float)3 * ranDirX, (float)3 * ranDirY);

	//��������
	ranDirX+=3 * ranDirY;                    //ranDirY�������ã����������Ϣ
	if (ranDirX==-1||ranDirX==2) ranDirY = 0;
	else if (ranDirX <-2) ranDirY = 1;
	else if (ranDirX > 2) ranDirY = 2;
	else ranDirY = 3;dMakeSpriteName(name, 3);
	char *ani_name;
	//����Boss������
	if (strstr(name, "Boss")) ani_name = dMakeSpriteName(name, ranDirY);
	//����һ����˶�����
	else if(strstr(name, "enemy"))
	{
		ani_name = new char[8];
		strcpy(ani_name, name);
		ani_name[6] = (char)48;
		ani_name = dMakeSpriteName(ani_name, ranDirY);
	}
	//�����������ﶯ��
	else if(strstr(name, "netral"))
	{
		ani_name = new char[9];
		strcpy(ani_name, name);
		ani_name[7] = (char)48;
		ani_name = dMakeSpriteName(ani_name, ranDirY);
	}
	dAnimateSpritePlayAnimation( name, ani_name, 0);
}
//ʩ��
void enemyFire(const char *enemy, short pl_id)
{
	//����������
	static short fireAxis = 0;
	//��Boss�ȼ��ݼ�����
	if (fireAxis%(54 - enemy[9]))
	{
		//����ʩ�������ž���
		if (!(++fireAxis%(54 - enemy[9])))
			dPlayEffect("warning", 1.5f, dGetSpritePositionX(enemy), dGetSpritePositionY(enemy), 0);
		return;
	}
	fireAxis++;
	//����ģ����
	char *fire_name = dMakeSpriteName(enemy, 9);
	//����ʵ����
	char *thisFire = dMakeSpriteName(fire_name, 0);
	//����ʵ��
	dCloneSprite(fire_name, thisFire);

	//�����ͷŴ����꣬����*ʩ������+ʩ��������
	float fPosX = dGetSpritePositionX(enemy);
	float fPosY = dGetSpritePositionY(enemy);
	//�ͷż���
	dSetSpritePosition(thisFire, fPosX, fPosY);

	/*����ת����ҳ���*/
	//����Ƕ�
	char *pl_name = dMakeSpriteName("player", pl_id);
	float fRot = dCalLineRotation(
		dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name), fPosX, fPosY );
	//������ת
	dSetSpriteRotation(thisFire, fRot);
	//׷�����
	dSpriteMoveTo(thisFire, dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name), 20, 1);
	//��Ч
	PlaySound(NULL, NULL, SND_PURGE);
	PlaySound("game/data/audio/enemy_fire.wav", NULL, SND_ASYNC);
	//����ʱ��
	dSetSpriteLifeTime(thisFire, 1);
}
//׷��
void enemyPursue(const char *enemy, short pl_id)
{
	//���
	char *pl_name = dMakeSpriteName("player", pl_id);
	//׷���ٶ�
	float enemy_speed = 0;

	//��������
	short disX = (int)(dGetSpritePositionX(pl_name) - dGetSpritePositionX(enemy));
	short disY = (int)(dGetSpritePositionY(pl_name) - dGetSpritePositionY(enemy));
	disX = disX > 0 ? 2 : 0;             //disX��disY�������ã�
	disY = disY > 0 ? 0 : 1;             //disX + disY�洢������Ϣ
	char *ani_name;
	if (strstr(enemy, "Boss"))
	{
		//����Boss������
		ani_name = dMakeSpriteName(enemy, disX + disY);
		//Boss�ٶ�
		enemy_speed = 10;
	}
	else
	{
		//����һ����˶�����
		ani_name = new char[8];
		strcpy(ani_name, enemy);
		ani_name[6] = (char)48;
		ani_name = dMakeSpriteName(ani_name, disX + disY);
		//һ������ٶ�
		enemy_speed = 8;
	}
	dAnimateSpritePlayAnimation( enemy, ani_name, 0 );
	//׷�����
	dSpriteMoveTo(enemy, dGetSpritePositionX(pl_name), dGetSpritePositionY(pl_name), enemy_speed, 1);
}
//����
void enemyAttack(const char *enemy, short pl_id)
{
	//�����Boss
	if (strstr(enemy, "Boss"))
	{
		enemyFire(enemy, pl_id);
		//����Ǿ�ֹ�ͣ������˶�����
		if (dGetSpriteIsMounted( enemy )) return;
	}
	enemyPursue(enemy, pl_id);
}
//�س�
void bossBack(const char *boss)
{
	char *box = dMakeSpriteName("box", boss[9] - 48);
	dSpriteMoveTo(boss, dGetSpritePositionX(box), dGetSpritePositionY(box), 10, 1);

	//��������
	short disX = (int)(dGetSpritePositionX(box) - dGetSpritePositionX(boss));
	short disY = (int)(dGetSpritePositionY(box) - dGetSpritePositionY(boss));
	disX = disX > 0 ? 2 : 0;             //disX��disY�������ã�
	disY = disY > 0 ? 0 : 1;             //disX + disY�洢������Ϣ
	char *ani_name;
	//����Boss������
	ani_name = dMakeSpriteName(boss, disX + disY);
	dAnimateSpritePlayAnimation( boss, ani_name, 0 );
}
//��Ѱ���
void searchPl(const char *name, bool isPlAlive[2])
{
	//����Ҿ���
	float toPlDis[2];
	if (isPlAlive[0]) toPlDis[0] = getDistance("player0", name);
	else toPlDis[0] = 100;
	if (isPlAlive[1]) toPlDis[1] = getDistance("player1", name);
	else toPlDis[1] = 100;
	
	//������
	short pl_id = toPlDis[0] < toPlDis[1] ? 0 : 1;

	/*��Ӧ*/
	//����ҽ�Զ
	if (toPlDis[pl_id] > 35)
	{
		//boss�س��ر�
		if (strstr(name, "Boss")&&getDistance(dMakeSpriteName("box", name[9]), name) > 25)
			bossBack(name);
		else
		{
			//��Ұ����ͣ
			if (toPlDis[pl_id] > 125) dSetSpriteLinearVelocity( name, 0, 0 );
			//��Ұ�ڻ���
			else randMove(name);
		}
	}
	//���˿������
	else
	{
		//���˽���
		if (strstr(name, "netral")) randMove(name);
		//������������
		else if (strstr(name, "enemy")) enemyAttack(name, pl_id);
	}
}

//AI����
void aiRun(bool isPlAlive[2])
{
	Box *p = obList;
	Box *q = p->next;
	while (p->name)
	{
		if ( strstr(p->name, "enemy")||strstr(p->name, "netral") ) searchPl(p->name, isPlAlive);
		p = q;
		q = p->next;
	}
}