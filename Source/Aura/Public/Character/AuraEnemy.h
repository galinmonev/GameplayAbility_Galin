// Copyright Galin

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Enemynterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemynterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//** end Enemy Interface */
	
protected:	
	virtual void BeginPlay() override;

};
