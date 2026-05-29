#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YYWaveSpawner.generated.h"

class AYYEnemyBase;

UCLASS()
class YANGYU_API AYYWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	AYYWaveSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AYYEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 MaxAliveCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnRadius = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnHeightOffset = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 AliveEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 KilledEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	int32 ScorePerEnemy = 100;

	FTimerHandle SpawnTimerHandle;

	void SpawnEnemy();

	FVector GetRandomSpawnLocation() const;

	UFUNCTION()
	void HandleSpawnedEnemyDestroyed(AActor* DestroyedActor);

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StopSpawning();

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetKilledEnemyCount() const { return KilledEnemyCount; }

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintPure, Category = "Spawner")
	int32 GetAliveEnemyCount() const { return AliveEnemyCount; }
};