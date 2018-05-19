#pragma once
struct Coordinate
{
	int x;
	int y;
	int z;
};

struct 遍历结构
{
	int 人物指针;
	int 地图地址;
	int 首地址;
	int 尾地址;
	int 对象数量;
	int 对象地址;
	int 对象类型;
	int 对象阵营;
	int 对象血量;
	Coordinate 人物坐标;
};

struct 开关
{
	bool 自动入包;
	bool 自动刷图开关;
};

struct 地图数据//坐标类型
{
	char*  地图名称;//地图名称
	int 地图编号;  //地图编号
	vector<int> 地图通道;//地图通道
	Coordinate 起始坐标;
	Coordinate 终点坐标;
	int 宽;
	int 高;
	vector<Coordinate> 地图走法; //地图走法
	int 消耗疲劳;
};
struct 游戏地图  //副本小地图类型
{
	Coordinate 地图坐标;
	bool 地图左边;
	bool 地图右边;
	bool 地图上边;
	bool 地图下边;
	int 地图通道;
	int 背景颜色;
};
struct 地图节点
{
	int 地图F点;
	int 地图G点;
	int 地图H点;
	Coordinate 当前坐标;
	Coordinate 最终坐标;
};

struct 背包装备
{
	int 背包地址;
	int 装备首址;
	int 装备栏首址;
};

struct 任务结构
{
	int 任务指针;
	int 首地址;
	int 尾地址;
	int 任务数量;
	int 任务地址;
	int 任务类型;
	int 任务偏移;
	int 任务ID;
	int 任务条件;
	char *任务名称;
};

struct 全局变量
{
	int 刷图计次;
};