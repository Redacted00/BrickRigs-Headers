#pragma once

#include "UI/HUD/HUDIcons/HUDIconDomain.h"
#include "InteractionOptions.generated.h"

class ABrickPlayerController;

enum class EInteractionVisibility : uint8
{
	Hidden,
	IconsOnly,
	Visible
};

UENUM(BlueprintType)
enum class EInteractionOptionSlot : uint8
{
	Pri,
	Sec,
	Ter,
	Inventory
};

// NOTE: The delegates have to be script delegates, to support blueprints AND to have support for a == operation (which doesn't work with native delegates)
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteraction, ABrickPlayerController*, InPC);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnInteractionAxis, ABrickPlayerController*, InPC, float, Val);

USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

	// The delegate that will be called when the player interacts with this option
	UPROPERTY()
	FOnInteraction OnInteractionDelegate;
	// Called when the interaction key or axis has been released
	UPROPERTY()
	FOnInteraction OnReleasedDelegate;
	// Alternative axis delegate
	UPROPERTY()
	FOnInteractionAxis OnInteractionAxisDelegate;
	// The display text that is shown on screen for this option
	UPROPERTY(BlueprintReadWrite)
	FText DisplayText;
	// Whether this option can currently be executed
	UPROPERTY(BlueprintReadWrite)
	bool bIsEnabled;

	FInteractionOption()
		: bIsEnabled(true)
	{
	}

	bool IsValid() const
	{
		return OnInteractionDelegate.IsBound() || OnReleasedDelegate.IsBound() || OnInteractionAxisDelegate.IsBound();
	}

	bool IsAxis() const
	{
		return OnInteractionAxisDelegate.IsBound();
	}

	bool operator==(const FInteractionOption& Other) const
	{
		return OnInteractionDelegate == Other.OnInteractionDelegate && OnReleasedDelegate == Other.OnReleasedDelegate && OnInteractionAxisDelegate == Other.OnInteractionAxisDelegate && DisplayText.EqualTo(Other.DisplayText) && bIsEnabled == Other.bIsEnabled;
	}

	bool operator!=(const FInteractionOption& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct FInteractionOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHUDIconDomain Domain;

protected:
	// Primary interaction options that are associated to specific keys
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EInteractionOptionSlot, FInteractionOption> Options;
	// Additional options that aren't bound to keys but unlimited in their amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteractionOption> AdditionalOptions;

public:
	// ~Constructor
	FInteractionOptions(EHUDIconDomain InDomain = EHUDIconDomain::HUD)
		: Domain(InDomain)
	{
	}

	bool operator==(const FInteractionOptions& Other) const
	{
		if (Domain != Other.Domain || AdditionalOptions != Other.AdditionalOptions || Options.Num() != Other.Options.Num())
		{
			return false;
		}

		// Compare the map manually, unfortunately there isn't a built in operation
		for (const auto& Pair : Options)
		{
			auto* FoundOther = Other.Options.Find(Pair.Key);
			if (!FoundOther || *FoundOther != Pair.Value)
			{
				return false;
			}
		}

		return true;
	}

	bool operator!=(const FInteractionOptions& Other) const
	{
		return !(*this == Other);
	}

	const auto& GetOptions() const
	{
		return Options;
	}

	const auto* GetOption(EInteractionOptionSlot OptionSlot) const
	{
		return Options.Find(OptionSlot);
	}

	const auto& GetAdditionalOptions() const
	{
		return AdditionalOptions;
	}

	bool IsOptionValid(EInteractionOptionSlot OptionSlot) const
	{
		auto* FoundOption = GetOption(OptionSlot);
		return FoundOption && FoundOption->IsValid();
	}

	auto& AddOption(EInteractionOptionSlot OptionSlot)
	{
		ensure(!Options.Contains(OptionSlot));
		return Options.FindOrAdd(OptionSlot);
	}

	auto& AddAdditionalOption()
	{
		return AdditionalOptions[AdditionalOptions.Add(FInteractionOption())];
	}
};
