// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// initialising the original Actor location
	OriginalActorLocation = GetOwner()->GetActorLocation();

}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Get Current Location
    FVector currentLocation = GetOwner()->GetActorLocation();
    FVector destinationLocation;

    if (shouldMove) {
        // Get destination Location for moving
        destinationLocation = OriginalActorLocation + MoveOffset;
    }
    else {
        // Get destination Location for moving back
        destinationLocation = OriginalActorLocation;
    }

    // Calculate Speed which is Speed= distance / time
    float speed = FVector::Dist(currentLocation, destinationLocation) / MoveTime;
    // Find new Location
    FVector newLocation = FMath::VInterpConstantTo(currentLocation, destinationLocation, DeltaTime, speed);
    // Set New location
    GetOwner()->SetActorLocation(newLocation);
}

void UMover::SetShouldMove(bool value) {
	shouldMove = value;
}
