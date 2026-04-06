


#include "Gameplay/Items/ItemPickUp.h"
#include "Components\StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CombatPlayerState.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"

DEFINE_LOG_CATEGORY(LogItem);

// Sets default values
AItemPickUp::AItemPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));
	if (SphereColliderComponent)
	{
		RootComponent = SphereColliderComponent;
	}
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetupAttachment(RootComponent);
	}

	#if WITH_EDITOR
	VerifyEditorConstructor();
	#endif
}

#if WITH_EDITOR
void AItemPickUp::VerifyEditorConstructor()
{
	if (!ItemInfo.ItemAbilities.IsEmpty() || !ItemInfo.ItemEffects.IsEmpty())
	{
		UE_LOG(LogItem, Error, TEXT("SETUP ERROR: Item %s's does not have at least one Item Ability or effects"), *GetName());
	}
	if (ItemInfo.name.IsNone())
	{
		UE_LOG(LogItem, Error, TEXT("SETUP ERROR: Item %s's does not have at a name set it is FItemInfomation"), *GetName());
	}
}
#endif

// Called when the game starts or when spawned
void AItemPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickUp::OnPickup()
{
	SetLifeSpan(1.0f);
}

void AItemPickUp::OnInteract()
{

}

FItemInfomation AItemPickUp::GetItemInfo()
{
	return ItemInfo;
}
