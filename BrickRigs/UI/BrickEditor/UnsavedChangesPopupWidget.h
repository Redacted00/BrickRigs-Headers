// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "UnsavedChangesPopupWidget.generated.h"

UCLASS()
class BRICKRIGS_API UUnsavedChangesPopupParams : public UPopupParams
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UUnsavedChangesPopupWidget : public UPopupWidget
{
	GENERATED_BODY()
};
