#pragma once
class GetDirection
{
public:
	GetDirection();

	~GetDirection();

	int 汇编_取偏移地址(int 基质, vector<int> 偏移地址);

	int 地图_整理坐标(vector<Coordinate> map_way_array, vector<Coordinate>& real_map_way);

	void 地图_路径算法(vector<vector<游戏地图>> map_flag, Coordinate start_point, Coordinate end_point, int map_width, int map_heigth, vector<Coordinate>& map_way_array);

	bool 地图_判断方向(int cross_orientation, int orientation);

	void 地图_生成地图(int map_width, int map_heigth, vector<int> map_cross, vector<vector<游戏地图>>& map_room);

	void 地图_显示地图(vector<vector<游戏地图>> map_array, int map_width, int map_heigth, vector<vector<游戏地图>>& map_flag);

	int 地图_获取走法(vector<int> map_cross, int map_width, int map_heigth, Coordinate start_point, Coordinate end_point, vector<Coordinate>& real_map_way);

	地图数据 Get_data_Map(int 深渊模式);

	int Calculation_direction(Coordinate 当前房间, Coordinate 下个房间);

	Coordinate 房间判断(int 调用);

};

