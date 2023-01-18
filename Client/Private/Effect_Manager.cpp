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

void CEffect_Manager::Load_Effect(const _tchar * EffectName)
{
	m_Effect.push_back(EffectName);

	_tchar szName[MAX_PATH] = TEXT("../Bin_Data/Effect/");
	wcscat_s(szName, EffectName);
	wcscat_s(szName, TEXT(".bin"));

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(szName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return;

	_int iEffectSize;

	//이펙트 정보 저장
	CEffect::EFFECT_INFO EffectInfo;
	ReadFile(hFile, &EffectInfo, sizeof(CEffect::EFFECT_INFO), &dwByte, nullptr);

	//텍스처 정보 저장
	_int iTextureSize;
	ReadFile(hFile, &iTextureSize, sizeof(_int), &dwByte, nullptr);

	vector<CEffect_Texture::TEXTURE_INFO> TexInfo;

	for (_uint j = 0; j < iTextureSize; ++j) {
		CEffect_Texture::TEXTURE_INFO TextureInfo;

		ReadFile(hFile, &TextureInfo, sizeof(CEffect_Texture::TEXTURE_INFO), &dwByte, nullptr);

		TexInfo.push_back(TextureInfo);
	}

	// 메쉬 정보 저장 

	// 파티클 정보 저장

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(EffectName,
		CEffect::Create(m_pDevice, m_pContext, EffectInfo, TexInfo))))
		return;

	RELEASE_INSTANCE(CGameInstance);
	
	CloseHandle(hFile);

	return;
}

void CEffect_Manager::Create_Effect(_uint iEffectNum)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(m_Effect[iEffectNum], LEVEL_GAMEPLAY, TEXT("Layer_Effect"))))
		return ;

	RELEASE_INSTANCE(CGameInstance);
}

void CEffect_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
