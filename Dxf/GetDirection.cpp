#include "stdafx.h"
#include "GetDirection.h"
#include "Functions.h"


GetDirection::GetDirection()
{
}


GetDirection::~GetDirection()
{
}

int GetDirection::���_ȡƫ�Ƶ�ַ(int ����, vector<int>ƫ�Ƶ�ַ)
{
	try {
		int ��ַ = ����;
		for (int i = 0; i < (int)ƫ�Ƶ�ַ.size(); i++)
		{
			��ַ = pApi.readInteger(��ַ);//��д_->���ڴ�������(��ַ, -1);
			��ַ = ��ַ + ƫ�Ƶ�ַ[i];
		}
		return ��ַ;
	}
	catch (...) {
		printf("���_ȡƫ�Ƶ�ַ ERROR");
		return 0;
	}


}

int GetDirection::��ͼ_��������(vector<Coordinate> map_way_array, vector<Coordinate> &real_map_way)
{
	int x, y;
	int k = 0;
	Coordinate tmp_coordinate;
	for (unsigned int i = 0; i < map_way_array.size(); i++)
	{
		x = (map_way_array[i].x + 2) % 3;
		y = (map_way_array[i].y + 2) % 3;
		if (x == 0 && y == 0)   //�ж��Ƿ��ܱ�3����
		{
			tmp_coordinate.x = (map_way_array[i].x + 2) / 3 - 1;   //��ԭ����
			tmp_coordinate.y = (map_way_array[i].y + 2) / 3 - 1;
			real_map_way.insert(real_map_way.begin() + k, tmp_coordinate);
			k++;
		}
	}
	return(k);
}

void GetDirection::��ͼ_·���㷨(vector<vector<��Ϸ��ͼ>> map_flag, Coordinate start_point, Coordinate end_point, int map_width, int map_heigth, vector<Coordinate> &map_way_array)
{
	bool �Ѵ��ڿ����б�, �Ѵ��ڹر��б�;
	Coordinate ���������;
	��ͼ�ڵ� �����ڵ�, ��ʱ�ڵ�;
	vector<��ͼ�ڵ�> �����б�, �ر��б�;
	int ��̱��;
	int ��СFֵ, Ԥ��Gֵ;
	unsigned int x, y;

	��ʱ�ڵ�.��ǰ����.x = start_point.x;
	��ʱ�ڵ�.��ǰ����.y = start_point.y;
	map_flag[start_point.x][start_point.y].������ɫ = 0x00FF00; //��ɫ
	map_flag[end_point.x][end_point.y].������ɫ = 0x0000FF; //��ɫ    //�±������ټ�1��ǰ����������
	�����б�.insert(�����б�.begin(), ��ʱ�ڵ�);
	do
	{
		//���Ƚ������ȼ������ҳ����·���ڵ�
		��СFֵ = 0;
		for (y = 0; y < �����б�.size(); y++)
		{
			if (��СFֵ == 0)
			{
				��СFֵ = �����б�[0].��ͼF��;
				��̱�� = y;
			}
			if (�����б�[y].��ͼF�� < ��СFֵ)
			{
				��СFֵ = �����б�[y].��ͼF��;
				��̱�� = y;
			}
		}
		��ʱ�ڵ� = �����б�[��̱��];
		�����б�.erase(�����б�.begin() + ��̱��);
		�ر��б�.insert(�ر��б�.begin(), ��ʱ�ڵ�);
		if (��ʱ�ڵ�.��ǰ����.x != start_point.x || ��ʱ�ڵ�.��ǰ����.y != start_point.y)
		{
			if (��ʱ�ڵ�.��ǰ����.x != end_point.x || ��ʱ�ڵ�.��ǰ����.y != end_point.y)
			{
				map_flag[��ʱ�ڵ�.��ǰ����.x][��ʱ�ڵ�.��ǰ����.y].������ɫ = 0x0080FF;  //�Ȼ�
			}
		}
		for (y = 0; y < �ر��б�.size(); y++)
		{
			if (�ر��б�[y].��ǰ����.x == end_point.x && �ر��б�[y].��ǰ����.y == end_point.y)
			{
				�����ڵ� = �ر��б�[y];
				do
				{
					for (unsigned int x = 0; x < �ر��б�.size(); x++)
					{
						if (�ر��б�[x].��ǰ����.x == �����ڵ�.��������.x && �ر��б�[x].��ǰ����.y == �����ڵ�.��������.y)
						{
							�����ڵ� = �ر��б�[x];
							break;
						}
					}
					if (�����ڵ�.��ǰ����.x != start_point.x || �����ڵ�.��ǰ����.y != start_point.y)
					{
						map_flag[�����ڵ�.��ǰ����.x][�����ڵ�.��ǰ����.y].������ɫ = 0x00D8D8;  //�ۻ�
						map_way_array.insert(map_way_array.begin(), �����ڵ�.��ǰ����);
					}
				} while (�����ڵ�.��ǰ����.x != start_point.x || �����ڵ�.��ǰ����.y != start_point.y);
				map_way_array.insert(map_way_array.begin(), start_point);
				map_way_array.insert(map_way_array.end(), end_point);  //���
				return;
			}
		}
		for (y = 0; y < 4; y++)
		{
			if (y == 0)
			{
				���������.x = ��ʱ�ڵ�.��ǰ����.x;
				���������.y = ��ʱ�ڵ�.��ǰ����.y - 1;
			}
			else if (y == 1)
			{
				���������.x = ��ʱ�ڵ�.��ǰ����.x - 1;
				���������.y = ��ʱ�ڵ�.��ǰ����.y;
			}
			else if (y == 2)
			{
				���������.x = ��ʱ�ڵ�.��ǰ����.x + 1;
				���������.y = ��ʱ�ڵ�.��ǰ����.y;
			}
			else
			{
				���������.x = ��ʱ�ڵ�.��ǰ����.x;
				���������.y = ��ʱ�ڵ�.��ǰ����.y + 1;
			}
			if (���������.x < 0 || ���������.x >(map_width - 1) || ���������.y < 0 || ���������.y >(map_heigth - 1))
				continue;
			if (map_flag[���������.x][���������.y].������ɫ == 0x000000)
				continue;
			�Ѵ��ڹر��б� = false;
			for (x = 0; x < �ر��б�.size(); x++)
			{
				if (�ر��б�[x].��ǰ����.x == ���������.x && �ر��б�[x].��ǰ����.y == ���������.y)
				{
					�Ѵ��ڹر��б� = true;
					break;
				}
			}
			if (�Ѵ��ڹر��б�)
				continue;
			�Ѵ��ڿ����б� = false;
			for (x = 0; x < �����б�.size(); x++)
			{
				if (�����б�[x].��ǰ����.x == ���������.x && �����б�[x].��ǰ����.y == ���������.y)
				{
					if (���������.x != ��ʱ�ڵ�.��ǰ����.x || ���������.y != ��ʱ�ڵ�.��ǰ����.y)
						Ԥ��Gֵ = 14;
					else
						Ԥ��Gֵ = 10;
					if (��ʱ�ڵ�.��ͼG�� + Ԥ��Gֵ < �����б�[x].��ͼG��)
						�����б�[x].�������� = ��ʱ�ڵ�.��ǰ����;
					�Ѵ��ڿ����б� = true;
					break;
				}
			}
			if (�Ѵ��ڿ����б� == false)
			{
				if (���������.x == ��ʱ�ڵ�.��ǰ����.x || ���������.y == ��ʱ�ڵ�.��ǰ����.y)
					Ԥ��Gֵ = 10;
				else
					Ԥ��Gֵ = 14;
				�����ڵ�.��ͼG�� = ��ʱ�ڵ�.��ͼG�� + Ԥ��Gֵ;
				�����ڵ�.��ͼH�� = abs(end_point.x - ���������.x) * 10 + abs(end_point.y - ���������.y) * 10;
				�����ڵ�.��ͼF�� = �����ڵ�.��ͼG�� + �����ڵ�.��ͼH��;
				�����ڵ�.��ǰ���� = ���������;
				�����ڵ�.�������� = ��ʱ�ڵ�.��ǰ����;
				�����б�.insert(�����б�.begin(), �����ڵ�);
			}
		}
	} while (�����б�.size() != 0);
}

bool GetDirection::��ͼ_�жϷ���(int cross_orientation, int orientation)
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

void GetDirection::��ͼ_���ɵ�ͼ(int map_width, int map_heigth, vector<int> map_cross, vector<vector<��Ϸ��ͼ>> &map_room)
{
	map_room.clear();
	map_room.resize(map_width);   //��ʼ������
	for (int x = 0; x < map_width; x++)
	{
		map_room[x].resize(map_heigth);   //��ʼ��ÿ�е�����
	}
	int i = 0;
	for (int y = 0; y < map_heigth; y++)
	{
		for (int x = 0; x < map_width; x++)
		{
			map_room[x][y].��ͼ����.x = x;
			map_room[x][y].��ͼ����.y = y;
			map_room[x][y].��ͼͨ�� = map_cross[i];
			map_room[x][y].��ͼ��� = ��ͼ_�жϷ���(map_cross[i], 0);
			map_room[x][y].��ͼ�ұ� = ��ͼ_�жϷ���(map_cross[i], 1);
			map_room[x][y].��ͼ�ϱ� = ��ͼ_�жϷ���(map_cross[i], 2);
			map_room[x][y].��ͼ�±� = ��ͼ_�жϷ���(map_cross[i], 3);
			map_room[x][y].������ɫ = 0xFFFFFF;  //��ɫ
			i++;
			if (map_room[x][y].��ͼͨ�� == 0)
				map_room[x][y].������ɫ = 0x000000;  //��ɫ
		}
	}
}

void GetDirection::��ͼ_��ʾ��ͼ(vector<vector<��Ϸ��ͼ>> map_array, int map_width, int map_heigth, vector<vector<��Ϸ��ͼ>> &map_flag)
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
			map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 2].������ɫ = 0xFFFFFF;   //��ɫ
			if (map_array[x][y].��ͼ���)
				map_flag[(x + 1) * 3 - 3][(y + 1) * 3 - 2].������ɫ = 0xFFFFFF;
			if (map_array[x][y].��ͼ�ұ�)
				map_flag[(x + 1) * 3 - 1][(y + 1) * 3 - 2].������ɫ = 0xFFFFFF;
			if (map_array[x][y].��ͼ�ϱ�)
				map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 3].������ɫ = 0xFFFFFF;
			if (map_array[x][y].��ͼ�±�)
				map_flag[(x + 1) * 3 - 2][(y + 1) * 3 - 1].������ɫ = 0xFFFFFF;
		}
	}
}

int GetDirection::��ͼ_��ȡ�߷�(vector<int> map_cross, int map_width, int map_heigth, Coordinate start_point, Coordinate end_point, vector<Coordinate> &real_map_way)
{
	Coordinate start_coordinate;
	Coordinate end_coordinate;
	vector<vector<��Ϸ��ͼ>> map_flag;
	vector<vector<��Ϸ��ͼ>> map_array;
	vector<Coordinate> cross_way;

	if (start_point.x == end_point.x && start_point.y == end_point.y)
	{
		real_map_way.clear();
		real_map_way.resize(0);
		return 0;
	}
	��ͼ_���ɵ�ͼ(map_width, map_heigth, map_cross, map_array);
	��ͼ_��ʾ��ͼ(map_array, map_width, map_heigth, map_flag);
	start_coordinate.x = start_point.x * 3 - 2;
	start_coordinate.y = start_point.y * 3 - 2;
	end_coordinate.x = end_point.x * 3 - 2;
	end_coordinate.y = end_point.y * 3 - 2;
	��ͼ_·���㷨(map_flag, start_coordinate, end_coordinate, map_width * 3, map_heigth * 3, cross_way);
	return ��ͼ_��������(cross_way, real_map_way);
}


��ͼ���� GetDirection::Get_data_Map(int ��Ԩģʽ = 0)
{
	��ͼ����  map_data;
	try {
		int tmp_address, map_count, map_num, һ����ַ, ������ַ, ��ߵ�ַ, �����ַ;
		һ����ַ = pApi.readInteger(pApi.readInteger(�̵��ַ - 8) + ʱ���ַ);
		if (һ����ַ) {
			������ַ = pApi.readInteger(һ����ַ + ����ƫ��);
			if (������ַ) {
				map_num = Decode(������ַ + BOSS����X - 24) + 1;
				��ߵ�ַ = pApi.readInteger(������ַ + ���ƫ��);
				map_data.�� = pApi.readInteger(��ߵ�ַ + map_num * 8 - 8);
				map_data.�� = pApi.readInteger(��ߵ�ַ + map_num * 8 - 4);
				�����ַ = pApi.readInteger(������ַ + ����ƫ��);
				tmp_address = pApi.readInteger(�����ַ + map_num * 20 - 16);
				map_count = map_data.�� * map_data.��;
				for (int i = 0; i < map_count; i++)
				{
					map_data.��ͼͨ��.insert(map_data.��ͼͨ��.begin() + i, pApi.readInteger(tmp_address + i * 4));
				}
				map_data.��ʼ����.x = pApi.readInteger(һ����ַ + ��ʼ����X) + 1;
				map_data.��ʼ����.y = pApi.readInteger(һ����ַ + ��ʼ����Y) + 1;
				if (��Ԩģʽ == 0)
				{
					map_data.�յ�����.x = Decode(������ַ + BOSS����X) + 1;
					map_data.�յ�����.y = Decode(������ַ + BOSS����Y) + 1;
				}
				else if (��Ԩģʽ == 1)
				{
					map_data.�յ�����.x = Decode(������ַ + ��Ԩ����X) + 1;
					map_data.�յ�����.y = Decode(������ַ + ��Ԩ����Y) + 1;
				}
				map_data.����ƣ�� = this->��ͼ_��ȡ�߷�(map_data.��ͼͨ��, map_data.��, map_data.��, map_data.��ʼ����, map_data.�յ�����, map_data.��ͼ�߷�);
				return map_data;
			}
		}
	}
	catch (...) {
		printf("��ȡ��ͼ���� ERROR");
	}
	printf("δ��ȡ��ͼ����");
	return map_data;
}

int GetDirection::Calculation_direction(Coordinate ��ǰ����, Coordinate �¸�����)
{
	int ���� = 0;
	int x = ��ǰ����.x - �¸�����.x;
	int y = ��ǰ����.y - �¸�����.y;
	if (x == 0)
	{
		if (y == 1)
		{
			���� = 2;//��
		}
		else
		{
			���� = 3;//��
		}
	}
	else if (y == 0)
	{
		if (x == 1)
		{
			���� = 0;//��
		}
		else
		{
			���� = 1;//��
		}
	}
	return ����;
}

Coordinate GetDirection::�����ж�(int ����)
{
	try {
		Coordinate ����ֵ;
		if (���� == 1) {
			int һ����ַ, ����;
			һ����ַ = pApi.readInteger(pApi.readInteger(�̵��ַ + 8) + ʱ���ַ);
			if (һ����ַ) {
				���� = pApi.readInteger(pApi.readInteger(һ����ַ + ����ƫ��));
				����ֵ.x = ����;
				����ֵ.y = ����;
			}
		}
	}
	catch (...) {
		printf("�����ж� ERROR\n");
	}

}


