#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"

extern "C"
{
	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);

	//Lens function - Needed for CWE to recognize the lens.
	__declspec(dllexport) void(__cdecl* ALS_LensSpecial)(ObjectMaster*, ObjectMaster*);

	//Define Models
	ModelInfo* MDLAIChao;
	ModelInfo* MDLHollowLens;

	//Generate an ID for each lens
	int HollowLensID;

	NJS_TEXNAME HollowTex[10];
	NJS_TEXLIST hollow_texlist = { arrayptrandlength(HollowTex) };

	BlackMarketItemAttributes HollowLens = { 1000, 500, 0, -1, -1, 0 };

	//Define a pointer function for pEvolveFunc
	static bool AIEvolve(ObjectMaster* tp)
	{
		Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);
		if (tp->Data1.Chao->ChaoDataBase_ptr->Alignment > 0.75 && eye_color == (HollowLensID + 1))
		{
			PrintDebug("Chao evolving into AI Chao");
			return true;
		}
		else
			return false;
	}

	//main CWE Load function -- Important stuff like adding your CWE mod goes here
	void CWELoad(CWE_REGAPI* cwe_api, ObjectMaster* tp)
	{
		cwe_api->RegisterChaoTexlistLoad("HollowLens", &hollow_texlist);

		//Register your lens to the black market:
		cwe_api->RegisterChaoSpecial(MDLHollowLens->getmodel(), &hollow_texlist, &HollowLens, ALS_LensSpecial, NULL, "Hollow Lens", "The sockets are empty.", false);

		//Associate the lens as a custom eye color:
		cwe_api->RegisterEyeColor("HollowLens", &hollow_texlist, HollowLensID);

		//Define Character Chao data:
		CWE_API_CHAO_DATA CharChao_pData =
		{
			MDLAIChao->getmodel(),	//pObject
			{0},				//pSecondEvoList[5]

			"AIChao",			//TextureName
			7,				//TextureCount
			0xFF247201,			//IconColor - hex, 6 bytes
			ICON_TYPE_BALL,			//IconType
			NULL,				//pIconData

			AIEvolve,			//pEvolveFunc 

			0,				//Flags
			"AIChao",			//Name
			"cwe_rt_aichao",			//id
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

		MDLAIChao = new ModelInfo(pathStr + "AIChao.sa2mdl");
		MDLHollowLens = new ModelInfo(pathStr + "lensHollow.sa2mdl");

		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
