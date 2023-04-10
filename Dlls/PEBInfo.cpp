#include "pch.h"
#include "PEBInfo.h"

PEBInfo::PEBInfo() : pData(nullptr)
{
    PPEB _peb;
    _asm {
        mov eax, fs: [0x30] ;
        mov _peb, eax
    }

    FrontDiff = sizeof(pData->InLoadOrderModuleList);
    Pldr = _peb->Ldr;
    FIRST = &(Pldr->InMemoryOrderModuleList);
    CURLST = FIRST->Flink;
}

std::vector<PEBInfo::Info> PEBInfo::GetInfo()
{
    std::vector<Info> Allinfo;
    bool setLoadInfo = false;
    if (LoadInfo.size() == 0)
        setLoadInfo = true;
    do
    {
        Info info;
        pData = (PLDR_DATA_TABLE_ENTRY_SELF)((unsigned)CURLST - FrontDiff);
        if (!pData) throw "pData lost!";
        info.DllBase = pData->DllBase;
        info.FullDllName = pData->FullDllName;
        info.EntryPoint = pData->EntryPoint;
        info.ImageSize = pData->ImageSize;
        info.PLIST = &pData->InMemoryOrderLinks;
        Allinfo.push_back(info);
        if (setLoadInfo)
        {
            info.PLIST = &pData->InLoadOrderModuleList;
            LoadInfo.push_back(info);
            info.PLIST = &pData->InInitializationOrderModuleList;
            IniInfo.push_back(info);
        }
        CURLST = CURLST->Flink;
    } while (CURLST != FIRST);
    if (nowInfo.size() == 0) nowInfo = Allinfo;
    return Allinfo;
}

bool PEBInfo::HidInfo(const wchar_t* dllName)
{
    HMODULE hMod = GetModuleHandleW(dllName);
    if (hMod) return HidInfo(hMod);
    return false;
}

bool PEBInfo::HidInfo(LPVOID adrBase)
{
    int successNum = 0;
    for (auto& i : nowInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            successNum++;
            break;
        }
    }
    for (auto& i : LoadInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            successNum++;
            break;
        }
    }
    for (auto& i : IniInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            successNum++;
            break;
        }
    }
    deleteSignatures(adrBase);  //���������
    return (successNum == 3);
}

void PEBInfo::deleteSignatures(LPVOID adrBase)
{
    //��ȡdosͷ��PEͷ��ַ
    IMAGE_DOS_HEADER dosH{};
    IMAGE_NT_HEADERS ntH{};
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)adrBase;
    PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (unsigned)adrBase);

    //�޸ĵ�ַ���ڴ�����
    DWORD dOld;
    VirtualProtect(dosHeader, sizeof(dosH), PAGE_EXECUTE_READWRITE, &dOld);
    VirtualProtect(ntHeader, sizeof(ntH), PAGE_EXECUTE_READWRITE, &dOld);

    //���������
    memcpy(dosHeader, &dosH, sizeof(dosH));
    memcpy(ntHeader, &ntH, sizeof(ntH));

}

void PEBInfo::RecoverInfo()
{
    for (auto& i : nowInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    for (auto& i : LoadInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    for (auto& i : IniInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    return;
}
