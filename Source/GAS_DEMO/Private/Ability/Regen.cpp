// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Regen.h"

void URegen::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true))
	{
		return;
	}
}
