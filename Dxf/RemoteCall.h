#pragma once
void Send_���ʰȡ(int ��Ʒ��ַ, int x, int y, bool is_send = true);

void Send_ѡ���ɫ1(bool is_send = true);

void Send_ѡ���ɫ2(int λ��, bool is_send = true);

void Send_���ؽ�ɫ(bool is_send = true);

void Send_����CALL(int ����ָ��, int x, int y, int z, bool is_send = true);

void Send_���зɻ�(int MaxMapId, int MinMapId, int x, int y, bool is_send = true);

void Send_�������(int λ������, int ���� = 1, bool is_send = true);

void Send_�������(int ��ƷID, bool is_send = true);

void Send_����ֽ�(int λ������, bool is_send = true);

void Send_�������(int x, int y, bool is_send = true);

void Send_�����ͼ(int ��ͼ��Ϣ, int ��ͼ�Ѷ�, int ��ͼ��Ԩ = 0, int ��ͼ��ϰ = 0, bool is_send = true);

void Send_Զ�Ž�ͼ(int ��ͼ��Ϣ, int ��ͼ�Ѷ�, bool is_send = true);

void Send_���ѡͼ(bool is_send = true);

void Send_����س�(bool is_send = true);

void Send_����ٴ�(bool is_send = true);

void Send_��ȡ����(bool is_send = true);

void Send_��������(bool is_send = true);

void Send_������(int ����, bool is_send = true);

void Send_�ͷ�CALL(int ��������, int ����, int �˺�, int x, int y, int z, bool is_send = true);

void Send_�������(int ��ͼID, int ����ID, bool is_send = true);

void Send_����CALL(int ָ��, int ����, int �˺�, int x, int y, int z, int ��С = 1, bool is_send = true);

void Send_�������(int ����ID, bool is_send = true);

void Send_������(int ����ID, bool is_send = true);

void Send_����ύ(int ����ID, bool is_send = true);

void Send_����CALL(int ����ID, bool is_send = true);

void Send_�������(bool is_send = true);

void RemoteCall(const char * name, LPCVOID fun, LPCVOID params, int p_size, int size, bool is_send);

void InitRemoteCall();

bool ReHookMsg();
