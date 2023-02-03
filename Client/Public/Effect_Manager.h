#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Characters.h"

BEGIN(Client)


class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

	//	RGK : ������
	//  TAN : ź����
	//  WATER : ���� ȣ��(���ڴ� ����)

public:	
	enum EFFECT { EFF_HIT,																						// �⺻ ����Ʈ
		EFF_TANATTACK1, EFF_TANATTACK2_1, EFF_TANATTACK2_2, EFF_TANATTACK3, EFF_TANATTACK4,						// ź���� ��Ÿ
		EFF_WATER2_1, EFF_WATER6_1, EFF_WATER8_1,																// ���� ȣ��
		EFF_RGKATK1, EFF_RGKATK2_1, EFF_RGKATK2_2, EFF_RGKATK3, EFF_RGKATK4,									// ������ ��Ÿ
		EFF_RGKSKL_3KIEN_START, EFF_RGKSKL_3KIEN_MAIN, EFF_RGKSKL_2NOBORI_1, EFF_RGKSKL_2NOBORI_2,				// ������ ��ų
		EFF_RGKSKL_5TIGER_CHARGE, EFF_RGKSKL_5TIGER_MAIN,														// ������ ��ų
		EFF_END		
	};	

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	void Load_Effect(const _tchar* EffectName);
	void Create_Effect(_uint iEffectNum, CCharacters* pTarget);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<const _tchar*>			m_Effect;

public:
	virtual void Free() override;
};

END