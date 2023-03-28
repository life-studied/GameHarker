#include "pch.h"
#include "INJECT.h"
#include <fstream>

void _stdcall INJECTCode()
{
	unsigned address = 0xCCCCCCCC;				//创建一个指针指向LoadLibrary的dll，此处的0xCCCCCCC只是一个象征，被编译后的字节码会再次进行替换
	PREMOTE_DATA p = (PREMOTE_DATA)address;
	p->f_LoadLibrary(p->dllName);
	
	//恢复入口点代码
	unsigned dEntry = p->EntryPoint;
	char* entryCode = (char*)p->EntryPoint;
	entryCode[0] = p->oldCode[0];
	entryCode[1] = p->oldCode[1];
	entryCode[2] = p->oldCode[2];
	entryCode[3] = p->oldCode[3];
	entryCode[4] = p->oldCode[4];

	//跳转回入口点重新执行
	__asm
	{
		mov eax, dEntry
		jmp eax
	}
	
}

DWORD _stdcall RemoteThreadProc(PREMOTE_DATA pdata)
{
	unsigned base = pdata->f_GetModuleHandleA(0);		//得到基址
	DWORD dRet;
	pdata->EntryPoint += base;		//基址+入口点地址，获取入口点的内存地址
	pdata->f_VirtualProtect((LPVOID)pdata->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dRet);		//修改入口点的内存属性
	char* entryCode = (char*)pdata->EntryPoint;

	//保存原来的入口点代码
	pdata->oldCode[0] = entryCode[0];
	pdata->oldCode[1] = entryCode[1];
	pdata->oldCode[2] = entryCode[2];
	pdata->oldCode[3] = entryCode[3];
	pdata->oldCode[4] = entryCode[4];

	//修改入口点代码
	entryCode[0] = 0xE9;		//JMP，跳转到INJECT函数里

	int Distence = pdata->HOOKFunctionAdr - pdata->EntryPoint - 5;		//计算jmp的距离
	int* entryDis = (int*)(pdata->EntryPoint + 1);
	entryDis[0] = Distence;		//将距离写入
	return 1;

}

BOOL INJECT::StartProcess(const wchar_t* GameExe, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* LPinfo, bool pause)
{
	STARTUPINFO si{};					//获取进程信息的结构体
	si.cb = sizeof(si);					//必须要 no why

	DWORD dFlags = 0;
	if (pause) dFlags = CREATE_SUSPENDED;

	return CreateProcess(						//固定格式
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
	//写入远程代码
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
	//修正远程代码
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
	* 远程线程注入的测试
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
	memcpy(_data->dllName, dllName, (length + 1) * 2);		//wchar_t 2字节
	/*CString wTxt;
	wTxt.Format(L"%X", _data->f_LoadLibrary);
	AfxMessageBox(wTxt);*/
}

void* INJECT::ImageLoad(const wchar_t* filename)
{
	std::ifstream streamReader(filename, std::ios::binary);
	streamReader.seekg(0, std::ios::end);		//游标移到文件结尾
	unsigned filesize = streamReader.tellg();			//获取游标当前位置 - 文件开始位置，此处为文件大小
	char* _data = new char[filesize];					//分配内存
	streamReader.seekg(0, std::ios::beg);		//跳转回开始
	streamReader.read(_data, filesize);		//读取文件
	streamReader.close();
	return _data;
}
