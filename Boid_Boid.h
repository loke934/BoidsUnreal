#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid_Boid.generated.h"

class USphereComponent;
class ABoid_BoidSpawner;

UCLASS()
class BOIDS_API ABoid_Boid : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoid_Boid();

	UFUNCTION()
	void DoFlockingMovement(float DeltaTime);

private:
	UFUNCTION()
	void SetNeighbors(TArray<AActor*> OverlappingActors);
	
	UFUNCTION()
	FVector CalculateAlignment(TArray<AActor*> OverlappingActors);
	
	UFUNCTION()
	FVector CalculateCohesion(TArray<AActor*> OverlappingActors);
	
	UFUNCTION()
	FVector CalculateSeparation(TArray<AActor*> OverlappingActors);
	
	UFUNCTION()
	FVector CalculateToStayInRadius();
	
public:	
	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	USphereComponent* Collision;

	UPROPERTY(EditDefaultsOnly, Category ="Weights")
	float CohesionWeight = 3;

	UPROPERTY(EditDefaultsOnly, Category ="Weights")
	float AlignmentWeight = 1;

	UPROPERTY(EditDefaultsOnly, Category ="Weights")
	float SeparationWeight = 4;

	UPROPERTY(EditDefaultsOnly, Category ="Weights")
	float StayWithinRadiusWeight = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float DistanceToNeighbour = 100.f;

	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float Speed = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float MaxSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float FieldOfView = 180;

	UPROPERTY()
	float FoVRad;
	
	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float RotationLerp = 0.7f;

	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float MoveLerp = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category ="Flocking")
	float CohesionLerp = 0.5f;
	
	UPROPERTY()
	ABoid_BoidSpawner* OriginPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Flocking")
	float MaxDistToOrigin = 800.f;
};
