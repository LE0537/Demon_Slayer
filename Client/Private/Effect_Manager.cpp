#include "stdafx.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Effect.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CEffect_Manager::Load_Effect()
{
	const _tchar* cPullName = TEXT("../Bin_Data/Effect/Effect.bin");

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(cPullName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return;

	_int iEffectSize;

	ReadFile(hFile, &iEffectSize, sizeof(_int), &dwByte, nullptr);
	for (_uint i = 0; i < iEffectSize; ++i) {
		//이펙트 정보 저장
		char* EffectName[MAX_PATH];
		ReadFile(hFile, EffectName, sizeof(char) * MAX_PATH, &dwByte, nullptr);

		CEffect::EFFECT_INFO EffectInfo;
		ReadFile(hFile, &EffectInfo, sizeof(CEffect::EFFECT_INFO), &dwByte, nullptr);

		//텍스처 정보 저장
		_int iTextureSize;
		ReadFile(hFile, &iTextureSize, sizeof(_int), &dwByte, nullptr);

		vector<CEffect_Texture::TEXTURE_INFO> TexInfo;
		for (_uint j = 0; j < iTextureSize; ++j) {
			CEffect_Texture::TEXTURE_INFO TextureInfo;

			//ReadFile(hFile, m_szNewTextureName, sizeof(char) * MAX_PATH, &dwByte, nullptr);

			ReadFile(hFile, &TextureInfo, sizeof(CEffect_Texture::TEXTURE_INFO), &dwByte, nullptr);

			TexInfo.push_back(TextureInfo);
		}

		// 메쉬 정보 저장 

		// 파티클 정보 저장

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		char szName[MAX_PATH] = "Prototype_GameObject_Effect";
		strcat_s(szName, to_string(i).c_str());

		_tchar szRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szName, strlen(szName), szRealPath, MAX_PATH);

		if (FAILED(pGameInstance->Add_Prototype(szRealPath,
			CEffect::Create(m_pDevice, m_pContext, *EffectName, EffectInfo, TexInfo))))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}

	CloseHandle(hFile);

	return;
}

void CEffect_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
