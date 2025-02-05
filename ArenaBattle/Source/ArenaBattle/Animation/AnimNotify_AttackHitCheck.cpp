// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// �ִϸ��̼��� ����ϴ� Mesh�� Owner�� 'ABAnimationAttackInterface'�� ������ Ŭ�������� üũ 
		IABAnimationAttackInterface* AttackActor = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackActor)
		{
			AttackActor->AttackHitCheck();
		}
	}
}
