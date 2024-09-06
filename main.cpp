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
    int GreenFNAFLensID;
    int PinkFNAFLensID;
    int PurpleFNAFLensID;
    int YellowFNAFLensID;

    //Define Models
    ModelInfo* MDLFreddyFazChao;
    ModelInfo* MDLBonnieBunChao;

    NJS_TEXNAME FNAFAccessoriesTex[10];
    NJS_TEXLIST fnafaccessories_texlist = { arrayptrandlength(FNAFAccessoriesTex) };

    NJS_TEXNAME BlueFNAFLensTex[10];
    NJS_TEXLIST bluefnaflens_texlist = { arrayptrandlength(BlueFNAFLensTex) };

    NJS_TEXNAME GreenFNAFLensTex[10];
    NJS_TEXLIST greenfnaflens_texlist = { arrayptrandlength(GreenFNAFLensTex) };

    NJS_TEXNAME PinkFNAFLensTex[10];
    NJS_TEXLIST pinkfnaflens_texlist = { arrayptrandlength(PinkFNAFLensTex) };

    NJS_TEXNAME PurpleFNAFLensTex[10];
    NJS_TEXLIST purplefnaflens_texlist = { arrayptrandlength(PurpleFNAFLensTex) };

    NJS_TEXNAME YellowFNAFLensTex[10];
    NJS_TEXLIST yellowfnaflens_texlist = { arrayptrandlength(YellowFNAFLensTex) };

    BlackMarketItemAttributes BMClassicTopHat = { 1000, 500, 0, -1, -1, 0 };
    BlackMarketItemAttributes BMClassicBearTie = { 1000, 500, 0, -1, -1, 0 };
    BlackMarketItemAttributes BMFNAFLens = { 1000, 500, 0, -1, -1, 0 };

    ModelInfo* MDLClassicTopHat;
    ModelInfo* MDLClassicBearTie;
    ModelInfo* MDLFNAFLens;

    //Define a pointer function for pEvolveFunc
    static bool FreddyFazEvolve(ObjectMaster* tp)
    {
        Uint16* accessories = (Uint16*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x614);

        if (accessories[Head] == ClassicTopHatID + 1 && (tp->Data1.Chao->ChaoDataBase_ptr->SA2AnimalBehavior, SA2BAnimalFlags::SA2BAnimalFlags_Bear))
        {
            //PrintDebug("Chao evolving into FreddyFaz");
            return true;
        }
        else
            return false;
    }

    static bool BonnieBunEvolve(ObjectMaster* tp)
    {
        Uint16* accessories = (Uint16*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x614);

        //std::string PrintAccessoryString = std::to_string(accessories[Generic2]);
        //PrintDebug(PrintAccessoryString.c_str());

        if (accessories[Generic2] == 10 && (tp->Data1.Chao->ChaoDataBase_ptr->SA2AnimalBehavior, SA2BAnimalFlags::SA2BAnimalFlags_Rabbit))
        {

            //PrintDebug("Chao evolving into BonnieBun");
            return true;
        }
        else
            return false;
    }

    //main CWE Load function -- Important stuff like adding your CWE mod goes here
    void CWELoad(CWE_REGAPI* cwe_api, ObjectMaster* tp)
    {

        cwe_api->RegisterChaoTexlistLoad("FNAFAccessories", &fnafaccessories_texlist);

        cwe_api->RegisterChaoTexlistLoad("BlueFNAFLens", &bluefnaflens_texlist);
        cwe_api->RegisterChaoTexlistLoad("GreenFNAFLens", &greenfnaflens_texlist);
        cwe_api->RegisterChaoTexlistLoad("PinkFNAFLens", &pinkfnaflens_texlist);
        cwe_api->RegisterChaoTexlistLoad("PurpleFNAFLens", &purplefnaflens_texlist);
        cwe_api->RegisterChaoTexlistLoad("YellowFNAFLens", &yellowfnaflens_texlist);

        ClassicTopHatID = cwe_api->RegisterChaoAccessory(Head, MDLClassicTopHat->getmodel(), &fnafaccessories_texlist, &BMClassicTopHat, "Classic Top Hat", "Hor, hor hor hor, hor!");
        ClassicBearTieID = cwe_api->RegisterChaoAccessory(Generic2, MDLClassicBearTie->getmodel(), &fnafaccessories_texlist, &BMClassicBearTie, "Classic Bear Bowtie", "Back to Basics!");

        //Register your lens to the black market:
        BlueFNAFLensID = cwe_api->RegisterChaoSpecial(MDLFNAFLens->getmodel(), &bluefnaflens_texlist, &BMFNAFLens, ALS_LensSpecial, NULL, "Blue Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);
        GreenFNAFLensID = cwe_api->RegisterChaoSpecial(MDLFNAFLens->getmodel(), &greenfnaflens_texlist, &BMFNAFLens, ALS_LensSpecial, NULL, "Green Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);
        PinkFNAFLensID = cwe_api->RegisterChaoSpecial(MDLFNAFLens->getmodel(), &pinkfnaflens_texlist, &BMFNAFLens, ALS_LensSpecial, NULL, "Pink Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);
        PurpleFNAFLensID = cwe_api->RegisterChaoSpecial(MDLFNAFLens->getmodel(), &purplefnaflens_texlist, &BMFNAFLens, ALS_LensSpecial, NULL, "Purple Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);
        YellowFNAFLensID = cwe_api->RegisterChaoSpecial(MDLFNAFLens->getmodel(), &yellowfnaflens_texlist, &BMFNAFLens, ALS_LensSpecial, NULL, "Yellow Animatronic Lens", "Lenses made for a type of robot, but they work on chao too.", false);

        cwe_api->RegisterEyeColor("BlueFNAFLens", &bluefnaflens_texlist, BlueFNAFLensID);
        cwe_api->RegisterEyeColor("GreenFNAFLens", &greenfnaflens_texlist, GreenFNAFLensID);
        cwe_api->RegisterEyeColor("PinkFNAFLens", &pinkfnaflens_texlist, PinkFNAFLensID);
        cwe_api->RegisterEyeColor("PurpleFNAFLens", &purplefnaflens_texlist, PurpleFNAFLensID);
        cwe_api->RegisterEyeColor("YellowFNAFLens", &yellowfnaflens_texlist, YellowFNAFLensID);

        //Define Character Chao data:
        CWE_API_CHAO_DATA FreddyChao_pData =
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

        CWE_API_CHAO_DATA BonnieChao_pData =
        {
            MDLBonnieBunChao->getmodel(),    //pObject
            {0},                //pSecondEvoList[5]

            "BonnieBunChao",            //TextureName
            8,                //TextureCount
            0xFFE05773,            //IconColor - hex, 6 bytes
            ICON_TYPE_BALL,            //IconType
            NULL,                //pIconData

            BonnieBunEvolve,            //pEvolveFunc 

            0,                //Flags
            "BonnieBun",            //Name
            "cwe_rt_BonnieBn",            //id
        };

        //add the Chao Type
        cwe_api->AddChaoType(&FreddyChao_pData);
        cwe_api->AddChaoType(&BonnieChao_pData);

    }

    //initialization function - MUST exist in order to have CWE and SA2 see your mod
    __declspec(dllexport) void Init(const char* path)
    {
        HMODULE h = GetModuleHandle(L"CWE");

        std::string pathStr = std::string(path) + "\\";

        //Lens function - This talks to CWE to get the lens to work when registering it as a special object.
        ALS_LensSpecial = (decltype(ALS_LensSpecial))GetProcAddress(GetModuleHandle(L"CWE"), "ALS_LensSpecial");

        MDLFreddyFazChao = new ModelInfo(pathStr + "FreddyFazChao.sa2mdl");
        MDLBonnieBunChao = new ModelInfo(pathStr + "BonnieBunChao.sa2mdl");
        MDLFNAFLens = new ModelInfo(pathStr + "lensNeut.sa2mdl");
        MDLClassicTopHat = new ModelInfo(pathStr + "ClassicTopHat.sa2mdl");
        MDLClassicBearTie = new ModelInfo(pathStr + "ClassicBearBowtie.sa2mdl");

        RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
        RegisterDataFunc(CWELoad);
    }
    __declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}