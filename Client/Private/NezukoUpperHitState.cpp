#include "stdafx.h"
#include "NezukoUpperHitState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"

using namespace Nezuko;

CUpperHitState::CUpperHitState(CNezuko::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime)
	: m_fBoundPower(fBoundPower), m_fJumpPower(fJumpPower), m_fKnockBackPower(fKnockBackPower), m_fJumpTime(fJumpTime)
{
	m_eStateType = eType;
	m_eHitType = eHitType;
}

CNezukoState * CUpperHitState::HandleInput(CNezuko* pNezuko)
{

	return nullptr;
}

CNezukoState * CUpperHitState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (!m_bReset && m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (pNezuko->Get_PlayerInfo().iUnicCount < 3 && pNezuko->Get_PlayerInfo().iUnicBar < pNezuko->Get_PlayerInfo().iUnicMaxBar)
		{
			pNezuko->Set_UnicBar(33);
			pNezuko->Get_BattleTarget()->Set_UnicBar(16);
			if (pNezuko->Get_PlayerInfo().iUnicBar >= pNezuko->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pNezuko->Get_PlayerInfo().iUnicCount < 3)
				{
					pNezuko->Reset_UnicBar();
					pNezuko->Set_UnicCount(1);
				}
				else
					pNezuko->Set_UnicBar(pNezuko->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		m_bReset = true;
	}

	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	switch (m_eHitType)
	{
	case Client::CCharacters::HIT_UPPER:
		return UpperState(pNezuko, fTimeDelta);
		break;
	case Client::CCharacters::HIT_BOUND:
		return BoundState(pNezuko, fTimeDelta);
		break;
	case Client::CCharacters::HIT_KNOCKBACK:
		return KnockBackState(pNezuko, fTimeDelta);
		break;
	case Client::CCharacters::HIT_UPPER_2:
		return Upper2State(pNezuko, fTimeDelta);
		break;
	case Client::CCharacters::HIT_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CUpperHitState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{


	pNezuko->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CUpperHitState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	m_vVelocity = { 10.f,10.f,10.f };

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_UPPER_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_UPPER_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);

		if (pNezuko->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pNezuko->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_UPPER_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_UPPER_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex(), true);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_UPPER_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_UPPER_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_DOWN_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_DOWN_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	}
}



void CUpperHitState::Increase_Height(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 50.f;


	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.y += m_vVelocity.y * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_vPosition.y += m_vVelocity.y * fTimeDelta;
			vec = XMVectorSetY(vec, m_vPosition.y);
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}
}

void CUpperHitState::Fall_Height(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;


	m_fJumpTime += 0.035f;

	static _float fGravity = 50.f;
	static _float fVelocity = 21.25f;
	static _float fStartHeight = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//fStartHeight + m_vVelocity.y * fTimeDelta + 0.5 * fGravity * fTimeDelta * fTimeDelta;


	// m_vVelocity.x = velocity

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	m_vPosition.y -= m_vVelocity.y * fTimeDelta;

	m_vVelocity.x = 10.f;
	m_vVelocity.z = 10.f;

	m_vPosition.x += XMVectorGetX(vLookAt) * m_vVelocity.x * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_vVelocity.z * fTimeDelta * 1.1f;


	m_vVelocity.y = 10.f;
	m_vPosition.y = fStartHeight + m_vVelocity.y * m_fJumpTime - (0.5f * 15.f * m_fJumpTime * m_fJumpTime);

	//_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pNezuko->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::KnockBack_Player(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	m_vVelocity.y += 50.f * fTimeDelta;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;



	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;

	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

	m_vPosition.y -= 3.f;

	//m_vPosition.y -= XMVectorGetY(vTest) * fTimeDelta;

	//m_vPosition.y = fStartHeight + 30.f * m_fJumpTime - (0.5 * 40.f * m_fJumpTime * m_fJumpTime);
#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pNezuko->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Bound_Player(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;



	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pNezuko->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Upper_Player(CNezuko* pNezuko, _float fTimeDelta)
{


	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	// bound = 10
	// jump power 15

	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;

	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pNezuko->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}





}

CNezukoState * CUpperHitState::KnockBackState(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			pNezuko->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		KnockBack_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		KnockBack_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_END:
		KnockBack_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}





	return nullptr;
}

CNezukoState * CUpperHitState::BoundState(CNezuko* pNezuko, _float fTimeDelta)
{

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			pNezuko->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
			
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
			return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		Bound_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		Bound_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_END:
		Bound_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}

CNezukoState * CUpperHitState::UpperState(CNezuko* pNezuko, _float fTimeDelta)
{

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			
			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if(m_fDelay >=0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		Upper_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		Upper_Player(pNezuko, fTimeDelta);

		if (m_bNextAnim)
		{
			pNezuko->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
		}
		break;
	case Client::CNezukoState::TYPE_END:
		//Bound_Player(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}

	return nullptr;
}

CNezukoState * CUpperHitState::Upper2State(CNezuko * pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}





	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		Increase_Height(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		Fall_Height(pNezuko, fTimeDelta);

		if (m_bNextAnim == true)
			return new CUpperHitState(m_eHitType, TYPE_END, 0.f, 0.f, 0.f);
		break;
	case Client::CNezukoState::TYPE_END:
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}


void CUpperHitState::Exit(CNezuko* pNezuko)
{
	pNezuko->Set_HitTime(0.3f);
	//pNezuko->Get_Model()->Reset_Anim(pNezuko->Get_AnimIndex());
}



