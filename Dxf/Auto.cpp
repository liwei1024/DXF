#include "stdafx.h"
#include "Auto.h"
#include "RemoteCall.h"
#include "Functions.h"
#include "BeastQuest.h"

void �Զ�����()
{
	int status = pApi.readInteger(��Ϸ״̬);
	if (status == 0) {
		Send_ѡ���ɫ2(_ȫ�ֱ���.��ǰ��ɫ + 1);
		Sleep(3000);
		return;
	}
	else if (status == 1) {
		if (ȡ��ɫƣ��() == 0 || ȡ��ɫ�ȼ�() == 90) {
			_ȫ�ֱ���.��ǰ��ɫ += 1;
			Send_���ؽ�ɫ();
			Sleep(2000);
		}
		else {
			if (��������()) {
				while (pApi.readInteger(��Ϸ״̬) != 3)
				{
					Sleep(1000);
				}
			}
			else {
				ȫ�ֿ���.�Զ����鿪�� = false;
			}
			return;
		}
	}
	else if (status == 3) {
		BeastQuest BQ;
		if (�Ƿ���() && BQ.�������()) {
			if (�Ƿ���BOSS��()) {
				if (!BQ.�Ƿ��й���() && �Ƿ���()) {
					Send_�������(0, 0);
					Sleep(200);
					ģ�ⰴ��(VK_ESCAPE);
					Sleep(500);
					if (BQ.ȡͨ���̵�()) {
						ģ�ⰴ��(VK_ESCAPE);
						Sleep(1000);
						_ȫ�ֱ���.ˢͼ�ƴ� = _ȫ�ֱ���.ˢͼ�ƴ� + 1;
						printf(" %d \n", _ȫ�ֱ���.ˢͼ�ƴ�);
						Send_����س�();
						return;
					}
				}
			}
			else {
				BQ.���ܹ�ͼ();
				Sleep(1000);
				return;
			}
		}
		else {
			// ���
			return;
		}
	}
}

void �Զ�ˢͼ() 
{

}

