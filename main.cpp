#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"

extern "C"
{
	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);

	//Define Models
	ModelInfo* MDLScarabChao;
	ModelInfo* MDLscarabmask;

	int ScarabMaskID;

	NJS_TEXNAME ScarabTex[10];
	NJS_TEXLIST scarab_texlist = { arrayptrandlength(ScarabTex) };

	BlackMarketItemAttributes BMScarabMask = { 1000, 500, 0, -1, -1, 0 };

	//Define a pointer function for pEvolveFunc
	static bool ScarabEvolve(ObjectMaster* tp)
	{
		Uint16* accessories = (Uint16*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x614);
		Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);
		//std::string PrintAccessoryString = std::to_string(ScarabMaskID);
		//PrintDebug(PrintAccessoryString.c_str());
		//std::string PrintEyeColorString = std::to_string(eye_color);
		//PrintDebug(PrintEyeColorString.c_str());
		if (eye_color == 7 && accessories[Face] == (ScarabMaskID + 1))
		{
			PrintDebug("Chao evolving into Scarab");
			return true;
		}
		else
			return false;
	}

	//main CWE Load function -- Important stuff like adding your CWE mod goes here
	void CWELoad(CWE_REGAPI* cwe_api, ObjectMaster* tp)
	{

		cwe_api->RegisterChaoTexlistLoad("ScarabMask", &scarab_texlist);
		ScarabMaskID = cwe_api->RegisterChaoAccessory(Face, MDLscarabmask->getmodel(), &scarab_texlist, &BMScarabMask, "Scarab Mask", "Don\'t worry, this crossover is authorized.");

		//Define Character Chao data:
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
		cwe_api->AddChaoType(&CharChao_pData);
	}

	//initialization function - MUST exist in order to have CWE and SA2 see your mod
	__declspec(dllexport) void Init(const char* path)
	{
		HMODULE h = GetModuleHandle(L"CWE");

		std::string pathStr = std::string(path) + "\\";

		MDLScarabChao = new ModelInfo(pathStr + "ScarabChao.sa2mdl");
		MDLscarabmask = new ModelInfo(pathStr + "ScarabMask.sa2mdl");

		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
