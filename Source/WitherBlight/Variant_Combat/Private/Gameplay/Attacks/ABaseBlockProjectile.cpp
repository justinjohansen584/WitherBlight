


#include "Gameplay/Attacks/ABaseBlockProjectile.h"
#include "Data\Structs\FAttackAndItemDataStructs.h"
#include "Gameplay/Component/BlockGatherShootComponent.h"
#include "Components\BoxComponent.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemInterface.h>

DEFINE_LOG_CATEGORY(LogProjectile);

// Sets default values
ABaseBlockProjectile::ABaseBlockProjectile()  //Most likely need a constructor that pass in projectile Data so it has it from the get go
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //Set up Static Mesh Component
    if (!BoxColliderComponent)
    {
        MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
        MeshComponent->SetStaticMesh(ProjMesh);
        MeshComponent->SetCollisionProfileName(FName("NoCollision"));
        //Set up Box Collider
        if (!BoxColliderComponent)
        {
            BoxColliderComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollider"));
            BoxColliderComponent->SetGenerateOverlapEvents(false);
            BoxColliderComponent->InitBoxExtent(MeshComponent->Bounds.BoxExtent);
            RootComponent = BoxColliderComponent;
            BoxColliderComponent->SetCollisionProfileName(FName("Projectile"));
            BoxColliderComponent->SetNotifyRigidBodyCollision(true);
        }
        MeshComponent->SetupAttachment(GetRootComponent());
    }
    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(BoxColliderComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }
}

void ABaseBlockProjectile::BeforeSpawnSetup(AActor* projOwner, UBlockGatherShootComponent* blockGatherComponent, FTransform spawnTransform)
{
    ProjInfo.Owner = projOwner;
    ProjInfo.OwnerGatherShootComp = blockGatherComponent;
    ProjInfo.SpawnTransform = spawnTransform;
    UAbilitySystemComponent* ASCOwner = Cast<IAbilitySystemInterface>(projOwner)->GetAbilitySystemComponent();
    FGameplayEffectContextHandle OwnerContexctHandle = ASCOwner->MakeEffectContext();
    OwnerContexctHandle.AddInstigator(GetInstigator(), this);
    OwnerContexctHandle.AddOrigin(GetActorLocation());
    ProjInfo.OwnerASC = ASCOwner;
    ProjInfo.ContextHandle = OwnerContexctHandle;
    BoxColliderComponent->IgnoreActorWhenMoving(projOwner, true);
}

// Called when the game starts or when spawned
void ABaseBlockProjectile::BeginPlay()
{
	Super::BeginPlay();
    if (BoxColliderComponent)
    {
        BoxColliderComponent->SetGenerateOverlapEvents(false);
        BoxColliderComponent->OnComponentHit.AddDynamic(this, &ABaseBlockProjectile::OnBoxColliderComponentHit);
    }
    //MeshComponent->SetSimulatePhysics(true);
	
}

void ABaseBlockProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    OnProjectileNotifyHit.Clear();
}

// Called every frame
void ABaseBlockProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBlockProjectile::OnBoxColliderComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Actor %s's has hit %s's"), *GetName(), *OtherActor->GetName());
    if (!bHasHitOccured && OtherActor && OtherActor != this)
    {
        bHasHitOccured = true;
        // Tell whoever is listening that we hit something!
        ProjInfo.ContextHandle.AddHitResult(Hit);
        OnProjectileNotifyHit.Broadcast(this, HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
        //Destroy();
        SetLifeSpan(1.0f);
    }
}

// test when it is placed
#if WITH_EDITOR
void ABaseBlockProjectile::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // This runs whenever the Actor is placed or compiled in the Editor.
    if (MeshComponent->GetStaticMesh() == nullptr)
    {
        // Issue a warning to the editor log
        UE_LOG(LogTemp, Error, TEXT("SETUP ERROR: Actor %s's Does not have a Static mesh for the static mesh component"), *GetName());

    }
    else if (BoxColliderComponent->Bounds.BoxExtent == MeshComponent->Bounds.BoxExtent)
    {
        UE_LOG(LogTemp, Error, TEXT("SETUP ERROR: Protectile Actor %s's Box Collider is not at greater or equil the static mesh bounds"), *GetName());
    }
}
#endif // WITH_EDITOR


