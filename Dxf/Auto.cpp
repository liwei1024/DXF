#include "stdafx.h"
#include "Auto.h"
#include "RemoteCall.h"
#include "Functions.h"
#include "BeastQuest.h"

void 自动剧情()
{
	int status = pApi.readInteger(游戏状态);
	if (status == 0) {
		Send_选择角色2(_全局变量.当前角色 + 1);
		Sleep(3000);
		return;
	}
	else if (status == 1) {
		if (取角色疲劳() == 0 || 取角色等级() == 90) {
			_全局变量.当前角色 += 1;
			Send_返回角色();
			Sleep(2000);
		}
		else {
			if (剧情任务()) {
				while (pApi.readInteger(游戏状态) != 3)
				{
					Sleep(1000);
				}
			}
			else {
				全局开关.自动剧情开关 = false;
			}
			return;
		}
	}
	else if (status == 3) {
		BeastQuest BQ;
		if (是否开门() && BQ.超级入包()) {
			if (pApi.readInteger(对话基址) == 1) {
				模拟按键(VK_ESCAPE);
				模拟按键(VK_SPACE);
				return;
			}
			if (是否在BOSS房()) {
				if (!BQ.是否有怪物() && 是否开门()) {
					Send_组包翻牌(0, 0);
					Sleep(200);
					if (pApi.readInteger(对话基址) == 1) {
						模拟按键(VK_ESCAPE);
						模拟按键(VK_SPACE);
						return;
					}
					if (BQ.取通关商店()) {
						模拟按键(VK_ESCAPE);
						if (装备处理()) {
							_全局变量.刷图计次 = _全局变量.刷图计次 + 1;
							printf(" %d \n", _全局变量.刷图计次);
							模拟按键(VK_SPACE);
							Sleep(4000);
							return;
						}
					}
				}
			}
			else {
				BQ.智能过图();
				Sleep(1000);
				return;
			}
		}
		else {
			// 打怪
			return;
		}
	}
}

void 自动刷图() 
{

}

