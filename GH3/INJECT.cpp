#include "pch.h"
#include "INJECT.h"
#include <fstream>

void _stdcall INJECTCode()
{
	LoadLibrary(L"D:\\coding_workspace\\vs2022\\GH3\\Release\\Dlls.dll");
}

BOOL INJECT::StartProcess(const wchar_t* GameExe, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* LPinfo)
{
	STARTUPINFO si{};					//获取进程信息的结构体
	si.cb = sizeof(si);					//必须要 no why

	return CreateProcess(						//固定格式
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

BOOL INJECT::CreateRemoteData(HANDLE hProcess)
{
	LPVOID adrRemote = VirtualAllocEx(hProcess, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//写入远程代码
	SIZE_T lwt;
	WriteProcessMemory(hProcess, adrRemote, INJECTCode, 0x200, &lwt);
	return 0;
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
