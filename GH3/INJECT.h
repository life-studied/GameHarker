#pragma once

typedef unsigned int (WINAPI* _LoadLibrary)(wchar_t* dllName);		//指向LoadLibrary的函数指针
typedef struct _REMOTE_DATA 
{
	wchar_t dllName[0xff];		//dll路径
	_LoadLibrary f_LoadLibrary;
}* PREMOTE_DATA;

class INJECT
{
public:
	BOOL StartProcess(
		const wchar_t* GameExe,
		const wchar_t* GamePath,
		wchar_t* GameCmds,
		PROCESS_INFORMATION* LPinfo
	);

	void* ImageLoad(const wchar_t* filename);
	void UnloadImage(void* _data);
	DWORD GetEntryPoint(const wchar_t* filename);

public:
	//分配远程空间内存
	BOOL CreateRemoteData(HANDLE hProcess, const wchar_t* dllName);
	void CodeRemoteData(PREMOTE_DATA _data, const wchar_t* dllName);		//编码远程参数
};

