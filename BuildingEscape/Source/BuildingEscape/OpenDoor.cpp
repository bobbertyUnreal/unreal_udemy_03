// Copyright Robert Allen 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); // Find owning actor
	if (!PressurePlate) 
	{	
		UE_LOG(LogTemp, Error, TEXT("Missing pressure plate"));
	}
}

// Opens the door
void UOpenDoor::OpenDoor()
{
	// Pass open request to blueprint
	OnOpenRequest.Broadcast();
}

// Close the door
void UOpenDoor::CloseDoor()
{	
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // Set the door rotation
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame	
	if (!PressurePlate) { return; }
	if (GetMassOnPlate() > TriggerMass) // If the ActorThatOpens is in the volume
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetRealTimeSeconds();
	}

	// Check if it is time to close the door
	if (GetWorld()->GetRealTimeSeconds() - DoorLastOpenTime > DoorCloseDelay) // If the close door delay has elapsed
	{
		CloseDoor();
	}
}

float UOpenDoor::GetMassOnPlate() {
	float TotalMass = 0.0f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;	
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through the mass in Kg
	for (auto& Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Mass: %f"), TotalMass);
	return TotalMass;
}

