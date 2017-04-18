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
		// Pass open request to blueprint
		OnOpen.Broadcast();
	}
	else
	{
		// Pass close request to blueprint
		OnClose.Broadcast();
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

