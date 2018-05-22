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
		cmp ecx, 消息ID
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

	// 1.写入hook代码
	printf("申请 _NewCodeAddr ...\n");
	int _NewCodeAddr = pApi.allocMemory("NewCodeAddr", 128);
	if (!_NewCodeAddr) {
		printf("申请 _NewCodeAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Write HookCode ...\n");
	if (pApi.writeMemory(_NewCodeAddr, NewCode, 128) == false) {
		printf("Write _NewCodeAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Set NewCodeAddr ...\n");
	if (pApi.writeInteger(NewCodeAddr, _NewCodeAddr) == false) {
		printf("Write NewCodeAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Get HookAddr ...\n");
	_HookAddr = ToolsApi::getModleAddrByPid(pApi.ProcessId, L"CrossAdapter4DNF.dll") + 0x12DAA;//CrossAdapter4DNF.dll+0x12DAA
	if (!_HookAddr) {
		printf("Get hookAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Set HookAddr ...\n");
	if (pApi.writeInteger(HookAddr, _HookAddr) == false) {
		printf("Write HookAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Set ReturnAddr ...\n");
	if (pApi.writeInteger(ReturnAddr, _HookAddr + 0x9) == false) {
		printf("Write ReturnAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("Set Hook ...\n");
	if (pApi.writeMemory(_HookAddr, SetHookAddr, 0x9) == false) {
		printf("SetHookAddr 失败!\n");
		system("pause");
		exit(1);
	}
	printf("哦尼玛的彩虹鸡巴K！！！！！\n");
}

//=======================================================================

#define Asm_缓冲CALL(cdov){\
	__asm {\
		__asm push cdov\
		__asm mov ecx, 发包基址\
		__asm mov ecx, [ecx]\
		__asm mov eax, 缓冲CALL\
		__asm call eax\
	}\
}

#define Asm_密包CALL(param,len,type){\
	int cdov = 0;\
	if (len == 1)cdov = 密包CALL;\
	if (len == 2)cdov = 密包CALL + 0x30;\
	if (len == 3)cdov = 密包CALL + 0x60;\
	if (len == 4)cdov = 密包CALL + 0x90;\
	if(type == true){\
		__asm {\
			__asm mov eax, param\
			__asm push dword ptr[eax]\
			__asm mov ecx, 发包基址\
			__asm mov ecx, dword ptr ss : [ecx]\
			__asm mov eax, cdov\
			__asm call eax\
		}\
	}else {\
		__asm {\
			__asm push param\
			__asm mov ecx, 发包基址\
			__asm mov ecx, dword ptr ss : [ecx]\
			__asm mov eax, cdov\
			__asm call eax\
		}\
	}\
}

#define Asm_密钥CALL(bytes,len){\
	vector<byte> b = bytes;\
	int addr = pApi.allocMemory("Asm_密钥CALL",len);\
	pApi.writeMemory(addr, bytes, len);\
	__asm {\
		__asm push len\
		__asm push addr\
		__asm call 密钥CALL\
	}\
}

#define Asm_发包CALL(){\
	__asm {\
		__asm mov eax, 发包CALL\
		__asm call eax\
	}\
}

void __declspec(naked) Asm_组包拾取()
{
	Asm_缓冲CALL(43);
	Asm_密包CALL(参数地址, 3, true);
	Asm_密包CALL(0, 1, false);
	Asm_密包CALL(2, 1, false);
	Asm_密包CALL(参数地址 + 0x4, 2, true);
	Asm_密包CALL(参数地址 + 0x8, 2, true);
	Asm_密包CALL(0, 2, false);
	Asm_密包CALL(参数地址 + 0xc, 2, true);
	Asm_密包CALL(参数地址 + 0x10, 2, true);
	Asm_密包CALL(0, 2, false);
	Asm_密包CALL(0, 2, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_选择角色1()
{
	Asm_缓冲CALL(7);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_选择角色2()
{
	Asm_缓冲CALL(4);
	Asm_密包CALL(参数地址, 3, true);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_返回角色()
{
	Asm_缓冲CALL(0x7);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_坐标CALL()
{
	__asm {
		mov eax, 参数地址
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

void __declspec(naked) Asm_城市飞机()
{
	Asm_缓冲CALL(36);
	Asm_密包CALL(参数地址, 1, true);
	Asm_密包CALL(参数地址 + 0x4, 1, true);
	Asm_密包CALL(参数地址 + 0x8, 2, true);
	Asm_密包CALL(参数地址 + 0xc, 2, true);
	Asm_密包CALL(5, 1, false);
	Asm_密包CALL(9, 2, false);
	Asm_密包CALL(2, 2, false);
	Asm_密包CALL(0, 3, false);
	Asm_密包CALL(0, 1, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包卖物()
{
	/*Asm_缓冲CALL(22);
	Asm_密包CALL(0, 1, false);
	Asm_密包CALL(参数地址 + 0x0, 2, true);//位置
	Asm_密包CALL(参数地址 + 0x4, 3, true);//数量
	Asm_密包CALL(1, 3, false);			  //1000 商店id
	Asm_密包CALL(参数地址 + 0x8, 3, true);
	Asm_发包CALL();*/

	Asm_缓冲CALL(594)
	Asm_密包CALL(2, 3, false)
	Asm_密包CALL(0, 3, false)
	Asm_发包CALL()
	Asm_缓冲CALL(22)
	Asm_密包CALL(0, 1, false)
	Asm_密包CALL(参数地址 + 0x0, 2, true)
	Asm_密包CALL(参数地址 + 0x4, 3, true)
	Asm_密包CALL(1, 3, false)
	Asm_密包CALL(参数地址 + 0x8, 3, true)
	Asm_发包CALL()
	__asm ret
}

void Asm_组包买物()
{
	Asm_缓冲CALL(21)
	Asm_密包CALL(参数地址 + 0x0, 3, true)//物品ID
	Asm_密包CALL(43209, 3, false)
	Asm_密包CALL(2, 3, false)
	Asm_密包CALL(1, 3, false)
	Asm_发包CALL()
}

void __declspec(naked) Asm_组包分解()
{
	Asm_缓冲CALL(26);
	Asm_密包CALL(参数地址 + 0x0, 2, true);
	Asm_密包CALL(0, 1, false);
	Asm_密包CALL(65535, 2, false);
	Asm_密包CALL(319, 3, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包翻牌()
{
	Asm_缓冲CALL(69);
	Asm_发包CALL();
	Asm_缓冲CALL(70);
	Asm_发包CALL();
	Asm_缓冲CALL(71);
	Asm_密包CALL(参数地址 + 0x0, 1, true);
	Asm_密包CALL(参数地址 + 0x4, 1, true);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包进图()
{
	Asm_缓冲CALL(16);
	Asm_密包CALL(参数地址 + 0x0, 3, true);
	Asm_密包CALL(参数地址 + 0x4, 1, true);
	Asm_密包CALL(0, 2, false);
	Asm_密包CALL(参数地址 + 0x8, 1, true);
	Asm_密包CALL(参数地址 + 0xc, 1, true);
	Asm_密包CALL(65535, 2, false);
	Asm_密包CALL(0, 3, false);
	Asm_密包CALL(0, 3, false);
	Asm_密包CALL(0, 1, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_远古进图()
{
	__asm {
		push 16
		call buffer
		mov eax, 参数地址
		push dword ptr[eax] // 地图信息
		call encrypt_60
		push 2
		call encrypt
		mov eax, 参数地址
		push dword ptr[eax + 0x4] // 地图难度
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
			mov ecx, 发包基址
			mov ecx, dword ptr[ecx]
			mov eax, 缓冲CALL
			call eax
			ret 4
			encrypt :
		push dword ptr[esp + 4]
			mov ecx, 发包基址
			mov ecx, dword ptr[ecx]
			mov eax, 密包CALL
			call eax
			ret 4
			encrypt_30 :
		push dword ptr[esp + 4]
			mov ecx, 发包基址
			mov ecx, dword ptr[ecx]
			mov eax, 密包CALL
			add eax, 0x30
			call eax
			ret 4
			encrypt_60 :
		push dword ptr[esp + 4]
			mov ecx, 发包基址
			mov ecx, dword ptr[ecx]
			mov eax, 密包CALL
			add eax, 0x60
			call eax
			ret 4
			sendpacket:
		mov eax, 发包CALL
			call eax
			ret
	}
}

void __declspec(naked) Asm_组包选图()
{
	Asm_缓冲CALL(15);
	Asm_密包CALL(0, 3, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包回城()
{
	Asm_缓冲CALL(42);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包再次()
{
	Asm_缓冲CALL(72);
	Asm_密包CALL(1, 1, false);
	Asm_密包CALL(0, 1, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_领取胶囊()
{
	Asm_缓冲CALL(603);
	Asm_密包CALL(0, 3, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_背包整理()
{
	Asm_缓冲CALL(20);
	Asm_密包CALL(6, 3, false);
	Asm_密包CALL(16, 1, false);
	Asm_密包CALL(0, 1, false);
	Asm_密包CALL(24, 1, false);
	Asm_密包CALL(1, 1, false);
	Asm_密包CALL(32, 1, false);
	Asm_密包CALL(0, 1, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_存金入库()
{
	Asm_缓冲CALL(307);
	Asm_密包CALL(参数地址, 3, true);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包剧情()
{
	Asm_缓冲CALL(16)
	Asm_密包CALL(参数地址, 3, true)//地图ID
	Asm_密包CALL(0, 1, false)//地图难度
	Asm_密包CALL(0, 2, false)
	Asm_密包CALL(0, 1, false)
	Asm_密包CALL(0, 1, false)
	Asm_密包CALL(65535, 2, false)
	Asm_密包CALL(0, 3, false)
	Asm_密包CALL(0, 1, false)
	Asm_密包CALL(参数地址 + 0x4, 3, true)//任务id
	Asm_发包CALL()
	__asm ret
}

void __declspec(naked) Asm_释放CALL()
{
	__asm {

		mov eax, 参数地址
		push dword ptr[eax] //z
		push dword ptr[eax + 0x4]//y
		push dword ptr[eax + 0x8]//x
		push dword ptr[eax + 0xc]//伤害
		push dword ptr[eax + 0x10]//代码
		mov eax, dword ptr ds : [eax + 14]
		push dword ptr ds : [eax]
		mov eax, 释放CALL
		call eax
		add esp, 0x18
		ret
	}
}

void __declspec(naked) Asm_技能CALL()
{
	__asm {
		mov eax, 参数地址
		mov esi, 技能CALL
		call esi
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		retn
	}
}

void __declspec(naked) Asm_组包接受()
{
	Asm_缓冲CALL(31);
	Asm_密包CALL(31, 2, false);
	Asm_密包CALL(参数地址, 2, true);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包完成()
{
	Asm_缓冲CALL(33);
	Asm_密包CALL(33, 2, false);
	Asm_密包CALL(参数地址, 2, true);
	Asm_密包CALL(0, 1, false);
	Asm_密包CALL(0, 1, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_组包提交()
{
	Asm_缓冲CALL(34);
	Asm_密包CALL(34, 2, false);
	Asm_密包CALL(参数地址, 2, true);
	Asm_密包CALL(-1, 2, false);
	Asm_密包CALL(1, 2, false);
	Asm_密包CALL(65535, 2, false);
	Asm_发包CALL();
	__asm ret
}

void __declspec(naked) Asm_区域CALL()
{
	__asm {
		pushad
		mov eax, 参数地址
		push dword ptr[eax] //z
		push - 1
		mov ecx, 区域ECX
		mov ecx, dword ptr[ecx]
		mov eax, 区域CALL
		call eax
		popad
		ret
	}
}


void Asm_测试组包()
{
	Asm_缓冲CALL(599)
	Asm_密包CALL(0, 1, false)
	Asm_密包CALL(11, 3, false)
	Asm_密包CALL(2587, 2, false)
	Asm_密包CALL(266, 2, false)
	Asm_密包CALL(65535, 2, false)
	Asm_发包CALL()
}

void Send_组包拾取(int 物品地址, int x, int y, bool is_send)
{
	struct
	{
		int 物品地址;
		int 人物x;
		int 人物y;
		int 物品x;
		int 物品y;
	}params;
	params.物品地址 = 物品地址;
	params.人物x = x;
	params.人物y = y;
	params.物品x = x + (int)(rand() % 6);
	params.物品y = y + (int)(rand() % 6);
	RemoteCall(__FUNCTION__, Asm_组包拾取, &params, sizeof(params), 1024, is_send);
}

void Send_选择角色1(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_选择角色1, NULL, 0, 1024, is_send);
}

void Send_选择角色2(int 位置, bool is_send)
{
	struct
	{
		int 位置;
	}params;
	params.位置 = 位置;
	RemoteCall(__FUNCTION__, Asm_选择角色2, &params, sizeof(params), 1024, is_send);
}

void Send_返回角色(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_返回角色, NULL, 0, 1024, is_send);
}

void Send_坐标CALL(int 对象指针, int x, int y, int z, bool is_send)
{
	struct
	{
		int 物品地址;
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
	params.物品地址 = 对象指针;
	params.z = z;
	params.y = y;
	params.x = x;
	RemoteCall(__FUNCTION__, Asm_坐标CALL, &params, sizeof(params), 1024, is_send);
}

void Send_城市飞机(int MaxMapId, int MinMapId, int x, int y, bool is_send)
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
	RemoteCall(__FUNCTION__, Asm_城市飞机, &params, sizeof(params), 1024, is_send);
}

void Send_组包卖物(int 位置索引, int 数量, bool is_send)
{
	struct
	{
		int 位置索引;
		int 数量;
		int 位置加上数量;
	}params;
	params.位置索引 = 位置索引;
	params.数量 = 数量;
	params.位置加上数量 = 位置索引 * 2 + 数量 * 2 + 1;
	RemoteCall(__FUNCTION__, Asm_组包卖物, &params, sizeof(params), 1024, is_send);
}

void Send_组包买物(int 物品ID, bool is_send)
{
	struct
	{
		int 物品ID;
	}params;
	params.物品ID = 物品ID;
	RemoteCall(__FUNCTION__, Asm_组包买物, &params, sizeof(params), 1024, is_send);
}

void Send_组包分解(int 位置索引, bool is_send)
{
	struct
	{
		int 位置索引;
	}params;
	params.位置索引 = 位置索引;
	RemoteCall(__FUNCTION__, Asm_组包分解, &params, sizeof(params), 1024, is_send);
}

void Send_组包翻牌(int x, int y, bool is_send)
{
	struct
	{
		int x;
		int y;
	}params;
	params.x = x;
	params.y = y;
	RemoteCall(__FUNCTION__, Asm_组包翻牌, &params, sizeof(params), 1024, is_send);
}

void Send_组包进图(int 地图信息, int 地图难度, int 地图深渊, int 地图练习, bool is_send)
{
	struct
	{
		int 地图信息;
		int 地图难度;
		int 地图深渊;
		int 地图练习;
	}params;
	params.地图信息 = 地图信息;
	params.地图难度 = 地图难度;
	params.地图深渊 = 地图深渊;
	params.地图练习 = 地图练习;
	RemoteCall(__FUNCTION__, Asm_组包进图, &params, sizeof(params), 1024, is_send);
}

void Send_远古进图(int 地图信息, int 地图难度, bool is_send)
{
	struct
	{
		int 地图信息;
		int 地图难度;
	}params;
	params.地图信息 = 地图信息;
	params.地图难度 = 地图难度;
	RemoteCall(__FUNCTION__, Asm_远古进图, &params, sizeof(params), 1024, is_send);
}

void Send_组包选图(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_组包选图, NULL, 0, 1024, is_send);
}

void Send_组包回城(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_组包回城, NULL, 0, 1024, is_send);
}

void Send_组包再次(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_组包再次, NULL, 0, 1024, is_send);
}

void Send_领取胶囊(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_领取胶囊, NULL, 0, 1024, is_send);
}

void Send_背包整理(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_背包整理, NULL, 0, 1024, is_send);
}

void Send_存金入库(int 数量, bool is_send)
{
	struct
	{
		int 数量;
	}params;
	params.数量 = 数量;
	RemoteCall(__FUNCTION__, Asm_存金入库, &params, sizeof(params), 1024, is_send);
}

void Send_释放CALL(int 触发对象, int 代码, int 伤害, int x, int y, int z, bool is_send) {
	struct
	{
		int z;
		int y;
		int x;
		int 伤害;
		int 代码;
		int 触发对象;
	}params;
	params.z = z;
	params.y = y;
	params.x = x;
	params.伤害 = 伤害;
	params.代码 = 代码;
	params.触发对象 = 触发对象;
	RemoteCall(__FUNCTION__, Asm_释放CALL, &params, sizeof(params), 1024, is_send);
}

void Send_组包剧情(int 地图ID,  int 任务ID, bool is_send)
{
	struct
	{
		int 地图ID;
		int 任务ID;
	}params;
	params.地图ID = 地图ID;
	params.任务ID = 任务ID;
	RemoteCall(__FUNCTION__, Asm_组包剧情, &params, sizeof(params), 1024, is_send);
}

void Send_技能CALL(int 指针, int 代码, int 伤害, int x, int y, int z, int 大小, bool is_send) {

	int 技能结构[23] = { 0 };
	技能结构[0] = 指针;//指针
	技能结构[2] = 代码;
	技能结构[3] = 伤害;
	技能结构[6] = x;
	技能结构[7] = y;
	技能结构[8] = z;
	技能结构[22] = 大小;
	RemoteCall(__FUNCTION__, Asm_技能CALL, &技能结构, sizeof(技能结构), 1024, is_send);
}

void Send_组包接受(int 任务ID, bool is_send)
{
	struct
	{
		int 任务ID;
	}params;
	params.任务ID = 任务ID;
	RemoteCall(__FUNCTION__, Asm_组包接受, &params, sizeof(params), 1024, is_send);
}

void Send_组包完成(int 任务ID, bool is_send)
{
	struct
	{
		int 任务ID;
	}params;
	params.任务ID = 任务ID;
	RemoteCall(__FUNCTION__, Asm_组包完成, &params, sizeof(params), 1024, is_send);
}

void Send_组包提交(int 任务ID, bool is_send)
{
	struct
	{
		int 任务ID;
	}params;
	params.任务ID = 任务ID;
	RemoteCall(__FUNCTION__, Asm_组包提交, &params, sizeof(params), 1024, is_send);
}

void Send_区域CALL(int 任务ID, bool is_send)
{
	struct
	{
		int 任务ID;
	}params;
	params.任务ID = 任务ID;
	RemoteCall(__FUNCTION__, Asm_区域CALL, &params, sizeof(params), 1024, is_send);
}

void Send_测试组包(bool is_send)
{
	RemoteCall(__FUNCTION__, Asm_测试组包, NULL, 0, 1024, is_send);
}



void RemoteCall(const char * name, LPCVOID fun, LPCVOID params, int p_size, int size, bool is_send)
{
	if (params != NULL) {
		pApi.writeMemory(参数地址, params, p_size);
	}
	int addr = pApi.allocMemory(name, size);
	if (!addr) {
		printf("%s 失败!\n", name);
		system("pause");
		exit(1);
	}
	if (pApi.writeMemory(addr, fun, size) == false) {
		printf("%s WriteMemory 失败!\n", name);
		system("pause");
		exit(1);
	}
	if (is_send) {
		SendMessage(pApi.hWnd, 消息ID, addr, 0);
	}
}

void InitRemoteCall()
{
	HookWindowsMsg();

	Send_组包拾取(0, 0, 0, false);

	Send_选择角色1(false);

	Send_选择角色2(0, false);

	Send_返回角色(false);

	Send_坐标CALL(0, 0, 0, 0, false);

	Send_城市飞机(0, 0, 0, 0, false);

	Send_组包卖物(0, 0, false);

	Send_组包分解(0, false);

	Send_组包翻牌(0, 0, false);

	Send_组包进图(0, 0, 0, 0, false);

	Send_远古进图(0, 0, false);

	Send_组包选图(false);

	Send_组包回城(false);

	Send_组包再次(false);

	Send_领取胶囊(false);

	Send_背包整理(false);

	Send_存金入库(0, false);

	Send_组包剧情(0, 0, false);

	Send_释放CALL(0, 0, 0, 0, 0, 0, false);

	Send_技能CALL(0, 0, 0, 0, 0, 0, 0, false);

	Send_组包接受(0, false);

	Send_组包完成(0, false);

	Send_组包提交(0, false);

	Send_区域CALL(0, false);

	Send_组包买物(0, false);

	Send_测试组包(false);
}