#pragma once

#include <math.h>
#include "CommanderModel.h"
#include "Vector2D.h"

using namespace std;

class WarshipModel
{
public:
	Vector2D position;
	string	name;
	string	id;
	string	nation;
	int		type;
	int		speed;
	int		agile;			// ���ݶȣ��漰ת�ٵ�
	int		vision;			// ��Ұ
	int		firePower;		// ����
	CommanderModel* commander;	// ָ�ӹ�

	// virtual float getFireDamage();
	// virtual float setFireDamage();
};


/*
���׽�
------------------------------------
| ǰ�� | Φ�� | ���� | ���� | ���� |
------------------------------------
| ���� | ���� | ���� | ȼ�� | ���� |
------------------------------------
*/
class IroncladWarship: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int gun2[2]; int bridge[2];  int gun3[2];
	int navi[2]; int engine[2]; int material[2]; int fuel[2];  int torpedo[2];
};

/*
Ѳ��
----------------------------
| ǰ�� | Φ�� | ���� | ���� |
-----------------------------------
| ���� + ���� | ���� + ȼ�� | ���� |
-----------------------------------
*/
class Cruiser : public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];  int gun2[2];
	int naviEngine[2]; int materialFuel[2];  int torpedo[2];
};

/*
�ڽ�
----------------------------
| ǰ�� | Φ�� | ���� | ���� |
----------------------------
| ���� + ���� + ���� + ȼ�� |
----------------------------
*/
class Gunboat: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];  int gun3[2];
	int naviEngineMaterialFuel[2];
};

/*
����ͧ
---------------------
| ǰ�� | Φ�� | ���� |
----------------------------------
| ���� + ���� + ���� + ȼ�� | ���� |
----------------------------------
*/
class TorpedoCruiser: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];
	int naviEngineMaterialFuel[2];  int torpedo[2];
};