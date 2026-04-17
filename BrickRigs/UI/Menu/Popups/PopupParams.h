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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPopupWidget> PopupClass;
	// ~Properties

	void SetOwningPlayer(ABasePlayerController* InPC)
	{
		OwningPC = InPC;
	}

	UFUNCTION(BlueprintPure)
	ABasePlayerController* GetOwningPlayer() const
	{
		return OwningPC;
	}

	template <class T = ABasePlayerController>
	T* GetOwningPlayer() const
	{
		return Cast<T>(OwningPC);
	}

	// Called after the popup has been opened
	virtual void NativeOnPopupOpened()
	{
		OnPopupOpened();
	}
	
	// Called after the popup has been closed
	virtual void NativeOnPopupClosed(const EPopupResult Result)
	{
		OnPopupClosed(Result);
		OnClosedDelegate.ExecuteIfBound(Result);
	}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPopupOpened();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPopupClosed(const EPopupResult Result);

public:
	UFUNCTION(BlueprintCallable)
	void BindOnPopupClosedDelegate(FOnPopupClosedScript Delegate)
	{
		OnClosedDelegate.BindUFunction(Delegate.GetUObject(), Delegate.GetFunctionName());
	}
};

UCLASS(Blueprintable)
class BRICKRIGS_API UMessagePopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Properties
	// The text to display as the title
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TitleText;
	// Whether a cancel button should be shown
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanCancel;
	// ~Properties
};
