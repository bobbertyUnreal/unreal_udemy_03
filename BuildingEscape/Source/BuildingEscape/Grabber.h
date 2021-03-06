// Copyright Robert Allen 2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far the player can reach in cm
	float Reach = 100.f;

	// A null physics handle reference to be updated at runtime
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// A null input component reference to be updated at runtime
	UInputComponent* InputComponent = nullptr;

	// Line trace and grab what's in reach
	void Grab();
	
	// Called when grab is released
	void Release();

	// Find attached physics handle component
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Get the player view point location
	FVector GetReachStart();

	// Get the end of the line trace vector
	FVector GetReachEnd();
};
