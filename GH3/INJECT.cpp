#include "pch.h"
#include "INJECT.h"
#include <fstream>

void _stdcall INJECTCode()
{
	unsigned address = 0xCCCCCCCC;
	PREMOTE_DATA p = (PREMOTE_DATA)address;
	p->f_LoadLibrary(p->dllName);
}

BOOL INJECT::StartProcess(const wchar_t* GameExe, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* LPinfo)
{
	STARTUPINFO si{};					//��ȡ������Ϣ�Ľṹ��
	si.cb = sizeof(si);					//����Ҫ no why

	return CreateProcess(						//�̶���ʽ
		GameExe,
		GameCmds,
		NULL, NULL, TRUE,
		CREATE_SUSPENDED,
		NULL,
		GamePath,
		&si,
		LPinfo
	);
}

void INJECT::UnloadImage(void* _data)
{
	delete[] _data;
}

DWORD INJECT::GetEntryPoint(const wchar_t* filename)
{
	void* image = ImageLoad(filename);

	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)image;
	unsigned PEAddress = dosHeader->e_lfanew + unsigned(image);

	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)PEAddress;
	DWORD dEntryPoint = ntHeader->OptionalHeader.AddressOfEntryPoint;

	UnloadImage(image);
	return dEntryPoint;
}

BOOL INJECT::CreateRemoteData(HANDLE hProcess, const wchar_t* dllName)
{
	LPVOID adrRemote = VirtualAllocEx(hProcess, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//д��Զ�̴���
	SIZE_T lwt;
	LPVOID adrRemoteData = LPVOID((unsigned)adrRemote + 0x2000);
	_REMOTE_DATA remoteData{};

	WriteProcessMemory(hProcess, adrRemote, INJECTCode, 0x200, &lwt);

	CodeRemoteData(&remoteData, dllName);
	WriteProcessMemory(hProcess, adrRemoteData, &remoteData, sizeof(remoteData), &lwt);
	return 0;
}

void INJECT::CodeRemoteData(PREMOTE_DATA _data, const wchar_t* dllName)
{
	short length{};
	for (length = 0; dllName[length]; length++);

	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
	_data->f_LoadLibrary = (_LoadLibrary)GetProcAddress(hKernel, "LoadLibraryW");
	memcpy(_data->dllName, dllName, (length + 1) * 2);		//wchar_t 2�ֽ�
	/*CString wTxt;
	wTxt.Format(L"%X", _data->f_LoadLibrary);
	AfxMessageBox(wTxt);*/
}

void* INJECT::ImageLoad(const wchar_t* filename)
{
	std::ifstream streamReader(filename, std::ios::binary);
	streamReader.seekg(0, std::ios::end);		//�α��Ƶ��ļ���β
	unsigned filesize = streamReader.tellg();			//��ȡ�α굱ǰλ�� - �ļ���ʼλ�ã��˴�Ϊ�ļ���С
	char* _data = new char[filesize];					//�����ڴ�
	streamReader.seekg(0, std::ios::beg);		//��ת�ؿ�ʼ
	streamReader.read(_data, filesize);		//��ȡ�ļ�
	streamReader.close();
	return _data;
}
