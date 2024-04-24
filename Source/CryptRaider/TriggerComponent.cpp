// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// this is Injecting from Mover Component(Secret Wall) in Event Graph
void  UTriggerComponent::setMover(UMover* mover)
{
	Mover = mover;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialising the original Actor location
	OriginalActorLocation = GetOwner()->GetActorLocation();

	UE_LOG(LogTemp, Display, TEXT("Hello From Trigger Component "));

}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* actor = GetAcceptableActor();
	if (actor != nullptr) {

		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (component != nullptr) {
			// we are stopping physics for Gargoyle
			component->SetSimulatePhysics(false);
		}
		// we are atttaching Gargoyle to Trigger Component (which is door invisible component)
		actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		UE_LOG(LogTemp, Display, TEXT("Unlocking Door 1 with Actor %s"), *actor->GetActorNameOrLabel());
		Mover->SetShouldMove(true);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Re-locking Door"));
		Mover->SetShouldMove(false);
	}

}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> actorsList;
	// Finding all overlap actors and copy them into list
	GetOverlappingActors(actorsList);

	for (AActor* actor : actorsList) {
		bool hasAcceptableTag = actor->ActorHasTag(AcceptableActorTag); // this tag is used to find tthe Gaaargoyle
		bool isGrabbedTag = actor->ActorHasTag("Grabbed"); // this is used to know wetther it is grabbed or releaased
		if (hasAcceptableTag && !isGrabbedTag) {
			return actor;
		}
	}
	return nullptr;
}

