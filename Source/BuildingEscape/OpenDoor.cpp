// Copyright Aaron Ramos 2020

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"


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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	//OpenAngle = OpenAngle + InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component, but no PressurePlate set!"), *GetOwner()->GetName());
	}	
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		//DoorLastOpened = when the door was opened
	}
	else if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay)
	{
		//if the door has been open longer than x seconds
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::InterpEaseInOut(CurrentYaw, OpenAngle, DeltaTime, 0.8f / DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::InterpEaseInOut(CurrentYaw, InitialYaw, DeltaTime, 0.6f / DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}