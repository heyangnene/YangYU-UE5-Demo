#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YYWaveSpawner.generated.h"

class AYYEnemyBase;

UCLASS()
class YANGYU_API AYYWaveSpawner : public AActor		//YYWaveSpawner类继承自AActor。
{
	GENERATED_BODY()

public:
	AYYWaveSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AYYEnemyBase> EnemyClass;		//允许在蓝图选择一个AYYEnemyBase的子类作为要生成的敌人类型。

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 3.0f;		//每隔3秒生成一个敌人。

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 MaxAliveCount = 5;		//同时存在的最大敌人数量。

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnRadius = 150.0f;		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnHeightOffset = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 AliveEnemyCount = 0;		//当前存在的敌人数量。

	FTimerHandle SpawnTimerHandle;		//用于控制生成敌人的定时器。

	void SpawnEnemy();

	FVector GetRandomSpawnLocation() const;

	UFUNCTION()
	void HandleSpawnedEnemyDestroyed(AActor* DestroyedActor);

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StopSpawning();
};