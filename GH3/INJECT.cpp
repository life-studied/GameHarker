#include "pch.h"
#include "INJECT.h"
#include <fstream>

void _stdcall INJECTCode()
{
	unsigned address = 0xCCCCCCCC;				//����һ��ָ��ָ��LoadLibrary��dll���˴���0xCCCCCCCֻ��һ�����������������ֽ�����ٴν����滻
	PREMOTE_DATA p = (PREMOTE_DATA)address;
	p->f_LoadLibrary(p->dllName);
	
	//�ָ���ڵ����
	unsigned dEntry = p->EntryPoint;
	char* entryCode = (char*)p->EntryPoint;
	entryCode[0] = p->oldCode[0];
	entryCode[1] = p->oldCode[1];
	entryCode[2] = p->oldCode[2];
	entryCode[3] = p->oldCode[3];
	entryCode[4] = p->oldCode[4];

	//��ת����ڵ�����ִ��
	__asm
	{
		mov eax, dEntry
		jmp eax
	}
	
}

DWORD _stdcall RemoteThreadProc(PREMOTE_DATA pdata)
{
	unsigned base = pdata->f_GetModuleHandleA(0);		//�õ���ַ
	DWORD dRet;
	pdata->EntryPoint += base;		//��ַ+��ڵ��ַ����ȡ��ڵ���ڴ��ַ
	pdata->f_VirtualProtect((LPVOID)pdata->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dRet);		//�޸���ڵ���ڴ�����
	char* entryCode = (char*)pdata->EntryPoint;

	//����ԭ������ڵ����
	pdata->oldCode[0] = entryCode[0];
	pdata->oldCode[1] = entryCode[1];
	pdata->oldCode[2] = entryCode[2];
	pdata->oldCode[3] = entryCode[3];
	pdata->oldCode[4] = entryCode[4];

	//�޸���ڵ����
	entryCode[0] = 0xE9;		//JMP����ת��INJECT������

	int Distence = pdata->HOOKFunctionAdr - pdata->EntryPoint - 5;		//����jmp�ľ���
	int* entryDis = (int*)(pdata->EntryPoint + 1);
	entryDis[0] = Distence;		//������д��
	return 1;

}

BOOL INJECT::StartProcess(const wchar_t* GameExe, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* LPinfo, bool pause)
{
	STARTUPINFO si{};					//��ȡ������Ϣ�Ľṹ��
	si.cb = sizeof(si);					//����Ҫ no why

	DWORD dFlags = 0;
	if (pause) dFlags = CREATE_SUSPENDED;

	return CreateProcess(						//�̶���ʽ
		GameExe,
		GameCmds,
		NULL, NULL, TRUE,
		dFlags,
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

BOOL INJECT::CreateRemoteData(HANDLE hProcess, const wchar_t* GameExe, const wchar_t* dllName)
{
	LPVOID adrRemote = VirtualAllocEx(hProcess, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//д��Զ�̴���
	SIZE_T lwt;
	LPVOID adrRemoteData = (LPVOID)((unsigned)adrRemote + 0x2000);
	LPVOID adrRemoteProc = (LPVOID)((unsigned)adrRemote + 0x500);
	_REMOTE_DATA remoteData{};
	remoteData.EntryPoint = (unsigned)GetEntryPoint(GameExe);
	remoteData.HOOKFunctionAdr = (unsigned)adrRemote;
	CodeRemoteData(&remoteData, dllName);
	WriteProcessMemory(hProcess, adrRemoteData, &remoteData, sizeof(remoteData), &lwt);
	
	//CString wTxt;
	//wTxt.Format(L"%X", adrRemote);
	//AfxMessageBox(wTxt);
	//����Զ�̴���
	char _code[0x200];
	memcpy(_code, INJECTCode, sizeof(_code));
	for (int i = 0; i < 0x100; i++)			//
	{
		unsigned* pcode = (unsigned*)(&_code[i]);
		if (pcode[0] == 0xCCCCCCCC)
		{
			pcode[0] = (unsigned)adrRemoteData;
			break;
		}
	}
	

	WriteProcessMemory(hProcess, adrRemote, _code, 0x200, &lwt);
	WriteProcessMemory(hProcess, adrRemoteProc, RemoteThreadProc, 0x200, &lwt);
	
	DWORD dwThreadId;
	HANDLE remoteHdl = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);
	
	
	WaitForSingleObject(remoteHdl, INFINITE);
	//CString wTxt;
	//wTxt = L"This";
	//AfxMessageBox(wTxt);
	CloseHandle(remoteHdl);
	
	/*
	* Զ���߳�ע��Ĳ���
	DWORD dwThreadId;
	HANDLE remoteHdl = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemote, NULL, 0, &dwThreadId);
	WaitForSingleObject(remoteHd1, INFINITE);
	CloseHandle(remoteHdl);
	*/
	return 0;
}

void INJECT::CodeRemoteData(PREMOTE_DATA _data, const wchar_t* dllName)
{
	short length{};
	for (length = 0; dllName[length]; length++);

	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
	_data->f_LoadLibrary = (_LoadLibrary)GetProcAddress(hKernel, "LoadLibraryW");
	_data->f_GetModuleHandleA = (_GetModuleHandleA)GetProcAddress(hKernel, "GetModuleHandleA");
	_data->f_VirtualProtect = (_VirtualProtect)GetProcAddress(hKernel, "VirtualProtect");
	//CString wTxt;
	//wTxt.Format(L"%X", _data->f_VirtualProtect);
	//AfxMessageBox(wTxt);
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
