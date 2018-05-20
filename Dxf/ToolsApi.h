#pragma once
class ToolsApi
{
public:
	ToolsApi();

	~ToolsApi();
	static char * unicodeToAnsi(TCHAR * wstr);

	static TCHAR * ansiToUnicode(char * str);

	static int getModleAddrByPid(DWORD process_id, LPCWSTR modle_name);
	/*删除自身*/
	static bool deleteSelf();
	/*清除文件*/
	static void clearFile(const char * f_path);
	/*提升权限*/
	static bool WINAPI enablePrivileges();
	/*获取系统API地址*/
	static int getWinApiAddr(LPCSTR modle_name, LPCSTR fun_name);

	static byte * vectorByteToByte(vector<byte> bytes);

	static vector<byte> byteToVectorByte(byte * bytes, int len);
};

