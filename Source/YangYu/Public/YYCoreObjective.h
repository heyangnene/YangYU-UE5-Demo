#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YYCoreObjective.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UYYHealthComponent;

UCLASS()
class YANGYU_API AYYCoreObjective : public AActor
{
	GENERATED_BODY()
	
public:
	AYYCoreObjective();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core")
	UStaticMeshComponent* CoreMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core")
	UYYHealthComponent* HealthComponent;

	UFUNCTION()
	void HandleCoreDestroyed();

	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

public:
	UFUNCTION(BlueprintPure, Category = "Core")
	UYYHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category = "Core")
	void RepairCore(float RepairAmount);
};