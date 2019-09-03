// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain_ActorComponent.h"
#include "ArtificialNN/ArtificialNN.h"
#include "FlyingPigFlipbookActor.h"
#include "Engine/Engine.h"
// Sets default values for this component's properties
UBrain_ActorComponent::UBrain_ActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrain_ActorComponent::BeginPlay()
{
	Super::BeginPlay();

	ann = new ArtificialNN(3, 2, 1, 6, 0.2f);
	pig = Cast<AFlyingPigFlipbookActor>(GetOwner());

}


// Called every frame
void UBrain_ActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!pig)
	{
		GEngine->AddOnScreenDebugMessage(
			0, 60.0f, FColor::Cyan,
			FString("The brain can't find the Pig.")
		);
		return;
	}
}
