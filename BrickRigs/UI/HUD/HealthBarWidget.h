// Copyright Fluppisoft, 2018

#pragma once

#include "Character/CharacterHealth.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UInvincibilityWidget;
class ABaseCharacter;

/**
 * Used to display the character's current health
 */
UCLASS(Abstract)
class BRICKRIGS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The associated character, needed to unbind delegates
	TWeakObjectPtr<ABaseCharacter> Character;
	// Current health of the character
	float TargetHealth;
	// Currently displayed health
	float CurrentHealth;
	// Cached healing state
	ECharacterHealingState HealingState;
	// Cached invincibility state
	uint8 bCanBeDamaged : 1;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = HealthBar)
	float HealthInterpSpeed;
	// ~Properties

public:
	// ~Constructor
	UHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	// ~Super Interface

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(float NewHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealingState(ECharacterHealingState NewState);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanBeDamaged(bool bNewCanBeDamaged);
	// ~Blueprint Interface

private:
	// Callback for the player controller
	void SetCharacter(ABaseCharacter* NewCharacter);
	void OnCharacterChanged(ABaseCharacter* NewCharacter);
	// Callbacks for the character
	void SetHealth(float NewHealth);
	void OnHealthChanged(float NewHealth);
	void SetHealingState(ECharacterHealingState NewState);
	void OnHealingStateChanged(ECharacterHealingState NewState);
	void SetCanBeDamaged(bool bNewCanBeDamaged);
	void OnCanBeDamagedChanged(bool bNewCanBeDamaged);
	// Helper function to set the current health
	void SetCurrentHealth(float NewHealth);
	// Update the visibility of the widget
	void UpdateHealthBarVisibility();
	// Callback for the user settings
	UFUNCTION()
	void OnHUDVisibilityChanged(EHUDVisibility NewVisibility);
};
