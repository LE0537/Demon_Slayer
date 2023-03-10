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

	//이펙트 정보 저장
	CEffect::EFFECT_INFO EffectInfo;
	ReadFile(hFile, &EffectInfo, sizeof(CEffect::EFFECT_INFO), &dwByte, nullptr);

	//텍스처 정보 저장
	_int iTextureSize;
	ReadFile(hFile, &iTextureSize, sizeof(_int), &dwByte, nullptr);

	vector<CEffect_Texture::TEXTURE_INFO> TexInfo;

	for (_int j = 0; j < iTextureSize; ++j) {
		CEffect_Texture::TEXTURE_INFO TextureInfo;

		ReadFile(hFile, &TextureInfo, sizeof(CEffect_Texture::TEXTURE_INFO), &dwByte, nullptr);

		TexInfo.push_back(TextureInfo);
	}

	// 메쉬 정보 저장 
	_int iMeshSize;
	ReadFile(hFile, &iMeshSize, sizeof(_int), &dwByte, nullptr);

	vector<CEffect_Mesh::MESH_INFO> MeshInfoes;

	for (_int j = 0; j < iMeshSize; ++j) {
		CEffect_Mesh::MESH_INFO MeshInfo;

		ReadFile(hFile, &MeshInfo, sizeof(CEffect_Mesh::MESH_INFO), &dwByte, nullptr);

		MeshInfoes.push_back(MeshInfo);
	}

	// 파티클 정보 저장
	_int iParticleSize;
	ReadFile(hFile, &iParticleSize, sizeof(_int), &dwByte, nullptr);

	vector<CEffect_Particle::PARTICLE_INFO> ParticleInfoes;

	for (_int j = 0; j < iParticleSize; ++j) {
		CEffect_Particle::PARTICLE_INFO ParticleInfo;

		ReadFile(hFile, &ParticleInfo, sizeof(CEffect_Particle::PARTICLE_INFO), &dwByte, nullptr);

		ParticleInfoes.push_back(ParticleInfo);
	}

	// New 파티클 정보 저장
	_int iNewParticleSize;
	ReadFile(hFile, &iNewParticleSize, sizeof(_int), &dwByte, nullptr);

	if (iNewParticleSize < 0 || iNewParticleSize > 30) {
		iNewParticleSize = 0;
	}

	vector<CEffect_Particle_New::PARTICLE_INFO> NewParticleInfoes;

	for (_int j = 0; j < iNewParticleSize; ++j) {
		CEffect_Particle_New::PARTICLE_INFO NewParticleInfo;

		ReadFile(hFile, &NewParticleInfo, sizeof(CEffect_Particle_New::PARTICLE_INFO), &dwByte, nullptr);

		NewParticleInfoes.push_back(NewParticleInfo);
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(EffectName,
		CEffect::Create(m_pDevice, m_pContext, m_Effect.size() - 1, EffectInfo, TexInfo, MeshInfoes, ParticleInfoes, NewParticleInfoes))))
		return;

	RELEASE_INSTANCE(CGameInstance);
	
	CloseHandle(hFile);

	return;
}

void CEffect_Manager::Create_Effect(_uint iEffectNum, void* pTarget)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_Effect.size() == 0) {
		int i = 0;
	}

	if (FAILED(pGameInstance->Add_GameObject(m_Effect[iEffectNum], g_iLevel, TEXT("Layer_Effect"), pTarget)))
		return ;

	RELEASE_INSTANCE(CGameInstance);
} 

void CEffect_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
