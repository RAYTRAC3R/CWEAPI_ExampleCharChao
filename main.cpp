#include "pch.h"

#include "cwe_api.h"

#include "ModelInfo.h" 

static ModelInfo* MDLScarabChao;

// just a simple code snippet to see usage of the function

static int ScarabMaskID;

// the accessory ID fetching cannot go in EarlyLoad, since we need our code
// to run once the accessory has already been registered through the JSONs
extern "C" __declspec(dllexport) void CWEAPI_Load(CWE_API* pAPI) {
    ScarabMaskID = pAPI->pRegister->pAccessory->GetAccessoryIndex("acc67b6f597");
}

static bool ScarabEvolve(ObjectMaster* tp) {
	Uint16* accessories = (Uint16*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x614);
	Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);
	// std::string PrintAccessoryString = std::to_string(ScarabMaskID);
	// PrintDebug(PrintAccessoryString.c_str());
	// std::string PrintEyeColorString = std::to_string(eye_color);
	// PrintDebug(PrintEyeColorString.c_str());
	if (eye_color == 7 && accessories[Face] == (ScarabMaskID + 1))
	{
		// PrintDebug("Chao evolving into Scarab");
		return true;
	}
	else
		return false;
}

extern "C" {
    __declspec(dllexport) void CWEAPI_EarlyLoad(CWE_API* pAPI) {
        CWE_API_CHAO_DATA CharChao_pData =
		{
			MDLScarabChao->getmodel(),	//pObject
			{0},				//pSecondEvoList[5]

			"ScarabChao",			//TextureName
			7,				//TextureCount
			0xFFFBAC66,			//IconColor - hex, 6 bytes
			ICON_TYPE_SPIKY,			//IconType
			NULL,				//pIconData

			ScarabEvolve,			//pEvolveFunc 

			0,				//Flags
			"Scarab",			//Name
			"cwe_rt_scarab",			//id
		};

        //add the Chao Type
        pAPI->pRegister->pChao->AddChaoType(&CharChao_pData);
    }

    __declspec(dllexport) void Init(const char* path) {
        std::string pathStr = std::string(path) + "\\";

        MDLScarabChao = new ModelInfo(pathStr + "ScarabChao.sa2mdl");
    }

    __declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}