// Copyright Robert Allen 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Opens the door
void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f)); // Set the door rotation
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
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) // If the ActorThatOpens is in the volume
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

