// Fill out your copyright notice in the Description page of Project Settings.


#include "HordesManager.h"
#include <NavMesh/RecastNavMesh.h>
#include <Kismet/KismetMathLibrary.h>
#include "AIController.h"
#include "NavigationSystem.h"
//static ConstructorHelpers::FClassFinder<AAIController> PlayerPawnBPClass(TEXT("/Game/JoaquinGraciosin/Blueprints/Player/MyPlayerCharacter"));

// Sets default values
AHordesManager::AHordesManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_currentHorde = 0;
	_remainingEnemies = 0;
	_finalHorde = false;
}

// Called when the game starts or when spawned
void AHordesManager::BeginPlay()
{
	Super::BeginPlay();
	OnEnemyDie.AddUniqueDynamic(this, &AHordesManager::EnemyDie);
	SetLastEnemiesEvent.AddUniqueDynamic(this, &AHordesManager::PrepareFinalRoundEnemies);
	StartHorde();
}

// Called every frame
void AHordesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordesManager::StartHorde() {
	SpawnEnemies(Hordes[_currentHorde]._leftEnemies);
	SpawnEnemies(Hordes[_currentHorde]._rightEnemies);
}

void AHordesManager::PrepareNextHorde() {
	++_currentHorde;
	if (_currentHorde == Hordes.Num()) {
		PrepareFinalRound();
	}
	_remainingEnemies = Hordes[_currentHorde]._leftEnemies.NEnemies + Hordes[_currentHorde]._rightEnemies.NEnemies;
	StartHorde();
}

void AHordesManager::SpawnEnemies(FEnemiesToSpawn EnemiesToSpawn) {
	for (int i = 0; i < EnemiesToSpawn.NEnemies; ++i) {
		//Get enemy to spawn
		TSubclassOf<AEnemyBase> enemyToSpawn;
		int enemyIdentifier = FMath::RandRange(0, EnemiesToSpawn.Enemies.Num() - 1);
		enemyToSpawn = EnemiesToSpawn.Enemies[enemyIdentifier];
		//Calculate transform
		FTransform transform = EnemiesToSpawn.SpawnPlace->GetActorTransform();
		//Spawnear Enemigo
		AEnemyBase* enemy = TrySpawnEnemy(transform, enemyToSpawn);
		if (enemy) {
			UClass* EnemyControllerClass = enemy->AIControllerClass;
			AAIController* EnemyController = GetWorld()->SpawnActor<AAIController>(EnemyControllerClass);
			EnemyController->Possess(enemy);
		}
		else {
			--_remainingEnemies;
		}
	}
}

AEnemyBase* AHordesManager::TrySpawnEnemy(FTransform spawnTransform, TSubclassOf<AEnemyBase> enemyClass)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation Result;
	bool fResult = false;
	AEnemyBase* enemy = nullptr;
	int i = 0;
	do
	{
		fResult = NavSys->GetRandomPointInNavigableRadius(spawnTransform.GetLocation(), 200, Result);
		if (fResult)
		{
			FTransform definitiveSpawn = spawnTransform;
			definitiveSpawn.SetLocation(Result.Location);
			enemy = GetWorld()->SpawnActor<AEnemyBase>(enemyClass, definitiveSpawn);
		}
		++i;
	} while (!enemy && i <= 3);
	return enemy;
}

UClass* AHordesManager::GetClassFromBlueprintAsset(const FAssetData& Asset)
{
	const UBlueprint* BlueprintAsset = Cast<UBlueprint>(Asset.GetAsset());
	if (BlueprintAsset)
	{
		return BlueprintAsset->GeneratedClass;
	}
	return nullptr;
}

void AHordesManager::PrepareFinalRound() {
	if (EnemiesFinalRound.Num() > 0) {
		_finalHorde = true;
		_remainingEnemies = EnemiesFinalRound.Num();
		SpawnFinalRoundEnemies();
	}
	else {
		GameWin();
	}
}

void AHordesManager::SpawnFinalRoundEnemies() {
	for (int i = 0; i < EnemiesFinalRound.Num(); ++i) {
		int spawnIdentifier = FMath::RandRange(0, FinalRoundSpawns.Num() - 1);
		FTransform transform = FinalRoundSpawns[spawnIdentifier]->GetActorTransform();
		AEnemyBase* enemy = TrySpawnEnemy(transform, EnemiesFinalRound[i]);
		if (enemy) {
			UClass* EnemyControllerClass = enemy->AIControllerClass;
			AAIController* EnemyController = GetWorld()->SpawnActor<AAIController>(EnemyControllerClass);
			EnemyController->Possess(enemy);
		}
		else {
			--_remainingEnemies;
		}
	}
}

void AHordesManager::GameWin() {
	OnWin.Broadcast();
}

UFUNCTION()
void AHordesManager::EnemyDie() {
	--_remainingEnemies;
	if (_remainingEnemies <= 0) {
		if (!_finalHorde) {
			PrepareNextHorde();
		}
		else {
			GameWin();
		}
	}
}

UFUNCTION()
void AHordesManager::PrepareFinalRoundEnemies(TArray<TSubclassOf<AEnemyBase>> Enemies) {
	if (Enemies.Num() > 0) {
		EnemiesFinalRound = Enemies;
	}
}