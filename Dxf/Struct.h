#pragma once
struct Coordinate
{
	int x;
	int y;
	int z;
};

struct �����ṹ
{
	int ����ָ��;
	int ��ͼ��ַ;
	int �׵�ַ;
	int β��ַ;
	int ��������;
	int �����ַ;
	int ��������;
	int ������Ӫ;
	int ����Ѫ��;
	Coordinate ��������;
};



struct ��ͼ����//��������
{
	char*  ��ͼ����;//��ͼ����
	int ��ͼ���;  //��ͼ���
	vector<int> ��ͼͨ��;//��ͼͨ��
	Coordinate ��ʼ����;
	Coordinate �յ�����;
	int ��;
	int ��;
	vector<Coordinate> ��ͼ�߷�; //��ͼ�߷�
	int ����ƣ��;
};
struct ��Ϸ��ͼ  //����С��ͼ����
{
	Coordinate ��ͼ����;
	bool ��ͼ���;
	bool ��ͼ�ұ�;
	bool ��ͼ�ϱ�;
	bool ��ͼ�±�;
	int ��ͼͨ��;
	int ������ɫ;
};
struct ��ͼ�ڵ�
{
	int ��ͼF��;
	int ��ͼG��;
	int ��ͼH��;
	Coordinate ��ǰ����;
	Coordinate ��������;
};

struct ����װ��
{
	int ������ַ;
	int װ����ַ;
	int װ������ַ;
};

struct ����ṹ
{
	int ����ָ��;
	int �׵�ַ;
	int β��ַ;
	int ��������;
	int �����ַ;
	int ��������;
	int ����ƫ��;
	int ����ID;
	int ���񸱱�;
	char *��������;
	char *��������;
};

struct ����
{
	bool �Զ���� = false;
	bool �Զ����鿪�� = false;
	bool �Զ�ˢͼ���� = false;
};

struct ȫ�ֱ���
{
	int ˢͼ�ƴ� = 0;
	int ��ǰ��ɫ = 0;
};

struct RoomInfo {
	Coordinate coordinate;
	Coordinate parent_coordinate;
	int weight;
	int numbering;
	bool up;
	bool bottom;
	bool left;
	bool right;
};

struct MapInfo {
	int width;
	int height;
	const char * name;
	vector<int> channel;
	Coordinate start_coordinate;
	Coordinate end_coordinate;
};