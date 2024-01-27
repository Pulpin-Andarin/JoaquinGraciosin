// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "HordesManager.generated.h"

USTRUCT(BlueprintType)
struct FEnemiesToSpawn {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AEnemyBase>> Enemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SpawnPlace;
	FEnemiesToSpawn(){
		NEnemies = 3;
	}
};

USTRUCT(BlueprintType)
struct FHorde {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEnemiesToSpawn _leftEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEnemiesToSpawn _rightEnemies;
	FHorde() {

	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDieSignature);
UCLASS()
class JOAQUINGRACIOSIN_API AHordesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHordesManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FHorde> Hordes;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnemyDieSignature OnEnemyDie;
private:
	int _currentHorde;
	int _remainingEnemies;

	void StartHorde();
	void PrepareNextHorde();
	void SpawnEnemies(FEnemiesToSpawn EnemiesToSpawn);
	AEnemyBase* TrySpawnEnemy(FTransform spawnTransform, TSubclassOf<AEnemyBase> enemyClass);
	UClass* GetClassFromBlueprintAsset(const FAssetData& Asset);
	UFUNCTION()
	void EnemyDie();
};
