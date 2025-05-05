// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackFinishNotify.h"


void UAttackFinishNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
