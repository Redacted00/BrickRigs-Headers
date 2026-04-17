// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraBrickWidget.generated.h"

class UCameraBrick;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UCameraBrickWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The associated camera brick
	TWeakObjectPtr<UCameraBrick> CameraBrick;
	// Last time the range has been updated
	float LastTargetRangeUpdateTime;
	// ~Variables

protected:
	// ~Properties
	// Whether the target range should be updated at all, can be disabled for performance
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	bool bUpdateTargetRange;
	// How often the range display should be updated
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetRangeUpdateInterval;
	// ~Properties

public:
	// ~Constructor
	UCameraBrickWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	// ~Super Interface

	// ~Setter
	void SetCameraBrick(UCameraBrick* NewCamera);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTargetRange(bool bHitSomething, float Distance);
	// ~Blueprint Interface
};
