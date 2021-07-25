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
	int		agile;			// 敏捷度，涉及转速等
	int		vision;			// 视野
	int		firePower;		// 火力
	CommanderModel* commander;	// 指挥官

	// virtual float getFireDamage();
	// virtual float setFireDamage();
};


/*
铁甲舰
------------------------------------
| 前炮 | 桅杆 | 中炮 | 舰桥 | 后炮 |
------------------------------------
| 导向 | 动力 | 物资 | 燃料 | 鱼雷 |
------------------------------------
*/
class IroncladWarship: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int gun2[2]; int bridge[2];  int gun3[2];
	int navi[2]; int engine[2]; int material[2]; int fuel[2];  int torpedo[2];
};

/*
巡洋舰
----------------------------
| 前炮 | 桅杆 | 舰桥 | 后炮 |
-----------------------------------
| 导向 + 动力 | 物资 + 燃料 | 鱼雷 |
-----------------------------------
*/
class Cruiser : public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];  int gun2[2];
	int naviEngine[2]; int materialFuel[2];  int torpedo[2];
};

/*
炮舰
----------------------------
| 前炮 | 桅杆 | 舰桥 | 后炮 |
----------------------------
| 导向 + 动力 + 物资 + 燃料 |
----------------------------
*/
class Gunboat: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];  int gun3[2];
	int naviEngineMaterialFuel[2];
};

/*
鱼雷艇
---------------------
| 前炮 | 桅杆 | 舰桥 |
----------------------------------
| 导向 + 动力 + 物资 + 燃料 | 鱼雷 |
----------------------------------
*/
class TorpedoCruiser: public WarshipModel
{
public:
	int gun1[2]; int mast[2]; int bridge[2];
	int naviEngineMaterialFuel[2];  int torpedo[2];
};