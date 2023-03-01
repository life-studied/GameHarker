#pragma once
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
	BOOL CreateRemoteData(HANDLE hProcess);
};

