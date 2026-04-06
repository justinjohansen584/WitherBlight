// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Environment/EnviromentalBase.h"
#include "TimerManager.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Gameplay/Component/BlockGatherShootComponent.h"

DEFINE_LOG_CATEGORY(LogDestructibleEnviroment);

// Sets default values
AEnviromentalBase::AEnviromentalBase()
{
    GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Component"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);
    GeometryCollectionComponent->SetGenerateOverlapEvents(true);
    BlockInfo.Actor = this;
}

// Called when the game starts or when spawned
void AEnviromentalBase::BeginPlay()
{
	Super::BeginPlay();
}


void AEnviromentalBase::GrabBlock(UActorComponent* playerTargetComp)
{
    InitializeGrabBlock(playerTargetComp);
    if (GeometryCollectionComponent)
    {
        // Check if the required Curve asset is set in the Editor
        if (MovementCurve)
        {
            FTimerDelegate TimerDel;
            //Set up and find index
            int32 CurIndex = ValidIndexs[FMath::RandRange(0, ValidIndexs.Num() - 1)];
            FBlockTimerInfo* ActiveGrabTimerInfo = &MovementTimers[CurIndex];
            ActiveGrabTimerInfo->elpasedTime = 0.0f;
            ActiveGrabTimerInfo->TargetComponent = playerTargetComp;
            UE_LOG(LogTemp, Warning, TEXT("Current Index %d"), CurIndex);
            ActiveGrabTimerInfo->index = CurIndex;
            ActiveIndexs.Add(CurIndex);
            ValidIndexs.Remove(CurIndex);
            //Set up the map
            GeometryCollectionComponent->SetAnchoredByIndex(CurIndex, false);
            // BINDING WITH PAYLOAD: This is where we pass the ID!
            TimerDel.BindUFunction(this, FName("TimelineUpdate"), CurIndex);
            // Set timer to call CustomUpdate every frame (0.0f)
            GetWorldTimerManager().SetTimer(MovementTimers[CurIndex].TimerHandle, TimerDel, 0.01f, true);
            //Set the Collossion profile name of effected components;
            const TArray<int32> TimerIndexs = { ActiveGrabTimerInfo->index };
            FName ProfileName = "GrabbedPiece";
            GeometryCollectionComponent->SetPerParticleCollisionProfileName(TimerIndexs, ProfileName);

        }
        else
        {
            UE_LOG(LogDestructibleEnviroment, Error, TEXT("MovementCurve is NOT set on %s. Timeline cannot play."), *GetName());
        }
    }
    else
    {
        UE_LOG(LogDestructibleEnviroment, Error, TEXT("Geometry Collection is not instanted on %s."), *GetName());
    }
}

void AEnviromentalBase::InitializeGrabBlock(UActorComponent* playerTargetComp)
{
    StartLocation = GetActorLocation();
    TargetComponent = Cast<USceneComponent>(playerTargetComp);
    if (!TargetComponent)
    {
        TargetComponent = playerTargetComp->GetOwner()->GetRootComponent();
    }
    //setup the array that contains the Starting Transforms of all the Particles if it has not been set up yet
    if (InitialRestTransforms.IsEmpty())
    {
        InitialRestTransforms = GeometryCollectionComponent->GetInitialLocalRestTransforms();
    }
    //setup the array that contains the Dynamic Transforms of all the Particles if it has not been set up yet
    if (CurrentTransformsArray.IsEmpty())
    {
        CurrentTransformsArray.Reserve(InitialRestTransforms.Num());
        CurrentTransformsArray = InitialRestTransforms;
    }
    //setup the array that contains the indexs for all the Particles of the Geometry Collection
    if (ValidIndexs.IsEmpty())
    {
        for (int i = 0; i < InitialRestTransforms.Num(); i++)
        {
            ValidIndexs.Add(i);
        }
    }
    if (GeometryCollectionComponent)
    {
        if (MovementTimers.IsEmpty())
        {
            RestData = GeometryCollectionComponent->RestCollection->GetGeometryCollection();
            GeometryCollectionComponent->CrumbleActiveClusters();
            MovementTimers.SetNum(InitialRestTransforms.Num());
        }
    }
    else
    {
        UE_LOG(LogDestructibleEnviroment, Error, TEXT("Geometry Collection is not instanted on %s."), *GetName());
    }
}


void AEnviromentalBase::TimelineUpdate(int32 CurIndex)
{
    float MinTime, MaxTime;
    MovementCurve->GetTimeRange(MinTime, MaxTime);

    FBlockTimerInfo* TimerInfo = &MovementTimers[CurIndex];
    TimerInfo->elpasedTime += 0.01f; // Matches the timer interval
    float CurveValue = MovementCurve->GetFloatValue(TimerInfo->elpasedTime);

    // Stop when curve ends
    if (TimerInfo->elpasedTime >= MaxTime)
    {
        GetWorldTimerManager().ClearTimer(TimerInfo->TimerHandle);
    }

    if (!CurrentTransformsArray.IsEmpty())
    {
        //Update location Directly
        FVector TargetWorldLocation = TargetComponent->GetComponentLocation();
        FVector TargetLocalLocation = TargetWorldLocation - GeometryCollectionComponent->GetComponentLocation();
        FVector NewLocation = FMath::Lerp(CurrentTransformsArray[CurIndex].GetLocation(), TargetLocalLocation, CurveValue);

        CurrentTransformsArray[CurIndex].SetLocation(NewLocation);

        if(GeometryCollectionComponent)
        {
           //THIS WORKS BUT RE does all visible objects
            GeometryCollectionComponent->SetLocalRestTransforms(CurrentTransformsArray, true);
            //This Might live on the unit
            float CheckRadius = 50.0f;
            // SQUARE the radius once (or use a pre-calculated constant)
            float CheckRadiusSquared = FMath::Square(CheckRadius);
            // PERFORM THE CHECK
            //UE_LOG(LogTemp, Error, TEXT("Index %d, DistSquared %f, radSquared %f"), TimerInfo->index, FVector::DistSquared(TargetLocalLocation, NewLocation) ,CheckRadiusSquared);
            if (FVector::DistSquared(TargetLocalLocation, NewLocation) < CheckRadiusSquared)
            {
                //Remove the Index from Active Indexs
                ActiveIndexs.Remove(CurIndex);
                //make and spawn partcle effect
                UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
                    DissolveNiagaraSystem,                                                                       // UNiagaraSystem*
                    TargetComponent,                                                                             // USceneComponent* to attach to
                    FName(RestData->BoneName[CurIndex]),                                                         // Socket Name (Optional)
                    FVector(0.f),                                                                                // Relative Location
                    FRotator(0.f),                                                                               // Relative Rotation
                    EAttachLocation::KeepRelativeOffset,
                    true                                                                                         // Auto Destroy
                );
                NiagaraComp->SetVariableVec3(TEXT("Box Size"), RestData->BoundingBox[CurIndex].GetSize() + (5,5,5));
                NiagaraComp->SetVariablePosition(TEXT("Shape Position"), TargetWorldLocation);
                GeometryCollectionComponent->bAllowRemovalOnSleep = true;
                CurrentTransformsArray[CurIndex].SetLocation(FVector(0.0,0.0,0.0));
                GeometryCollectionComponent->PutRigidBodyToSleep(FName(RestData->BoneName[CurIndex]));
                GetWorldTimerManager().ClearTimer(TimerInfo->TimerHandle);
                
                UBlockGatherShootComponent* GatherComponent = Cast<UBlockGatherShootComponent>(TimerInfo->TargetComponent);
                if (GatherComponent->OnBlockGathered.IsBound())
                {
                    GatherComponent->OnBlockGathered.Broadcast(BlockInfo);
                }
            }
        }
        else 
        {
            UE_LOG(LogDestructibleEnviroment, Error, TEXT("Geometry Collection is not instanted on %s."), *GetName());
        }
    }
}


// test when it is placed
#if WITH_EDITOR
void AEnviromentalBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
        // This runs whenever the Actor is placed or compiled in the Editor.
        if (GeometryCollectionComponent == nullptr || GeometryCollectionComponent->GetRestCollection() == nullptr)
        {
            // Issue a warning to the editor log
            UE_LOG(LogDestructibleEnviroment, Error, TEXT("SETUP ERROR: Actor %s's Does not have a rest collection"), *GetName());

        }
        if (MovementCurve == nullptr)
        {
            UE_LOG(LogDestructibleEnviroment, Error, TEXT("MovementCurve is NOT set on %s. Timeline cannot play."), *GetName());
        }

        if (GeometryCollectionComponent)
        {
            // 1. Find the property by its name string
            FProperty* Prop = GeometryCollectionComponent->GetClass()->FindPropertyByName(TEXT("bRemoveOnSleep"));

            if (Prop)
            {
                // 2. Cast it to a Bool Property
                if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Prop))
                {
                    // 3. Set the value directly in the component's memory
                    BoolProp->SetPropertyValue_InContainer(GeometryCollectionComponent, true);
                }
            }

            FProperty* MinProp = GeometryCollectionComponent->GetClass()->FindPropertyByName(TEXT("RemovalDurationMin"));
            if (FFloatProperty* FloatProp = CastField<FFloatProperty>(MinProp))
            {
                FloatProp->SetPropertyValue_InContainer(GeometryCollectionComponent, 0.0f);
            }
            FProperty* MaxProp = GeometryCollectionComponent->GetClass()->FindPropertyByName(TEXT("RemovalDurationMax"));
            if (FFloatProperty* FloatProp = CastField<FFloatProperty>(MinProp))
            {
                FloatProp->SetPropertyValue_InContainer(GeometryCollectionComponent, 0.1f);
            }
        }   
}
#endif // WITH_EDITOR

