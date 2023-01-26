#pragma once
#include "Client_Defines.h"
#include "GameObj.h"


BEGIN(Engine)
class CCollider;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CCharacters : public CGameObj
{
protected:
	CCharacters(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacters(const CCharacters& rhs);
	virtual ~CCharacters() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

protected:
	typedef struct tagInfo {
		wstring		strName; //ĳ���� �̸�;
		_bool		bOni;    //true = ���� , false = �ΰ�;
		_int		iMaxHp;  
		_int		iHp;
		_int		iSkMaxBar; //�ƽ���ų������
		_int		iSkBar;    //��ų������
		_int		iUnicMaxBar; //�ƽ��ʻ�������
		_int		iUnicBar;    //�ʻ�������
		_int		iDmg;      //��Ÿ ������
		_int		iCombo;
		_float		fComboTime;
		_bool		bPowerUp;
		_float		fPowerUpTime;
		_int		iFriendMaxBar; //�ƽ�ģ��������
		_int		iFriendBar;    //ģ��������
		_bool		bGuard;
		_bool		bJump;
	}PLAYERINFO;

public:
	PLAYERINFO Get_PlayerInfo() { return m_tInfo; }
	void	   Set_Hp(_int _iDmg) { m_tInfo.iHp += _iDmg; }
	void	   Set_bGuard(_bool _bGuard) { m_tInfo.bGuard = _bGuard; }
	void	   Set_BattleTarget(CCharacters* _BattleTarget) { m_pBattleTarget = _BattleTarget; }
	CCharacters*	Get_BattleTarget() { return m_pBattleTarget; }
	CCollider* Get_SphereCollider() { return m_pSphereCom; }
	_int	   Get_i1P() { return m_i1p; }
	void	   Set_iTargetIndex(_int _iTargetIndex) { m_iTargetIndex = _iTargetIndex; }
	_int       Get_iTargetIndex() { return m_iTargetIndex; }
	
	virtual	void  Take_Damage(_float _fPow) = 0;
	virtual	void  Get_GuardHit(_int eType) = 0;
protected:
	PLAYERINFO		m_tInfo;
	CCharacters*	m_pBattleTarget = nullptr;
	_int			m_iTargetIndex = 0;

	CCollider*				m_pSphereCom = nullptr;
	_int					m_i1p = 0;
public:
//	static CCharacters* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
