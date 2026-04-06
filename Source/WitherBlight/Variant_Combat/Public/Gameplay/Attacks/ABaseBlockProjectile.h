

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/Structs/FAttackAndItemDataStructs.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ABaseBlockProjectile.generated.h"

class UStaticMesh;
class UStaticMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;
class UBlockGatherShootComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogProjectile, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FProjectileHitDelegate, ABaseBlockProjectile*, Projectile,  UPrimitiveComponent*, HitComp, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit);

UCLASS(Abstract)
class ABaseBlockProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBlockProjectile();

	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destoryed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FProjectileHitDelegate OnProjectileNotifyHit;

protected:
	UFUNCTION(BlueprintCallable, Category = "Components")
	virtual void OnBoxColliderComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxColliderComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UStaticMesh> ProjMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FProjectileInfomation ProjInfo;

public:
	UFUNCTION(BlueprintCallable, Category = "Components")
	virtual void BeforeSpawnSetup(AActor* projOwner, UBlockGatherShootComponent* blockGatherComponent, FTransform spawnTransform);
	
	/** Returns the current proj info */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	FORCEINLINE FProjectileInfomation GetProjInfo() { return ProjInfo; }

	/** Returns the set proj info */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	FORCEINLINE void SetProjInfo(FProjectileInfomation projectileInfo) { ProjInfo = projectileInfo; }

protected:
	UPROPERTY()
	bool bHasHitOccured = false;
};
