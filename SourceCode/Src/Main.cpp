//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"

#include <Mmsystem.h>  
#pragma comment ( lib, "Winmm.lib" )  
///////////////////////////////////////////////////////////////////////////////////////////
//
// ���������
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// ��ʼ����Ϸ����
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : �ڴ�ʹ��API���Ĵ��ڱ���
	dSetWindowTitle("ARPG�ɽ�");
	
	//ȫ��
	dResizeWindow( GetSystemMetrics( SM_CXFULLSCREEN ), GetSystemMetrics( SM_CYFULLSCREEN ) );
	//�������
	dShowCursor( 0 );
	//���ſ�ʼ����bgm��Ⱥɽ�ɺ�
	PlaySound(NULL, NULL, SND_PURGE);
	//PlaySound("game/data/audio/bgm_qunshan.wav", NULL, SND_ASYNC|SND_LOOP);


	// ������ѭ����������Ļͼ��ˢ�µȹ���
	while( dEngineMainLoop() )
	{
		// ��ȡ���ε���֮���ʱ�����ݸ���Ϸ�߼�����
		float	fTimeDelta	=	dGetTimeDelta();

		// ִ����Ϸ��ѭ��
		GameMainLoop( fTimeDelta );
	};

	// �ر���Ϸ����
	dShutdownGameEngine();
	return 0;
}

//
extern int g_iGameState;

//==========================================================================
//
// ���沶׽����ƶ���Ϣ�󣬽����õ�������
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	//��Ϸδ��ʼǰ������LessonX�㣬��ͬ
	if (!g_iGameState)
	{
		//����ƶ�
		dSetSpritePosition( "cursor",  fMouseX,  fMouseY );
		//����ת��
		float fRot = 90 + dCalLineRotation( dGetSpritePositionX( "butterFly"),
			dGetSpritePositionY( "butterFly"), fMouseX,  fMouseY);
		dSetSpriteRotation( "butterFly", fRot );
		//׷����
		dSpriteMoveTo( "butterFly", fMouseX, fMouseY, 20, 1 );

		//��һ�����λ��״̬
		static short lastMouseState = 0;

		if (dIsPointInSprite( "startNew", fMouseX, fMouseY ))
		{
			//���״̬δ�ı䣬����
			if (lastMouseState==1) return;
			//״̬����
			lastMouseState = 1;
			//ѡ��ı�
			dSetStaticSpriteFrame( "startNew", 1 );
			dSetStaticSpriteFrame( "startOld", 2 );
			//������Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/BLIP.wav", NULL, SND_ASYNC);
		}
		else if (dIsPointInSprite( "startOld", fMouseX, fMouseY ))
		{
			if(lastMouseState==2) return;
			lastMouseState = 2;
			dSetStaticSpriteFrame( "startOld", 3 );
			dSetStaticSpriteFrame( "startNew", 0 );
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/BLIP.wav", NULL, SND_ASYNC);
		}
		else
		{
			if(!lastMouseState) return;
			lastMouseState = 0;
			dSetStaticSpriteFrame( "startNew", 0 );
			dSetStaticSpriteFrame( "startOld", 2 );
		}
		//���أ�������LessonX
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// ���沶׽�������Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//

extern bool isNew;
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	if (!g_iGameState)
	{
		//�����ʼ����Ϸ״̬Ϊ1��isNew = true
		if (dIsPointInSprite( "startNew", fMouseX, fMouseY ))
		{
			isNew = true;
			g_iGameState = 1;
			//������Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/whoosh_mono.wav", NULL, SND_ASYNC);
		}
		//�����������Ϸ״̬Ϊ1
		else if (dIsPointInSprite( "startOld", fMouseX, fMouseY ))
		{
			if (isNew) return;
			g_iGameState = 1;
			//������Ч
			PlaySound(NULL, NULL, SND_PURGE);
			PlaySound("game/data/audio/whoosh_mono.wav", NULL, SND_ASYNC);
		}
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseClick(iMouseType, fMouseX, fMouseY);
}
//==========================================================================
//
// ���沶׽��굯����Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	if (!g_iGameState)
	{
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽���̰�����Ϣ�󣬽����õ�������
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
	if (!g_iGameState)
	{
		if (dIsCursorOn()||iKey!=KEY_SPACE) return;
		//�������
		dCursorOn();
		//����ʧ����Ϣ
		dSetSpriteVisible( "over", 0 );
		//���ſ�ʼ����bgm��Ⱥɽ�ɺ�
		PlaySound(NULL, NULL, SND_PURGE);
		//PlaySound("game/data/audio/bgm_qunshan.wav", NULL, SND_ASYNC|SND_LOOP);
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}
//==========================================================================
//
// ���沶׽���̵�����Ϣ�󣬽����õ�������
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
//
void dOnKeyUp( const int iKey )
{
	if (!g_iGameState)
	{
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyUp(iKey);
}

//===========================================================================
//
// ���沶׽�������뾫����ײ֮�󣬵��ô˺���
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��鷢�ͼ�������ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	if (!g_iGameState)
	{
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// ���沶׽������������߽���ײ֮�󣬵��ô˺���.
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��������߽�����
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	
	if (!g_iGameState)
	{
		return;
	}

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColWorldLimit(szName, iColSide);
}

