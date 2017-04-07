// Copyright Robert Allen 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

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
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	/// Look for attached physics handle and assign to reference
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // Templatized finder gets the physics handle
	if (PhysicsHandle)
	{
		// Physics handle is found
	} else {
		UE_LOG(LogTemp, Error, TEXT("/s missing physics handle component"),	*GetOwner()->GetName());
	};

	/// Look for attached input component and assign to reference
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); // Templatized finder gets the input component
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("/s missing input component"), *GetOwner()->GetName());
	};
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
};


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this click
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	/*
	UE_LOG(LogTemp, Warning, TEXT("Location is %s and rotation is %s"), 
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString()
	);
	*/
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Draw a red line
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Set up collision channel
	// Use constructor to create FCollisionObjectQueryParams using collision channel enum
	FCollisionObjectQueryParams CollisionChannel(ECollisionChannel::ECC_PhysicsBody);

	// Set up trace parameters
	//Use constructor to create FCollisionQueryParams
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line trace out to reach distance
	FHitResult ObjectHit;	
	GetWorld()->LineTraceSingleByObjectType(
		OUT ObjectHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		CollisionChannel,
		TraceParameters
	);
	/// Get object name and log it to the console
	AActor* ActorHit = ObjectHit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace is hitting: %s"),
			*(ActorHit->GetName()) // Dereference the FString with a *
		);
	}
	
	// See what we hit


}

