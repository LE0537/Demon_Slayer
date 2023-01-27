#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

public:
	enum EFFECT { EFF_HIT, EFF_TANJIRO_ATTACK1, EFF_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	void Load_Effect(const _tchar* EffectName);
	void Create_Effect(_uint iEffectNum, _matrix mtrWorld);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<const _tchar*>			m_Effect;

public:
	virtual void Free() override;
};

END