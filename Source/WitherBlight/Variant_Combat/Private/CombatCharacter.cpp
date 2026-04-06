// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "UI\CombatLifeBar.h"
#include "UI\UCombatBlockText.h"
#include "TimerManager.h"
#include "Engine/LocalPlayer.h"
#include "CombatPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Component/BlockGatherShootComponent.h"
#include "Gameplay/Component/UWitherAbilitySystemComponent.h"
#include "Data/Structs/FAttackAndItemDataStructs.h"
#include "Data/AttributeSets/UGSAttributeSetBase.h"
#include "Gameplay/Attacks/ABaseBlockProjectile.h"
#include "GameplayEffectTypes.h"
#include "CombatPlayerState.h"
#include "Gameplay\Items\ItemPickUp.h"
#include "Interfaces\PlayerInteract.h"
#include "Gameplay\Component\ItemInventoryManager.h"

DEFINE_LOG_CATEGORY(LogCombatCharacter);

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// bind the attack montage ended delegate
	OnAttackMontageEnded.BindUObject(this, &ACombatCharacter::AttackMontageEnded);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

	// Configure character movement
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = DefaultCameraDistance;
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->bEnableCameraRotationLag = true;
	}

	// create the orbiting camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	// create the life bar widget component
	LifeBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeBar"));
	if (LifeBar)
	{
		LifeBar->SetupAttachment(RootComponent);
	}

	//Create the Block Text Count Widge Component
	BlockCountText = CreateDefaultSubobject<UWidgetComponent>(TEXT("BlockCountText"));
	if (BlockCountText)
	{
		BlockCountText->SetupAttachment(RootComponent);
		BlockCountText->AddWorldRotation(FRotator::MakeFromEuler(FVector(0, 0, 180)));
		BlockCountText->AddWorldOffset(FVector(-50.0f, 0.0f, 0.0f));
	}

	//setup vlock gathershoot component
	BlockGather = CreateDefaultSubobject<UBlockGatherShootComponent>(TEXT("BlockGather"));
	if (BlockGather)
	{
		BlockGather->SetupAttachment(RootComponent);
	}

	// set the player tag
	Tags.Add(FName("Player"));

	DeadTag = FGameplayTag::RequestGameplayTag("Gameplay.State.Dead");
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag("Gameplay.Effect.RemoveOnDeath");

	BlockTraceRadius = CharacterStats.BlockTraceRadius;
	BlockTraceRadius = CharacterStats.BlockTraceRadius;
	DangerTraceDistance = CharacterStats.DangerTraceDistance;
	DangerTraceRadius = CharacterStats.DangerTraceRadius;
	BaseDamage = CharacterStats.BaseDamage;

}

FCharacterStats ACombatCharacter::GetCharacterStats()
{ 
	return CharacterStats; 
}

void ACombatCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACombatCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACombatCharacter::LightAttackPressed()
{
	// route the input
	DoLightAttackStart();
}

void ACombatCharacter::ChargedAttackPressed()
{
	// route the input
	DoChargedAttackStart();
}

void ACombatCharacter::ChargedAttackReleased()
{
	// route the input
	DoChargedAttackEnd();
}

void ACombatCharacter::ToggleCamera()
{
	// call the BP hook
	BP_ToggleCamera();
}

void ACombatCharacter::Interact()
{
	DoInteract();
}

void ACombatCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACombatCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACombatCharacter::DoLightAttackStart()
{
	// are we already playing an attack animation?
	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	// perform a Light attack
	LightAttack();
}

void ACombatCharacter::DoLightAttackEnd()
{
	// stub
}

void ACombatCharacter::DoChargedAttackStart()
{
	// raise the charging attack flag
	bIsChargingAttack = true;

	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	ChargedAttack();
}

void ACombatCharacter::DoChargedAttackEnd()
{
	// lower the charging attack flag
	bIsChargingAttack = false;

	// if we've done the charge loop at least once, release the charged attack right away
	if (bHasLoopedChargedAttack)
	{
		CheckChargedAttack();
	}
}

void ACombatCharacter::DoInteract()
{
	FVector Start = GetPawnViewLocation();
	FVector End = Start + (GetViewRotation().Vector() * 300.f); // 3 meters range

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, Params);
	// Use a custom Collision Channel (e.g., ECC_GameTraceChannel1 for "Pickup")
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		// Try to cast the hit actor to your Item class
		if (HitActor && HitActor->Implements<UPlayerInteract>())
		{
			Cast<IPlayerInteract>(HitActor)->OnInteract();
			if (HitActor->Implements<UItemsAbility>())
			{
				Cast<IItemsAbility>(HitActor)->OnPickup();
				if (AItemPickUp* TouchedItem = Cast<AItemPickUp>(HitResult.GetActor()))
				{
					ACombatPlayerState* PS = GetPlayerState<ACombatPlayerState>();
					if (!PS) return;

					UItemInventoryManager* InvManager = PS->GetItemInventoryManager();
					if (!InvManager) return;

					InvManager->AddItemToInventory(TouchedItem);
				}
			}
		}
	}
	DrawDebugLine(
		GetWorld(),
		Start,
		bHit ? HitResult.Location : End, // Draw to hit location if hit, otherwise to the end point
		bHit ? FColor::Green : FColor::Red, // Color: Green if hit, Red if not
		false,      // bPersistentLines: true for permanent lines, false for temporary
		5.0f,       // LifeTime: duration in seconds (e.g., 5.0f). If bPersistentLines is false and this is -1, it lasts one frame.
		0,          // DepthPriority: 0 for foreground
		3.0f        // Thickness: line thickness
		);
}

void ACombatCharacter::BlockGathered(int32 newTotal)
{
	//TotalGatheredBlocks += blockInfo.Worth;
	BlockCountTextWidget->ChangeTextAmount(newTotal);
}

void ACombatCharacter::ResetHP()
{
	if (PlayerStateAbilitySystemComponent)
	{
		PlayerStateAbilitySystemComponent->ResetHP();
		// update the life bar
		LifeBarWidget->SetLifePercentage(1.0f);
	}
}

void ACombatCharacter::LightAttack()
{
	// raise the attacking flag
	bIsAttacking = true;

	// reset the Light count
	LightCount = 0;

	// notify enemies they are about to be attacked
	NotifyEnemiesOfIncomingAttack();

	// play the attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(LightAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, LightAttackMontage);
		}
	}

}

void ACombatCharacter::ChargedAttack()
{
	// raise the attacking flag
	bIsAttacking = true;

	// reset the charge loop flag
	bHasLoopedChargedAttack = false;

	// notify enemies they are about to be attacked
	NotifyEnemiesOfIncomingAttack();

	// play the charged attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ChargedAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChargedAttackMontage);
		}
	}
}

void ACombatCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// reset the attacking flag
	bIsAttacking = false;

	// check if we have a non-stale cached input
	if (GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= AttackInputCacheTimeTolerance)
	{
		// are we holding the charged attack button?
		if (bIsChargingAttack)
		{
			// do a charged attack
			ChargedAttack();
		}
		else
		{
			// do a regular attack
			LightAttack();
		}
	}
}

void ACombatCharacter::DoAttackTrace(FName DamageSourceBone)
{
	//Old Code
	/* This is now a shoot instead of a punch
	* 
	* 
	* 
	FVector CenterLocation = GetActorLocation();
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // What types of objects to check (WorldStatic, Pawn, PhysicsBody, etc.)
	AActor* IgnoreActor = this;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(IgnoreActor);

	// 1. Perform the Overlap Query
	bool bSuccess = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CenterLocation,
		BlockTraceRadius,
		ObjectTypes,
		AActor::StaticClass(), // Filter by a base class (AActor is usually sufficient for the overlap)
		ActorsToIgnore,
		OverlappedActors
	);

	 sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the provided socket location, sweep forward
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * BlockTraceRadius);

	// check for pawn and world dynamic collision object types
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(BlockTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				
				// knock upwards and away from the impact normal
				const FVector Impulse = (CurrentHit.ImpactNormal * -MeleeKnockbackImpulse) + (FVector::UpVector * MeleeLaunchImpulse);
				// pass the damage event to the actor
				Damageable->ApplyDamage(BaseDamage, this, CurrentHit.ImpactPoint, Impulse);
				// call the BP handler to play effects, etc.
				DealtDamage(BaseDamage, CurrentHit.ImpactPoint);
				
			}
		}
	}
	*/

	FVector CameraLocation;
	FRotator CameraRotation;
	GetLocalViewingPlayerController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//Spawn Blocks
	FVector CenterLocation = GetActorLocation();
	// start at the provided socket location, sweep forward
	const FVector StartLocation = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector EndLocation = CameraLocation + (CameraRotation.Vector() * BlockTraceRadius);//StartLocation + (GetActorForwardVector() * BlockTraceRadius);
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(EndLocation, StartLocation);
	const FVector SpawnScale(1.0f, 1.0f, 1.0f);
	const FTransform SpawnTransform(SpawnRotation, StartLocation, SpawnScale);
	ABaseBlockProjectile* SpawnedProjectile = this->GetWorld()->SpawnActorDeferred<ABaseBlockProjectile>(ProjectileClass, SpawnTransform, this, this);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->BeforeSpawnSetup(this, BlockGather, SpawnTransform);
		SpawnedProjectile->OnProjectileNotifyHit.AddDynamic(this, &ACombatCharacter::HandleProjectileHit);
	}
	SpawnedProjectile->FinishSpawning(SpawnTransform);

}

void ACombatCharacter::CheckLight()
{
	// are we playing a non-charge attack animation?
	if (bIsAttacking && !bIsChargingAttack)
	{
		// is the last attack input not stale?
		if (GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= LightInputCacheTimeTolerance)
		{
			// consume the attack input so we don't accidentally trigger it twice
			CachedAttackInputTime = 0.0f;

			// increase the Light counter
			++LightCount;

			// do we still have a Light section to play?
			if (LightCount < LightSectionNames.Num())
			{
				// notify enemies they are about to be attacked
				NotifyEnemiesOfIncomingAttack();

				// jump to the next Light section
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_JumpToSection(LightSectionNames[LightCount], LightAttackMontage);
				}
			}
		}
	}
}

void ACombatCharacter::CheckChargedAttack()
{
	// raise the looped charged attack flag
	bHasLoopedChargedAttack = true;

	// jump to either the loop or the attack section depending on whether we're still holding the charge button
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_JumpToSection(bIsChargingAttack ? ChargeLoopSection : ChargeAttackSection, ChargedAttackMontage);
	}
}

void ACombatCharacter::NotifyEnemiesOfIncomingAttack()
{
	// sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the actor location, sweep forward
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * DangerTraceDistance);

	// check for pawn object types only
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(DangerTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				// notify the enemy
				Damageable->NotifyDanger(GetActorLocation(), this);
			}
		}
	}
}

/*
void ACombatCharacter::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	// pass the damage event to the actor
	FDamageEvent DamageEvent;
	const float ActualDamage = TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);

	// only process knockback and effects if we received nonzero damage
	if (ActualDamage > 0.0f)
	{
		// apply the knockback impulse
		GetCharacterMovement()->AddImpulse(DamageImpulse, true);

		// is the character ragdolling?
		if (GetMesh()->IsSimulatingPhysics())
		{
			// apply an impulse to the ragdoll
			GetMesh()->AddImpulseAtLocation(DamageImpulse * GetMesh()->GetMass(), DamageLocation);
		}

		// pass control to BP to play effects, etc.
		ReceivedDamage(ActualDamage, DamageLocation, DamageImpulse.GetSafeNormal());
	}

}
*/

void ACombatCharacter::HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags)
{

	const UGSAttributeSetBase* BaseSet = Cast<UGSAttributeSetBase>(PlayerStateAbilitySystemComponent->GetAttributeSet(UGSAttributeSetBase::StaticClass()));
	if (BaseSet)
	{

		float CurrentHealth = BaseSet->GetHealth();
		float MaxHealth = BaseSet->GetMaxHealth();


		if (DamageAmount > 0.0f)
		{
			if (CurrentHealth <= 0.0f)
			{
				// die
				HandleDeath();
			}
			else
			{
				// update the life bar
				LifeBarWidget->SetLifePercentage(CurrentHealth / MaxHealth);

				// enable partial ragdoll physics, but keep the pelvis vertical
				GetMesh()->SetPhysicsBlendWeight(0.5f);
				GetMesh()->SetBodySimulatePhysics(PelvisBoneName, false);
			}
		}
	}
}

void ACombatCharacter::HandleDeath()
{
	// disable movement while we're dead
	GetCharacterMovement()->DisableMovement();

	// enable full ragdoll physics
	GetMesh()->SetSimulatePhysics(true);

	// hide the life bar
	LifeBar->SetHiddenInGame(true);

	BlockCountText->SetHiddenInGame(true);

	// pull back the camera
	GetCameraBoom()->TargetArmLength = DeathCameraDistance;

	// schedule respawning
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ACombatCharacter::RespawnCharacter, RespawnTime, false);

}

void ACombatCharacter::NotifyDanger(const FVector& DangerLocation, AActor* DangerSource)
{
	// stub
}

bool ACombatCharacter::IsAlive() const
{
		return PlayerStateAbilitySystemComponent->IsAlive();
}

void ACombatCharacter::RespawnCharacter()
{
	// destroy the character and let it be respawned by the Player Controller
	Destroy();
}

/*
float ACombatCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// only process damage if the character is still alive
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	// reduce the current HP
	CurrentHP -= Damage;

	// have we run out of HP?
	if (CurrentHP <= 0.0f)
	{
		// die
		HandleDeath();
	}
	else
	{
		// update the life bar
		LifeBarWidget->SetLifePercentage(CurrentHP / MaxHP);

		// enable partial ragdoll physics, but keep the pelvis vertical
		GetMesh()->SetPhysicsBlendWeight(0.5f);
		GetMesh()->SetBodySimulatePhysics(PelvisBoneName, false);
	}

	// return the received damage amount
	return Damage;
}
*/

void ACombatCharacter::HandleProjectileHit(ABaseBlockProjectile* Projectile, UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	IAbilitySystemInterface* OtherActorAbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (OtherActorAbilityInterface)
	{
		FProjectileInfomation CurrentProjInfo = Projectile->GetProjInfo();
		FGameplayEffectSpecHandle InitSpecHandle = CurrentProjInfo.OwnerASC->MakeOutgoingSpec(CurrentProjInfo.InitDamageEffectClass, 1.f, CurrentProjInfo.ContextHandle);
		CurrentProjInfo.OwnerASC->ApplyGameplayEffectSpecToTarget(*InitSpecHandle.Data.Get(), OtherActorAbilityInterface->GetAbilitySystemComponent());
	}
	Projectile->OnProjectileNotifyHit.RemoveDynamic(this, &ACombatCharacter::HandleProjectileHit);
}

UAbilitySystemComponent* ACombatCharacter::GetAbilitySystemComponent() const
{
	return PlayerStateAbilitySystemComponent;
}

void ACombatCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// is the character still alive?
	if (GetPlayerState<ACombatPlayerState>()->GetHealth() >= 0.0f)
	{
		// disable ragdoll physics
		GetMesh()->SetPhysicsBlendWeight(0.0f);
	}
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// get the life bar from the widget component
	LifeBarWidget = Cast<UCombatLifeBar>(LifeBar->GetUserWidgetObject());
	check(LifeBarWidget);
	BlockCountTextWidget = Cast<UCombatBlockText>(BlockCountText->GetUserWidgetObject());
	check(BlockCountTextWidget);

	// initialize the camera
	GetCameraBoom()->TargetArmLength = DefaultCameraDistance;

	// save the relative transform for the mesh so we can reset the ragdoll later
	MeshStartingTransform = GetMesh()->GetRelativeTransform();

	// set the life bar color
	LifeBarWidget->SetBarColor(LifeBarColor);
	BlockCountTextWidget->ChangeTextAmount(0);

	PlayerStateAbilitySystemComponent = Cast<UWitherAbilitySystemComponent>(Cast<IAbilitySystemInterface>(GetPlayerState<ACombatPlayerState>())->GetAbilitySystemComponent());

	SantyChecks();
}

void ACombatCharacter::SantyChecks()
{
	checkf(CameraBoom != nullptr, TEXT("%s failed to initialize Camera Boom!"), *GetName());
	checkf(FollowCamera != nullptr, TEXT("%s failed to initialize FollowCamera!"), *GetName());
	checkf(LifeBar != nullptr, TEXT("%s failed to initialize LifeBar!"), *GetName());
	checkf(BlockCountText != nullptr, TEXT("%s failed to initialize BlockCountText!"), *GetName());
	checkf(BlockGather != nullptr, TEXT("%s failed to initialize BlockGatherComponent!"), *GetName());
	checkf(PlayerStateAbilitySystemComponent != nullptr, TEXT("%s failed to initialize ASC!"), *GetName());
	checkf(ProjectileClass, TEXT("%s failed to initialize ProjectileClass!"), *GetName());
}

void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Look);

		// Light Attack
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &ACombatCharacter::LightAttackPressed);

		// Charged Attack
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Started, this, &ACombatCharacter::ChargedAttackPressed);
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Completed, this, &ACombatCharacter::ChargedAttackReleased);

		// Camera Side Toggle
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &ACombatCharacter::ToggleCamera);

		//Interact with world
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Interact);
	}
}

void ACombatCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// update the respawn transform on the Player Controller
	if (ACombatPlayerController* PC = Cast<ACombatPlayerController>(GetController()))
	{
		PC->SetRespawnTransform(GetActorTransform());
	}
}

