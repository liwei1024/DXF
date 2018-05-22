#include "stdafx.h"
#include "RemoteCall.h"
#include "BaseAddr.h"


#define ReturnAddr		0x400400
#define NewCodeAddr		0x400404
#define HookAddr		0x400408

int _HookAddr = 0;

void  __declspec(naked) NewCode()
{
	__asm
	{
		mov ecx, dword ptr[ebp + 0xC]
		cmp ecx, ��ϢID
		je newcode
		jmp originalcode
		newcode :
			mov ecx, dword ptr[ebp + 0x10]
			call ecx
			add esp, 0x8
			pop ebp
			ret
		originalcode :
			mov eax, fs : [00000000]
			push eax
			push ebx
			push esi
			mov ecx, ReturnAddr
			mov ecx, dword ptr[ecx]
			jmp ecx
	}
}



void __declspec(naked) SetHookAddr()
{
	__asm
	{
		mov eax, NewCodeAddr
		mov eax, dword ptr[eax]
		jmp eax
	}
}

void __declspec(naked) ReHook()
{
	__asm {
		mov eax, fs : [00000000]
		push eax
		push ebx
		push esi
	}
}

bool ReHookMsg() 
{
	return pApi.writeMemory(_HookAddr, ReHook, 0x9);
}

void HookWindowsMsg()
{

	// 1.д��hook����
	printf("���� _NewCodeAddr ...\n");
	int _NewCodeAddr = pApi.allocMemory("NewCodeAddr", 128);
	if (!_NewCodeAddr) {
		printf("���� _NewCodeAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Write HookCode ...\n");
	if (pApi.writeMemory(_NewCodeAddr, NewCode, 128) == false) {
		printf("Write _NewCodeAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Set NewCodeAddr ...\n");
	if (pApi.writeInteger(NewCodeAddr, _NewCodeAddr) == false) {
		printf("Write NewCodeAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Get HookAddr ...\n");
	_HookAddr = ToolsApi::getModleAddrByPid(pApi.ProcessId, L"CrossAdapter4DNF.dll") + 0x12DAA;//CrossAdapter4DNF.dll+0x12DAA
	if (!_HookAddr) {
		printf("Get hookAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Set HookAddr ...\n");
	if (pApi.writeInteger(HookAddr, _HookAddr) == false) {
		printf("Write HookAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Set ReturnAddr ...\n");
	if (pApi.writeInteger(ReturnAddr, _HookAddr + 0x9) == false) {
		printf("Write ReturnAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Set Hook ...\n");
	if (pApi.writeMemory(_HookAddr, SetHookAddr, 0x9) == false) {
		printf("SetHookAddr ʧ��!\n");
		system("pause");
		exit(1);
	}
	printf("Ŷ����Ĳʺ缦��K����������\n");
}

//=======================================================================

#define Asm_����CALL(cdov){\
	__asm {\
		__asm push cdov\
		__asm mov ecx, ������ַ\
		__asm mov ecx, [ecx]\
		__asm mov eax, ����CALL\
		__asm call eax\
	}\
}

#define Asm_�ܰ�CALL(param,len,type){\
	int cdov = 0;\
	if (len == 1)cdov = �ܰ�CALL;\
	if (len == 2)cdov = �ܰ�CALL + 0x30;\
	if (len == 3)cdov = �ܰ�CALL + 0x60;\
	if (len == 4)cdov = �ܰ�CALL + 0x90;\
	if(type == true){\
		__asm {\
			__asm mov eax, param\
			__asm push dword ptr[eax]\
			__asm mov ecx, ������ַ\
			__asm mov ecx, dword ptr ss : [ecx]\
			__asm mov eax, cdov\
			__asm call eax\
		}\
	}else {\
		__asm {\
			__asm push param\
			__asm mov ecx, ������ַ\
			__asm mov ecx, dword ptr ss : [ecx]\
			__asm mov eax, cdov\
			__asm call eax\
		}\
	}\
}

#define Asm_��ԿCALL(bytes,len){\
	vector<byte> b = bytes;\
	int addr = pApi.allocMemory("Asm_��ԿCALL",len);\
	pApi.writeMemory(addr, bytes, len);\
	__asm {\
		__asm push len\
		__asm push addr\
		__asm call ��ԿCALL\
	}\
}

#define Asm_����CALL(){\
	__asm {\
		__asm mov eax, ����CALL\
		__asm call eax\
	}\
}

void __declspec(naked) Asm_���ʰȡ()
{
	Asm_����CALL(43);
	Asm_�ܰ�CALL(������ַ, 3, true);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_�ܰ�CALL(2, 1, false);
	Asm_�ܰ�CALL(������ַ + 0x4, 2, true);
	Asm_�ܰ�CALL(������ַ + 0x8, 2, true);
	Asm_�ܰ�CALL(0, 2, false);
	Asm_�ܰ�CALL(������ַ + 0xc, 2, true);
	Asm_�ܰ�CALL(������ַ + 0x10, 2, true);
	Asm_�ܰ�CALL(0, 2, false);
	Asm_�ܰ�CALL(0, 2, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_ѡ���ɫ1()
{
	Asm_����CALL(7);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_ѡ���ɫ2()
{
	Asm_����CALL(4);
	Asm_�ܰ�CALL(������ַ, 3, true);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_���ؽ�ɫ()
{
	Asm_����CALL(0x7);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_����CALL()
{
	__asm {
		mov eax, ������ַ
		mov esi, dword ptr[eax]
		mov edi, dword ptr[esi]
		push dword ptr[eax + 0x4]
		push dword ptr[eax + 0x8]
		push dword ptr[eax + 0xc]
		mov eax, dword ptr[edi + 000000A8h]
		mov ecx, esi
		call eax
		ret
	}
}

void __declspec(naked) Asm_���зɻ�()
{
	Asm_����CALL(36);
	Asm_�ܰ�CALL(������ַ, 1, true);
	Asm_�ܰ�CALL(������ַ + 0x4, 1, true);
	Asm_�ܰ�CALL(������ַ + 0x8, 2, true);
	Asm_�ܰ�CALL(������ַ + 0xc, 2, true);
	Asm_�ܰ�CALL(5, 1, false);
	Asm_�ܰ�CALL(9, 2, false);
	Asm_�ܰ�CALL(2, 2, false);
	Asm_�ܰ�CALL(0, 3, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_�������()
{
	/*Asm_����CALL(22);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_�ܰ�CALL(������ַ + 0x0, 2, true);//λ��
	Asm_�ܰ�CALL(������ַ + 0x4, 3, true);//����
	Asm_�ܰ�CALL(1, 3, false);			  //1000 �̵�id
	Asm_�ܰ�CALL(������ַ + 0x8, 3, true);
	Asm_����CALL();*/

	Asm_����CALL(594)
	Asm_�ܰ�CALL(2, 3, false)
	Asm_�ܰ�CALL(0, 3, false)
	Asm_����CALL()
	Asm_����CALL(22)
	Asm_�ܰ�CALL(0, 1, false)
	Asm_�ܰ�CALL(������ַ + 0x0, 2, true)
	Asm_�ܰ�CALL(������ַ + 0x4, 3, true)
	Asm_�ܰ�CALL(1, 3, false)
	Asm_�ܰ�CALL(������ַ + 0x8, 3, true)
	Asm_����CALL()
	__asm ret
}

void Asm_�������()
{
	Asm_����CALL(21)
	Asm_�ܰ�CALL(������ַ + 0x0, 3, true)//��ƷID
	Asm_�ܰ�CALL(43209, 3, false)
	Asm_�ܰ�CALL(2, 3, false)
	Asm_�ܰ�CALL(1, 3, false)
	Asm_����CALL()
}

void __declspec(naked) Asm_����ֽ�()
{
	Asm_����CALL(26);
	Asm_�ܰ�CALL(������ַ + 0x0, 2, true);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_�ܰ�CALL(65535, 2, false);
	Asm_�ܰ�CALL(319, 3, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_�������()
{
	Asm_����CALL(69);
	Asm_����CALL();
	Asm_����CALL(70);
	Asm_����CALL();
	Asm_����CALL(71);
	Asm_�ܰ�CALL(������ַ + 0x0, 1, true);
	Asm_�ܰ�CALL(������ַ + 0x4, 1, true);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_�����ͼ()
{
	Asm_����CALL(16);
	Asm_�ܰ�CALL(������ַ + 0x0, 3, true);
	Asm_�ܰ�CALL(������ַ + 0x4, 1, true);
	Asm_�ܰ�CALL(0, 2, false);
	Asm_�ܰ�CALL(������ַ + 0x8, 1, true);
	Asm_�ܰ�CALL(������ַ + 0xc, 1, true);
	Asm_�ܰ�CALL(65535, 2, false);
	Asm_�ܰ�CALL(0, 3, false);
	Asm_�ܰ�CALL(0, 3, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_Զ�Ž�ͼ()
{
	__asm {
		push 16
		call buffer
		mov eax, ������ַ
		push dword ptr[eax] // ��ͼ��Ϣ
		call encrypt_60
		push 2
		call encrypt
		mov eax, ������ַ
		push dword ptr[eax + 0x4] // ��ͼ�Ѷ�
		call encrypt
		push 0
		call encrypt
		push 0
		call encrypt
		push 65535
		call encrypt_30
		push 0
		call encrypt_60
		push 0
		call encrypt_60
		push 0
		call encrypt
		call sendpacket
		ret
	}
	__asm {
	buffer:
		push dword ptr[esp + 4]
			mov ecx, ������ַ
			mov ecx, dword ptr[ecx]
			mov eax, ����CALL
			call eax
			ret 4
			encrypt :
		push dword ptr[esp + 4]
			mov ecx, ������ַ
			mov ecx, dword ptr[ecx]
			mov eax, �ܰ�CALL
			call eax
			ret 4
			encrypt_30 :
		push dword ptr[esp + 4]
			mov ecx, ������ַ
			mov ecx, dword ptr[ecx]
			mov eax, �ܰ�CALL
			add eax, 0x30
			call eax
			ret 4
			encrypt_60 :
		push dword ptr[esp + 4]
			mov ecx, ������ַ
			mov ecx, dword ptr[ecx]
			mov eax, �ܰ�CALL
			add eax, 0x60
			call eax
			ret 4
			sendpacket:
		mov eax, ����CALL
			call eax
			ret
	}
}

void __declspec(naked) Asm_���ѡͼ()
{
	Asm_����CALL(15);
	Asm_�ܰ�CALL(0, 3, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_����س�()
{
	Asm_����CALL(42);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_����ٴ�()
{
	Asm_����CALL(72);
	Asm_�ܰ�CALL(1, 1, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_��ȡ����()
{
	Asm_����CALL(603);
	Asm_�ܰ�CALL(0, 3, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_��������()
{
	Asm_����CALL(20);
	Asm_�ܰ�CALL(6, 3, false);
	Asm_�ܰ�CALL(16, 1, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_�ܰ�CALL(24, 1, false);
	Asm_�ܰ�CALL(1, 1, false);
	Asm_�ܰ�CALL(32, 1, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_������()
{
	Asm_����CALL(307);
	Asm_�ܰ�CALL(������ַ, 3, true);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_�������()
{
	Asm_����CALL(16)
	Asm_�ܰ�CALL(������ַ, 3, true)//��ͼID
	Asm_�ܰ�CALL(0, 1, false)//��ͼ�Ѷ�
	Asm_�ܰ�CALL(0, 2, false)
	Asm_�ܰ�CALL(0, 1, false)
	Asm_�ܰ�CALL(0, 1, false)
	Asm_�ܰ�CALL(65535, 2, false)
	Asm_�ܰ�CALL(0, 3, false)
	Asm_�ܰ�CALL(0, 1, false)
	Asm_�ܰ�CALL(������ַ + 0x4, 3, true)//����id
	Asm_����CALL()
	__asm ret
}

void __declspec(naked) Asm_�ͷ�CALL()
{
	__asm {

		mov eax, ������ַ
		push dword ptr[eax] //z
		push dword ptr[eax + 0x4]//y
		push dword ptr[eax + 0x8]//x
		push dword ptr[eax + 0xc]//�˺�
		push dword ptr[eax + 0x10]//����
		mov eax, dword ptr ds : [eax + 14]
		push dword ptr ds : [eax]
		mov eax, �ͷ�CALL
		call eax
		add esp, 0x18
		ret
	}
}

void __declspec(naked) Asm_����CALL()
{
	__asm {
		mov eax, ������ַ
		mov esi, ����CALL
		call esi
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		retn
	}
}

void __declspec(naked) Asm_�������()
{
	Asm_����CALL(31);
	Asm_�ܰ�CALL(31, 2, false);
	Asm_�ܰ�CALL(������ַ, 2, true);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_������()
{
	Asm_����CALL(33);
	Asm_�ܰ�CALL(33, 2, false);
	Asm_�ܰ�CALL(������ַ, 2, true);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_�ܰ�CALL(0, 1, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_����ύ()
{
	Asm_����CALL(34);
	Asm_�ܰ�CALL(34, 2, false);
	Asm_�ܰ�CALL(������ַ, 2, true);
	Asm_�ܰ�CALL(-1, 2, false);
	Asm_�ܰ�CALL(1, 2, false);
	Asm_�ܰ�CALL(65535, 2, false);
	Asm_����CALL();
	__asm ret
}

void __declspec(naked) Asm_����CALL()
{
	__asm {
		pushad
		mov eax, ������ַ
		push dword ptr[eax] //z
		push - 1
		mov ecx, ����ECX
		mov ecx, dword ptr[ecx]
		mov eax, ����CALL
		call eax
		popad
		ret
	}
}


void Asm_�������()
{
	Asm_����CALL(599)
	Asm_�ܰ�CALL(0, 1, false)
	Asm_�ܰ�CALL(11, 3, false)
	Asm_�ܰ�CALL(2587, 2, false)
	Asm_�ܰ�CALL(266, 2, false)
	Asm_�ܰ�CALL(65535, 2, false)
	Asm_����CALL()
}

void Send_���ʰȡ(int ��Ʒ��ַ, int x, int y, bool is_send)
{
	struct
	{
		int ��Ʒ��ַ;
		int ����x;
		int ����y;
		int ��Ʒx;
		int ��Ʒy;
	}params;
	params.��Ʒ��ַ = ��Ʒ��ַ;
	params.����x = x;
	params.����y = y;
	params.��Ʒx = x + (int)(rand() % 6);
	params.��Ʒy = y + (int)(rand() % 6);
	RemoteCall(__FUNCTION__, Asm_���ʰȡ, &params, sizeof(params), 1024, is_send);
}

void Send_ѡ���ɫ1(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_ѡ���ɫ1, NULL, 0, 1024, is_send);
}

void Send_ѡ���ɫ2(int λ��, bool is_send)
{
	struct
	{
		int λ��;
	}params;
	params.λ�� = λ��;
	RemoteCall(__FUNCTION__, Asm_ѡ���ɫ2, &params, sizeof(params), 1024, is_send);
}

void Send_���ؽ�ɫ(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_���ؽ�ɫ, NULL, 0, 1024, is_send);
}

void Send_����CALL(int ����ָ��, int x, int y, int z, bool is_send)
{
	struct
	{
		int ��Ʒ��ַ;
		int z;
		int y;
		int x;
	}params;
	if (x > 300) {
		x = x - 100;
	}
	else {
		x = x + 100;
	}
	params.��Ʒ��ַ = ����ָ��;
	params.z = z;
	params.y = y;
	params.x = x;
	RemoteCall(__FUNCTION__, Asm_����CALL, &params, sizeof(params), 1024, is_send);
}

void Send_���зɻ�(int MaxMapId, int MinMapId, int x, int y, bool is_send)
{
	struct
	{
		int MaxMapId;
		int MinMapId;
		int x;
		int y;
	}params;
	params.MaxMapId = MaxMapId;
	params.MinMapId = MinMapId;
	params.x = x;
	params.y = y;
	RemoteCall(__FUNCTION__, Asm_���зɻ�, &params, sizeof(params), 1024, is_send);
}

void Send_�������(int λ������, int ����, bool is_send)
{
	struct
	{
		int λ������;
		int ����;
		int λ�ü�������;
	}params;
	params.λ������ = λ������;
	params.���� = ����;
	params.λ�ü������� = λ������ * 2 + ���� * 2 + 1;
	RemoteCall(__FUNCTION__, Asm_�������, &params, sizeof(params), 1024, is_send);
}

void Send_�������(int ��ƷID, bool is_send)
{
	struct
	{
		int ��ƷID;
	}params;
	params.��ƷID = ��ƷID;
	RemoteCall(__FUNCTION__, Asm_�������, &params, sizeof(params), 1024, is_send);
}

void Send_����ֽ�(int λ������, bool is_send)
{
	struct
	{
		int λ������;
	}params;
	params.λ������ = λ������;
	RemoteCall(__FUNCTION__, Asm_����ֽ�, &params, sizeof(params), 1024, is_send);
}

void Send_�������(int x, int y, bool is_send)
{
	struct
	{
		int x;
		int y;
	}params;
	params.x = x;
	params.y = y;
	RemoteCall(__FUNCTION__, Asm_�������, &params, sizeof(params), 1024, is_send);
}

void Send_�����ͼ(int ��ͼ��Ϣ, int ��ͼ�Ѷ�, int ��ͼ��Ԩ, int ��ͼ��ϰ, bool is_send)
{
	struct
	{
		int ��ͼ��Ϣ;
		int ��ͼ�Ѷ�;
		int ��ͼ��Ԩ;
		int ��ͼ��ϰ;
	}params;
	params.��ͼ��Ϣ = ��ͼ��Ϣ;
	params.��ͼ�Ѷ� = ��ͼ�Ѷ�;
	params.��ͼ��Ԩ = ��ͼ��Ԩ;
	params.��ͼ��ϰ = ��ͼ��ϰ;
	RemoteCall(__FUNCTION__, Asm_�����ͼ, &params, sizeof(params), 1024, is_send);
}

void Send_Զ�Ž�ͼ(int ��ͼ��Ϣ, int ��ͼ�Ѷ�, bool is_send)
{
	struct
	{
		int ��ͼ��Ϣ;
		int ��ͼ�Ѷ�;
	}params;
	params.��ͼ��Ϣ = ��ͼ��Ϣ;
	params.��ͼ�Ѷ� = ��ͼ�Ѷ�;
	RemoteCall(__FUNCTION__, Asm_Զ�Ž�ͼ, &params, sizeof(params), 1024, is_send);
}

void Send_���ѡͼ(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_���ѡͼ, NULL, 0, 1024, is_send);
}

void Send_����س�(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_����س�, NULL, 0, 1024, is_send);
}

void Send_����ٴ�(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_����ٴ�, NULL, 0, 1024, is_send);
}

void Send_��ȡ����(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_��ȡ����, NULL, 0, 1024, is_send);
}

void Send_��������(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_��������, NULL, 0, 1024, is_send);
}

void Send_������(int ����, bool is_send)
{
	struct
	{
		int ����;
	}params;
	params.���� = ����;
	RemoteCall(__FUNCTION__, Asm_������, &params, sizeof(params), 1024, is_send);
}

void Send_�ͷ�CALL(int ��������, int ����, int �˺�, int x, int y, int z, bool is_send) {
	struct
	{
		int z;
		int y;
		int x;
		int �˺�;
		int ����;
		int ��������;
	}params;
	params.z = z;
	params.y = y;
	params.x = x;
	params.�˺� = �˺�;
	params.���� = ����;
	params.�������� = ��������;
	RemoteCall(__FUNCTION__, Asm_�ͷ�CALL, &params, sizeof(params), 1024, is_send);
}

void Send_�������(int ��ͼID,  int ����ID, bool is_send)
{
	struct
	{
		int ��ͼID;
		int ����ID;
	}params;
	params.��ͼID = ��ͼID;
	params.����ID = ����ID;
	RemoteCall(__FUNCTION__, Asm_�������, &params, sizeof(params), 1024, is_send);
}

void Send_����CALL(int ָ��, int ����, int �˺�, int x, int y, int z, int ��С, bool is_send) {

	int ���ܽṹ[23] = { 0 };
	���ܽṹ[0] = ָ��;//ָ��
	���ܽṹ[2] = ����;
	���ܽṹ[3] = �˺�;
	���ܽṹ[6] = x;
	���ܽṹ[7] = y;
	���ܽṹ[8] = z;
	���ܽṹ[22] = ��С;
	RemoteCall(__FUNCTION__, Asm_����CALL, &���ܽṹ, sizeof(���ܽṹ), 1024, is_send);
}

void Send_�������(int ����ID, bool is_send)
{
	struct
	{
		int ����ID;
	}params;
	params.����ID = ����ID;
	RemoteCall(__FUNCTION__, Asm_�������, &params, sizeof(params), 1024, is_send);
}

void Send_������(int ����ID, bool is_send)
{
	struct
	{
		int ����ID;
	}params;
	params.����ID = ����ID;
	RemoteCall(__FUNCTION__, Asm_������, &params, sizeof(params), 1024, is_send);
}

void Send_����ύ(int ����ID, bool is_send)
{
	struct
	{
		int ����ID;
	}params;
	params.����ID = ����ID;
	RemoteCall(__FUNCTION__, Asm_����ύ, &params, sizeof(params), 1024, is_send);
}

void Send_����CALL(int ����ID, bool is_send)
{
	struct
	{
		int ����ID;
	}params;
	params.����ID = ����ID;
	RemoteCall(__FUNCTION__, Asm_����CALL, &params, sizeof(params), 1024, is_send);
}

void Send_�������(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_�������, NULL, 0, 1024, is_send);
}



void RemoteCall(const char * name, LPCVOID fun, LPCVOID params, int p_size, int size, bool is_send)
{
	if (params != NULL) {
		pApi.writeMemory(������ַ, params, p_size);
	}
	int addr = pApi.allocMemory(name, size);
	if (!addr) {
		printf("%s ʧ��!\n", name);
		system("pause");
		exit(1);
	}
	if (pApi.writeMemory(addr, fun, size) == false) {
		printf("%s WriteMemory ʧ��!\n", name);
		system("pause");
		exit(1);
	}
	if (is_send) {
		SendMessage(pApi.hWnd, ��ϢID, addr, 0);
	}
}

void InitRemoteCall()
{
	HookWindowsMsg();

	Send_���ʰȡ(0, 0, 0, false);

	Send_ѡ���ɫ1(false);

	Send_ѡ���ɫ2(0, false);

	Send_���ؽ�ɫ(false);

	Send_����CALL(0, 0, 0, 0, false);

	Send_���зɻ�(0, 0, 0, 0, false);

	Send_�������(0, 0, false);

	Send_����ֽ�(0, false);

	Send_�������(0, 0, false);

	Send_�����ͼ(0, 0, 0, 0, false);

	Send_Զ�Ž�ͼ(0, 0, false);

	Send_���ѡͼ(false);

	Send_����س�(false);

	Send_����ٴ�(false);

	Send_��ȡ����(false);

	Send_��������(false);

	Send_������(0, false);

	Send_�������(0, 0, false);

	Send_�ͷ�CALL(0, 0, 0, 0, 0, 0, false);

	Send_����CALL(0, 0, 0, 0, 0, 0, 0, false);

	Send_�������(0, false);

	Send_������(0, false);

	Send_����ύ(0, false);

	Send_����CALL(0, false);

	Send_�������(0, false);

	Send_�������(false);
}