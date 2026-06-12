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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Spawner")
	int32 AliveEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Score")
	int32 KilledEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Score")
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	int32 ScorePerEnemy = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Wave")
	int32 TotalWaves = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 Wave1EnemyCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 Wave2EnemyCount = 7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 Wave3EnemyCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveBreakTime = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Wave")
	int32 CurrentWave = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Wave")
	int32 SpawnedInCurrentWave = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Wave")
	int32 KilledInCurrentWave = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Wave")
	bool bAllWavesCompleted = false;

	FTimerHandle SpawnTimerHandle;

	FTimerHandle WaveBreakTimerHandle;

	void SpawnEnemy();

	FVector GetRandomSpawnLocation() const;

	UFUNCTION()
	void HandleSpawnedEnemyDestroyed(AActor* DestroyedActor);

	void StartNextWave();

	void FinishCurrentWave();

	int32 GetCurrentWaveEnemyCount() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetCurrentWave() const { return CurrentWave; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetTotalWaves() const { return TotalWaves; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetSpawnedInCurrentWave() const { return SpawnedInCurrentWave; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetKilledInCurrentWave() const { return KilledInCurrentWave; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetEnemiesInCurrentWave() const { return GetCurrentWaveEnemyCount(); }

	UFUNCTION(BlueprintPure, Category = "Wave")
	bool AreAllWavesCompleted() const { return bAllWavesCompleted; }
};