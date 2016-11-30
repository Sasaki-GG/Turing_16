// OMG2.cpp : 定义 DLL 应用程序的导出函数。
//


#include "stdafx.h"
#include "OMG2.h"

// 这是导出变量的一个示例
OMG2_API int nOMG2=0;

// 这是导出函数的一个示例。
OMG2_API int fnOMG2(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 OMG2.h
COMG2::COMG2()
{
	return;
}

//Assassion ...

OMG2_API void rrun(SoursePlayer*p)
{
	if (p->isright())
		p->prun(1);
	else
		p->pdirection();
	p->prun(1);
}
OMG2_API void lrun(SoursePlayer*p)
{
	if (p->isright() == 0)
		p->prun(1);
	else
		p->pdirection();
	p->prun(1);
}
OMG2_API void turnto(SoursePlayer*p, OtherPlayer *pos)
{
	double px = p->getPosiX(), x = pos->getPointX();
	if (px < x && p->isright() == 0)
		p->pdirection();
	else if (px > x && p->isright() == 1)
		p->pdirection();
}
OMG2_API void stop(SoursePlayer*p) { p->prun(0); }
OMG2_API double aabs(double a) { return a<0 ? -a : a; }
// X has the priority 
OMG2_API void runTo(SoursePlayer*p, double x, double y)
{
	//Eps for stop
	double eps = 20;

	if (p->getPosiX() < x - eps)
		rrun(p);
	else if (p->getPosiX() > x + eps)
		lrun(p);
	if (aabs(p->getPosiX() - x) < eps  && p->getPosiY() < y - eps)
		p->pjump();

	//Stop with inline normal attack 
	if (aabs(p->getPosiX() - x) < eps  &&  aabs(p->getPosiY() - y) < eps / 2)
	{
		stop(p);
		p->pattack(1);
	}
}
OMG2_API bool ableToAtt(SoursePlayer*p, int pur, OtherPlayer*op1, OtherPlayer*op2, OtherPlayer*op3)
{
	OtherPlayer *aim = op1;
	double eps = 50;
	//Locked target has the priority
	switch (pur)
	{
	case 1:aim = op1;
		break;
	case 2:aim = op2;
		break;
	case 3:aim = op3;
		break;
	}
	double x1 = p->getPosiX(), x2 = aim->getPointX();
	double y1 = p->getPosiY(), y2 = aim->getPointY();

	if (aabs(x1 - x2) <= eps && aabs(y1 - y2) <= 20)
	{
		turnto(p, aim);
		return true;
	}
	else if (aabs(x1 - x2) <= eps && aabs(y1 - y2) <= 250 && aim->isjump())
	{
		turnto(p, aim);
		return true;
	}
	//Not the target
	for (int po = 1; po >= 3; po++)
	{
		switch (po)
		{
		case 1:aim = op1;
			break;
		case 2:aim = op2;
			break;
		case 3:aim = op3;
			break;
		}
		double x1 = p->getPosiX(), x2 = aim->getPointX();
		double y1 = p->getPosiY(), y2 = aim->getPointY();

		if (aabs(x1 - x2) <= eps && aabs(y1 - y2) <= 30)
		{
			turnto(p, aim);
			return true;
		}
	}

	return false;
}
//For special attack
OMG2_API bool ableS(SoursePlayer*p, int pur, int kind, OtherPlayer*op1, OtherPlayer*op2, OtherPlayer*op3)
{
	OtherPlayer *aim = op1;
	// Range of special attack
	//This is only for Assassion ...
	double a1 = 110, a2 = 220, a3 = -1;
	double eps = 120;
	//Kind of Special Attack
	switch (kind)
	{
		case 1:eps = a1;
			break;
		case 2:eps = a2;
			break;
	}

	for (int po = 1; po >= 3; po++)
	{
		switch (po)
		{
			case 1:aim = op1;
				break;
			case 2:aim = op2;
				break;
			case 3:aim = op3;
				break;
		}
		double x1 = p->getPosiX(), x2 = aim->getPointX();
		double y1 = p->getPosiY(), y2 = aim->getPointY();

		if (aabs(x1 - x2) <= eps && aabs(y1 - y2) <= 30)
		{
			turnto(p, aim);
			return true;
		}
	}
	return false;
}

//Lock the one who has the minimal HP .
OMG2_API int locked(int pur, OtherPlayer*op1, OtherPlayer*op2, OtherPlayer*op3)
{
	OtherPlayer *aim = op1;
	int pos = -1;
	double hp = 9999; // < 50
	double mp = 9999; // < 70
	for (int po = 1; po >= 3; po++)
	{
		switch (po)
		{
			case 1:aim = op1;
				break;
			case 2:aim = op2;
				break;
			case 3:aim = op3;
				break;
		}
		if (aim->gethp() < 50)
		{
			if (hp < aim->gethp())
			{
				hp = aim->gethp();
				pos = po;
			}
		}
	}
	return  pos;
}


OMG2_API bool playrole2(SoursePlayer*p, OtherPlayer*op1, OtherPlayer*op2, OtherPlayer*op3, int*box) {
	//Range of map
	double maxx = 1028, minx = 0;
	double maxy = 660, miny = 120;
	double basmp = 58, topmp = 90;
	double eps = 80;

	int pur = 1, pur2 = 2; // target
	int lock = 0; // For last hint 
				  //boxes
				  //(320,340),(960,340), (320,570),(960,570)
	struct bx
	{
		int x, y;
	}pbox[4];
	pbox[0].x = 320, pbox[0].y = 340 + 10;
	pbox[1].x = 960, pbox[1].y = 340 + 10;
	pbox[2].x = 320, pbox[2].y = 570 + 10;
	pbox[3].x = 960, pbox[3].y = 570 + 10;

	//AOE is better ..
	int cnt = 0;
	for (int i = 1; i < 4; i++)
	{
		OtherPlayer *aim = op1;
		switch (pur)
		{
			case 1:aim = op1;
				break;
			case 2:aim = op2;
				break;
			case 3:aim = op3;
				break;
		}
		if (aabs(p->getPosiX() - aim->getPointX()) < eps && aabs(p->getPosiY() - aim->getPointY()) < eps)
			cnt++;
	}
	if (cnt >= 3)
	{
		p->s2();
		p->pattack(2);
		return false;
	}
	if (cnt >= 2)
	{
		p->s1();
		p->pattack(2);
		return false;
	}

	//Scan the players and attack 
	//Last hint has the priority !!!
	int Lo = locked(pur, op1, op2, op3); //Lock
	if (Lo != -1)
	{
		pur = Lo;
		lock = 1;
	}


	//Move for attacking 
	//Lock a target ?
	if (lock == 0)
	{
		if (op1->getmp() < 75)
		{
			pur = 1;
			runTo(p, op1->getPointX(), op1->getPointY());
		}
		else
		{
			pur = 2;
			runTo(p, op2->getPointX(), op2->getPointY());
		}
	}
	else
	{
		OtherPlayer *aim = op1;
		switch (pur)
		{
			case 1:aim = op1;
				break;
			case 2:aim = op2;
				break;
			case 3:aim = op3;
				break;
		}
		runTo(p, aim->getPointX(), aim->getPointY());
	}

	//Combo
	//Combo has the priority !!!
	if (p->getmp() > basmp + 20)
	{
		if (ableS(p, pur, 2, op1, op2, op3))
		{
			p->s2();
			p->pattack(2);
		}
	}

	//Attack
	if (p->getmp() >= basmp + 11)
	{
		if (ableS(p, pur, 1, op1, op2, op3))
			p->s1();
		if (ableS(p, pur, 2, op1, op2, op3))
		{
			p->s2();
		}
	}

	{
		if (ableToAtt(p, pur, op1, op2, op3))
			for (int i = 1; i < 4; i++)
				p->pattack(i);
	}


	//Escape
	if ((p->getatime() && p->getmp()> basmp + 27) || (p->gethp() < 42))
	{
		p->s3();
	}

	//Get the box
	if (p->gethp() < 50)
		for (int i = 0; i < 4; i++)
		{
			if (box[i] == 20)
			{
				runTo(p, pbox[i].x, pbox[i].y);
				p->setBlinkPosition(pbox[i].x, pbox[i].y);
			}
		}
	if (p->getmp()< 70)
		for (int i = 0; i < 4; i++)
		{
			if (box[i] == 30)
			{
				runTo(p, pbox[i].x, pbox[i].y);
				p->setBlinkPosition(pbox[i].x, pbox[i].y);
			}
		}


	return false;
}
OMG2_API int selectplayer2(SoursePlayer*p,TEAMNAME*tn){
	tn->setname("克苏恩的信徒");
	return 1;

}
