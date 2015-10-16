/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"

#include <Mmsystem.h>  
#pragma comment ( lib, "Winmm.lib" )  
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();



///******************************ȫ�ֱ���***************************************/

//�ؿ�
short chapter = 0;

//ʵ������
short ob_num = 0;

//��ͼ���ĵ�
float map_centerX = 44, map_centerY = -73.5f;

//�Ƿ�������Ϸ
bool isNew = true;

//��Ϸʱ��
float runTime = 0;

//����
short key = 0;
const short
K_LEI = 2,             //��
K_SHUI = 8,            //ˮ
K_FENG = 1,            //��
K_HUO = 4,             //��
K_TU = 16;             //��

///******************************���ܺ���***************************************/

//���ݻ�ԭ
void dataClear();
//���عؿ�
void loadChapter();

//���õ�ͼλ��
void setMap();

//����������߶�������¼��ҳ��򣬲����������ᣬ���ID
void playerWork(long axis, short pl_id);

//���¼�����ȴʱ��
void magRestChange(short pl_id);
//������ұ���ʱ��
void proTimeChange(short pl_id);

//�ƶ���ͼ
void moveMap();

//������Ϣ
void sendMessage(char* message);

//������루δ���㹴�ɣ�
float getDistance(const char *startName, const char *endName);

//Ѫ�����٣�������⣩
void hpLose(short pl_id, short hurt);
//�������ӣ�������⣩
void expAdd(short pl_id, short value);

///*******************************AI�趨****************************************/

//׷��
void enemyPursue(const char *enemy, short pl_id);

//����
void aiRun(bool isPlAlive[2]);

///******************************�������***************************************/

short getPlHp(short pl_id);
short getPlMag(short pl_id);
short getPlExp(short pl_id);
short getPlLv(short pl_id);
short getPlGold(short pl_id);
short getPlWeapon(short pl_id);

short getPlDirX(short pl_id);
short getPlDirY(short pl_id);

float getResusX(short pl_id);
float getResusY(short pl_id);

float getProTime(short pl_id);

void setPlHp(short pl_id, short value);
void setPlMag(short pl_id, short value);
void setPlExp(short pl_id, short value);
void setPlLv(short pl_id, short value);

void setResusPos(short pl_id, float resusX, float resusY);

void setProTime(short pl_id, short value);

///******************************��ҹ���***************************************/

bool addPlGold(short pl_id, short value);

///******************************��Ҷ���***************************************/

//������
void plPhyAct(short pl_id);
//��������
void plMagAct(short pl_id);
//�������ƣ���������ֵ��״̬������1��̧��-1��
void plMove(const int iKey, short upOrDown);

///******************************��ҹ���***************************************/

//ˢ������
void rebuildPl();

///********************************�̵�*****************************************/

//�����̵�
void openShop(short pl_id);
//�˳��̵�
void closeShop();
//ѡ����Ʒ
void chooseGood(short choose);
//ȷ����Ʒ
void confirmGood(bool confirm);

///********************************����*****************************************/

#define Box struct box
extern Box
{
	char *name;
	short value;
	Box *next;
} *obList;

//ȡ��
short *pickBox(const char *name);
//����
void cutBox(const char *name);
//����
void destroyList();

/*******************************************************************************/


//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( (getPlHp(0) > 0 || getPlHp(1) > 0) && chapter<=0 )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void GameInit()
{	
	//���ݻ�ԭ
	dataClear();

	//���عؿ�
	loadChapter();

	//�������
	dCloneSprite( "xiaoyao", "player0" );                       //����ʵ��
	dSetSpritePosition( "player0", getResusX(0), getResusY(0) );//���볡��
	dSetSpriteWorldLimitMode( "player0", WORLD_LIMIT_STICKY );  //�߽��޶�
	dSetSpriteRestitution( "player0", 0.1f );                   //��������

	dCloneSprite( "linger", "player1" );                        //����ʵ��
	dSetSpritePosition( "player1", getResusX(1), getResusY(1)); //���볡��
	dSetSpriteWorldLimitMode( "player1", WORLD_LIMIT_STICKY );  //�߽��޶�
	dSetSpriteRestitution( "player1", 0.1f );                   //��������

	//������ʾ
	sendMessage("�ռ�ɢ��������飬��ͨ���յ��·");

	//��������
	dSetSpriteVisible( "LEI", key & K_LEI );
	dSetSpriteVisible( "shui", key & K_SHUI );
	dSetSpriteVisible( "feng", key & K_FENG );
	dSetSpriteVisible( "huo", key & K_HUO );
	dSetSpriteVisible( "tu", key & K_TU );
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
	runTime+=fDeltaTime;

	//������
	static long axis = 0;
	//0.2s��֡����
	if (runTime > (double)axis/5)
	{
		axis+=1;

		/*0.2s*/
		//�������߶���֡ͼ
		playerWork(axis, 0);
		playerWork(axis, 1);
		//���¼�����ȴʱ��
		magRestChange(0);
		magRestChange(1);
		//������ұ���ʱ��
		proTimeChange(0);
		proTimeChange(1);

		/*1.0s*/
		if (!(axis%5))
		{
			bool isPlAlive[2] = {getPlHp(0)>0, getPlHp(1)>0};
			//AI����
			aiRun(isPlAlive);
		}
	}

	/*����UI*/
	char textStr[6];
	//����
	sprintf(textStr, "%d/%d", getPlHp(0), 8 + 2 * getPlLv(0));
	dSetTextString("hpText0", textStr);
	sprintf(textStr, "%d/%d", getPlHp(1), 8 + 2 * getPlLv(1));
	dSetTextString("hpText1", textStr);
	//����
	sprintf(textStr, "%d/%d", getPlMag(0), 9 + 3 * getPlLv(0));
	dSetTextString("magText0", textStr);
	sprintf(textStr, "%d/%d", getPlMag(1), 9 + 3 * getPlLv(1));
	dSetTextString("magText1", textStr);
	//����
	dDrawRect( 27, -14, 39, -13.6f, 1, 0, 255, 255, 255, 255 );
	dDrawLine( 27, -13.8f, 27 + 12 * (float)getPlExp(0) / (20 + 20 * getPlLv(0)), -13.8f, 8, 0, 255, 255, 255, 255 );
	dDrawRect( -39, -14, -27, -13.6f, 1, 0, 255, 255, 255, 255 );
	dDrawLine( -39, -13.8f, -39 + 12 * (float)getPlExp(1) / (20 + 20 * getPlLv(1)), -13.8f, 8, 0, 255, 255, 255, 255 );
	//�ȼ�
	dSetTextValue("lvText0", getPlLv(0));
	dSetTextValue("lvText1", getPlLv(1));
	//ͭ��
	dSetTextValue("goldText0", getPlGold(0));
	dSetTextValue("goldText1", getPlGold(1));

	//�ƶ���ͼ
	moveMap();
}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
	//ɾ�����
	if (getPlHp(0))
	{
		//��¼����λ��
		setResusPos(0, dGetSpritePositionX("player0"), dGetSpritePositionY("player0"));
		dDeleteSprite("player0");

		if (!getPlHp(1)) setResusPos(1, getResusX(0), getResusY(0));
	}
	if (getPlHp(1))
	{
		setResusPos(1, dGetSpritePositionX("player1"), dGetSpritePositionY("player1"));
		dDeleteSprite("player1");
		if (!getPlHp(0)) setResusPos(0, getResusX(1), getResusY(1));
	}

	//��������
	destroyList();

	//����ʱ��
	Sleep(1500);

	//���뿪ʼ����
	dLoadMap( "startPage.t2d" );

	//�ҷ�ȫ����ʾʧ����Ϣ
	if (getPlHp(0)<=0&&getPlHp(1)<=0) dSetSpriteVisible( "over", 1 );
	else
	{
		//�������
		dCursorOn();
		//���ſ�ʼ����bgm��Ⱥɽ�ɺ�
		PlaySound(NULL, NULL, SND_PURGE);
		//PlaySound("game/data/audio/bgm_qunshan.wav", NULL, SND_ASYNC|SND_LOOP);
	}
}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseMove( const float fMouseX, const float fMouseY )
{
	//����ƶ�
	dSetSpritePosition( "cursor",  fMouseX,  fMouseY );
	//�رհ�ť
	if (dIsPointInSprite( "close", fMouseX, fMouseY ))
		dSetStaticSpriteFrame( "close", 1 );
	else dSetStaticSpriteFrame( "close", 0 );
	//ȷ�ϰ�ť
	if (dIsPointInSprite( "buttonY", fMouseX, fMouseY ))
	{
		if (dGetStaticSpriteFrame( "buttonY" )==0)
		dSetStaticSpriteFrame( "buttonY", 1 );
	}
	else if (dGetStaticSpriteFrame( "buttonY" )==1) dSetStaticSpriteFrame( "buttonY", 0 );
	//ȡ����ť
	if (dIsPointInSprite( "buttonN", fMouseX, fMouseY ))
	{
		if (dGetStaticSpriteFrame( "buttonN" )==3)
		dSetStaticSpriteFrame( "buttonN", 4 );
	}
	else if (dGetStaticSpriteFrame( "buttonN" )==4) dSetStaticSpriteFrame( "buttonN", 3 );
}
//==========================================================================
//
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	if (dIsPointInSprite( "close", fMouseX, fMouseY ))
		closeShop();
	else if (dIsPointInSprite( "good1", fMouseX, fMouseY ))
		chooseGood(1);
	else if (dIsPointInSprite( "good2", fMouseX, fMouseY ))
		chooseGood(2);
	else if (dIsPointInSprite( "good8", fMouseX, fMouseY ))
		chooseGood(8);
	else if (dIsPointInSprite( "buttonY", fMouseX, fMouseY ))
		confirmGood(true);
	else if (dIsPointInSprite( "buttonN", fMouseX, fMouseY ))
		confirmGood(false);
}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
	switch (iKey)
	{
	//�ص���ʼ����
	case KEY_ESCAPE:
		if (g_iGameState)
		{
			g_iGameState = 0;
			GameEnd();
		}break;

	//����
	case KEY_R:
		plPhyAct(0);break;
	case KEY_T:
		plMagAct(0);break;
	case KEY_COMMA:
		plPhyAct(1);break;
	case KEY_PERIOD:
		plMagAct(1);break;

	//�ƶ�
	default:
		plMove(iKey, 1);break;
	}
}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{
	switch (iKey)
	{
	//����
	case KEY_R:
		//����ͼ�����
		dSetSpriteColorAlpha(dMakeSpriteName("phyIcon", 0), 255);
		break;
	case KEY_COMMA:
		//����ͼ�����
		dSetSpriteColorAlpha(dMakeSpriteName("phyIcon", 1), 255);
		break;

	//�ƶ�
	default:
		plMove(iKey, -1);break;
	}
}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	//�����˵ķ���
	if (strstr(szTarName, "Boss")&&strstr(szTarName, "9"))
	{
		if (strstr(szSrcName, "player"))
		{
			//ֹͣ�з�����ײ
			dSetSpriteCollisionReceive( szTarName, 0 );
			if (!getProTime(szSrcName[6]-48))
				hpLose(szSrcName[6]-48, szTarName[9] - 45);
		}
	}
	//�����˱���
	else if (strstr(szTarName, "enemy"))
	{
		short *pEnemy_hp = pickBox(szTarName);
		if (strstr(szSrcName, "player"))
		{
			if (!getProTime(szSrcName[6]-48))
				hpLose(szSrcName[6]-48,*pEnemy_hp%10 + *pEnemy_hp/10);
		}
		else if (strstr(szSrcName, "ob_attack"))
		{
			//ֹͣ���ܵ���ײ
			dSetSpriteCollisionSend( szSrcName, 0 );
			//�˺��ж�
			*pEnemy_hp-=getPlWeapon(szSrcName[9]-48)+ getPlLv(szSrcName[9]-48);
			//AI����
			enemyPursue(szTarName, szSrcName[9]-48);
			//��������
			expAdd(szSrcName[9]-48, 1);
		}
		else if (strstr(szSrcName, "ob_magic"))
		{
			//ֹͣ���ܵ���ײ
			dSetSpriteCollisionSend( szSrcName, 0 );
			//�˺��ж�
			*pEnemy_hp-=*pickBox(szSrcName);
			//AI����
			enemyPursue(szTarName, szSrcName[8]-48);
			//��������
			expAdd(szSrcName[8]-48, 3);
		}
		if (*pEnemy_hp <= 0)
		{
			//boss��ը
			if (strstr(szTarName, "Boss"))
				dPlayEffect("bossBurst", 1, dGetSpritePositionX(szTarName), dGetSpritePositionY(szTarName), 0);
			else if (strstr(szTarName, "enemy"))
			{
				//����ʵ��
				dCloneSprite("enemyBurst", "ob_enemyBurst");
				//���볡��
				dSetSpritePosition("ob_enemyburst", dGetSpritePositionX(szTarName), dGetSpritePositionY(szTarName));
				//����ʱ��
				dSetSpriteLifeTime("ob_enemyBurst", 1);
			}
			cutBox(szTarName);
		}
	}
	//����
	else if (strstr(szTarName, "box"))
	{
		if (strstr(szSrcName, "player"))
		{
			//ȡ�����ӵ���ײ
			dSetSpriteCollisionReceive( szTarName, 0 );
			dSetStaticSpriteFrame( szTarName, 12 + dGetStaticSpriteFrame( szTarName ) );
			switch ((int)szTarName[3] - 48)
			{
			case 1:
				{
					if (key & K_LEI)
					{
						sendMessage("��������Ѿ�����");
						break;
					}
					key|=K_LEI;
					dSetSpriteVisible( "lei", key & K_LEI );
					sendMessage("�õ���������");
				}break;
			case 3:
				{
					if (key & K_SHUI)
					{
						sendMessage("��������Ѿ�����");
						break;
					}
					key|=K_SHUI;
					dSetSpriteVisible( "shui", key & K_SHUI );
					sendMessage("�õ���ˮ����");
				}break;
			case 0:
				{
					if (key & K_FENG)
					{
						sendMessage("��������Ѿ�����");
						break;
					}
					key|=K_FENG;
					dSetSpriteVisible( "feng", key & K_FENG );
					sendMessage("�õ��˷�����");
				}break;
			case 2:
				{
					if (key & K_HUO)
					{
						sendMessage("��������Ѿ�����");
						break;
					}
					key|=K_HUO;
					dSetSpriteVisible( "huo", key & K_HUO );
					sendMessage("�õ��˻�����");
				}break;
			case 4:
				{
					if (key & K_TU)
					{
						sendMessage("��������Ѿ�����");
						break;
					}
					key|=K_TU;
					dSetSpriteVisible( "tu", key & K_TU );
					sendMessage("�õ���������");
				}break;
			case 5:
				{
					addPlGold(szSrcName[6] - 48, 100);
					sendMessage("��������100ͭ�壡");
				}
				break;
			case 6:
				{
					addPlGold(szSrcName[6] - 48, 300);
					sendMessage("��������300ͭ�壡");
				}break;
			default:
				break;
			}
			//��Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/get.wav", NULL, SND_ASYNC);
		}
	}
	//·��
	else if(strstr(szTarName, "wulinglun"))
	{
		if (strstr(szSrcName, "player"))
		{
			if (key < 31) sendMessage("���������֣���Ҫ���������������ͨ��");
			else
			{
				sendMessage("����ˮ�������������λ�������ֿ�");
				dDeleteSprite("wulinglun");
			}
		}
	}
	//�յ�
	else if (strstr(szTarName, "final"))
	{
		if (strstr(szSrcName, "player"))
		{
			sendMessage("��ϲ�������յ㣡");
			chapter++;
		}
	}
	//�̵�
	else if (strstr(szTarName, "shop"))
	{
		if (strstr(szSrcName, "player"))
		{
			short pl_id = szSrcName[6] - 48;
			openShop(pl_id);
		}
	}
	//�����
	else if (strstr(szTarName, "resus"))
	{
		if (strstr(szSrcName, "player"))
		{
			//�������
			short anotherPl = szSrcName[6] - 48 ? 0 : 1;
			if (!getPlHp(anotherPl))
			{
				char *pl_name = dMakeSpriteName("player", anotherPl);
				if (anotherPl)
				{
					sendMessage("�����̫���ˣ���������������ˣ�");
					dCloneSprite( "linger", pl_name );
				}
				else
				{
					sendMessage("��ң��磬������£�����Ϊ��Ҳ����������");
					dCloneSprite( "xiaoyao", pl_name );
				}
				dSetSpritePosition( pl_name, dGetSpritePositionX(szSrcName), dGetSpritePositionY(szSrcName) );
				dSetSpriteWorldLimitMode( pl_name, WORLD_LIMIT_STICKY );
				dSetSpriteRestitution( pl_name, 0.1f );
				setProTime(pl_name[6] - 48, 3);
				setPlHp(anotherPl, 4 + getPlLv(anotherPl));
				setPlMag(anotherPl, 5 + 2 * getPlLv(anotherPl));
			}
		}
	}
}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	if (!strcmp(szName, "player0")&&getPlHp(1)&&
		getDistance("player0", "player1") > 15)
		sendMessage("����������");
	else if (!strcmp(szName, "player1")&&getPlHp(0)&&
		getDistance("player0", "player1") > 15)
		sendMessage("��ң���������ҵ���");
}
