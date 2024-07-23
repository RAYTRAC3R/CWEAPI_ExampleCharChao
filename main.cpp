#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"

extern "C"
{
    //registering data functions. - Needs to exist.
    void (*RegisterDataFunc)(void* ptr);

    //Define Models
    ModelInfo* MDLFreddyFazChao;

    //Define a pointer function for pEvolveFunc
    static bool FreddyFazEvolve(ObjectMaster* tp)
    {
        if (tp->Data1.Chao->ChaoDataBase_ptr->Alignment < 0.50 && tp->Data1.Chao->ChaoDataBase_ptr->SA2BCharacterBonds[2].a > 75)
        {
            PrintDebug("Chao evolving into FreddyFaz");
            return true;
        }
        else
            return false;
    }

    //main CWE Load function -- Important stuff like adding your CWE mod goes here
    void CWELoad(CWE_REGAPI* cwe_api, ObjectMaster* tp)
    {

        //Define Character Chao data:
        CWE_API_CHAO_DATA CharChao_pData =
        {
            MDLFreddyFazChao->getmodel(),    //pObject
            {0},                //pSecondEvoList[5]

            "FreddyFazChao",            //TextureName
            8,                //TextureCount
            0xFFFBAC66,            //IconColor - hex, 6 bytes
            ICON_TYPE_BALL,            //IconType
            NULL,                //pIconData

            FreddyFazEvolve,            //pEvolveFunc 

            0,                //Flags
            "FreddyFaz",            //Name
            "cwe_rt_FreddyFz",            //id
        };

        //add the Chao Type
        cwe_api->AddChaoType(&CharChao_pData);
    }

    //initialization function - MUST exist in order to have CWE and SA2 see your mod
    __declspec(dllexport) void Init(const char* path)
    {
        HMODULE h = GetModuleHandle(L"CWE");

        std::string pathStr = std::string(path) + "\\";

        MDLFreddyFazChao = new ModelInfo(pathStr + "FreddyFazChao.sa2mdl");

        RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
        RegisterDataFunc(CWELoad);
    }
    __declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}