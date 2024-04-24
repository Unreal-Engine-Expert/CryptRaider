// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = getPhysicsHandleComponent();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {

		FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;
		PhysicsHandle->SetTargetLocationAndRotation(
			targetLocation,
			GetComponentRotation()
		);
	}

}


void UGrabber::GRABBER_Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = getPhysicsHandleComponent();
	if (PhysicsHandle == nullptr) return;

	FHitResult hitResult;
	bool hasHit = getGrabbableInReach(hitResult);
	if (hasHit) {
		UE_LOG(LogTemp, Display, TEXT("Actor Grabbed"));
		UE_LOG(LogTemp, Display, TEXT("Actor Hit = %s"), *(hitResult.GetActor()->GetActorNameOrLabel()));

		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->SetSimulatePhysics(true);
		// this is used because we need to wake up all rigid body becuase their pyhsics goes to sleep after sometime
		hitComponent->WakeAllRigidBodies();
		// Aadding a Tag to Gargoyle
		AActor* actor = hitResult.GetActor();
		actor->Tags.Add("Grabbed");
		actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Display, TEXT("Tag 'Grabbed' added to actor: %s"), *(hitResult.GetActor()->GetActorNameOrLabel()));
		// This will grab the component at location and camera rotation
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);

		// For Getting Statue Location
		//DrawDebugSphere(GetWorld(),hitResult.ImpactPoint,10,10,FColor::Green,false,5);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Bla Bla No Actor Hit...!"));
	}
}

void UGrabber::GRABBER_Release()
{

	UPhysicsHandleComponent* PhysicsHandle = getPhysicsHandleComponent();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
		UPrimitiveComponent* grabbedComponent = PhysicsHandle->GetGrabbedComponent();
		// waking body to grabb
		grabbedComponent->WakeAllRigidBodies();
		// Remove the "Grabbed" tag from the actor
		grabbedComponent->GetOwner()->Tags.Remove("Grabbed");
		// releasing the component
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Display, TEXT("Actor Released"));
	}

}

UPhysicsHandleComponent* UGrabber::getPhysicsHandleComponent() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Pyhsics Handle for Grabber is Null, please recheck "));
	}
	return PhysicsHandle;
}

bool UGrabber::getGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector startLocation = GetComponentLocation();
	FVector endLocation = startLocation + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(),startLocation, endLocation,FColor::Red);
	//DrawDebugSphere(GetWorld(),endLocation,10,10,FColor::Blue,false,5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		startLocation,
		endLocation,
		FQuat::Identity, // currently we dont need any rotation yet
		ECC_GameTraceChannel2, // Find it from the config folder and filter by name " Grabber "
		sphere
	);
}