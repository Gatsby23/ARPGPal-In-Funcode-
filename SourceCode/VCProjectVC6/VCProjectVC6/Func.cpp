#include "CommonAPI.h"

#include <time.h>
#include <Mmsystem.h>  
#pragma comment ( lib, "Winmm.lib" )  
///******************************ȫ�ֱ���***************************************/

//�ؿ�
extern short chapter;

//ʵ������
extern short ob_num;

//��ͼ���ĵ�
extern float map_centerX, map_centerY;

//�Ƿ�������Ϸ
extern bool isNew;

//����
extern short key;

///******************************�������***************************************/

short getPlHp(short pl_id);
short getPlLv(short pl_id);

short getPlDirX(short pl_id);
short getPlDirY(short pl_id);

void setPlLv(short pl_id, short value);

void setResusPos(short pl_id, float resusX, float resusY);

//ˢ������
void rebuildPl();

///********************************����*****************************************/

#define Box struct box
#define NEW (Box*)malloc(sizeof(Box))
Box
{
	char *name;
	short value;
	Box *next;
} *obList;

//����
void creatList();
//���䣨����szSrcNameΪNULL��ʾ������ʵ����
void addBox(const char *szSrcName, const char *szMyName, short value);

///******************************���ܺ���***************************************/

//���ݻ�ԭ
void dataClear()
{
	if (isNew)
	{
		//��һ��
		chapter = 0;

		isNew = false;
		//�ȼ���ԭ
		setPlLv(0, 1);
		setPlLv(1, 1);

		//��ͼλ�û�ԭ
		map_centerX = 44, map_centerY = -73.5f;

		//��Ҹ������껹ԭ
		setResusPos(0, -10, 10); setResusPos(1, -15, 10);

		//���黹ԭ
		key = 0;
	}

	/*ȫ�ֱ�����ԭ*/

	//ˢ���������
	rebuildPl();

	//ʵ��������
	ob_num = 0;

	//�����������
	srand(time(NULL));

	//�ؿ����޼��
	if (chapter > 0) chapter = 0;
}

//���õ�ͼ
void setMap()
{
	dSetSpritePosition( "map", map_centerX, map_centerY);
	float map_moveX = map_centerX - 44;
	float map_moveY = map_centerY + 73.5f;
	if (map_moveX||map_moveY)
	{
		Box *p = obList;
		Box *q = p->next;
		while (p->name)
		{
			if (!dGetSpriteIsMounted( p->name )&&!strstr(p->name, "good"))
				dSetSpritePosition(p->name, map_moveX + dGetSpritePositionX(p->name),
				map_moveY + dGetSpritePositionY(p->name));
			p = q;
			q = p->next;
		}
	}
}

//���عؿ�
void loadChapter()
{
	//�������
	dCursorOff();

	//������Ϸ����
	char chapterName[] = "chapter0.t2d";
	chapterName[7] = (char)(chapter + 48);
	dLoadMap( chapterName );

	//������Ϸ����bgm������
	PlaySound(NULL, NULL, SND_PURGE);
	//PlaySound("game/data/audio/bgm_dielian.wav", NULL, SND_ASYNC|SND_LOOP);

	//�½�����
	creatList();

	//*������������
	short netral_race, netral_indi;                      //���塢����
	for (netral_race = 1; netral_race < 7; netral_race++)
	{
		for (netral_indi = 1; netral_indi < 6; netral_indi++)
		{
			char *netral_name = dMakeSpriteName("netral", 10 * netral_race + netral_indi);
			addBox(NULL, netral_name, netral_race);      //�������ã�race����hp
		}
	}
	//*�����������
	short enemy_race, enemy_indi;                        //���塢����
	for (enemy_race = 1; enemy_race < 10; enemy_race++)
	{
		for (enemy_indi = 1; enemy_indi < 11 - enemy_race; enemy_indi++)
		{
			char *enemy_name = dMakeSpriteName("enemy", 10 * enemy_race + enemy_indi);
			addBox(NULL, enemy_name, 1 + enemy_race);        //�������ã�race����hp
			//���÷�������
			dSetSpriteRestitution( enemy_name, 0.3f );
		}
	}
	//*����Boss����
	for (enemy_race = 0; enemy_race < 5; enemy_race++)
	{
		char *enemy_name = dMakeSpriteName("enemyBoss", enemy_race);
		addBox(NULL, enemy_name, 23 + 10 * enemy_race);   //�������ã�race����hp
		//�����˶���Boss��������
		if (!dGetSpriteIsMounted( enemy_name )) dSetSpriteRestitution( enemy_name, 0.3f );
	}
	
	//���õ�ͼλ��
	setMap();
}

//�ƶ���ͼ
void moveMap()
{
	//��¼��ͼ��һ֡�˶�״̬��true:Vx!=0, false:Vx==0
	static bool lastMapVxState = false;

	//���֮ǰ��ͼ����
	if (!lastMapVxState)
	{
		//��ҵ��˶����ƣ�����+����
		float trend0, trend1;
		trend0 = 20 * getPlDirX(0) + dGetSpritePositionX("player0");
		trend1 = 20 * getPlDirX(1) + dGetSpritePositionX("player1");
		if (!getPlHp(0)) trend0 = trend1;
		if (!getPlHp(1)) trend1 = trend0;
		//��ͼ�ƶ���ȡ���˶�����
		if (dGetSpriteLinearVelocityX("map")||dGetSpriteLinearVelocityY("map"))
			trend0 = trend1 = 0;

		//�˶����ƣ�����Խ����ͷ
		if (trend0 > 40 && trend1 > 40 && map_centerX> -88)
		{
			//��ͼ��������䶯
			map_centerX-=22;
			//�ƶ���ͼ���µ�����
			dSpriteMoveTo("map", map_centerX, map_centerY, 30, 1);
		}
		//�˶����ƣ�����Խ����ͷ
		else if (trend0 < -40 && trend1 < -40 && map_centerX < 88)
		{
			map_centerX+=22;
			dSpriteMoveTo("map", map_centerX, map_centerY, 30, 1);
		}
	}

	/*y�᷽������һ�δ���ͬ��*/
	if (!lastMapVxState)
	{
		float trend0, trend1;
		trend0 = 10 * getPlDirY(0) + dGetSpritePositionY("player0");
		trend1 = 10 * getPlDirY(1) + dGetSpritePositionY("player1");
		if (!getPlHp(0)) trend0 = trend1;
		if (!getPlHp(1)) trend1 = trend0;
		if (dGetSpriteLinearVelocityX("map")||dGetSpriteLinearVelocityY("map"))
			trend0 = trend1 = 0;

		if (trend0 < -25 && trend1 < -25 && map_centerY < 73.5f)
		{
			map_centerY+=14.7f;
			dSpriteMoveTo("map", map_centerX, map_centerY, 30, 1);
		}
		else if (trend0 > 25 && trend1 > 25 && map_centerY > -73.5f)
		{
			map_centerY-=14.7f;
			dSpriteMoveTo("map", map_centerX, map_centerY, 30, 1);
		}
	}

	/*���ͼͬʱ�ƶ�*/
	//��ͼ�˶��ٶ�
	float mapSpeedX = dGetSpriteLinearVelocityX("map");
	float mapSpeedY = dGetSpriteLinearVelocityY("map");
	//���֮ǰ��ͼ�˶�
	if (lastMapVxState)
	{
		//��Ҹ����ͼ
		if (getPlHp(0)) dSetSpriteLinearVelocity("player0", mapSpeedX, mapSpeedY);
		if (getPlHp(1)) dSetSpriteLinearVelocity("player1", mapSpeedX, mapSpeedY);

		Box *p = obList;
		Box *q = p->next;
		while (p->name)
		{
			if (!dGetSpriteIsMounted( p->name )&&!(strstr(p->name, "good")))
			{
				dSetSpriteLinearVelocity(p->name, mapSpeedX, mapSpeedY);
			}
			p = q;
			q = p->next;
		}
	}
	//д���ͼ��һ֡�˶����
	lastMapVxState = (mapSpeedX||mapSpeedY);
}

//������Ϣ
void sendMessage(char* message)
{
	//���ͼ��
	extern float runTime;
	static float lastCallTime = 0;
	if (runTime - lastCallTime < 0.8f) return;
	lastCallTime = runTime;

	//����ʵ��
	dCloneSprite("textBox", "ob_textBox");
	dCloneSprite("messageText", "ob_messageText");
	//���볡��
	dSetSpritePosition("ob_textBox", 0, 0);
	dSetSpritePosition("ob_messageText", 0, 0);
	//��ʾ����
	dSetTextString("ob_messageText", message);
	//��������
	dSetSpriteLinearVelocityY("ob_textBox", -5);
	dSetSpriteLinearVelocityY("ob_messageText", -5);
	//1s����ʧ
	dSetSpriteLifeTime("ob_textBox", 3);
	dSetSpriteLifeTime("ob_messageText", 3);
}

//������루δ���㹴�ɣ�
float getDistance(const char *startName, const char *endName)
{
	float startX = dGetSpritePositionX(startName);
	float startY = dGetSpritePositionY(startName);
	float endX = dGetSpritePositionX(endName);
	float endY = dGetSpritePositionY(endName);

	if (startX > endX) startX-=endX;
	else startX = endX-startX;
	if (startY > endY) startY-=endY;
	else startY = endY-startY;
	return startX + startY;
}