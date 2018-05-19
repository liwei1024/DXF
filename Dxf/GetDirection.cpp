#include "stdafx.h"
#include "GetDirection.h"
#include "Functions.h"


GetDirection::GetDirection()
{
}


GetDirection::~GetDirection()
{
}

int GetDirection::汇编_取偏移地址(int 基质, vector<int>偏移地址)
{
	try {
		int 地址 = 基质;
		for (int i = 0; i < (int)偏移地址.size(); i++)
		{
			地址 = pApi.readInteger(地址);//读写_->读内存整数型(地址, -1);
			地址 = 地址 + 偏移地址[i];
		}
		return 地址;
	}
	catch (...) {
		printf("汇编_取偏移地址 ERROR");
		return 0;
	}


}

int GetDirection::地图_整理坐标(vector<Coordinate> map_way_array, vector<Coordinate> &real_map_way)
{
	int x, y;
	int k = 0;
	Coordinate tmp_coordinate;
	for (unsigned int i = 0; i < map_way_array.size(); i++)
	{
		x = (map_way_array[i].x + 2) % 3;
		y = (map_way_array[i].y + 2) % 3;
		if (x == 0 && y == 0)   //判断是否能被3整除
		{
			tmp_coordinate.x = (map_way_array[i].x + 2) / 3 - 1;   //还原坐标
			tmp_coordinate.y = (map_way_array[i].y + 2) / 3 - 1;
			real_map_way.insert(real_map_way.begin() + k, tmp_coordinate);
			k++;
		}
	}
	return(k);
}

void GetDirection::地图_路径算法(vector<vector<游戏地图>> map_flag, Coordinate start_point, Coordinate end_point, int map_width, int map_heigth, vector<Coordinate> &map_way_array)
{
	bool 已存在开放列表, 已存在关闭列表;
	Coordinate 待检测坐标;
	地图节点 待检测节点, 临时节点;
	vector<地图节点> 开放列表, 关闭列表;
	int 最短编号;
	int 最小F值, 预测G值;
	unsigned int x, y;

	临时节点.当前坐标.x = start_point.x;
	临时节点.当前坐标.y = start_point.y;
	map_flag[start_point.x][start_point.y].背景颜色 = 0x00FF00; //绿色
	map_flag[end_point.x][end_point.y].背景颜色 = 0x0000FF; //红色    //下标无需再减1，前面已作处理
	开放列表.insert(开放列表.begin(), 临时节点);
	do
	{
		//首先进行优先级排序，找出最短路径节点
		最小F值 = 0;
		for (y = 0; y < 开放列表.size(); y++)
		{
			if (最小F值 == 0)
			{
				最小F值 = 开放列表[0].地图F点;
				最短编号 = y;
			}
			if (开放列表[y].地图F点 < 最小F值)
			{
				最小F值 = 开放列表[y].地图F点;
				最短编号 = y;
			}
		}
		临时节点 = 开放列表[最短编号];
		开放列表.erase(开放列表.begin() + 最短编号);
		关闭列表.insert(关闭列表.begin(), 临时节点);
		if (临时节点.当前坐标.x != start_point.x || 临时节点.当前坐标.y != start_point.y)
		{
			if (临时节点.当前坐标.x != end_point.x || 临时节点.当前坐标.y != end_point.y)
			{
				map_flag[临时节点.当前坐标.x][临时节点.当前坐标.y].背景颜色 = 0x0080FF;  //橙黄
			}
		}
		for (y = 0; y < 关闭列表.size(); y++)
		{
			if (关闭列表[y].当前坐标.x == end_point.x && 关闭列表[y].当前坐标.y == end_point.y)
			{
				待检测节点 = 关闭列表[y];
				do
				{
					for (unsigned int x = 0; x < 关闭列表.size(); x++)
					{
						if (关闭列表[x].当前坐标.x == 待检测节点.最终坐标.x && 关闭列表[x].当前坐标.y == 待检测节点.最终坐标.y)
						{
							待检测节点 = 关闭列表[x];
							break;
						}
					}
					if (待检测节点.当前坐标.x != start_point.x || 待检测节点.当前坐标.y != start_point.y)
					{
						map_flag[待检测节点.当前坐标.x][待检测节点.当前坐标.y].背景颜色 = 0x00D8D8;  //嫩黄
						map_way_array.insert(map_way_array.begin(), 待检测节点.当前坐标);
					}
				} while (待检测节点.当前坐标.x != start_point.x || 待检测节点.当前坐标.y != start_point.y);
				map_way_array.insert(map_way_array.begin(), start_point);
				map_way_array.insert(map_way_array.end(), end_point);  //最后
				return;
			}
		}
		for (y = 0; y < 4; y++)
		{
			if (y == 0)
			{
				待检测坐标.x = 临时节点.当前坐标.x;
				待检测坐标.y = 临时节点.当前坐标.y - 1;
			}
			else if (y == 1)
			{
				待检测坐标.x = 临时节点.当前坐标.x - 1;
				待检测坐标.y = 临时节点.当前坐标.y;
			}
			else if (y == 2)
			{
				待检测坐标.x = 临时节点.当前坐标.x + 1;
				待检测坐标.y = 临时节点.当前坐标.y;
			}
			else
			{
				待检测坐标.x = 临时节点.当前坐标.x;
				待检测坐标.y = 临时节点.当前坐标.y + 1;
			}
			if (待检测坐标.x < 0 || 待检测坐标.x >(map_width - 1) || 待检测坐标.y < 0 || 待检测坐标.y >(map_heigth - 1))
				continue;
			if (map_flag[待检测坐标.x][待检测坐标.y].背景颜色 == 0x000000)
				continue;
			已存在关闭列表 = false;
			for (x = 0; x < 关闭列表.size(); x++)
			{
				if (关闭列表[x].当前坐标.x == 待检测坐标.x && 关闭列表[x].当前坐标.y == 待检测坐标.y)
				{
					已存在关闭列表 = true;
					break;
				}
			}
			if (已存在关闭列表)
				continue;
			已存在开放列表 = false;
			for (x = 0; x < 开放列表.size(); x++)
			{
				if (开放列表[x].当前坐标.x == 待检测坐标.x && 开放列表[x].当前坐标.y == 待检测坐标.y)
				{
					if (待检测坐标.x != 临时节点.当前坐标.x || 待检测坐标.y != 临时节点.当前坐标.y)
						预测G值 = 14;
					else
						预测G值 = 10;
					if (临时节点.地图G点 + 预测G值 < 开放列表[x].地图G点)
						开放列表[x].最终坐标 = 临时节点.当前坐标;
					已存在开放列表 = true;
					break;
				}
			}
			if (已存在开放列表 == false)
			{
				if (待检测坐标.x == 临时节点.当前坐标.x || 待检测坐标.y == 临时节点.当前坐标.y)
					预测G值 = 10;
				else
					预测G值 = 14;
				待检测节点.地图G点 = 临时节点.地图G点 + 预测G值;
				待检测节点.地图H点 = abs(end_point.x - 待检测坐标.x) * 10 + abs(end_point.y - 待检测坐标.y) * 10;
				待检测节点.地图F点 = 待检测节点.地图G点 + 待检测节点.地图H点;
				待检测节点.当前坐标 = 待检测坐标;
				待检测节点.最终坐标 = 临时节点.当前坐标;
				开放列表.insert(开放列表.begin(), 待检测节点);
			}
		}
	} while (开放列表.size() != 0);
}

bool GetDirection::地图_判断方向(int cross_orientation, int orientation)
{
	unsigned char orientation_array[4];
	unsigned char orientation_vector[16][4] = { { 0, 0, 0, 0 },{ 0, 1, 0, 0 },{ 0, 0, 1, 0 },{ 0, 1, 1, 0 },{ 1, 0, 0, 0 },{ 1, 1, 0, 0 },{ 1, 0, 1, 0 },{ 1, 1, 1, 0 },
	{ 0, 0, 0, 1 },{ 0, 1, 0, 1 },{ 0, 0, 1, 1 },{ 0, 1, 1, 1 },{ 1, 0, 0, 1 },{ 1, 1, 0, 1 },{ 1, 0, 1, 1 },{ 1, 1, 1, 1 } };
	if (cross_orientation >= 0 && cross_orientation <= 15)
		for (int i = 0; i < 4; i++)
			orientation_array[i] = orientation_vector[cross_orientation][i];
	else
		for (int i = 0; i < 4; i++)
			orientation_array[i] = 0;
	if (orientation_array[orientation] == 1)
		return true;
	else
		return false;
}

void GetDirection::地图_生成地图(int map_width, int map_heigth, vector<int> map_cross, vector<vector<游戏地图>> &map_room)
{
	map_room.clear();
	map_room.resize(map_width);   //初始化行数
	for (int x = 0; x < map_width; x++)
	{
		map_room[x].resize(map_heigth);   //初始化每行的列数
	}
	int i = 0;
	for (int y = 0; y < map_heigth; y++)
	{
		for (int x = 0; x < map_width; x++)
		{
			map_room[x][y].地图坐标.x = x;
			map_room[x][y].地图坐标.y = y;
			map_room[x][y].地图通道 = map_cross[i];
			map_room[x][y].地图左边 = 地图_判断方向(map_cross[i], 0);
			map_room[x][y].地图右边 = 地图_判断方向(map_cross[i], 1);
			map_room[x][y].地图上边 = 地图_判断方向(map_cross[i], 2);
			map_room[x][y].地图下边 = 地图_判断方向(map_cross[i], 3);
			map_room[x][y].背景颜色 = 0xFFFFFF;  //白色
			i++;
			if (map_room[x][y].地图通道 == 0)
				map_room[x][y].背景颜色 = 0x000000;  //黑色
		}
	}
}

void GetDirection::地图_显示地图(vector<vector<游戏地图>> map_array, int map_width, int map_heigth, vector<vector<游戏地图>> &map_flag)
{
	map_flag.clear();
	map_flag.resize(map_width * 3);
	for (int x = 0; x < map_width * 3; x++)
	{
		map_flag[x].resize(map_heigth * 3);
	}
	for (int y = 0; y < map_heigth; y++)
	{
		for (int x = 0; x < map_width; x++)
		{
			map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 2].背景颜色 = 0xFFFFFF;   //白色
			if (map_array[x][y].地图左边)
				map_flag[(x + 1) * 3 - 3][(y + 1) * 3 - 2].背景颜色 = 0xFFFFFF;
			if (map_array[x][y].地图右边)
				map_flag[(x + 1) * 3 - 1][(y + 1) * 3 - 2].背景颜色 = 0xFFFFFF;
			if (map_array[x][y].地图上边)
				map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 3].背景颜色 = 0xFFFFFF;
			if (map_array[x][y].地图下边)
				map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 1].背景颜色 = 0xFFFFFF;
		}
	}
}

int GetDirection::地图_获取走法(vector<int> map_cross, int map_width, int map_heigth, Coordinate start_point, Coordinate end_point, vector<Coordinate> &real_map_way)
{
	Coordinate start_coordinate;
	Coordinate end_coordinate;
	vector<vector<游戏地图>> map_flag;
	vector<vector<游戏地图>> map_array;
	vector<Coordinate> cross_way;

	if (start_point.x == end_point.x && start_point.y == end_point.y)
	{
		real_map_way.clear();
		real_map_way.resize(0);
		return 0;
	}
	地图_生成地图(map_width, map_heigth, map_cross, map_array);
	地图_显示地图(map_array, map_width, map_heigth, map_flag);
	start_coordinate.x = start_point.x * 3 - 2;
	start_coordinate.y = start_point.y * 3 - 2;
	end_coordinate.x = end_point.x * 3 - 2;
	end_coordinate.y = end_point.y * 3 - 2;
	地图_路径算法(map_flag, start_coordinate, end_coordinate, map_width * 3, map_heigth * 3, cross_way);
	return 地图_整理坐标(cross_way, real_map_way);
}


地图数据 GetDirection::Get_data_Map(int 深渊模式 = 0)
{
	地图数据  map_data;
	try {
		int tmp_address, map_count, map_num, 一级地址, 二级地址, 宽高地址, 数组地址;
		一级地址 = pApi.readInteger(pApi.readInteger(商店基址 - 8) + 时间基址);
		if (一级地址) {
			二级地址 = pApi.readInteger(一级地址 + 门型偏移);
			if (二级地址) {
				map_num = Decode(二级地址 + BOSS房间X - 24) + 1;
				宽高地址 = pApi.readInteger(二级地址 + 宽高偏移);
				map_data.宽 = pApi.readInteger(宽高地址 + map_num * 8 - 8);
				map_data.高 = pApi.readInteger(宽高地址 + map_num * 8 - 4);
				数组地址 = pApi.readInteger(二级地址 + 数组偏移);
				tmp_address = pApi.readInteger(数组地址 + map_num * 20 - 16);
				map_count = map_data.宽 * map_data.高;
				for (int i = 0; i < map_count; i++)
				{
					map_data.地图通道.insert(map_data.地图通道.begin() + i, pApi.readInteger(tmp_address + i * 4));
				}
				map_data.起始坐标.x = pApi.readInteger(一级地址 + 起始坐标X) + 1;
				map_data.起始坐标.y = pApi.readInteger(一级地址 + 起始坐标Y) + 1;
				if (深渊模式 == 0)
				{
					map_data.终点坐标.x = Decode(二级地址 + BOSS房间X) + 1;
					map_data.终点坐标.y = Decode(二级地址 + BOSS房间Y) + 1;
				}
				else if (深渊模式 == 1)
				{
					map_data.终点坐标.x = Decode(二级地址 + 深渊房间X) + 1;
					map_data.终点坐标.y = Decode(二级地址 + 深渊房间Y) + 1;
				}
				map_data.消耗疲劳 = this->地图_获取走法(map_data.地图通道, map_data.宽, map_data.高, map_data.起始坐标, map_data.终点坐标, map_data.地图走法);
				return map_data;
			}
		}
	}
	catch (...) {
		printf("获取地图数据 ERROR");
	}
	printf("未获取地图数据");
	return map_data;
}

int GetDirection::Calculation_direction(Coordinate 当前房间, Coordinate 下个房间)
{
	int 方向 = 0;
	int x = 当前房间.x - 下个房间.x;
	int y = 当前房间.y - 下个房间.y;
	if (x == 0)
	{
		if (y == 1)
		{
			方向 = 2;//上
		}
		else
		{
			方向 = 3;//下
		}
	}
	else if (y == 0)
	{
		if (x == 1)
		{
			方向 = 0;//左
		}
		else
		{
			方向 = 1;//右
		}
	}
	return 方向;
}

Coordinate GetDirection::房间判断(int 调用)
{
	try {
		Coordinate 返回值;
		if (调用 == 1) {
			int 一级地址, 开门;
			一级地址 = pApi.readInteger(pApi.readInteger(商店基址 + 8) + 时间基址);
			if (一级地址) {
				开门 = pApi.readInteger(pApi.readInteger(一级地址 + 开门偏移));
				返回值.x = 开门;
				返回值.y = 开门;
			}
		}
	}
	catch (...) {
		printf("房间判断 ERROR\n");
	}

}


