#include "CommonAPI.h"

///********************************����*****************************************/

#define Box struct box
#define NEW (Box*)malloc(sizeof(Box))
extern Box
{
	char *name;
	short value;
	Box *next;
} *obList;

//����
void creatList()
{
	//��������
	obList = NEW;
	obList->name = "listHead";
	obList->value = 0;
	obList->next = NEW;
	obList->next->name = NULL;
}

//���䣨����szSrcNameΪNULL��ʾ������ʵ����
void addBox(const char *szSrcName, const char *szMyName, short value)
{
	//����ṩ��ģ��
	if (szSrcName) dCloneSprite(szSrcName, szMyName);

	Box *p, *q;
	p = obList;
	while(p->name)
	{
		q = p->next;
		p = q;
	}
	p->name = new char[1 + strlen(szMyName)];
	strcpy(p->name, szMyName);
	p->value = value;
	p->next = NEW;
	p->next->name = NULL;
}

//ȡ��
short *pickBox(const char *name)
{
	if (!strcmp(obList->name, name)) return &obList->value;
	Box *p, *q;
	p = obList;
	q = p->next;
	while(q->name&&strcmp(q->name, name))
	{
		p = q;
		q = p->next;
	}
	if (!q->name) return NULL;
	return &q->value;
}

//����
void cutBox(const char *name)
{
	if (!strcmp(obList->name, name)) free(obList);
	Box *p, *q;
	p = obList;
	q = p->next;
	while(q->name&&strcmp(q->name, name))
	{
		p = q;
		q = p->next;
	}
	//����
	if (!q->name) return;
	p->next = q->next;
	dDeleteSprite( name );
	free(q);
}

//����
void destroyList()
{
	Box *p = obList;
	Box *q = p->next;
	while (p->name)
	{
		dDeleteSprite( p->name );
		free(p);
		p = q;
		q = p->next;
	}
	free(p);
}
