// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 애니메이션을 재생하는 Mesh의 Owner가 'ABAnimationAttackInterface'를 구현한 클래스인지 체크 
		IABAnimationAttackInterface* AttackActor = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackActor)
		{
			AttackActor->AttackHitCheck();
		}
	}
}
