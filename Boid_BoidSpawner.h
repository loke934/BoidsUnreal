// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid_BoidSpawner.generated.h"

class ABoid_Boid;

UCLASS()
class BOIDS_API ABoid_BoidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoid_BoidSpawner();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	TSubclassOf<ABoid_Boid> ClassToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	int NumberOfAgents = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	float SpawnRangeMin = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	float SpawnRangeMax = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	TArray<ABoid_Boid*> Boids;

};
