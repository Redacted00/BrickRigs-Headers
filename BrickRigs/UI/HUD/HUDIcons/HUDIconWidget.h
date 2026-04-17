// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "UI/BrickUIStyle.h"
#include "HUDIconDomain.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDIconWidget.generated.h"

class UHUDIconComponent;
class UHUDIconPanelWidget;
class UHUDIconContainerWidget;
class UBrickImage;
class UTexture2D;
class ABrickPlayerState;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDIconWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Cached pointer to the outer panel
	TWeakObjectPtr<UHUDIconPanelWidget> PanelWidget;
	// Cached pointer to our owning container widget
	TWeakObjectPtr<UHUDIconContainerWidget> ContainerWidget;
	// The type domain this widget belongs to
	EHUDIconDomain IconDomain;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	// ~Widgets

public:
	// ~Properties
	// The desired size of the icon
	UPROPERTY(EditDefaultsOnly, Category = HUDIcon)
	FVector2D IconSize;
	// ~Properties

	// ~Constructor
	UHUDIconWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override final;
	virtual void NativeDestruct() override final;
	// ~Super Interface

	// Sets the owning panel widget
	void SetPanelWidget(UHUDIconPanelWidget* InPanel);
	// Can be overridden to initialize our widget
	void InitializeIcon(UHUDIconContainerWidget* Container, EHUDIconDomain InDomain);
	// Can be used to do cleaning up when changing the component
	void UninitializeIconInternal();
	// Called from the container widget if the local player changed teams
	virtual void OnLocalPlayerChangedTeam(const FGenericTeamId& InTeamId);
	// Get the current team id of the local player
	FGenericTeamId GetLocalPlayerTeamId() const;
	// Get the component associated with this widget
	UHUDIconComponent* GetHUDIconComponent() const;
	// Returns the actor owning the icon component
	AActor* GetOwningActor() const;
	// Convenience template version
	template <class T>
	T* GetOwningActor() const
	{
		return Cast<T>(GetOwningActor());
	}

	// Allows the widget to do something with the draw distance
	virtual void UpdateDrawDistance(float NewDist, float& InOutIconScale)
	{
	}

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	FText GetIconDisplayName() const;
	UFUNCTION(BlueprintNativeEvent)
	bool GetIconWorldRotation(float& OutRotation) const;
	UFUNCTION(BlueprintNativeEvent)
	void InitializeIcon();
	UFUNCTION(BlueprintNativeEvent)
	void PostInitializeIcon();
	UFUNCTION(BlueprintNativeEvent)
	void UninitializeIcon();
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIconRotation(float NewRotation);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIconStyle(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InStyleState);
	// ~Blueprint Interface

protected:
	// Can be used to change the image icon
	UFUNCTION(BlueprintCallable)
	void SetIconSlot(FBrickUIIconSlot NewSlot);
	// Overrides the color style
	UFUNCTION(BlueprintCallable)
	void SetIconColorStyle(EBrickUIColorStyle NewColorStyle);
	// Updates the displayed name
	UFUNCTION(BlueprintCallable)
	void UpdateNameText();

	// Sets the teams associated with the icon, can also be bound to callbacks
	void SetIconTeamIds(const TSet<FGenericTeamId>& InTeamIds);
};
