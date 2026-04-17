// Copyright Fluppisoft, 2020

#pragma once

#include "UGC/UGCTypes.h"
#include "Interfaces/OnlineUGCInterface.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "UGCTaskPopupWidget.generated.h"

UENUM(BlueprintType)
enum class EUGCTaskPopupType : uint8
{
	None,
	Download,
	Upload
};

UCLASS()
class BRICKRIGS_API UUGCTaskPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Properties
	// The type of task to display
	EUGCTaskPopupType TaskType;
	// ~Properties
};

/**
 * This popup is displayed to show the upload or download progress
 */
UCLASS(Abstract)
class BRICKRIGS_API UUGCTaskPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The currently targeted progress
	float TargetProgress;
	// The currently displayed progress
	float CurrentProgress;
	// Whether the task is finished
	bool bIsFinished;
	// Whether the task has finished successfully
	bool bWasSuccessful;
	// Cached failure reason
	FText FailureReasonText;
	// ~Variables

protected:
	// ~Properties
	// Speed at which the visual progress is interpolated
	UPROPERTY(EditDefaultsOnly, Category = UGCTask)
	float ProgressInterpSpeed;
	// ~Properties

public:
	// ~Constructor
	UUGCTaskPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual FText GetPopupTitle_Implementation() const override;
	virtual bool CanPopupBeClosed_Implementation() const override;
	// ~Super Interface

protected:
	// Gets the UGC task type from the popup params
	EUGCTaskPopupType GetUGCTaskType() const;

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsFinished(bool bNewFinished, bool bNewSuccess);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProgress(float NewProgress);
	// ~Blueprint Interface

private:
	// Sets the current progress target
	void SetTargetProgress(float NewProgress, bool bImmediate = false);
	// Sets the displayed progress
	void SetCurrentProgress(float NewProgress);
	// Marks the task as finished or in progress
	void SetIsFinished(bool bNewFinished, bool bNewSuccess, bool bForceUpdate = false);
	// To be called when a complete callback has been invoked
	void OnUGCTaskComplete(bool bSuccess, const FText& ErrorMessage);
	// Callback for the brick editor
	void OnUGCDownloadComplete(bool bSuccess);
	// Callback for the game instance
	void OnUGCUploadComplete(EUGCUploadResult Result, const FOnlineUGCItemIdWrapper& NewItemId);
	// Converts the upload result enum to a display text
	FText GetUGCUploadFailureText(EUGCUploadResult Result) const;
};
