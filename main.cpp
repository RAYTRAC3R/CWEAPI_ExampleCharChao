#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"

extern "C"
{
    //registering data functions. - Needs to exist.
    void (*RegisterDataFunc)(void* ptr);

    //Lens function - Needed for CWE to recognize the lens.
    __declspec(dllexport) void(__cdecl* ALS_LensSpecial)(ObjectMaster*, ObjectMaster*);

    int ClassicTopHatID;
    int ClassicBearTieID;

    //Generate an ID for each lens
    int BlueFNAFLensID;

    //Define Models
    ModelInfo* MDLFreddyFazChao;

    NJS_TEXNAME FNAFAccessoriesTex[10];
    NJS_TEXLIST fnafaccessories_texlist = { arrayptrandlength(FNAFAccessoriesTex) };

    NJS_TEXNAME BlueFNAFLensTex[10];
    NJS_TEXLIST bluefnaflens_texlist = { arrayptrandlength(BlueFNAFLensTex) };

    BlackMarketItemAttributes BMClassicTopHat = { 1000, 500, 0, -1, -1, 0 };
    BlackMarketItemAttributes BMClassicBearTie = { 1000, 500, 0, -1, -1, 0 };
    BlackMarketItemAttributes BMBlueFNAFLens = { 1000, 500, 0, -1, -1, 0 };

    ModelInfo* MDLClassicTopHat;
    ModelInfo* MDLClassicBearTie;
    ModelInfo* MDLBlueFNAFLens;

    //Define a pointer function for pEvolveFunc
    static bool FreddyFazEvolve(ObjectMaster* tp)
    {
        Uint16* accessories = (Uint16*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x614);

        if (accessories[Head] == ClassicTopHatID + 1 && accessories[Generic2] == ClassicBearTieID + 1)
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

        cwe_api->RegisterChaoTexlistLoad("FNAFAccessories", &fnafaccessories_texlist);
        ClassicTopHatID = cwe_api->RegisterChaoAccessory(Head, MDLClassicTopHat->getmodel(), &fnafaccessories_texlist, &BMClassicTopHat, "Classic Top Hat", "Hor, hor hor hor, hor!");
        ClassicBearTieID = cwe_api->RegisterChaoAccessory(Generic2, MDLClassicBearTie->getmodel(), &fnafaccessories_texlist, &BMClassicBearTie, "Classic Bear Bowtie", "Back to Basics!");

        //Register your lens to the black market:
        BlueFNAFLensID = cwe_api->RegisterChaoSpecial(MDLBlueFNAFLens->getmodel(), &bluefnaflens_texlist, &BMBlueFNAFLens, ALS_LensSpecial, NULL, "Blue Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);

        cwe_api->RegisterEyeColor("BlueFNAFLens", &bluefnaflens_texlist, BlueFNAFLensID);

        //Define Character Chao data:
        CWE_API_CHAO_DATA CharChao_pData =
        {
            MDLFreddyFazChao->getmodel(),    //pObject
            {0},                //pSecondEvoList[5]

            "FreddyFazChao",            //TextureName
            8,                //TextureCount
            0xFFD29961,            //IconColor - hex, 6 bytes
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

        //Lens function - This talks to CWE to get the lens to work when registering it as a special object.
        ALS_LensSpecial = (decltype(ALS_LensSpecial))GetProcAddress(GetModuleHandle(L"CWE"), "ALS_LensSpecial");

        MDLFreddyFazChao = new ModelInfo(pathStr + "FreddyFazChao.sa2mdl");
        MDLBlueFNAFLens = new ModelInfo(pathStr + "lensNeut.sa2mdl");
        MDLClassicTopHat = new ModelInfo(pathStr + "ClassicTopHat.sa2mdl");
        MDLClassicBearTie = new ModelInfo(pathStr + "ClassicBearBowtie.sa2mdl");

        RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
        RegisterDataFunc(CWELoad);
    }
    __declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}