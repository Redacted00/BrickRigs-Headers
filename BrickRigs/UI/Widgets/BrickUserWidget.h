// Copyright Fluppisoft, 2021

#pragma once

#include "UI/Misc/TooltipContent.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickUserWidget.generated.h"

/**
 * Base class that should be used for all user widgets
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FGetTooltipContent, FTooltipContent&, OutContent);

public:
	// ~Delegates
	UPROPERTY(BlueprintReadWrite)
	FGetTooltipContent GetTooltipContentDelegate;
	// ~Delegates

	// Can be implemented to populate the tooltip info to display
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent)
	{
		return GetTooltipContent(OutContent);
	}

	// Can be implemented to customize the tooltip class
	virtual UClass* NativeGetTooltipClass() const
	{
		return GetTooltipClass();
	}

	// Can be used to customize the tooltip
	virtual void NativeInitializeTooltip(UTooltipWidget* Tooltip)
	{
		InitializeTooltip(Tooltip);
	}

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool GetTooltipContent(FTooltipContent& OutContent);
	UFUNCTION(BlueprintImplementableEvent)
	UClass* GetTooltipClass() const;
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeTooltip(UTooltipWidget* Tooltip);
};
