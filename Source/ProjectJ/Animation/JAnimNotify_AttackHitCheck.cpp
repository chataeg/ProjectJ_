// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JAnimNotify_AttackHitCheck.h"
#include "Interface/JAnimationAttackInterface.h"

void UJAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp)
	{
		IJAnimationAttackInterface* AttackPawn = Cast<IJAnimationAttackInterface>(MeshComp->GetOwner());
		if(AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
