// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS()
class GAS_DEMO_API UMelee : public UGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnAttackFinish(USkeletalMeshComponent* MeshComp);

	void RegisterCallback();
	
public:
	UMelee();
	
	UPROPERTY(EditDefaultsOnly)
	float ComboTimeWindow = 2.0f;
	
	float ComboStartTime;

	bool bIsCallbackRegistered = false;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* NormalAttackMontage;

	int AttackSequence = 0;
	UPROPERTY(EditDefaultsOnly)
	TMap<int,FName> AttackMapping = {{0,"ATK_0"},{1,"ATK_1"},{2,"ATK_2"}};
};
