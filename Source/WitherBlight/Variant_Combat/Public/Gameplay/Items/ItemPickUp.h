

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Gameplay\Items\Items.h"
#include "Interfaces/Items/IItemsAbility.h"
#include "Interfaces/PlayerInteract.h"
#include "Data\Structs\FAttackAndItemDataStructs.h"
#include "ItemPickUp.generated.h"

class UStaticMeshComponent;
class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

UCLASS(abstract)
class AItemPickUp : public AActor, public IItemsAbility, public IPlayerInteract
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereColliderComponent;
	
public:	
	// Sets default values for this actor's properties
	AItemPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	/* Start of IITemsAbility Interface */

	/* What happens when the Item is picked up */
	virtual void OnPickup() override;

	/* End of IITemsAbility Interface */

	/* Start of IPlayerInteract Interface */

	/* What happens when the Item is picked up */
	virtual void OnInteract() override;

	/* End of IPlayerInteract Interface */

	/* Getter for Item info */
	virtual FItemInfomation GetItemInfo();

private:

	virtual void VerifyEditorConstructor();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FItemInfomation ItemInfo;

	
};
