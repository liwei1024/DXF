#include "stdafx.h"
#include "Functions.h"
#include "RemoteCall.h"

int Decode(int addr)
{
	int eax, esi, edx, i;
	eax = pApi.readInteger(addr);
	esi = pApi.readInteger(解密基址);
	edx = eax >> 16;
	edx = pApi.readInteger(esi + edx * 4 + 36);
	eax = eax & 65535;
	eax = pApi.readInteger(edx + eax * 4 + 8468);
	edx = eax & 65535;
	esi = edx << 16;
	esi = esi | edx;
	i = pApi.readInteger(addr + 4);
	esi = esi ^ i;
	return esi;
}

void Encode(int addr, int value, int type)
{
	try {
		int encryptID, deviationParam, deviationAddr, data, lastAddr;
		short ax, si;
		encryptID = pApi.readInteger(addr);
		if (!encryptID) {
			printf("加密 encryptID ERROR");
			return;
		}
		deviationParam = pApi.readInteger(解密基址);
		if (!deviationParam) {
			printf("加密 deviationParam ERROR");
			return;
		}
		deviationParam = pApi.readInteger(deviationParam + (encryptID >> 16) * 4 + 36);
		if (!deviationParam) {
			printf("加密 deviationParam2 ERROR");
			return;
		}
		deviationAddr = deviationParam + (encryptID & 0xFFFF) * 4 + 8468;
		deviationParam = pApi.readInteger(deviationAddr);
		if (!deviationParam) {
			printf("加密 deviationParam3 ERROR");
			return;
		}
		data = deviationParam & 0xFFFF;
		data = data + (data << 16);
		ax = (short)(deviationParam & 0xFFFF);
		si = 0;
		if (type == 0)
		{
			lastAddr = addr & 15;
			switch (lastAddr)
			{
			case 0:
				si = (short)(value >> 16);
				si = (short)(si - ax);
				si = (short)(si + value);
				break;
			case 4:
				si = (short)((value & 0xFFFF) - (value >> 16));;
				break;
			case 8:
				si = (short)(value >> 16);
				si = (short)(si * value);
				break;
			case 12:
				si = (short)(value >> 16);
				si = (short)(si + value);
				si = (short)(si + ax);
				break;
			default:
				break;
			}
		}
		else if (type == 1)
			si = value & 16;
		else if (type == 2)
			si = value;
		else if (type == 3)
		{
			si = value >> 16;
			si += value;
		}
		ax = (short)(si ^ ax);
		data = data ^ value;
		pApi.writeInteger((addr + (type != 4 ? 4 : 8)), data);
		pApi.writeShort(deviationAddr + 2, ax);
	}
	catch (...) {
		printf("加密 ERROR");
	}

}

Coordinate 获取坐标(int addr)
{
	try {
		Coordinate 坐标;
		坐标.x = (int)pApi.readFloat(addr + X坐标);
		坐标.y = (int)pApi.readFloat(addr + Y坐标);
		坐标.z = (int)pApi.readFloat(addr + Z坐标);
		return 坐标;
	}
	catch (...)
	{
		printf("获取坐标 ERROR");
	}
}

Coordinate 当前房间(void)
{
	try {
		Coordinate xy;
		int 一级地址;
		一级地址 = pApi.readInteger(pApi.readInteger(pApi.readInteger(房间编号) + 时间基址) + 204);
		xy.x = pApi.readInteger(一级地址 + 当前房间X);
		xy.y = pApi.readInteger(一级地址 + 当前房间Y);
		return xy;
	}
	catch (...) {
		printf("当前房间 ERROR\n");
	}
}

Coordinate BOSS房间(void)
{
	try {
		Coordinate yx;
		int 一级地址;
		一级地址 = pApi.readInteger(pApi.readInteger(pApi.readInteger(房间编号) + 时间基址) + 204);
		if (一级地址) {
			yx.x = Decode(一级地址 + BOSS房间X);
			yx.y = Decode(一级地址 + BOSS房间Y);
		}
		return yx;
	}
	catch (...) {
		printf("BOSS房间 ERROR\n");
	}
}

bool 是否在BOSS房(void)
{
	try {
		Coordinate current;
		Coordinate BOSS;
		current = 当前房间();
		BOSS = BOSS房间();
		if (current.x == BOSS.x && current.y == BOSS.y)
		{
			return true;
		}
		else {
			return false;
		}
		return false;
	}
	catch (...) {
		printf("BOSS房间 ERROR\n");
	}
	return false;
}

bool 背包装备遍历(背包装备 &装备)
{
	装备.背包地址 = pApi.readInteger(背包基址);
	if (!装备.背包地址)return false;
	装备.装备首址 = pApi.readInteger(装备.背包地址 + 88);
	if (!装备.装备首址)return false;
	装备.装备栏首址 = 装备.装备首址 + 36;
	if (!装备.装备栏首址)return false;
	return true;
}

void 装备处理()
{
	背包装备 装备;
	if (背包装备遍历(装备) == false) {
		printf("背包装备遍历 失败\n");
		return;
	}
	int 装备地址, 装备属性, 装备名称地址;
	for (int i = 0; i < 56; i++)
	{
		装备地址 = pApi.readInteger(装备.装备栏首址 + i * 4);
		if (装备地址 != 0) {
			装备属性 = pApi.readInteger(装备地址 + 368); // 0白装 1 蓝装 2紫装
			装备名称地址 = pApi.readInteger(装备地址 + 36);
			char *装备名称 = pApi.readString(装备名称地址, 52);
			string str(装备名称);
			if ((str.find("传承", 0) == -1) && (str.find("周年", 0) == -1) && (str.find("活动", 0) == -1)) {
				if (装备属性 == 0 || 装备属性 == 1 || 装备属性 == 2) {
					printf(">>> 卖出 装备属性 %d 装备名称 %s \n", 装备属性, str.c_str());
					Send_组包卖物(i + 9);
				}
				/*else if (装备属性 == 2) {
					printf(">>> 分解 装备属性 %d 装备名称 %s \n", 装备属性, str.c_str());
					Send_组包分解(i + 9);
				}*/
				Sleep(1000);
			}
			delete[]装备名称;
		}
	}

	//Send_背包整理();
}

bool 是否开门()
{
	if (Decode(pApi.readInteger(pApi.readInteger(人物基址) + 地图偏移) + 开门偏移) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool 任务遍历(任务结构 &任务)
{
	任务.任务指针 = pApi.readInteger(任务基址);
	if (!任务.任务指针) {
		return false;
	}
	任务.首地址 = pApi.readInteger(任务.任务指针 + 104);
	if (!任务.首地址) {
		return false;
	}
	任务.尾地址 = pApi.readInteger(任务.任务指针 + 108);
	if (!任务.尾地址) {
		return false;
	}
	任务.任务数量 = (任务.尾地址 - 任务.首地址) / 4;
	return true;
}

void 完成任务(int 任务ID)
{
	Send_组包接受(任务ID);
	Sleep(100);
	Send_组包完成(任务ID);
	Sleep(100);
	Send_组包提交(任务ID);
}

void 任务遍历测试()
{
	任务结构 任务;
	if (!任务遍历(任务)) {
		printf("任务遍历失败\n");
	}
	for (int i = 0; i < 任务.任务数量; i++)
	{
		任务.任务地址 = pApi.readInteger(任务.首地址 + i * 4);
		任务.任务类型 = pApi.readInteger(任务.任务地址 + 308);
		任务.任务名称 = pApi.readString(pApi.readInteger(任务.任务地址 + 28)>7 ? pApi.readInteger(任务.任务地址 + 8) : 任务.任务地址 + 8, 50);
		if (任务.任务类型 == 0) {
			任务.任务副本 = pApi.readInteger(pApi.readInteger(任务.任务地址 + 676));
			任务.任务条件 = pApi.readString(pApi.readInteger(任务.任务地址 + 700), 100);
			任务.任务ID = pApi.readInteger(任务.任务地址);
			string 任务条件(任务.任务条件);
			if (任务.任务副本) {
				printf(">>>接取任务 任务ID: %d 任务副本：%d 任务条件: %s 任务名称：%s\n", 任务.任务ID, 任务.任务副本, 任务.任务条件, 任务.任务名称);
				区域_CALL(任务.任务副本);
				Sleep(500);
				Send_组包选图();
				Sleep(500);
				Send_组包剧情(任务.任务副本, 任务.任务ID);
			}
			else if (
				任务条件.find("hunt enemy") != -1 ||
				任务条件.find("meet npc") != -1 ||
				任务条件.find("hunt monster") != -1 ||
				任务条件.find("seek n meet npc") != -1 ||
				任务条件.find("question") != -1 ||
				任务条件.find("quest clear") != -1 ||
				任务条件.find("reach the range") != -1 ||
				任务条件.find("look cinematic") != -1
				)
			{
				 
				 完成任务(任务.任务ID);
				 delete[]任务.任务条件;
			}
			else {
				printf(">>>需手动完成 任务ID: %x 任务条件: %s 任务名称：%s\n", 任务.任务ID, 任务.任务条件, 任务.任务名称);
			}
		}
		delete[]任务.任务名称;
	}
	
}

int 获取主线任务ID()
{
	任务结构 任务;
	if (!任务遍历(任务)) {
		printf("任务遍历失败\n");
	}
	for (int i = 0; i < 任务.任务数量; i++)
	{
		任务.任务地址 = pApi.readInteger(任务.首地址 + i * 4);
		任务.任务类型 = pApi.readInteger(任务.任务地址 + 308);
		if (任务.任务类型 == 0) {
			任务.任务名称 = pApi.readString(pApi.readInteger(任务.任务地址 + 28)>7 ? pApi.readInteger(任务.任务地址 + 8) : 任务.任务地址 + 8, 50);
			任务.任务ID = pApi.readInteger(任务.任务地址);
			printf("%s \n", 任务.任务名称);
			delete[]任务.任务名称;
			Send_组包接受(任务.任务ID);
			Sleep(100);
			Send_组包完成(任务.任务ID);
			Sleep(100);
			Send_组包提交(任务.任务ID);
			return 任务.任务ID;
		}
		//
	}

}

void 区域_CALL(int 任务ID)//不可用
{
	int MaxMapId, MinMapId, x, y;
	Send_区域CALL(任务ID);
	MaxMapId = pApi.readInteger(pApi.readInteger(区域ECX) + 52164);
	//printf("MaxMapId %d\n", MaxMapId);
	MinMapId = pApi.readInteger(pApi.readInteger(区域ECX) + 52164 + 4);
	//printf("MinMapId %d\n", MinMapId);
	x = pApi.readInteger(pApi.readInteger(区域ECX) + 52164 + 8);
	//printf("x %d\n", x);
	y = pApi.readInteger(pApi.readInteger(区域ECX) + 52164 + 12);
	//printf("y %d\n", y);
	Send_城市飞机(MaxMapId, MinMapId,x,y);
}

/*void 内存按键(int K_Code,int s)
{
if (K_Code >= VK_F1 && K_Code <= VK_F12) {
K_Code = K_Code + 216;
}
else if (K_Code == VK_Q) {
K_Code = 285;
}
else if (K_Code == VK_W) {
K_Code = 286;
}
else if (K_Code == VK_E) {
K_Code = 287;
}
else if (K_Code == VK_R) {
K_Code = 288;
}
else if (K_Code == VK_T) {
K_Code = 289;
}
else if (K_Code == VK_Y) {
K_Code = 290;
}
else if (K_Code == VK_U) {
K_Code = 291;
}
else if (K_Code == VK_I) {
K_Code = 292;
}
else if (K_Code == VK_O) {
K_Code = 293;
}
else if (K_Code == VK_P) {
K_Code = 294;
}
else if (K_Code == VK_A) {
K_Code = 299;
}
else if (K_Code == VK_S) {
K_Code = 300;
}
else if (K_Code == VK_D) {
K_Code = 301;
}
else if (K_Code == VK_F) {
K_Code = 302;
}
else if (K_Code == VK_G) {
K_Code = 303;
}
else if (K_Code == VK_H) {
K_Code = 304;
}
else if (K_Code == VK_J) {
K_Code = 305;
}
else if (K_Code == VK_K) {
K_Code = 306;
}
else if (K_Code == VK_L) {
K_Code = 307;
}
else if (K_Code == VK_Z) {
K_Code = 313;
}
else if (K_Code == VK_X) {
K_Code = 314;
}
else if (K_Code == VK_C) {
K_Code = 315;
}
else if (K_Code == VK_V) {
K_Code = 316;
}
else if (K_Code == VK_B) {
K_Code = 317;
}
else if (K_Code == VK_N) {
K_Code = 318;
}
else if (K_Code == VK_M) {
K_Code = 319;
}
else if (K_Code == VK_ESC) {
K_Code = 270;
}
s = s + (int)(rand() % 6);
pApi.writeBytes(pApi.readInteger(按键基址) + K_Code, { 1 });
Sleep(s);
pApi.writeBytes(pApi.readInteger(按键基址) + K_Code, { 0 });
}*/

void 模拟按键(int keyCode, int s)
{
	s = s + (int)(rand() % 6);
	keybd_event(keyCode, MapVirtualKey(keyCode, 0), 0, 0);
	Sleep(s);
	keybd_event(keyCode, MapVirtualKey(keyCode, 0), KEYEVENTF_KEYUP, 0);
	Sleep(s);
}