// Copyright Galin


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Interaction/Enemynterface.h"
#include "InputActionValue.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	/*This below is taken for TopDown template Player Controller */
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	/* End of TopDown Template*/
		
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CurseTrace();
}

void AAuraPlayerController::CurseTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemynterface>(CursorHit.GetActor());

	/**
	 *Line trace from cursor. There are several scenrios
	 * A.LastActor is null && ThisACtor is null
	 * - do nothing
	 * B.Last Actor is null && ThisActor is valid
	 *	- this actor is enemy interface so call HighLight thisactor
	 *	C. LastActor is valid && ThisActor is null
	 *	 - last frame we had enemy interface so now we want to UnHighlisht LastActor
	 *	 D. Both Actors are valid, but LastActor != ThisActor
	 *	  - UnHihglihg LastActor and Highligh this actor (
	 *	  E. Both Actors are valid and are the same actor
	 *	   - Do nothing
	 */


	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//case B
			ThisActor->HighlightActor();
		}
		else
		{
			//case A - both are null,  do nothing
		}
	}
	else //Last Actori is valid
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else //both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//case E - do nothing
			}
		}
		
	}
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	//Add input  mapping contenxt
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	/*This below is taken for TopDown template Player Controller */
	// Setup mouse input events
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AAuraPlayerController::OnInputStarted);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::OnSetDestinationTriggered);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AAuraPlayerController::OnSetDestinationReleased);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AAuraPlayerController::OnSetDestinationReleased);
	/* End of TopDown Template*/
}

void AAuraPlayerController::OnInputStarted()
{
	StopMovement();
}

void AAuraPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
		UE_LOG(LogTemp, Log, TEXT("Curor Location: %s"), *CachedDestination.ToString());
	}

	// Move towards mouse pointer
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}

	
}

void AAuraPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// Move to where the cursor is pressed. It only works with nav mesh volume
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	}

	FollowTime = 0.f;
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation (0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>() )
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

