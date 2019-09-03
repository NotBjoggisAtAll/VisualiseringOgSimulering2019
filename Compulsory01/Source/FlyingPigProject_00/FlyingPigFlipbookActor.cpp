// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingPigFlipbookActor.h"

#include "GameFramework/Actor.h"
#include "Engine.h"



// Sets default values
AFlyingPigFlipbookActor::AFlyingPigFlipbookActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	noseDetector = Cast<UChildActorComponent>(
					paperFlipbookComponent->GetChildComponent(0));

}

// Called every frame
void AFlyingPigFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlyingPigFlipbookActor::OnPigHit(UPrimitiveComponent * hitComp, AActor * otherActor, UPrimitiveComponent * otherComp, FVector NormalImpulse, const FHitResult & hit)
{

}

void AFlyingPigFlipbookActor::Move(FVector direction, float DeltaTime)
{
	paperFlipbookComponent->AddForce(
		direction * 10.0f
	);
}


