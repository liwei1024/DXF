#include "stdafx.h"
#include "BeastQuest.h"
#include "RemoteCall.h"
#include "Functions.h"
#include "GetDirection.h"
#include "KeyCode.h"

BeastQuest::BeastQuest()
{
	int status = pApi.readInteger(游戏状态);
	if (status != 3) {
		printf("不在地图内\n");
		return;
	}
}

BeastQuest::~BeastQuest()
{
}

bool BeastQuest::地图遍历(遍历结构 & 结构)
{
	结构.人物指针 = pApi.readInteger(人物基址);
	结构.地图地址 = pApi.readInteger(结构.人物指针 + 地图偏移);

	if (结构.地图地址) {
		结构.首地址 = pApi.readInteger(结构.地图地址 + 地图首地址偏移);
		结构.尾地址 = pApi.readInteger(结构.地图地址 + 地图尾地址偏移);
		结构.对象数量 = (结构.尾地址 - 结构.首地址) / 4;
		return true;
	}
	else {
		return false;
	}
}

bool BeastQuest::超级入包()
{
	遍历结构 结构;
	bool res = 地图遍历(结构);
	if (!res) {
		printf("遍历地图失败");
		return true;
	}
	for (int i = 0; i < 结构.对象数量; i++)
	{
		结构.对象地址 = pApi.readInteger(结构.首地址 + 4 * i);
		if (结构.对象地址) {
			结构.对象类型 = pApi.readInteger(结构.对象地址 + 类型偏移);
			结构.对象阵营 = pApi.readInteger(结构.对象地址 + 阵营偏移);
			if (结构.对象类型 == 289 && 结构.对象阵营 == 200) {
				Coordinate 坐标 = 获取坐标(结构.人物指针);
				Send_组包拾取(Decode(结构.对象地址 + 160), 坐标.x, 坐标.y);
				return false;
			}
		}
	}
	return true;
}

void BeastQuest::评分()
{
	try {
		int 评分地址;
		评分地址 = pApi.readInteger(评分基址) + 评分偏移;
		if (评分地址) {
			if (Decode(评分地址) < 102400) {
				Encode(评分地址, 102400);
			}
		}
		else {
			printf("评分地址 ERROR");
		}
	}
	catch (...) {
		printf("评分 ERROR");
	}

}

void BeastQuest::人物跟随()
{
	遍历结构 结构; Coordinate 怪物坐标, 人物坐标;
	bool res = 地图遍历(结构);
	if (!res) {
		printf("遍历地图失败");
		return;
	}
	for (int i = 0; i < 结构.对象数量; i++)
	{
		结构.对象地址 = pApi.readInteger(结构.首地址 + 4 * i);
		if (结构.对象地址 && 结构.对象地址 != 结构.人物指针) {
			结构.对象类型 = pApi.readInteger(结构.对象地址 + 类型偏移);
			结构.对象阵营 = pApi.readInteger(结构.对象地址 + 阵营偏移);
			if ((结构.对象类型 == 273 || 结构.对象类型 == 529)) {
				结构.对象血量 = pApi.readInteger(结构.对象地址 + 血量偏移);
				if (!(结构.对象血量 > 0))return;
				if (!(结构.对象阵营 == 100 || 结构.对象阵营 == 101 || 结构.对象阵营 == 110 || 结构.对象阵营 == 50 || 结构.对象阵营 == 120))return;
				怪物坐标 = 获取坐标(结构.对象地址);
				人物坐标 = 获取坐标(结构.人物指针);
				if (abs(人物坐标.x - 怪物坐标.x) > 100 && abs(人物坐标.y - 怪物坐标.y) > 100) {
					Send_坐标CALL(结构.人物指针, 怪物坐标.x, 怪物坐标.y, 0);
				}
				return;
			}
		}
	}

}

void BeastQuest::坐标过图(int 方向ID)
{

	try
	{
		DWORD 人物指针 = pApi.readInteger(人物基址);
		DWORD 临时偏移 = pApi.readInteger(人物指针 + 188);
		if (临时偏移 == 0)
		{
			return;
		}
		DWORD 临时数据 = pApi.readInteger(商店基址 - 8);
		临时数据 = pApi.readInteger(临时数据 + 时间基址);
		临时数据 = pApi.readInteger(临时数据 + 坐标顺图偏移2);
		int 坐标结构 = 临时数据 + (方向ID + 方向ID * 8) * 4 + 坐标顺图偏移1 + 方向ID * 4;
		int x, y, xF, yF, cx, cy;
		x = pApi.readInteger(坐标结构 + 0);
		y = pApi.readInteger(坐标结构 + 4);
		xF = pApi.readInteger(坐标结构 + 8);
		yF = pApi.readInteger(坐标结构 + 12);
		if (方向ID == 0)
		{
			cx = x + xF + 20;
			cy = y + yF / 2;
		}
		if (方向ID == 1)
		{
			cx = x - 20;
			cy = y + yF / 2;
		}
		if (方向ID == 2)
		{
			cx = x + xF / 2;
			cy = y + yF + 20;
		}
		if (方向ID == 3)
		{
			cx = x + xF / 2;
			cy = y - 20;
		}
		Send_坐标CALL(人物指针, cx, cy, 0);
		Sleep(200);
		if (方向ID == 1) {
			Send_坐标CALL(人物指针, x + xF / 2 + 70, y, 0);
		}
		else {
			Send_坐标CALL(人物指针, x + xF / 2, y, 0);
		}
		
	}
	catch (...)
	{
		//基址_->公告(L"顺图CALL崩溃了！");
		printf("顺图CALL崩溃了！");
	}
}

void BeastQuest::智能过图()
{
	if (是否在BOSS房() == true)
	{
		return;
	}
	GetDirection GD;
	地图数据 Map_data = GD.Get_data_Map(0);
	int 方向 = GD.Calculation_direction(Map_data.地图走法[0], Map_data.地图走法[1]);
	this->坐标过图(方向);
}

void BeastQuest::全屏技能()
{
	遍历结构 结构;
	bool res = 地图遍历(结构);
	if (!res) {
		printf("遍历地图失败");
		return;
	}
	for (int i = 0; i < 结构.对象数量; i++)
	{
		结构.对象地址 = pApi.readInteger(结构.首地址 + 4 * i);
		if (结构.对象地址 && 结构.对象地址 != 结构.人物指针) {
			结构.对象类型 = pApi.readInteger(结构.对象地址 + 类型偏移);
			结构.对象阵营 = pApi.readInteger(结构.对象地址 + 阵营偏移);
			if ((结构.对象类型 == 273 || 结构.对象类型 == 529)) {
				结构.对象血量 = pApi.readInteger(结构.对象地址 + 血量偏移);
				if (!(结构.对象血量 > 0))return;
				if (!(结构.对象阵营 == 100 || 结构.对象阵营 == 101 || 结构.对象阵营 == 110 || 结构.对象阵营 == 50 || 结构.对象阵营 == 120))return;
				Coordinate 怪物坐标 = 获取坐标(结构.对象地址);
				Send_技能CALL(结构.人物指针, 26484, 113, 怪物坐标.x, 怪物坐标.y, 怪物坐标.z, 0);
				return;
			}
		}
	}
}

bool BeastQuest::是否有怪物()
{
	遍历结构 结构;
	bool res = 地图遍历(结构);
	if (!res) {
		printf("遍历地图失败");
		return false;
	}
	for (int i = 0; i < 结构.对象数量; i++)
	{
		结构.对象地址 = pApi.readInteger(结构.首地址 + 4 * i);
		if (结构.对象地址 && 结构.对象地址 != 结构.人物指针) {
			结构.对象类型 = pApi.readInteger(结构.对象地址 + 类型偏移);
			结构.对象阵营 = pApi.readInteger(结构.对象地址 + 阵营偏移);
			if ((结构.对象类型 == 273 || 结构.对象类型 == 529)) {
				结构.对象血量 = pApi.readInteger(结构.对象地址 + 血量偏移);
				if (结构.对象血量 > 0) {
					if (结构.对象阵营 == 100 || 结构.对象阵营 == 101 || 结构.对象阵营 == 110 || 结构.对象阵营 == 50 || 结构.对象阵营 == 120) {
						return true;
					}
				}

			}
		}
	}
	return false;
}

bool  BeastQuest::取通关商店()
{
	遍历结构 结构;
	bool res = 地图遍历(结构);
	if (!res) {
		printf("遍历地图失败");
		return false;
	}
	for (int i = 0; i < 结构.对象数量; i++)
	{
		结构.对象地址 = pApi.readInteger(结构.首地址 + 4 * i);
		if (结构.对象地址 && 结构.对象地址 != 结构.人物指针) {
			结构.对象类型 = pApi.readInteger(结构.对象地址 + 类型偏移);
			结构.对象阵营 = pApi.readInteger(结构.对象地址 + 阵营偏移);
			if (结构.对象阵营 == 200 || 结构.对象类型 == 33) {
				if (pApi.readInteger(结构.对象地址 + 1020) == 48026) {
					return true;
				}
			}
		}
	}
	return false;
}

void BeastQuest::图内操作()
{
	if (是否开门()) {
		if (超级入包()) {
			if (是否在BOSS房()) {
				if (!是否有怪物() && 是否开门()) {
					Send_组包翻牌(0, 0);
					Sleep(200);
					模拟按键(VK_ESCAPE);
					Sleep(500);
					if (取通关商店()) {
						模拟按键(VK_ESCAPE);
						Sleep(200);
						Send_组包再次();
						_全局变量.刷图计次 = _全局变量.刷图计次 + 1;
						printf(" %d \n", _全局变量.刷图计次);
						Sleep(4000);
						return;
					}

				}
			}
			else {
				智能过图();
				Sleep(2000);
				return;
			}
		}
	}
	else {
		//printf("12313");
		//人物跟随();
		//全屏技能();
	}
	return;
}

