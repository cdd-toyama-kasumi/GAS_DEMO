// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ECostType : uint8
{
	Health,
	Mana,
	Strength
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	FGameplayAbilityInfo() = default;
	FGameplayAbilityInfo(TSubclassOf<UGameplayAbility> AbilityClass, UMaterialInstance *AbilityIcon, float Cooldown, ECostType CostType, float Cost);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	UMaterialInstance *AbilityIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	float Cooldown = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	ECostType CostType = ECostType::Mana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	float Cost = 0.0f;
	

};

UCLASS()
class GAS_DEMO_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	UMaterialInstance *AbilityIcon;

	UFUNCTION(BlueprintCallable, Category="AbilityInfo")
	FGameplayAbilityInfo GetAbilityInfo(int Level);
};
