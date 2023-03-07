#include "stdafx.h"
#include "RuiUpperHitState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
#include "Level_GamePlay.h"
using namespace Rui;

CUpperHitState::CUpperHitState(CRui::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime)
	: m_fBoundPower(fBoundPower), m_fJumpPower(fJumpPower), m_fKnockBackPower(fKnockBackPower), m_fJumpTime(fJumpTime)
{
	m_eStateType = eType;
	m_eHitType = eHitType;
}

CRuiState * CUpperHitState::HandleInput(CRui* pRui)
{

	return nullptr;
}

CRuiState * CUpperHitState::Tick(CRui* pRui, _float fTimeDelta)
{
	if (!m_bReset && m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar)
		{
			pRui->Set_UnicBar(33);
			pRui->Get_BattleTarget()->Set_UnicBar(16);
			if (pRui->Get_PlayerInfo().iUnicBar >= pRui->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pRui->Get_PlayerInfo().iUnicCount < 3)
				{
					pRui->Reset_UnicBar();
					pRui->Set_UnicCount(1);
				}
				else
					pRui->Set_UnicBar(pRui->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		m_bReset = true;
	}


	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}

	switch (m_eHitType)
	{
	case Client::CCharacters::HIT_UPPER:
		return UpperState(pRui, fTimeDelta);
		break;
	case Client::CCharacters::HIT_BOUND:
		return BoundState(pRui, fTimeDelta);
		break;
	case Client::CCharacters::HIT_KNOCKBACK:
		return KnockBackState(pRui, fTimeDelta);
		break;
	case Client::CCharacters::HIT_UPPER_2:
		return Upper2State(pRui, fTimeDelta);
		break;
	case Client::CCharacters::HIT_END:
		break;
	default:
		break;
	}



	return nullptr;
}

CRuiState * CUpperHitState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	if(!m_bTrue)
		pRui->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CUpperHitState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	m_vVelocity = { 10.f,10.f,10.f };

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_UPPER_0);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_UPPER_0);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.01f);

		if (pRui->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pRui->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_UPPER_1);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_UPPER_1);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex(), true);
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.01f);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_UPPER_2);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_UPPER_2);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.01f);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_DOWN_0);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_DOWN_0);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.01f);
		break;
	case Client::CRuiState::TYPE_CHANGE:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_DOWN_1);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_DOWN_1);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	}
}



void CUpperHitState::Increase_Height(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 50.f;


	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.y += m_vVelocity.y * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		m_bNextAnim = true;
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_vPosition.y += m_vVelocity.y * fTimeDelta;
			vec = XMVectorSetY(vec, m_vPosition.y);
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}
}

void CUpperHitState::Fall_Height(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;


	m_fJumpTime += 0.035f;

	static _float fGravity = 50.f;
	static _float fVelocity = 21.25f;
	static _float fStartHeight = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//fStartHeight + m_vVelocity.y * fTimeDelta + 0.5 * fGravity * fTimeDelta * fTimeDelta;


	// m_vVelocity.x = velocity

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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

	//_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pRui->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::KnockBack_Player(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


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


	//_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pRui->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Bound_Player(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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


	//_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pRui->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Upper_Player(CRui* pRui, _float fTimeDelta)
{


	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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


	//_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pRui->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}





}

CRuiState * CUpperHitState::KnockBackState(CRui* pRui, _float fTimeDelta)
{
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			pRui->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		KnockBack_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_LOOP:
		KnockBack_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_END:
		KnockBack_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}





	return nullptr;
}

CRuiState * CUpperHitState::BoundState(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			pRui->Set_GodMode(true);
			
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
				if (!m_bTrue && pRui->Get_StoryDead())
				{
					CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
				
					CLevel_GamePlay::CHARACTERDESC	tCharacterDesc1p;
					tCharacterDesc1p.pSubChar = pRui->Get_BattleTarget();
					tCharacterDesc1p.matWorld = pRui->Get_Transform()->Get_World4x4();
					tCharacterDesc1p.bSub = false;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiHead"), g_iLevel, TEXT("Layer_RuiHead"), &tCharacterDesc1p)))
						return nullptr; 
					dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_RUI_DEAD);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QuestBattleCam(true);
					RELEASE_INSTANCE(CGameInstance);
					m_bTrue = true;
				}
				if (!pRui->Get_StoryDead())
					return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		Bound_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_LOOP:
		Bound_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_END:
		Bound_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}

CRuiState * CUpperHitState::UpperState(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			
			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			if(m_fDelay >=0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		Upper_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_LOOP:
		Upper_Player(pRui, fTimeDelta);

		if (m_bNextAnim)
		{
			pRui->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
		}
		break;
	case Client::CRuiState::TYPE_END:
		//Bound_Player(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}

	return nullptr;
}

CRuiState * CUpperHitState::Upper2State(CRui * pRui, _float fTimeDelta)
{


	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			if (m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}





	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		Increase_Height(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_LOOP:
		Fall_Height(pRui, fTimeDelta);

		if (m_bNextAnim == true)
			return new CUpperHitState(m_eHitType, TYPE_END, 0.f, 0.f, 0.f);
		break;
	case Client::CRuiState::TYPE_END:
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}


void CUpperHitState::Exit(CRui* pRui)
{
	pRui->Set_HitTime(0.3f);
	//pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
}



