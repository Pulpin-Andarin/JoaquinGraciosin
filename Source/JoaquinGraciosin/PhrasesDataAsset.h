// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PhrasesDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FClownAnswers
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MainPhrase;


	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> Answers;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> Points;

	 FClownAnswers() {}
};



USTRUCT(BlueprintType)
struct FPhrases
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PatientDescription1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PatientDescription2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PatientPhrase;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FClownAnswers> ClownPhrases;

	FPhrases() {}
};


/**
 * 
 */
UCLASS()
class JOAQUINGRACIOSIN_API UPhrasesDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPhrases> Phrases;
};
