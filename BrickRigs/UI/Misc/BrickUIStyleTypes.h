#pragma once

#include "CoreMinimal.h"
#include "Misc/FluEnumStatics.h"
#include "BrickUIStyleTypes.generated.h"

class UBrickUIStyle;

// UI style pointer which always returns a valid object
struct FBrickUIStyleSafePtr
{
	// ~Constructor
	FBrickUIStyleSafePtr(const UBrickUIStyle* InStyle = nullptr);

	// Used to get the style instance
	// NOTE: Always returns a valid pointer, no checking required
	const UBrickUIStyle* Get() const;
	// Convenient accessor
	const UBrickUIStyle* operator->() const
	{
		return Get();
	}

	// Whether the pointer has been set correctly
	bool IsValid() const
	{
		return StylePtr.IsValid();
	}

private:
	TWeakObjectPtr<const UBrickUIStyle> StylePtr;
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUIStyleStateEnum"))
enum class EBrickUIStyleState : uint8
{
	Default,
	Border,
	Button,
	Pressed,
	Focused,
	Selected,
	FocusedSelected,
	Marquee,
	Foreground,
	ForegroundPressed,
	ForegroundFocused,
	ForegroundSelected,
	ForegroundFocusedSelected,
	Background,
	Custom UMETA(Hidden),
	Max UMETA(Hidden)
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUIBrushStyleEnum"))
enum class EBrickUIBrushStyle : uint8
{
	Default,
	Button,
	FolderButton,
	MapButton,
	BorderlessButton,
	KeyBorder,
	Slider,
	Spacer,
	Custom,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBrickUIBrushStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FSlateBrush Brushes[static_cast<int32>(EBrickUIStyleState::Custom)];

	const FSlateBrush& GetBrush(EBrickUIStyleState InStyleState, const FSlateBrush& Fallback) const
	{
		if (InStyleState >= EBrickUIStyleState::Custom)
		{
			return Fallback;
		}
		return Brushes[FFluEnumStatics::ValueToIndex(InStyleState)];
	}
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUIColorStyleEnum"))
enum class EBrickUIColorStyle : uint8
{
	Default,
	Highlight,
	Positive,
	Negative,
	Neutral,
	EditorOutline,
	GameLogo,
	DeveloperLogo,
	Custom,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBrickUIColorStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FLinearColor Colors[static_cast<int32>(EBrickUIStyleState::Custom)];

	// ~Constructor
	FBrickUIColorStyle()
	{
		for (auto i = 0; i < static_cast<int32>(EBrickUIStyleState::Custom); ++i)
		{
			Colors[i] = FLinearColor::White;
		}
	}

	const FLinearColor& GetColor(EBrickUIStyleState InStyleState, const FLinearColor& Fallback) const
	{
		if (InStyleState >= EBrickUIStyleState::Custom)
		{
			return Fallback;
		}
		return Colors[static_cast<int32>(InStyleState)];
	}
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUITextStyleEnum"))
enum class EBrickUITextStyle : uint8
{
	Default,
	Bold,
	Small,
	Large,
	Positive,
	Negative,
	Neutral,
	HUD,
	HighContrast,
	Custom,
	Max UMETA(Hidden)
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUIIconAtlasEnum"))
enum class EBrickUIIconAtlas : uint8
{
	Default,
	MenuButtons,
	MouseKeys,
	GamepadKeys,
	HUDIcons,
	FireModes,
	AmmoTypes,
	HealthIcons,
	ScoreboardIcons,
	ConnectionIcons,
	ServerIcons,
	HUDNotifications,
	DashboardIcons,
	SocialMediaIcons,
	WeatherIcons,
	ConnectorSpacingIcons,
	SpinnerBrickShapeIcons,
	Custom,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBrickUIIconAtlas
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	uint8 NumX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	uint8 NumY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 StartIndex = 0;
};

USTRUCT(BlueprintType)
struct FBrickUIIconSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = -1))
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	uint8 ColumnSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	uint8 RowSpan;

	// ~Constructor
	FBrickUIIconSlot(int32 InIndex = INDEX_NONE, uint8 InColumnSpan = 1, int32 InRowSpan = 1)
		: Index(InIndex), ColumnSpan(InColumnSpan), RowSpan(InRowSpan)
	{
	}

	bool IsValid() const
	{
		return Index >= 0;
	}
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUISpacingStyleEnum"))
enum class EBrickUISpacingStyle : uint8
{
	Small,
	Medium,
	Large,
	Custom,
	Max UMETA(Hidden)
};

UENUM(BlueprintType, Meta = (ScriptName = "BrickUIPaddingStyleEnum"))
enum class EBrickUIPaddingStyle : uint8
{
	Border,
	Button,
	FolderButton,
	MapButton,
	BorderlessButton,
	KeyBorder,
	Slider,
	Custom,
	Max UMETA(Hidden)
};
