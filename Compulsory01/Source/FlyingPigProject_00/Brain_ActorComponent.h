// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Brain_ActorComponent.generated.h"

class AFlyingPigFlipbookActor;
class ArtificialNN;

struct Replay
{
	TArray<double> states;
	double reward;

	Replay(double xr, double ballz, double ballvx, double r)
	{
		states.Add(xr);
		states.Add(ballz);
		states.Add(ballvx);
		reward = r;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLYINGPIGPROJECT_00_API UBrain_ActorComponent : public UActorComponent
{
	GENERATED_BODY()

		AFlyingPigFlipbookActor* pig;
	ArtificialNN* ann;

	float reward = 0.0f;						//reward to associate with actions
	TArray<Replay*> replayMemory;					//memory - list of past actions and rewards
	int mCapacity = 10000;						//memory capacity

	float discount = 0.99f;						//how much future states affect rewards
	float exploreRate = 100.0f;					//chance of picking random action
	float maxExploreRate = 100.0f;					//max chance value
	float minExploreRate = 0.01f;					//min chance value
	float exploreDecay = 0.0001f;					//chance decay amount for each update

	int failCount = 0;						//count when the ball is dropped
	float timer = 0;						//timer to keep track of balancing
	float maxBalanceTime = 0;					//record time ball is kept balanced	


	TArray<double> SoftMax(TArray<double> values);
public:	
	// Sets default values for this component's properties
	UBrain_ActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
