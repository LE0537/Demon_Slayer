#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int			iCurrentCellIndex = -1;
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	void		Set_NaviIndex(_uint iNaviIndex) { m_NaviDesc.iCurrentCellIndex = iNaviIndex; }
	_float4		Get_NavigationHeight() const { return m_vPlayerHeight; }
	void		Set_HeightBool(_bool Height) { m_bHeight = Height; }
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationData);
	virtual HRESULT Initialize(void* pArg);

public:
	_bool isMove(_fvector vPosition, _fvector vMoveDir, _Out_ _float3 * pOut);
	_float3* Get_CellPoints();
	vector<class CCell*>* Get_Cells() { return &m_Cells; }
public:
	HRESULT Render();
	void	Find_CurrentCellIndex(_vector vPos);
	void	Navigation_Height(_fvector vecPosition);
private:
	NAVIDESC				m_NaviDesc;
	vector<class CCell*>	m_Cells;


private:
	class CShader*			m_pShader = nullptr;
	_float4					m_vPlayerHeight;
	_bool					m_bHeight = false;
private:
	HRESULT SetUp_Neighbor();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationData);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END