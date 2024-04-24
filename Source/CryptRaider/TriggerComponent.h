// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// this is Injecting from Mover Component(Secret Wall) in Event Graph
	UFUNCTION(BlueprintCallable)
	void setMover(UMover* mover);


private:
	// only require for calculation
	FVector OriginalActorLocation;

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag = "Unlock_Door_1";

	AActor* GetAcceptableActor() const;
	// this is Injecting from Mover Component(Secret Wall) in Event Graph
	UMover* Mover;
};
