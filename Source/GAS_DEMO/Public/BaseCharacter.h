// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackFinishNotify.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class GAS_DEMO_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
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
	
	UPROPERTY(EditDefaultsOnly)
	float CameraMoveHeight = 50.f;
	FVector CameraLocation;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmAddLength = 100.f;
	float SpringArmLength;

	//Normal Attack
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* NormalAttackMontage;

	int AttackSequence = 0;
	UPROPERTY(EditDefaultsOnly)
	TMap<int,FName> AttackMapping = {{0,"ATK_0"},{1,"ATK_1"},{2,"ATK_2"}};
	
	UPROPERTY(EditDefaultsOnly)
	float ComboTimeWindow = 2.0f;
	
	float ComboStartTime;
	
public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Lock();

	void StopLock();

	void Attack();

	void OnAttackFinish(USkeletalMeshComponent* MeshComp);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
};
