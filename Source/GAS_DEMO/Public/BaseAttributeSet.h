// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
struct FGameplayEffectModCallbackData;

#define ATTRIBUTE_ACCESSOR(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAS_DEMO_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, Mana)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, MaxMana)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, Strength)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSOR(UBaseAttributeSet, MaxStrength)

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

};
