#include "Boid_BoidSpawner.h"
#include "Boid_Boid.h"
#include "Kismet/KismetMathLibrary.h"

ABoid_BoidSpawner::ABoid_BoidSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ABoid_BoidSpawner::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < NumberOfAgents; ++i)
	{
		FVector Result = FMath::VRand();
		Result *= FMath::RandRange(SpawnRangeMin,SpawnRangeMax);
		FVector SpawnPosition = GetActorLocation() + Result;
		ABoid_Boid* SpawnedAgent = GetWorld()->SpawnActor<ABoid_Boid>(ClassToSpawn, SpawnPosition, UKismetMathLibrary::RandomRotator());
		SpawnedAgent->OriginPosition = this;
		Boids.Add(SpawnedAgent);
	}
}

void ABoid_BoidSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (ABoid_Boid* boid : Boids)
	{
		boid->DoFlockingMovement(DeltaSeconds);
	}
}


