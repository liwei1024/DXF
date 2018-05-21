#pragma once

int Decode(int addr);

void Encode(int addr, int value, int type = 0);

Coordinate 获取坐标(int addr);

Coordinate 当前房间(void);

Coordinate BOSS房间(void);

bool 是否在BOSS房(void);

void 装备处理();

bool 是否开门();

void 完成任务(int 任务ID);

bool 提交任务();

bool 剧情任务();

int 获取主线任务ID();

void 区域_CALL(int 任务ID);

void 模拟按键(int keyCode, int s = 100);

int 取角色疲劳();

int 取角色等级();


