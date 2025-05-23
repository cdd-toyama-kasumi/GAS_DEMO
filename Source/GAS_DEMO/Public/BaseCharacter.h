// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackFinishNotify.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "BaseCharacter.generated.h"

struct FGameplayAbilityInfo;
class UGameplayAbility;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;
class UMelee;
class URegen;
class UBaseGameplayAbility;

struct FInputActionValue;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeEvent,  float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeEvent,  float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrengthChangeEvent,  float, NewStrength);
UCLASS()
class GAS_DEMO_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	//Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttack;

	//View
	UPROPERTY(EditDefaultsOnly)
	float CameraMoveHeight = 50.f;
	FVector CameraLocation;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmAddLength = 100.f;
	float SpringArmLength;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer GameplayTagContainer;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMelee> MeleeAbility;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URegen> RegenAbility;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnHealthChangeEvent OnHealthChange;
	
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnManaChangeEvent OnManaChange;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnStrengthChangeEvent OnStrengthChange;

	void HealthChange(const FOnAttributeChangeData& HealthData);
	void ManaChange(const FOnAttributeChangeData& ManaData);
	void StrengthChange(const FOnAttributeChangeData& StrengthData);

	UFUNCTION()
	void ProcHealthChange(float NewHealth);
	UFUNCTION()
	void ProcManaChange(float NewMana);
	UFUNCTION()
	void ProcStrengthChange(float NewStrength);
	
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Ability")
	FGameplayAbilityInfo GetAbilityInfo(TSubclassOf<UBaseGameplayAbility>AbilityClass, int Level);
	
	UFUNCTION(BlueprintNativeEvent)
	void Death() ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Lock();

	void StopLock();

	UFUNCTION(BlueprintCallable)
	void OnAttackInput();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
			
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
};
