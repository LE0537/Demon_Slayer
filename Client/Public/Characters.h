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
class CModel;
END


BEGIN(Client)

class CCharacters : public CGameObj
{
public:
	enum HIT_TYPE {HIT_UPPER, HIT_UPPER_2 , HIT_BOUND, HIT_KNOCKBACK, HIT_END };
	enum PLAYER_TYPE {PLAYER_TANJIRO, PLAYER_KYOUJURO, PLAYER_RUI, PLAYER_AKAZA, PLAYER_NEZUKO, PLAYER_SHINOBU, PLAYER_END};
	enum SKILL_TYPE {SKILL_010, SKILL_020, SKILL_030, SKILL_040, SKILL_050, SKILL_060, SKILL_070, SKILL_080, SKILL_090 , SKILL_100, SKILL_110, SKILL_END};

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

	void		Set_PlayerOriginPosY(_float fTimeDelta);
	void			Set_Effect(CGameObj* _Effect) { m_pEffect = _Effect; }

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
		_int		iUnicCount; //필살게이지 갯수
		_int		iDmg;      //평타 데미지
		_int		iCombo;
		_float		fComboTime;
		_float		fPowerUp;    //개방 추뎀
		_float		fPowerUpTime;  //개방시간
		_int		iPowerIndex;
		_float		iFriendMaxBar; //맥스친구게이지
		_float		iFriendBar;    //친구게이지
		_bool		bGuard;
		_bool		bJump;
		_float		fHitTime;
		_bool		bSub;
		_bool		bChange;
		_int		iMaxGuard;
		_int		iGuard;
		_int		iAccDamage;
		_float      fGuardTime;
	}PLAYERINFO;

public:
	PLAYERINFO Get_PlayerInfo() { return m_tInfo; }
	void	   Set_Hp(_int _iDmg) { m_tInfo.iHp += _iDmg; m_tInfo.iAccDamage += abs(_iDmg); }
	void	   Set_AccDmg() { m_tInfo.iAccDamage = 0; }
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
	void	   Set_FriendSkillBar(_float _fSkill) { m_tInfo.iFriendBar += _fSkill; }
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
	void	   Set_ComboTime(_float _fTime) { m_tInfo.fComboTime = _fTime; }
	void	   Set_BattleStart(_bool m_bStart) { m_bBattleStart = m_bStart; }
	_uint	   Get_TargetState() const { return m_iState; }
	void	   Set_SubSkill(_int _iIndex) { m_iSubSkillIndex = _iIndex; }
	_int       Get_SubSkill() { return m_iSubSkillIndex; }
	void	   Set_GuardHp(_int _iGuard) { m_tInfo.iGuard += _iGuard; }
	void	   Set_ResetGuardHp() { m_tInfo.iGuard = m_tInfo.iMaxGuard; }
	void	   Set_GuardTime(_float _fGuard) { m_tInfo.fGuardTime = _fGuard; }
	_bool	   Get_StoryKey() { return m_bStoryKey; }
	void	   Set_StoryKey(_bool _bKey) { m_bStoryKey = _bKey; }
	void	   Set_UnicBar(_int _iUnic) { m_tInfo.iUnicBar += _iUnic; }
	void	   Reset_UnicBar(){ m_tInfo.iUnicBar = 0; }
	void	   Set_UnicCount(_int _iUnic) { m_tInfo.iUnicCount += _iUnic; }
	void	   Set_PowerUp(_float _fPower){ m_tInfo.fPowerUp = _fPower; }
	void	   Set_PowerUpTime(_float _fPower) { m_tInfo.fPowerUpTime = _fPower; }
	void	   Set_PowerIndex(_int _iIndex) { m_tInfo.iPowerIndex = _iIndex; }
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0) = 0;
	virtual	void  Get_GuardHit(_int eType) = 0;
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0) PURE;
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower) PURE;
	virtual void  Play_Scene() PURE;
	virtual CModel* Get_Model() { return nullptr; };

	void	   Set_Atk2(_bool bAtk2) { m_bAtk2 = bAtk2; }
	_bool	   Get_Atk2() { return m_bAtk2; }

	void	   Set_GodMode(_bool bGodMode) { m_bGodMode = bGodMode; }
	_bool	   Get_GodMode() const { return m_bGodMode; }

	void	   Set_InteractionCheck(_bool bCheck) { m_bInteractionCheck = bCheck; }
	_bool	   Get_InteractionCheck() { return m_bInteractionCheck; }

	void	   Set_SplSkl(_bool bSkpSkl) { 
		if(bSkpSkl == true)
			m_bSplSkl = true;
		else if (bSkpSkl == false)
		{
			m_bSplSkl = false;
			m_bSplEffect = false;
		}
	}
	_bool	   Get_SplSkl() const { return m_bSplSkl; }


	PLAYER_TYPE	Get_PlayerType() const { return m_ePlayerType; }
	SKILL_TYPE Get_SkillType() const { return m_eSkillType; }
	void Set_SkillType(SKILL_TYPE eType) { m_eSkillType = eType; }
	void Set_SceneRender(_bool bRender) { m_bSceneRender = bRender; }

	_float4x4* Get_WeaponWorld() { return m_WeaponWorld; }

protected:
	PLAYERINFO				m_tInfo;
	CCharacters*			m_pBattleTarget = nullptr;
	CCharacters*			m_pSubChar = nullptr;
	_int					m_iTargetIndex = 0;

	CCollider*				m_pSphereCom = nullptr;
	_int					m_i1p = 0;
	_bool					m_bChange = false;
	_float					m_fChangeTime = 0.f;
	CNavigation*			m_pNavigationCom = nullptr;
	_float					m_fDelta = 0.f;
	_float					m_fChangeDelay = 0.f;

	_int					m_iModelIndex = 0;
	_int					m_iAnimIndex = 0;
	_bool					m_bBattleStart = false;

	_uint					m_iState = 0;
	_bool					m_bStoryKey = false;
	//Sub
	_int					m_iSubSkillIndex = 0;

	_bool					m_bAtk2 = false;
	_bool					m_bGodMode = false;
	_bool					m_bSplSkl = false;
	_bool					m_bSplEffect = false;
	queue<_uint>			m_SplSklIndex;
	queue<_uint>			m_SplSklHitIndex;
	
	PLAYER_TYPE				m_ePlayerType = PLAYER_END;
	SKILL_TYPE				m_eSkillType = SKILL_END;


	CGameObj*				 m_pEffect = nullptr;

	_bool					m_bSceneRender = true;

	_float4x4*				m_WeaponWorld;
	_bool					m_bInteractionCheck = false;

public:
//	static CCharacters* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
