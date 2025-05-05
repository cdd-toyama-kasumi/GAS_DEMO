// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraLocation = CameraComponent->GetRelativeLocation();
	SpringArmLength = SpringArmComponent->TargetArmLength;
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;

	if (NormalAttackMontage)
	{
		TArray<FAnimNotifyEvent> Events = NormalAttackMontage->Notifies;
		for (FAnimNotifyEvent Event : Events)
		{
			UAttackFinishNotify* FinishEventNotify = Cast<UAttackFinishNotify>(Event.Notify);
			if (FinishEventNotify)
			{
				FinishEventNotify->OnNotified.AddUObject(this, &ABaseCharacter::OnAttackFinish);
				UE_LOG(LogTemp, Warning, TEXT("AttackFinishNotify registered successfully!"));
			}
		}
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void ABaseCharacter::Lock()
{
	SpringArmComponent->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	float Delta = GetWorld()->GetDeltaSeconds() * 5;
	FVector TargetLocation = CameraLocation;
	TargetLocation.Z += CameraMoveHeight;
	CameraComponent->SetRelativeLocation(FMath::Lerp(CameraComponent->GetRelativeLocation(), TargetLocation, Delta));

	float  TargetArmLength = SpringArmLength + SpringArmAddLength;
	SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, TargetArmLength, Delta);
}

void ABaseCharacter::StopLock()
{
	SpringArmComponent->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//float Delta = GetWorld()->GetDeltaSeconds();
	CameraComponent->SetRelativeLocation(CameraLocation);
	SpringArmComponent->TargetArmLength = SpringArmLength;
}

void ABaseCharacter::Attack()
{
	if (IsAttacking)
	{
		return;
	}
	if (NormalAttackMontage)
	{
		IsAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			float PlayRate = 1.0f;
			FName SectionName = *AttackMapping.Find(AttackSequence++);
			float bTime = PlayAnimMontage(NormalAttackMontage, PlayRate, *SectionName.ToString());
			if (bTime != 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("NormalAttackMontage started playing on section: %s"), *SectionName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to play NormalAttackMontage on section: %s"), *SectionName.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Animation instance is invalid!"));
		}

		if (AttackSequence >= AttackMapping.Num())
		{
			AttackSequence = 0;
		}
	}
}

void ABaseCharacter::OnAttackFinish(USkeletalMeshComponent* MeshComp)
{
	UE_LOG(LogTemp,Warning,TEXT("Attack Finish"));
	IsAttacking = false;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		Input->BindAction(LockAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Lock);
		Input->BindAction(LockAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopLock);

		Input->BindAction(NormalAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::Attack);
	}
}

