#pragma once
void Send_组包拾取(int 物品地址, int x, int y, bool is_send = true);

void Send_选择角色1(bool is_send = true);

void Send_选择角色2(int 位置, bool is_send = true);

void Send_返回角色(bool is_send = true);

void Send_坐标CALL(int 对象指针, int x, int y, int z, bool is_send = true);

void Send_城市飞机(int MaxMapId, int MinMapId, int x, int y, bool is_send = true);

void Send_组包卖物(int 位置索引, int 数量 = 1, bool is_send = true);

void Send_组包买物(int 物品ID, bool is_send = true);

void Send_组包分解(int 位置索引, bool is_send = true);

void Send_组包翻牌(int x, int y, bool is_send = true);

void Send_组包进图(int 地图信息, int 地图难度, int 地图深渊 = 0, int 地图练习 = 0, bool is_send = true);

void Send_远古进图(int 地图信息, int 地图难度, bool is_send = true);

void Send_组包选图(bool is_send = true);

void Send_组包回城(bool is_send = true);

void Send_组包再次(bool is_send = true);

void Send_领取胶囊(bool is_send = true);

void Send_背包整理(bool is_send = true);

void Send_存金入库(int 数量, bool is_send = true);

void Send_释放CALL(int 触发对象, int 代码, int 伤害, int x, int y, int z, bool is_send = true);

void Send_组包剧情(int 地图ID, int 任务ID, bool is_send = true);

void Send_技能CALL(int 指针, int 代码, int 伤害, int x, int y, int z, int 大小 = 1, bool is_send = true);

void Send_组包接受(int 任务ID, bool is_send = true);

void Send_组包完成(int 任务ID, bool is_send = true);

void Send_组包提交(int 任务ID, bool is_send = true);

void Send_区域CALL(int 任务ID, bool is_send = true);

void Send_测试组包(bool is_send = true);

void RemoteCall(const char * name, LPCVOID fun, LPCVOID params, int p_size, int size, bool is_send);

void InitRemoteCall();

bool ReHookMsg();
