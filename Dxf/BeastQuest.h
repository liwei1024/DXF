#pragma once
class BeastQuest
{
public:
	BeastQuest();

	~BeastQuest();

	bool 地图遍历(遍历结构 & 结构);

	bool 超级入包();

	void 评分();

	void 人物跟随();

	void 坐标过图(int 方向ID);

	void 智能过图();

	void 全屏技能();

	bool 是否有怪物();

	bool 取通关商店();

	void 图内操作();


};

