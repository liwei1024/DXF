#pragma once
class GetDirection
{
public:
	GetDirection();

	~GetDirection();

	int ���_ȡƫ�Ƶ�ַ(int ����, vector<int> ƫ�Ƶ�ַ);

	int ��ͼ_��������(vector<Coordinate> map_way_array, vector<Coordinate>& real_map_way);

	void ��ͼ_·���㷨(vector<vector<��Ϸ��ͼ>> map_flag, Coordinate start_point, Coordinate end_point, int map_width, int map_heigth, vector<Coordinate>& map_way_array);

	bool ��ͼ_�жϷ���(int cross_orientation, int orientation);

	void ��ͼ_���ɵ�ͼ(int map_width, int map_heigth, vector<int> map_cross, vector<vector<��Ϸ��ͼ>>& map_room);

	void ��ͼ_��ʾ��ͼ(vector<vector<��Ϸ��ͼ>> map_array, int map_width, int map_heigth, vector<vector<��Ϸ��ͼ>>& map_flag);

	int ��ͼ_��ȡ�߷�(vector<int> map_cross, int map_width, int map_heigth, Coordinate start_point, Coordinate end_point, vector<Coordinate>& real_map_way);

	��ͼ���� Get_data_Map(int ��Ԩģʽ);

	int Calculation_direction(Coordinate ��ǰ����, Coordinate �¸�����);

	Coordinate �����ж�(int ����);

};

