// Copyright Galin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemynterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	/*This below is taken for TopDown template Player Controller */
	/** Time Threshold to know if it was a short press */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	
	/* End of TopDown Template*/
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/*This below is taken for TopDown template Player Controller */
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	/* End of TopDown Template*/

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	void CurseTrace();
	TObjectPtr<IEnemynterface> LastActor;
	TObjectPtr<IEnemynterface> ThisActor;


	/*This below is taken for TopDown template Player Controller */
	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed
	/* End of TopDown Template*/
	
};
