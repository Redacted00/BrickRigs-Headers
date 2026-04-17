#pragma once

#include "CoreMinimal.h"
#include "PopupParams.generated.h"

class UPopupWidget;
class ABasePlayerController;

// Popup handles are used to uniquely identify popups in order to close them for example
USTRUCT(BlueprintType)
struct FPopupHandle
{
	GENERATED_BODY()
	;

public:
	// ~Constructor
	FPopupHandle()
		: Handle(0)
	{
	}

	bool IsValid() const
	{
		return Handle != 0;
	}

	void Reset()
	{
		Handle = 0;
	}

	bool operator==(const FPopupHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	const FPopupHandle& operator++()
	{
		Handle++;
		return *this;
	}

private:
	// ~Variables
	uint32 Handle;
	// ~Variables
};

UENUM(BlueprintType)
enum class EPopupResult : uint8
{
	Confirm,
	Cancel,
	Ignore,
	ForceClose
};

UENUM(BlueprintType)
enum class EPopupSizeRule : uint8
{
	Auto,
	Fill,
	FillH,
	FillV
};

DECLARE_DELEGATE_OneParam(FOnPopupClosed, EPopupResult);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPopupClosedScript, EPopupResult, Result);

UCLASS(Blueprintable)
class BRICKRIGS_API UPopupParams : public UObject
{
	GENERATED_BODY()

private:
	// ~Variables
	// The player who has spawned the popup
	UPROPERTY(Transient)
	ABasePlayerController* OwningPC;
	// ~Variables

public:
	// ~Delegates
	// Delegate called when the popup has been closed
	FOnPopupClosed OnClosedDelegate;
	// ~Delegates

	// ~Properties
	// The type of content widget
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPopupWidget> PopupClass;
	// ~Properties

	void SetOwningPlayer(ABasePlayerController* InPC)
	{
		OwningPC = InPC;
	}

	template <class T = ABasePlayerController>
	T* GetOwningPlayer() const
	{
		return Cast<T>(OwningPC);
	}

	// Called after the popup has been opened
	virtual void OnPopupOpened()
	{
	}

	// Called afte the popup has been closed
	virtual void OnPopupClosed(EPopupResult Result)
	{
		OnClosedDelegate.ExecuteIfBound(Result);
	}

	UFUNCTION(BlueprintCallable)
	void BindOnPopupClosedDelegate(FOnPopupClosedScript Delegate)
	{
		OnClosedDelegate.BindUFunction(Delegate.GetUObject(), Delegate.GetFunctionName());
	}
};

UCLASS()
class BRICKRIGS_API UMessagePopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Properties
	// The text to display as the title
	UPROPERTY(EditAnywhere)
	FText TitleText;
	// Whether a cancel button should be shown
	UPROPERTY(EditAnywhere)
	bool bCanCancel;
	// ~Properties
};
