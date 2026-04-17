// Copyright Fluppisoft, 2020

#pragma once

#include "Player/ClientDamageInfo.h"
#include "Settings/BrickUserSettings.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UCanvasPanel;
class UBrickImage;
class ABaseCharacter;
class ABrickCharacter;
class AInventoryItem;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// Cached reference to our character, needed so we can unbind delegates reliably
	UPROPERTY(Transient)
	ABaseCharacter* Character;
	// The current item the character is carrying
	UPROPERTY(Transient)
	AInventoryItem* CurrentItem;

	// Widgets that have been created for the crosshair
	UPROPERTY(Transient)
	TArray<UUserWidget*> CrosshairWidgets;
	// Current opacity of the crosshair widgets
	float CrosshairOpacity;
	// The current spread radius
	float CrosshairSpreadRadius;
	// Whether the hit marker animation is currently playing
	uint8 bIsHitAnimationPlaying : 1;
	// Whether the crosshair is currently visible
	uint8 bIsCrosshairVisible : 1;
	// If the attach winch elements are visible
	uint8 bIsAttachingWinch : 1;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCanvasPanel* CrosshairCanvas;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* HitMarkerImage;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	EHUDVisibility CrosshairHUDVisibility;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	EHUDVisibility HitMarkerHUDVisibility;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	int32 NumCrosshairWidgets;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float CrosshairRotationOffset;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float CrosshairAngleStep;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float CrosshairRadiusScale;
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float MinCrosshairRadius;
	// ~Properties

public:
	// ~Constructor
	UCrosshairWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitAnimation(const FClientDamageInfo& DamageInfo);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsAttachingWinch(bool bNewAttaching);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWinchAttachment(bool bBlockingHit, bool bWithinRange);
	// ~Blueprint Interface

private:
	// Updates the visibility of the widget
	void UpdateWidgetVisibility();
	// Used to show or hide the crosshair
	void SetCrosshairVisibility(bool bNewVisible);
	// Update the visibility state of the crosshair widgets
	void UpdateCrosshairVisibility();
	// Updates the opacity
	void SetCrosshairOpacity(float NewOpacity);
	// Updates the spread radius if needed
	void SetCrosshairSpreadRadius(float NewSpreadRadius);
	// Shows or hides the attach winch icon
	void SetIsAttachingWinch(bool bNewAttaching);
	// Sets the character reference and binds/unbinds delegates
	void SetCharacter(ABaseCharacter* InCharacter);
	void OnCharacterChanged(ABaseCharacter* InCharacter);
	// Sets the currently equipped item
	void SetCurrentItem(AInventoryItem* InItem);
	void OnCurrentItemChanged(AInventoryItem* InItem);
	// Callback for the player
	void OnDamagedCharacter(const FClientDamageInfo& DamageInfo);
	// Callback for the user settings
	UFUNCTION()
	void OnHUDVisibilityChanged(EHUDVisibility NewVisibility);
};
