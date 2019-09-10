// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingPigFlipbookActor.h"

#include "GameFramework/Actor.h"
#include "Engine.h"

#include "Brain_ActorComponent.h"


// Sets default values
AFlyingPigFlipbookActor::AFlyingPigFlipbookActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Brain = CreateDefaultSubobject<UBrain_ActorComponent>("The Brain");
}

// Called when the game starts or when spawned
void AFlyingPigFlipbookActor::BeginPlay()
{
	Super::BeginPlay();

	paperFlipbookComponent = Cast<UPaperFlipbookComponent>(GetRootComponent());
	paperFlipbookComponent->OnComponentHit.AddDynamic(
		this,
		&AFlyingPigFlipbookActor::OnPigHit
	);

	StartLocation = GetActorLocation();

}

// Called every frame
void AFlyingPigFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlyingPigFlipbookActor::OnPigHit(UPrimitiveComponent * hitComp, AActor * otherActor, UPrimitiveComponent * otherComp, FVector NormalImpulse, const FHitResult & hit)
{
	bFailed = true;
}

void AFlyingPigFlipbookActor::Move(FVector direction, float DeltaTime)
{
	paperFlipbookComponent->AddForce(
		direction * DeltaTime
	);
}


