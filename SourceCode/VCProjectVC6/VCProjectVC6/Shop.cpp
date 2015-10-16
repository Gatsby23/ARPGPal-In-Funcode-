#include "CommonAPI.h"

///******************************���ܺ���***************************************/

//������Ϣ
void sendMessage(char* message);

///******************************�������***************************************/

short getPlGold(short pl_id);
short getPlWeapon(short pl_id);
void setPlWeapon(short pl_id, short value);

///******************************��ҹ���***************************************/

//��������
bool addPlHp(short pl_id, short value);
//��������
bool addPlMag(short pl_id, short value);
//ʧȥͭǮ
bool losePlGold(short pl_id, short value);

///********************************����*****************************************/

//���䣨����szSrcNameΪNULL��ʾ������ʵ����
void addBox(const char *szSrcName, const char *szMyName, short value);

///********************************�̵�*****************************************/

//�˿���Ϣ
static struct custom
{
	short id;         //������
	short choose;     //���ѡ��
}thisCustom;

//�����̵�
void openShop(short id)
{
	//����ʧ��
	if (dIsCursorOn())
	{
		sendMessage("�̵���æ�����Ե�");
		return;
	}
	
	//��¼�˿�
	thisCustom.id = id;
	//��ֹ����ƶ�
	dSetSpriteImmovable(dMakeSpriteName("player", id), 1);

	//�������
	dCursorOn();
	dSetSpriteVisible( "shopCard", 1 );
	dSetSpriteVisible( "cursor", 1 );
	sendMessage("��ӭ����С�꣬�͹���������۸񹫵���");
}
//�˳��̵�
void closeShop()
{
	//�������
	dCursorOff();
	dSetSpriteVisible( "shopCard", 0 );
	dSetSpriteVisible( "cursor", 0 );
	sendMessage("�͹��´���������");

	//��������ƶ�
	dSetSpriteImmovable(dMakeSpriteName("player", thisCustom.id), 0);
}
//ѡ����Ʒ
void chooseGood(short choose)
{
	dSetTextValue("priceText", 100 * choose);
	if (getPlGold(thisCustom.id) < 100 * choose)
	{
		thisCustom.choose = 0;
		sendMessage("�͹٣�����ͭ�岻����������");
		dSetStaticSpriteFrame( "buttonY", 2 );
		dSetStaticSpriteFrame( "buttonN", 5 );
		return;
	}
	switch (choose)
	{
	case 1:
		sendMessage("������֥�����¿ɻָ��������͹���һ֧��");
		break;
	case 2:
		sendMessage("�󻹵������¿ɻָ��������͹����򲻣�");
		break;
	case 8:
		sendMessage("�޳���������Ǻö������͹�Ҫ�Ļ��Ҹ�������˵�");
		break;
	}
	thisCustom.choose = choose;
	dSetStaticSpriteFrame( "buttonY", 0 );
	dSetStaticSpriteFrame( "buttonN", 3 );
}
//ȷ����Ʒ
void confirmGood(bool confirm)
{
	if (!thisCustom.choose) return;
	if (!confirm) sendMessage("�͹���ͱ���ԥ�ˣ����Ǻö�������");
	else
	{
		bool succeed = false;
		switch (thisCustom.choose)
		{
		case 1:
			{
				if (addPlMag(thisCustom.id, 6)) succeed = true;
				else sendMessage("�͹٣��������������ģ������ûЧ������");
			}break;
		case 2:
			{
				//���޼��
				if (addPlHp(thisCustom.id, 4)) succeed = true;
				else sendMessage("�͹٣��������������ģ������ûЧ������");
			}break;
		case 8:
			{
				if (getPlWeapon(thisCustom.id)) sendMessage("��ѽ�͹٣����Ѿ�����ѽ��˰���");
				else
				{
					setPlWeapon(thisCustom.id, 2);
					//��ӵ�ͼ��
					char *ob_choose = dMakeSpriteName("ob_good8", thisCustom.id);
					addBox("good8", ob_choose, 0);
					dSetSpritePosition(ob_choose, (float)(thisCustom.id ? -1 : 1) * 29, -8);
					succeed = true;
				}
			}break;
		}
		if (succeed)
		{
			sendMessage("���ϣ����Ķ������ú�");
			losePlGold(thisCustom.id, 100 * thisCustom.choose);
		}
	}
	thisCustom.choose = 0;
	dSetStaticSpriteFrame( "buttonY", 2 );
	dSetStaticSpriteFrame( "buttonN", 5 );
}