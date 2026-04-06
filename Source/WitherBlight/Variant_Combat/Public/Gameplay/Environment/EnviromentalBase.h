// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/FBlockDataStructs.h"
#include "EnviromentalBase.generated.h"

class UGeometryCollectionComponent;
class UNiagaraSystem;
class FGeometryCollection;

DECLARE_LOG_CATEGORY_EXTERN(LogDestructibleEnviroment, Log, All);

/**
 *  Destructibles abstract class:
 *  - 
 *  - 
 *  - 
 *  - 
 *  - 
 */
UCLASS(abstract)
class AEnviromentalBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
	
	UPROPERTY(EditAnywhere, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> DissolveNiagaraSystem;
	
public:	
	// Sets default values for this actor's properties
	AEnviromentalBase();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(Category = "Block")
	virtual void GrabBlock(UActorComponent* playerTargetComp);

	UFUNCTION(Category = "Block")
	virtual void InitializeGrabBlock(UActorComponent* playerTargetComp);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	/** Returns GeometryCollection subobject **/
	FORCEINLINE class UGeometryCollectionComponent* GetGeometryCollection() const { return GeometryCollectionComponent; }

private:
	// 
	UPROPERTY(EditAnywhere, Category = "Timeline")
	TArray<FBlockTimerInfo> MovementTimers;

	// 
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* MovementCurve;

	// 
	UFUNCTION(Category = "Timeline")
	void TimelineUpdate(int32 CurIndex);

	TSharedPtr<FGeometryCollection, ESPMode::ThreadSafe> RestData;

	//The Transforms of the Initial local positions of all the particles of the Rest Collection
	TArray<FTransform> InitialRestTransforms;

	//Index Of Valid Particles
	TArray<int32> ValidIndexs;

	//Index Of Active Particles
	TArray<int32> ActiveIndexs;

	//The Current location of all particles
	TArray<FTransform> CurrentTransformsArray;

	FVector StartLocation;
	TObjectPtr<USceneComponent> TargetComponent;

	UPROPERTY(EditAnywhere, Category = "Info")
	FBlockInfo BlockInfo;

};
