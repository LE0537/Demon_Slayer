#include "stdafx.h"
#include "..\Public\KyoujuroAkazaScene.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CKyoujuroAkazaScene::CKyoujuroAkazaScene(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CKyoujuroAkazaScene::HandleInput(CKyoujuro * pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CKyoujuroAkazaScene::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Transform()->Set_PlayerLookAt(XMVectorSet(64.f, 20.f, 38.5f, 1.f));

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		if (pKyoujuro->Get_Model()->Get_CurrentTime_Index(CKyoujuro::ANIM_BATTLE_START) >= 0.8f)
			pKyoujuro->Set_SceneRender(true);

		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_BATTLE_START))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_BATTLE_START);

			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_IDLE);
			pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_IDLE, 0.01f);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_IDLE);
			pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_IDLE, true);
			return new CIdleState();
		}

		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	return nullptr;
}

CKyoujuroState * CKyoujuroAkazaScene::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CKyoujuroAkazaScene::Enter(CKyoujuro* pKyoujuro)
{
	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.459f, 0.f, 53.669f, 1.f));
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(XMVectorSet(64.f, 20.f, 38.5f, 1.f));
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_BATTLE_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_BATTLE_START, 0.2f);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_BATTLE_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}

}

void CKyoujuroAkazaScene::Exit(CKyoujuro* pKyoujuro)
{
}

void CKyoujuroAkazaScene::Fall_Height(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;

	vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
}
