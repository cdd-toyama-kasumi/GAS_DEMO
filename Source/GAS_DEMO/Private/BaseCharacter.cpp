// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "BaseGameplayAbility.h"
#include "Ability/Melee.h"
#include "Ability/Regen.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGameplayAbilityInfo ABaseCharacter::GetAbilityInfo(TSubclassOf<UBaseGameplayAbility> AbilityClass, int Level)
{
	UBaseGameplayAbility* AbilityInstance = AbilityClass->GetDefaultObject<UBaseGameplayAbility>();
	
	if (AbilitySystemComponent && AbilityInstance)
	{
		return AbilityInstance->GetAbilityInfo(Level);
	}
	
	return FGameplayAbilityInfo();
}

void ABaseCharacter::Death_Implementation()
{
	IsDead = true;
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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (MeleeAbility)
	{
		FGameplayAbilitySpec AbilitySpec(MeleeAbility, 1, 0);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	if (RegenAbility)
	{
		FGameplayAbilitySpec AbilitySpec(RegenAbility, 1, 0);
		AbilitySystemComponent->GiveAbilityAndActivateOnce(AbilitySpec);
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute()).AddUObject(this, &ABaseCharacter::HealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetManaAttribute()).AddUObject(this, &ABaseCharacter::ManaChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetStrengthAttribute()).AddUObject(this, &ABaseCharacter::StrengthChange);

	OnHealthChange.AddDynamic(this, &ABaseCharacter::ProcHealthChange);
	OnManaChange.AddDynamic(this, &ABaseCharacter::ProcManaChange);
	OnStrengthChange.AddDynamic(this, &ABaseCharacter::ProcStrengthChange);
}

void ABaseCharacter::HealthChange(const FOnAttributeChangeData& HealthData)
{
	OnHealthChange.Broadcast(HealthData.NewValue);
}

void ABaseCharacter::ManaChange(const FOnAttributeChangeData& ManaData)
{
	OnManaChange.Broadcast(ManaData.NewValue);
}

void ABaseCharacter::StrengthChange(const FOnAttributeChangeData& StrengthData)
{
	OnStrengthChange.Broadcast(StrengthData.NewValue);
}

void ABaseCharacter::ProcHealthChange(float NewHealth)
{
	UE_LOG(LogTemp, Error, TEXT("Name:%s Health: %f"), *GetName(), NewHealth);
	if (NewHealth <= 0)
	{
		Death();
	}
}

void ABaseCharacter::ProcManaChange(float NewMana)
{
	UE_LOG(LogTemp, Error, TEXT("Name:%s Mana: %f"), *GetName(), NewMana);
}

void ABaseCharacter::ProcStrengthChange(float NewStrength)
{
	UE_LOG(LogTemp, Error, TEXT("Name:%s Strength: %f"), *GetName(), NewStrength);
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

	float TargetArmLength = SpringArmLength + SpringArmAddLength;
	SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, TargetArmLength, Delta);
}

void ABaseCharacter::StopLock()
{
	SpringArmComponent->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraComponent->SetRelativeLocation(CameraLocation);
	SpringArmComponent->TargetArmLength = SpringArmLength;
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
		
		Input->BindAction(NormalAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::OnAttackInput);
	}

}

void ABaseCharacter::OnAttackInput()
{
	if (AbilitySystemComponent && MeleeAbility)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTagContainer);
		/*FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(MeleeAbility);
		if (Spec)
		{
			//AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}*/
	}
}