#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

public:
	enum EFFECT { EFFECT_HIT, EFFECT_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	void Load_Effect(const _tchar* EffectName);
	void Create_Effect(_uint iEffectNum);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<const _tchar*>			m_Effect;

public:
	virtual void Free() override;
};

END