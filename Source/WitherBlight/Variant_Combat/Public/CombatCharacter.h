// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatAttacker.h"
#include "Interfaces/CombatDamageable.h"
#include "AbilitySystemInterface.h"
#include "Animation/AnimInstance.h"
#include "Data/Structs/FAttackAndItemDataStructs.h"
#include "CombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
struct FCharacterStats;
struct FProjectileInfomation;
struct FGameplayTag;
class UCombatLifeBar;
class UCombatBlockText;
class UWidgetComponent;
class UBlockGatherShootComponent;
class UWitherAbilitySystemComponent;
class ABaseBlockProjectile;


DECLARE_LOG_CATEGORY_EXTERN(LogCombatCharacter, Log, All);

/**
 *  An enhanced Third Person Character with melee combat capabilities:
 *  - Light attack string
 *  - Press and hold charged attack
 *  - Damage dealing and reaction
 *  - Death
 *  - Respawning
 */
UCLASS(abstract)
class ACombatCharacter : public ACharacter, public ICombatAttacker, public ICombatDamageable, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Life bar widget component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> LifeBar;

	/** Life bar widget component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> BlockCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlockGatherShootComponent> BlockGather; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWitherAbilitySystemComponent> PlayerStateAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseBlockProjectile> ProjectileClass;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Light Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LightAttackAction;

	/** Charged Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ChargedAttackAction;

	/** Toggle Camera Side Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ToggleCameraAction;

	/** Interact with world action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	/** Max amount of HP the character will have on respawn */
	UPROPERTY(EditAnywhere, Category="Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 5.0f;

	/** Current amount of HP the character has */
	UPROPERTY(VisibleAnywhere, Category="Damage")
	float CurrentHP = 0.0f;

	/** Life bar widget fill color */
	UPROPERTY(EditAnywhere, Category="Damage")
	FLinearColor LifeBarColor;

	/** Name of the pelvis bone, for damage ragdoll physics */
	UPROPERTY(EditAnywhere, Category="Damage")
	FName PelvisBoneName;

	/** Pointer to the life bar widget */
	UPROPERTY(EditAnywhere, Category="Damage")
	TObjectPtr<UCombatLifeBar> LifeBarWidget;

	UPROPERTY(EditAnywhere, Category = "Block")
	TObjectPtr<UCombatBlockText> BlockCountTextWidget;

	/** Max amount of time that may elapse for a non-Light attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category="Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float AttackInputCacheTimeTolerance = 1.0f;

	/** Time at which an attack button was last pressed */
	float CachedAttackInputTime = 0.0f;

	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FCharacterStats CharacterStats;

	/** Distance ahead of the character that melee attack sphere collision traces will extend */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Trace", meta = (ClampMin = 0, ClampMax = 5000, Units="cm"))
	float BlockTraceRadius = 75.0f;

	/** Radius of the sphere trace for melee attacks */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Trace", meta = (ClampMin = 0, ClampMax = 2000, Units = "cm"))
	float MeleeTraceRadius = 75.0f;

	/** Distance ahead of the character that enemies will be notified of incoming attacks */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units="cm"))
	float DangerTraceDistance = 300.0f;

	/** Radius of the sphere trace to notify enemies of incoming attacks */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Trace", meta = (ClampMin = 0, ClampMax = 200, Units = "cm"))
	float DangerTraceRadius = 100.0f;

	/** Amount of damage a melee attack will deal */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	float BaseDamage = 1.0f;

	/** Amount of knockback impulse a melee attack will apply */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeKnockbackImpulse = 250.0f;

	/** Amount of upwards impulse a melee attack will apply */
	UPROPERTY(VisibleInstanceOnly, Category="Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeLaunchImpulse = 300.0f;

	/** AnimMontage that will play for Light attacks */
	UPROPERTY(EditAnywhere, Category="Attack|Light")
	UAnimMontage* LightAttackMontage;

	/** Names of the AnimMontage sections that correspond to each stage of the Light attack */
	UPROPERTY(EditAnywhere, Category="Attack|Light")
	TArray<FName> LightSectionNames;

	/** Max amount of time that may elapse for a Light attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category="Attack|Light", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float LightInputCacheTimeTolerance = 0.45f;

	/** Index of the current stage of the melee attack Light */
	int32 LightCount = 0;

	/** AnimMontage that will play for charged attacks */
	UPROPERTY(EditAnywhere, Category="Attack|Charged")
	UAnimMontage* ChargedAttackMontage;

	/** Name of the AnimMontage section that corresponds to the charge loop */
	UPROPERTY(EditAnywhere, Category="Attack|Charged")
	FName ChargeLoopSection;

	/** Name of the AnimMontage section that corresponds to the attack */
	UPROPERTY(EditAnywhere, Category="Attack|Charged")
	FName ChargeAttackSection;

	/** Flag that determines if the player is currently holding the charged attack input */
	bool bIsChargingAttack = false;
	
	/** If true, the charged attack hold check has been tested at least once */
	bool bHasLoopedChargedAttack = false;

	/** Camera boom length while the character is dead */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float DeathCameraDistance = 400.0f;

	/** Camera boom length when the character respawns */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float DefaultCameraDistance = 100.0f;

	/** Time to wait before respawning the character */
	UPROPERTY(EditAnywhere, Category="Respawn", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 3.0f;

	/** Attack montage ended delegate */
	FOnMontageEnded OnAttackMontageEnded;

	/** Character respawn timer */
	FTimerHandle RespawnTimer;

	/** Copy of the mesh's transform so we can reset it after ragdoll animations */
	FTransform MeshStartingTransform;

	//TagOnDead Creatures
	FGameplayTag DeadTag;

	FGameplayTag EffectRemoveOnDeathTag;

public:
	
	/** Constructor */
	ACombatCharacter();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Light attack input */
	void LightAttackPressed();

	/** Called for Light attack input pressed */
	void ChargedAttackPressed();

	/** Called for Light attack input released */
	void ChargedAttackReleased();

	/** Called for toggle camera side input */
	void ToggleCamera();

	/** Called to interact with Item */
	void Interact();

	/** BP hook to animate the camera side switch */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void BP_ToggleCamera();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles Light attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLightAttackStart();

	/** Handles Light attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLightAttackEnd();

	/** Handles charged attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackStart();

	/** Handles charged attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackEnd();

	/** Handles Interacting with the world */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoInteract();

protected:

	/** Resets the character's current HP to maximum */
	void ResetHP();

	/** Performs a Light attack */
	void LightAttack();

	/** Performs a charged attack */
	void ChargedAttack();

	/** Called from a delegate when the attack montage ends */
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	
public:

	// ~begin IAbilitySystemInterface interface

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// -end IAbilitySystemInterface interface

	// ~begin CombatAttacker interface

	/** Performs the collision check for an attack */
	virtual void DoAttackTrace(FName DamageSourceBone) override;

	/** Performs the Light string check */
	virtual void CheckLight() override;

	/** Performs the charged attack hold check */
	virtual void CheckChargedAttack() override;

	virtual FCharacterStats GetCharacterStats() override;

    virtual void BlockGathered(int32 newTotal) override;

	// ~end CombatAttacker interface

	// ~begin CombatDamageable interface

	/** Notifies nearby enemies that an attack is coming so they can react */
	void NotifyEnemiesOfIncomingAttack();

	//Delegate that is called when damage is called 
	virtual void HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags) override;

	/** Handles death events */
	virtual void HandleDeath() override;

	/** Allows reaction to incoming attacks */
	virtual void NotifyDanger(const FVector& DangerLocation, AActor* DangerSource) override;

	/*Does the owner have more than zero health*/
	virtual bool IsAlive() const override;

	// ~end CombatDamageable interface

	/** Called from the respawn timer to destroy and re-create the character */
	void RespawnCharacter();

	UFUNCTION()
	virtual void HandleProjectileHit(ABaseBlockProjectile* Projectile, UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	/** Overrides landing to reset damage ragdoll physics */
	virtual void Landed(const FHitResult& Hit) override;

protected:

	/** Blueprint handler to play damage dealt effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void DealtDamage(float Damage, const FVector& ImpactPoint);

	/** Blueprint handler to play damage received effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void ReceivedDamage(float Damage, const FVector& ImpactPoint, const FVector& DamageDirection);

protected:

	/** Initialization */
	virtual void BeginPlay() override;

	/** Cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handles input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles possessed initialization */
	virtual void NotifyControllerChanged() override;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	//Functions that contains all the 
	virtual void SantyChecks();
};
