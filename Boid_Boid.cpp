#include "Boid_Boid.h"
#include "Boid_BoidSpawner.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABoid_Boid::ABoid_Boid()
{
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	FoVRad = UKismetMathLibrary::DegreesToRadians(FieldOfView);
}

void ABoid_Boid::DoFlockingMovement(float DeltaTime)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	SetNeighbors(OverlappingActors);

	FVector Alignment = CalculateAlignment(OverlappingActors) * AlignmentWeight;
	if (Alignment.SquaredLength() > AlignmentWeight * AlignmentWeight)
	{
		Alignment.Normalize();
		Alignment *= AlignmentWeight;
	}
	FVector Cohesion = CalculateCohesion(OverlappingActors) * CohesionWeight;
	if (Cohesion.SquaredLength() > CohesionWeight * CohesionWeight)
	{
		Cohesion.Normalize();
		Cohesion *= CohesionWeight;
	}
	FVector Separation = CalculateSeparation(OverlappingActors) * SeparationWeight;
	if (Separation.SquaredLength() > SeparationWeight * SeparationWeight)
	{
		Separation.Normalize();
		Separation *= SeparationWeight;
	}
	FVector WithinRadius = CalculateToStayInRadius() * StayWithinRadiusWeight;
	if (WithinRadius.SquaredLength() > StayWithinRadiusWeight * StayWithinRadiusWeight)
	{
		WithinRadius.Normalize();
		WithinRadius *= StayWithinRadiusWeight;
	}
	
	FVector TotalMove = Alignment + Cohesion + Separation + WithinRadius;
	TotalMove *= Speed;
	
	FRotator Rotation = TotalMove.Rotation();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(),Rotation, DeltaTime,RotationLerp));
	FVector MoveDelta = FMath::VInterpTo(GetActorForwardVector(), TotalMove, DeltaTime, MoveLerp);
	if (MoveDelta.Length() > MaxSpeed)
	{
		MoveDelta.Normalize();
		MoveDelta *= MaxSpeed;
	}
	
	AddActorWorldOffset(MoveDelta);
}

void ABoid_Boid::SetNeighbors(TArray<AActor*> OverlappingActors)
{
	if (OverlappingActors.Num() <= 0)
		return;
	
	for (AActor* item : OverlappingActors)
	{
		FVector itemDir = item->GetActorLocation() - GetActorLocation();
		itemDir.Normalize();
		float Dot = UKismetMathLibrary::Dot_VectorVector(itemDir, GetActorForwardVector());
		UKismetMathLibrary::Acos(Dot);
		if (Dot > FoVRad * 0.5f)
		{
			OverlappingActors.Remove(item);
		}
	}
}

FVector ABoid_Boid::CalculateAlignment(TArray<AActor*> OverlappingActors)
{
	FVector Alignment = FVector::Zero();
	if (OverlappingActors.Num() <= 0) { return GetActorForwardVector();	}
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (Cast<ABoid_Boid>(OverlappingActor))
		{
			Alignment += OverlappingActor->GetActorForwardVector();
		}
	}
	Alignment /= OverlappingActors.Num();
	return Alignment;
}

FVector ABoid_Boid::CalculateCohesion(TArray<AActor*> OverlappingActors)
{
	FVector Cohesion = FVector::Zero();
	if (OverlappingActors.Num() <= 0) {	return Cohesion;}
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (Cast<ABoid_Boid>(OverlappingActor))
		{
			Cohesion += OverlappingActor->GetActorLocation();
		}
	}
	Cohesion /= OverlappingActors.Num();
	Cohesion -= GetActorLocation();
	Cohesion = FMath::VInterpTo(GetActorForwardVector(),Cohesion, GetWorld()->DeltaTimeSeconds,CohesionLerp);
	return Cohesion;
}

FVector ABoid_Boid::CalculateSeparation(TArray<AActor*> OverlappingActors)
{
	FVector Separation = FVector::Zero();
	if (OverlappingActors.Num() <= 0) { return Separation; }
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		FVector Distance = GetActorLocation() - OverlappingActor->GetActorLocation();
		if (Cast<ABoid_Boid>(OverlappingActor))
		{
			if (Distance.Length() < DistanceToNeighbour)
			{
				Separation += Distance;
			}
		}
	}
	Separation /= OverlappingActors.Num();
	return Separation;
}

FVector ABoid_Boid::CalculateToStayInRadius()
{
	FVector DistAgentToCenter = OriginPosition->GetActorLocation() - GetActorLocation();
	float DistLength = DistAgentToCenter.Length();
	
	if (DistLength < MaxDistToOrigin) { return FVector::Zero(); }
	return DistAgentToCenter * DistLength * DistLength;
}