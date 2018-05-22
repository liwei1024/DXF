#include "stdafx.h"
#include "BeastQuest.h"
#include "RemoteCall.h"
#include "Functions.h"
#include "GetDirection.h"
#include "KeyCode.h"

BeastQuest::BeastQuest()
{
	int status = pApi.readInteger(��Ϸ״̬);
	if (status != 3) {
		printf("���ڵ�ͼ��\n");
		return;
	}
}

BeastQuest::~BeastQuest()
{
}

bool BeastQuest::��ͼ����(�����ṹ & �ṹ)
{
	�ṹ.����ָ�� = pApi.readInteger(�����ַ);
	�ṹ.��ͼ��ַ = pApi.readInteger(�ṹ.����ָ�� + ��ͼƫ��);

	if (�ṹ.��ͼ��ַ) {
		�ṹ.�׵�ַ = pApi.readInteger(�ṹ.��ͼ��ַ + ��ͼ�׵�ַƫ��);
		�ṹ.β��ַ = pApi.readInteger(�ṹ.��ͼ��ַ + ��ͼβ��ַƫ��);
		�ṹ.�������� = (�ṹ.β��ַ - �ṹ.�׵�ַ) / 4;
		return true;
	}
	else {
		return false;
	}
}

bool BeastQuest::�������()
{
	�����ṹ �ṹ;
	bool res = ��ͼ����(�ṹ);
	if (!res) {
		printf("������ͼʧ��");
		return true;
	}
	for (int i = 0; i < �ṹ.��������; i++)
	{
		�ṹ.�����ַ = pApi.readInteger(�ṹ.�׵�ַ + 4 * i);
		if (�ṹ.�����ַ) {
			�ṹ.�������� = pApi.readInteger(�ṹ.�����ַ + ����ƫ��);
			�ṹ.������Ӫ = pApi.readInteger(�ṹ.�����ַ + ��Ӫƫ��);
			if (�ṹ.�������� == 289 && �ṹ.������Ӫ == 200) {
				Coordinate ���� = ��ȡ����(�ṹ.����ָ��);
				Send_���ʰȡ(Decode(�ṹ.�����ַ + 160), ����.x, ����.y);
				return false;
			}
		}
	}
	return true;
}

void BeastQuest::����()
{
	try {
		int ���ֵ�ַ;
		���ֵ�ַ = pApi.readInteger(���ֻ�ַ) + ����ƫ��;
		if (���ֵ�ַ) {
			if (Decode(���ֵ�ַ) < 102400) {
				Encode(���ֵ�ַ, 102400);
			}
		}
		else {
			printf("���ֵ�ַ ERROR");
		}
	}
	catch (...) {
		printf("���� ERROR");
	}

}

void BeastQuest::�������()
{
	�����ṹ �ṹ; Coordinate ��������, ��������;
	bool res = ��ͼ����(�ṹ);
	if (!res) {
		printf("������ͼʧ��");
		return;
	}
	for (int i = 0; i < �ṹ.��������; i++)
	{
		�ṹ.�����ַ = pApi.readInteger(�ṹ.�׵�ַ + 4 * i);
		if (�ṹ.�����ַ && �ṹ.�����ַ != �ṹ.����ָ��) {
			�ṹ.�������� = pApi.readInteger(�ṹ.�����ַ + ����ƫ��);
			�ṹ.������Ӫ = pApi.readInteger(�ṹ.�����ַ + ��Ӫƫ��);
			if ((�ṹ.�������� == 273 || �ṹ.�������� == 529)) {
				�ṹ.����Ѫ�� = pApi.readInteger(�ṹ.�����ַ + Ѫ��ƫ��);
				if (!(�ṹ.����Ѫ�� > 0))return;
				if (!(�ṹ.������Ӫ == 100 || �ṹ.������Ӫ == 101 || �ṹ.������Ӫ == 110 || �ṹ.������Ӫ == 50 || �ṹ.������Ӫ == 120))return;
				�������� = ��ȡ����(�ṹ.�����ַ);
				�������� = ��ȡ����(�ṹ.����ָ��);
				if (abs(��������.x - ��������.x) > 100 && abs(��������.y - ��������.y) > 100) {
					Send_����CALL(�ṹ.����ָ��, ��������.x, ��������.y, 0);
				}
				return;
			}
		}
	}

}

void BeastQuest::�����ͼ(int ����ID)
{

	try
	{
		DWORD ����ָ�� = pApi.readInteger(�����ַ);
		DWORD ��ʱƫ�� = pApi.readInteger(����ָ�� + 188);
		if (��ʱƫ�� == 0)
		{
			return;
		}
		DWORD ��ʱ���� = pApi.readInteger(�̵��ַ - 8);
		��ʱ���� = pApi.readInteger(��ʱ���� + ʱ���ַ);
		��ʱ���� = pApi.readInteger(��ʱ���� + ����˳ͼƫ��2);
		int ����ṹ = ��ʱ���� + (����ID + ����ID * 8) * 4 + ����˳ͼƫ��1 + ����ID * 4;
		int x, y, xF, yF, cx, cy;
		x = pApi.readInteger(����ṹ + 0);
		y = pApi.readInteger(����ṹ + 4);
		xF = pApi.readInteger(����ṹ + 8);
		yF = pApi.readInteger(����ṹ + 12);
		if (����ID == 0)
		{
			cx = x + xF + 20;
			cy = y + yF / 2;
		}
		if (����ID == 1)
		{
			cx = x - 20;
			cy = y + yF / 2;
		}
		if (����ID == 2)
		{
			cx = x + xF / 2;
			cy = y + yF + 20;
		}
		if (����ID == 3)
		{
			cx = x + xF / 2;
			cy = y - 20;
		}
		Send_����CALL(����ָ��, cx, cy, 0);
		Sleep(200);
		if (����ID == 1) {
			Send_����CALL(����ָ��, x + xF / 2 + 70, y, 0);
		}
		else {
			Send_����CALL(����ָ��, x + xF / 2, y, 0);
		}
		
	}
	catch (...)
	{
		//��ַ_->����(L"˳ͼCALL�����ˣ�");
		printf("˳ͼCALL�����ˣ�");
	}
}

void BeastQuest::���ܹ�ͼ()
{
	if (�Ƿ���BOSS��() == true)
	{
		return;
	}
	GetDirection GD;
	��ͼ���� Map_data = GD.Get_data_Map(0);
	int ���� = GD.Calculation_direction(Map_data.��ͼ�߷�[0], Map_data.��ͼ�߷�[1]);
	this->�����ͼ(����);
}

void BeastQuest::ȫ������()
{
	�����ṹ �ṹ;
	bool res = ��ͼ����(�ṹ);
	if (!res) {
		printf("������ͼʧ��");
		return;
	}
	for (int i = 0; i < �ṹ.��������; i++)
	{
		�ṹ.�����ַ = pApi.readInteger(�ṹ.�׵�ַ + 4 * i);
		if (�ṹ.�����ַ && �ṹ.�����ַ != �ṹ.����ָ��) {
			�ṹ.�������� = pApi.readInteger(�ṹ.�����ַ + ����ƫ��);
			�ṹ.������Ӫ = pApi.readInteger(�ṹ.�����ַ + ��Ӫƫ��);
			if ((�ṹ.�������� == 273 || �ṹ.�������� == 529)) {
				�ṹ.����Ѫ�� = pApi.readInteger(�ṹ.�����ַ + Ѫ��ƫ��);
				if (!(�ṹ.����Ѫ�� > 0))return;
				if (!(�ṹ.������Ӫ == 100 || �ṹ.������Ӫ == 101 || �ṹ.������Ӫ == 110 || �ṹ.������Ӫ == 50 || �ṹ.������Ӫ == 120))return;
				Coordinate �������� = ��ȡ����(�ṹ.�����ַ);
				Send_����CALL(�ṹ.����ָ��, 26484, 113, ��������.x, ��������.y, ��������.z, 0);
				return;
			}
		}
	}
}

bool BeastQuest::�Ƿ��й���()
{
	�����ṹ �ṹ;
	bool res = ��ͼ����(�ṹ);
	if (!res) {
		printf("������ͼʧ��");
		return false;
	}
	for (int i = 0; i < �ṹ.��������; i++)
	{
		�ṹ.�����ַ = pApi.readInteger(�ṹ.�׵�ַ + 4 * i);
		if (�ṹ.�����ַ && �ṹ.�����ַ != �ṹ.����ָ��) {
			�ṹ.�������� = pApi.readInteger(�ṹ.�����ַ + ����ƫ��);
			�ṹ.������Ӫ = pApi.readInteger(�ṹ.�����ַ + ��Ӫƫ��);
			if ((�ṹ.�������� == 273 || �ṹ.�������� == 529)) {
				�ṹ.����Ѫ�� = pApi.readInteger(�ṹ.�����ַ + Ѫ��ƫ��);
				if (�ṹ.����Ѫ�� > 0) {
					if (�ṹ.������Ӫ == 100 || �ṹ.������Ӫ == 101 || �ṹ.������Ӫ == 110 || �ṹ.������Ӫ == 50 || �ṹ.������Ӫ == 120) {
						return true;
					}
				}

			}
		}
	}
	return false;
}

bool  BeastQuest::ȡͨ���̵�()
{
	�����ṹ �ṹ;
	bool res = ��ͼ����(�ṹ);
	if (!res) {
		printf("������ͼʧ��");
		return false;
	}
	for (int i = 0; i < �ṹ.��������; i++)
	{
		�ṹ.�����ַ = pApi.readInteger(�ṹ.�׵�ַ + 4 * i);
		if (�ṹ.�����ַ && �ṹ.�����ַ != �ṹ.����ָ��) {
			�ṹ.�������� = pApi.readInteger(�ṹ.�����ַ + ����ƫ��);
			�ṹ.������Ӫ = pApi.readInteger(�ṹ.�����ַ + ��Ӫƫ��);
			if (�ṹ.������Ӫ == 200 || �ṹ.�������� == 33) {
				if (pApi.readInteger(�ṹ.�����ַ + 1020) == 48026) {
					return true;
				}
			}
		}
	}
	return false;
}

void BeastQuest::ͼ�ڲ���()
{
	if (�Ƿ���()) {
		if (�������()) {
			if (�Ƿ���BOSS��()) {
				if (!�Ƿ��й���() && �Ƿ���()) {
					Send_�������(0, 0);
					Sleep(200);
					ģ�ⰴ��(VK_ESCAPE);
					Sleep(500);
					if (ȡͨ���̵�()) {
						ģ�ⰴ��(VK_ESCAPE);
						Sleep(200);
						Send_����ٴ�();
						_ȫ�ֱ���.ˢͼ�ƴ� = _ȫ�ֱ���.ˢͼ�ƴ� + 1;
						printf(" %d \n", _ȫ�ֱ���.ˢͼ�ƴ�);
						Sleep(4000);
						return;
					}

				}
			}
			else {
				���ܹ�ͼ();
				Sleep(2000);
				return;
			}
		}
	}
	else {
		//printf("12313");
		//�������();
		//ȫ������();
	}
	return;
}

