// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayAbility.h"

FGameplayAbilityInfo::FGameplayAbilityInfo(TSubclassOf<UGameplayAbility> AbilityClass, UMaterialInstance* AbilityIcon,
	float Cooldown, ECostType CostType, float Cost):AbilityClass(AbilityClass),AbilityIcon(AbilityIcon),Cooldown(Cooldown),CostType(CostType),Cost(Cost)
{

}

FGameplayAbilityInfo UBaseGameplayAbility::GetAbilityInfo(int Level)
{
	UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();

	float Cooldown = 0.0;
	float Cost = 0.0;
	ECostType CostType = ECostType::Mana;
	if (CostEffect && CoolDownEffect)
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
		if (CostEffect->Modifiers.Num() > 0)
		{
			FGameplayModifierInfo CostEffectModifierInfo = CostEffect->Modifiers[0];
			CostEffectModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, Cost);
			FString EffectName = CostEffectModifierInfo.Attribute.AttributeName;
			if (EffectName == "Health")
			{
				CostType = ECostType::Health;
			}
			else if (EffectName == "Strength")
			{
				CostType = ECostType::Strength;
			}
			else if (EffectName == "Mana")
			{
				CostType = ECostType::Mana;
			}
			
			return FGameplayAbilityInfo(GetClass(), AbilityIcon, Cooldown, CostType, Cost);
		}
	}
	
	return FGameplayAbilityInfo();
}
