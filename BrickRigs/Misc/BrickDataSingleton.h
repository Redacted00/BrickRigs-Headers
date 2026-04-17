// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Misc/BrickPattern.h"
#include "GameplayTags.h"
#include "Physics/SurfaceTypeEffects.h"
#include "UI/Input/InputTableRow.h"
#include "UObject/Object.h"
#include "BrickDataSingleton.generated.h"

class UParticleSystem;
class USoundCue;
class USoundBase;
class USoundClass;
class UStaticMesh;
class UBrickMaterial;
class AItem;
class UDataTable;
class UTexture2D;
class UTextureRenderTarget2D;

USTRUCT(BlueprintType)
struct FLegacyBrickMaterialReplacement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBrickMaterial* BrickMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBrickPattern> Pattern;
	UPROPERTY(EditAnywhere)
	FLinearColor Color = FLinearColor::White;
};

/**
 *
 */
UCLASS(Blueprintable)
class BRICKRIGS_API UBrickDataSingleton : public UObject
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached list of all used input categories, generated from the input table
	UPROPERTY(Transient)
	TArray<UInputCategory*> InputCategories;
	// ~Variables

protected:
	// ~Properties
	// Input action data table asset
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UDataTable* InputTable;

	// Display names for item tags
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TMap<FGameplayTag, FText> ItemTagDisplayNames;
	// A list of color names for different grayscale values
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TMap<float, FText> GrayscaleColorNames;
	// A list of display names for different color hues
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TMap<float, FText> HueColorNames;

	// List of deprecated brick paint solid colors, used to color legacy vehicles
	UPROPERTY(EditDefaultsOnly, Category = Bricks)
	TMap<FName, FLinearColor> LegacyBrickPaints;
	// List of legacy brick materials (when the brick color was determined by the material)
	UPROPERTY(EditDefaultsOnly, Category = Bricks)
	TMap<FName, FLegacyBrickMaterialReplacement> LegacyBrickMaterials;

public:
	// TODO: Make private
	// Per surface type effects
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	FSurfaceTypeEffects SurfaceTypeEffects[SurfaceType_Max];

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundClass* MainSoundClass;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundClass* WorldSoundClass;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundClass* MusicSoundClass;

	// The brick that is used to render brick material thumbnails on
	UPROPERTY(EditDefaultsOnly, Category = Thumbnails)
	TSubclassOf<UBrickStaticInfo> MaterialThumbnailRenderBrick;
	// Item class to render when there is no item in the slot
	UPROPERTY(EditDefaultsOnly, Category = Thumbnails)
	TSoftClassPtr<AInventoryItem> NoItemThumbnailRenderClass;
	// The material that will be used to render texture thumbnails
	UPROPERTY(EditDefaultsOnly, Category = Thumbnails)
	UMaterialInterface* TextureThumbnailRenderMaterial;
	// Desired direction of the camera for capturing thumbnails
	UPROPERTY(EditDefaultsOnly, Category = Thumbnails)
	FVector ThumbnailRenderDirection;
	// Post process settings to use for thumbnail rendering
	UPROPERTY(EditDefaultsOnly, Category = Thumbnails)
	FPostProcessSettings ObjectThumbnailPostProcessSettings;
	// ~Properties

	// Static accessor to the game singleton instance
	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get Game Singleton"))
	static UBrickDataSingleton* Get();

	// ~Constructor
	UBrickDataSingleton();

	// Called from the game instance upon initialization
	void OnGameInstanceInit();

	// ~Input
	// Access the input table rows directly
	void GetInputTableRows(TMap<FName, const FInputTableRow*>& OutRows) const;
	// Version that outputs separate name and row arrays
	void GetInputTableRows(TArray<FName>& OutActionNames, TArray<const FInputTableRow*>& OutRows) const;
	// Find a single row
	FInputTableRow* GetInputTableRow(const FName& RowName) const;
	// Can be used to iterate input table rows without collecting them in an array or map first
	void ForEachInputTableRow(const TFunction<void(const FName&, const FInputTableRow&)>& Func) const;
	// Get all used input categories
	const TArray<UInputCategory*>& GetInputCategories() const
	{
		return InputCategories;
	}

	// ~Input

	// ~UI
	// Get the display name for a given tag, useful for inventory special slot names
	FText GetItemTagDisplayName(const FGameplayTag& Tag) const;
	// Returns an approximate display name for the given color
	UFUNCTION(BlueprintCallable)
	FText GetColorDisplayName(const FLinearColor& Color, bool bRoundValue = true) const;
	// ~UI

	// ~Effects
	// Get the surface type effects specifically for impact effects, takes care of falling back to default values
	void GetImpactEffects(EPhysicalSurface SurfaceType, FSurfaceTypeEffects& OutEffects) const;
	// Get the footstep sound for the given surface type
	USoundBase* GetFootstepSound(EPhysicalSurface SurfaceType);
	// ~Effects

	// ~Bricks
	// Try to find the color associated with the given paint name, return true if it exists
	bool ResolveBrickPaint(const FString& PaintName, FLinearColor& OutColor) const;
	// Try to find a replacement for a legacy material
	bool ResolveBrickMaterial(const FString& MaterialName, FLegacyBrickMaterialReplacement& OutReplacement) const;
	// ~Bricks
};
