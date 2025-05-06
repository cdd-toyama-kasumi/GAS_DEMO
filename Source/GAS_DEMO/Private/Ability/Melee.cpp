// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Melee.h"

#include "AttackFinishNotify.h"
#include "GameFramework/Character.h"

UMelee::UMelee()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UMelee::OnAttackFinish(USkeletalMeshComponent* MeshComp)
{
	UE_LOG(LogTemp,Warning,TEXT("Attack Finish"));
	ComboStartTime = GetWorld()->GetTimeSeconds();
}

void UMelee::RegisterCallback()
{
	if (NormalAttackMontage)
	{
		TArray<FAnimNotifyEvent> Events = NormalAttackMontage->Notifies;
		for (FAnimNotifyEvent Event : Events)
		{
			UAttackFinishNotify* FinishEventNotify = Cast<UAttackFinishNotify>(Event.Notify);
			if (FinishEventNotify)
			{
				FinishEventNotify->OnNotified.AddUObject(this, &UMelee::OnAttackFinish);
				UE_LOG(LogTemp, Warning, TEXT("AttackFinishNotify registered successfully!"));
				bIsCallbackRegistered = true;
			}
		}
	}
}

void UMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true))
    {
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

    if (!bIsCallbackRegistered)
    {
        RegisterCallback();
    }
	
    if (NormalAttackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("NowTime:%f ComboStartTime: %f"), GetWorld()->GetTimeSeconds(), ComboStartTime);
    	if (GetWorld()->GetTimeSeconds() - ComboStartTime > ComboTimeWindow)
    	{
    		AttackSequence = 0;
    	}

    	float PlayRate = 1.0f;
    	FName SectionName = *AttackMapping.Find(AttackSequence++);
    	float bTime = Character->PlayAnimMontage(NormalAttackMontage, PlayRate, *SectionName.ToString());
    	if (bTime != 0.0f)
    	{
    		UE_LOG(LogTemp, Warning, TEXT("NormalAttackMontage started playing on section: %s"), *SectionName.ToString());
    	}
    	else
    	{
    		UE_LOG(LogTemp, Error, TEXT("Failed to play NormalAttackMontage on section: %s"), *SectionName.ToString());
    	}

    	if (AttackSequence >= AttackMapping.Num())
    	{
    		AttackSequence = 0;
    	}
    }
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}