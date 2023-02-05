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
class CNavigation;

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
	_float4		Get_NavigationHeight();
	void		Set_NavigationHeight(_fvector vPosition);

public:
	typedef struct tagInfo {
		wstring		strName; //캐릭터 이름;
		_bool		bOni;    //true = 오니 , false = 인간;
		_int		iMaxHp;  
		_int		iHp;
		_int		iSkMaxBar; //맥스스킬게이지
		_int		iSkBar;    //스킬게이지
		_int		iUnicMaxBar; //맥스필살기게이지
		_int		iUnicBar;    //필살기게이지
		_int		iDmg;      //평타 데미지
		_int		iCombo;
		_float		fComboTime;
		_bool		bPowerUp;
		_float		fPowerUpTime;
		_int		iFriendMaxBar; //맥스친구게이지
		_int		iFriendBar;    //친구게이지
		_bool		bGuard;
		_bool		bJump;
		_float		fHitTime;
		_bool		bSub;
		_bool		bChange;
	}PLAYERINFO;

public:
	PLAYERINFO Get_PlayerInfo() { return m_tInfo; }
	void	   Set_Hp(_int _iDmg) { m_tInfo.iHp += _iDmg; }
	void	   Set_bGuard(_bool _bGuard) { m_tInfo.bGuard = _bGuard; }
	void	   Set_JumpState(_bool bJump) { m_tInfo.bJump = bJump; }
	void	   Set_BattleTarget(CCharacters* _BattleTarget) { m_pBattleTarget = _BattleTarget; }
	CCharacters*	Get_BattleTarget() { return m_pBattleTarget; }
	CCollider* Get_SphereCollider() { return m_pSphereCom; }
	_int	   Get_i1P() { return m_i1p; }
	void	   Set_1P(_int i1p) { m_i1p = i1p; }
	void	   Set_iTargetIndex(_int _iTargetIndex) { m_iTargetIndex = _iTargetIndex; }
	_int       Get_iTargetIndex() { return m_iTargetIndex; }
	void	   Set_SkillBar(_int _iSkill) { m_tInfo.iSkBar += _iSkill; }
	CNavigation*	Get_NavigationCom() { return m_pNavigationCom; }
	void	   Set_HitTime(_float _fHitTime) { m_tInfo.fHitTime = _fHitTime; }
	void	   Set_Sub(_bool _bSub){ m_tInfo.bSub = _bSub; }
	void	   Set_Change(_bool _bChange, _vector _vPos);
	_bool	   Get_Change() { return m_bChange; };
	void	   Set_SubChar(CCharacters* _pSubChar) { m_pSubChar = _pSubChar; }
	CCharacters* 	  Get_SubChar() { return m_pSubChar; }
	_float     Get_ChangeTime() { return m_fChangeTime; }
	void	   Change_Info(PLAYERINFO _tinfo);
	void	   Set_ChangeInfo(_bool _bChange) { m_tInfo.bChange = _bChange; }
	void	   Set_ChangeDelay(_float _fDelayTime) { m_fChangeDelay = _fDelayTime; }
	void	   Set_ModelIndex(_int _iIndex) { m_iModelIndex = _iIndex; }
	void	   Set_AnimIndex(_int _iIndex) { m_iAnimIndex = _iIndex; }
	void	   Set_Combo(_int _iCombo) { m_tInfo.iCombo += _iCombo; }
	void	   Reset_Combo() { m_tInfo.iCombo = 0; }
	void	   Set_ComboTime(_float _fTime) { m_tInfo.fComboTime += _fTime; }

	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0) = 0;
	virtual	void  Get_GuardHit(_int eType) = 0;
protected:
	PLAYERINFO		m_tInfo;
	CCharacters*	m_pBattleTarget = nullptr;
	CCharacters*	m_pSubChar = nullptr;
	_int			m_iTargetIndex = 0;

	CCollider*				m_pSphereCom = nullptr;
	_int					m_i1p = 0;
	_bool					m_bChange = false;
	_float					m_fChangeTime = 0.f;
	CNavigation*		m_pNavigationCom = nullptr;
	_float					m_fDelta = 0.f;
	_float					m_fChangeDelay = 0.f;

	_int					m_iModelIndex = 0;
	_int					m_iAnimIndex = 0;
public:
//	static CCharacters* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
