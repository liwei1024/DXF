#include "stdafx.h"
#include "Functions.h"
#include "RemoteCall.h"

int Decode(int addr)
{
	int eax, esi, edx, i;
	eax = pApi.readInteger(addr);
	esi = pApi.readInteger(���ܻ�ַ);
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
			printf("���� encryptID ERROR");
			return;
		}
		deviationParam = pApi.readInteger(���ܻ�ַ);
		if (!deviationParam) {
			printf("���� deviationParam ERROR");
			return;
		}
		deviationParam = pApi.readInteger(deviationParam + (encryptID >> 16) * 4 + 36);
		if (!deviationParam) {
			printf("���� deviationParam2 ERROR");
			return;
		}
		deviationAddr = deviationParam + (encryptID & 0xFFFF) * 4 + 8468;
		deviationParam = pApi.readInteger(deviationAddr);
		if (!deviationParam) {
			printf("���� deviationParam3 ERROR");
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
		printf("���� ERROR");
	}

}

Coordinate ��ȡ����(int addr)
{
		Coordinate ����;
		����.x = (int)pApi.readFloat(addr + X����);
		����.y = (int)pApi.readFloat(addr + Y����);
		����.z = (int)pApi.readFloat(addr + Z����);
		return ����;
}

Coordinate ��ǰ����(void)
{
	Coordinate xy;
	int һ����ַ;
	һ����ַ = pApi.readInteger(pApi.readInteger(pApi.readInteger(������) + ʱ���ַ) + 204);
	xy.x = pApi.readInteger(һ����ַ + ��ǰ����X);
	xy.y = pApi.readInteger(һ����ַ + ��ǰ����Y);
	return xy;
	
}

Coordinate BOSS����(void)
{
	
		Coordinate yx;
		int һ����ַ;
		һ����ַ = pApi.readInteger(pApi.readInteger(pApi.readInteger(������) + ʱ���ַ) + 204);
		if (һ����ַ) {
			yx.x = Decode(һ����ַ + BOSS����X);
			yx.y = Decode(һ����ַ + BOSS����Y);
		}
		return yx;
}

bool �Ƿ���BOSS��(void)
{
	try {
		Coordinate current;
		Coordinate BOSS;
		current = ��ǰ����();
		BOSS = BOSS����();
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
		printf("BOSS���� ERROR\n");
	}
	return false;
}

bool ����װ������(����װ�� &װ��)
{
	װ��.������ַ = pApi.readInteger(������ַ);
	if (!װ��.������ַ)return false;
	װ��.װ����ַ = pApi.readInteger(װ��.������ַ + 88);
	if (!װ��.װ����ַ)return false;
	װ��.װ������ַ = װ��.װ����ַ + 36;
	if (!װ��.װ������ַ)return false;
	return true;
}

void װ������()
{
	����װ�� װ��;
	if (����װ������(װ��) == false) {
		printf("����װ������ ʧ��\n");
		return;
	}
	int װ����ַ, װ������, װ�����Ƶ�ַ;
	for (int i = 0; i < 56; i++)
	{
		װ����ַ = pApi.readInteger(װ��.װ������ַ + i * 4);
		if (װ����ַ != 0) {
			װ������ = pApi.readInteger(װ����ַ + 368); // 0��װ 1 ��װ 2��װ
			װ�����Ƶ�ַ = pApi.readInteger(װ����ַ + 36);
			char *װ������ = pApi.readString(װ�����Ƶ�ַ, 52);
			string str(װ������);
			if ((str.find("����", 0) == -1) && (str.find("����", 0) == -1) && (str.find("�", 0) == -1)) {
				if (װ������ == 0 || װ������ == 1 || װ������ == 2) {
					printf(">>> ���� װ������ %d װ������ %s \n", װ������, str.c_str());
					Send_�������(i + 9);
				}
				/*else if (װ������ == 2) {
					printf(">>> �ֽ� װ������ %d װ������ %s \n", װ������, str.c_str());
					Send_����ֽ�(i + 9);
				}*/
				Sleep(1000);
			}
			delete[]װ������;
		}
	}

	//Send_��������();
}

bool �Ƿ���()
{
	if (Decode(pApi.readInteger(pApi.readInteger(�����ַ) + ��ͼƫ��) + ����ƫ��) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool �������(����ṹ &����)
{
	����.����ָ�� = pApi.readInteger(�����ַ);
	if (!����.����ָ��) {
		return false;
	}
	����.�׵�ַ = pApi.readInteger(����.����ָ�� + 104);
	if (!����.�׵�ַ) {
		return false;
	}
	����.β��ַ = pApi.readInteger(����.����ָ�� + 108);
	if (!����.β��ַ) {
		return false;
	}
	����.�������� = (����.β��ַ - ����.�׵�ַ) / 4;
	return true;
}

void �������(int ����ID)
{
	Send_�������(����ID);
	Sleep(100);
	Send_������(����ID);
	Sleep(100);
	Send_����ύ(����ID);
}

bool �ύ����()
{
	����ṹ ����;
	if (!�������(����)) {
		printf("�������ʧ��\n");
	}
	for (int i = 0; i < ����.��������; i++)
	{
		����.�����ַ = pApi.readInteger(����.�׵�ַ + i * 4);
		����.�������� = pApi.readInteger(����.�����ַ + 308);
		if (����.�������� == 0) {
			����.����ID = pApi.readInteger(����.�����ַ);
			Send_����ύ(����.����ID);
		}
	}
}

bool ��������()
{
	����ṹ ����;
	if (!�������(����)) {
		printf("�������ʧ��\n");
	}
	for (int i = 0; i < ����.��������; i++)
	{
		����.�����ַ = pApi.readInteger(����.�׵�ַ + i * 4);
		����.�������� = pApi.readInteger(����.�����ַ + 308);
		����.�������� = pApi.readString(pApi.readInteger(����.�����ַ + 28)>7 ? pApi.readInteger(����.�����ַ + 8) : ����.�����ַ + 8, 50);
		if (����.�������� == 0) {
			����.���񸱱� = pApi.readInteger(pApi.readInteger(����.�����ַ + 676));
			����.�������� = pApi.readString(pApi.readInteger(����.�����ַ + 700), 100);
			����.����ID = pApi.readInteger(����.�����ַ);
			string ��������(����.��������);
			if (����.���񸱱�) {
				printf(">>>��ȡ���� ����ID: %d ���񸱱���%d ��������: %s �������ƣ�%s\n", ����.����ID, ����.���񸱱�, ����.��������, ����.��������);
				����_CALL(����.���񸱱�);
				Sleep(500);
				Send_���ѡͼ();
				Sleep(500);
				Send_�������(����.���񸱱�, ����.����ID);
				return true;
			}
			else if (
				��������.find("hunt enemy") != -1 ||
				��������.find("meet npc") != -1 ||
				��������.find("hunt monster") != -1 ||
				��������.find("seek n meet npc") != -1 ||
				��������.find("question") != -1 ||
				��������.find("quest clear") != -1 ||
				��������.find("reach the range") != -1 ||
				��������.find("look cinematic") != -1
				)
			{
				 
				 �������(����.����ID);
				 delete[]����.��������;
				 return true;
			}
			else {
				
				printf(">>>���ֶ���� ����ID: %x ��������: %s �������ƣ�%s\n", ����.����ID, ����.��������, ����.��������);
				return false;
			}
		}
		delete[]����.��������;
	}
	return true;
}

int ��ȡ��������ID()
{
	����ṹ ����;
	if (!�������(����)) {
		printf("�������ʧ��\n");
	}
	for (int i = 0; i < ����.��������; i++)
	{
		����.�����ַ = pApi.readInteger(����.�׵�ַ + i * 4);
		����.�������� = pApi.readInteger(����.�����ַ + 308);
		if (����.�������� == 0) {
			����.�������� = pApi.readString(pApi.readInteger(����.�����ַ + 28)>7 ? pApi.readInteger(����.�����ַ + 8) : ����.�����ַ + 8, 50);
			����.����ID = pApi.readInteger(����.�����ַ);
			printf("%s \n", ����.��������);
			delete[]����.��������;
			Send_�������(����.����ID);
			Sleep(100);
			Send_������(����.����ID);
			Sleep(100);
			Send_����ύ(����.����ID);
			return ����.����ID;
		}
		//
	}

}

void ����_CALL(int ����ID)//������
{
	int MaxMapId, MinMapId, x, y;
	Send_����CALL(����ID);
	MaxMapId = pApi.readInteger(pApi.readInteger(����ECX) + 52164);
	//printf("MaxMapId %d\n", MaxMapId);
	MinMapId = pApi.readInteger(pApi.readInteger(����ECX) + 52164 + 4);
	//printf("MinMapId %d\n", MinMapId);
	x = pApi.readInteger(pApi.readInteger(����ECX) + 52164 + 8);
	//printf("x %d\n", x);
	y = pApi.readInteger(pApi.readInteger(����ECX) + 52164 + 12);
	//printf("y %d\n", y);
	Send_���зɻ�(MaxMapId, MinMapId,x,y);
}

void ģ�ⰴ��(int keyCode, int s)
{
	s = s + (int)(rand() % 6);
	keybd_event(keyCode, MapVirtualKey(keyCode, 0), 0, 0);
	Sleep(s);
	keybd_event(keyCode, MapVirtualKey(keyCode, 0), KEYEVENTF_KEYUP, 0);
	Sleep(s);
}

int ȡ��ɫƣ��()
{
	return Decode(���ƣ��)- Decode(��ǰƣ��);
}

int ȡ��ɫ�ȼ�()
{
	return pApi.readInteger(��ɫ�ȼ�);
}