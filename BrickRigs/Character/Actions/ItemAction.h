#pragma once

#include "Items/Firearm.h"
#include "Items/Animation/ItemAnimation.h"
#include "Items/InventoryItem.h"
#include "Character/Actions/CharacterAction.h"
#include "ItemAction.generated.h"

UCLASS()
class UItemAction : public UCharacterAction
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached struct property that is holding the animation struct
	FStructProperty* AnimationProperty;
	// Whether the item action has been started yet
	uint8 bStartedItemAction : 1;
	// ~Variables

public:
	// ~Constructor
	UItemAction();

	// ~Super Interface
	virtual bool CanStartAction() const override;
	virtual bool CanStartOtherAction(const UCharacterAction* OtherAction) const override;

private:
	virtual void StartAction() override final;

public:
	virtual void StartAnimation() override;
	virtual void OnActionFinished(bool bSuccess) override;
	// ~Super Interface

protected:
	// Return the item associated with this action
	AInventoryItem* GetItem() const;
	// Templated version
	template <class T>
	T* GetItem() const
	{
		return Cast<T>(GetItem());
	}

	// Get the name of the associated animation property on the item static info
	virtual FName GetAnimationName() const;
	// Returns the item animations as a specific type
	template <typename T>
	T* GetItemAnimation();
	// Return whether the action should be cancelled when the item is changed
	virtual bool ShouldCancelOnItemChange() const;
	// Whether the given item can be used for this action
	virtual bool IsItemSupported(AInventoryItem* InItem) const
	{
		return InItem != nullptr;
	}

	// Custom start function used for item actions, which is only called after the item is ready
	virtual void StartItemAction();
	// Callback for the tool anim async load
	void OnAnimAssetsLoaded();
	// Called by the delegate when the characters's current item changes during a tool action
	void OnCurrentItemChanged(AInventoryItem* NewItem);
};

template <typename T>
FORCEINLINE T* UItemAction::GetItemAnimation()
{
	AInventoryItem* Item = GetItem();
	if (Item)
	{
		auto* StaticInfo = Item->GetStaticInfoMutable();
		if (!AnimationProperty)
		{
			AnimationProperty = CastField<FStructProperty>(StaticInfo->GetClass()->FindPropertyByName(GetAnimationName()));
		}

		// Make sure the struct type matches
		if (AnimationProperty && (AnimationProperty->Struct == T::StaticStruct() || AnimationProperty->Struct->IsChildOf(T::StaticStruct())))
		{
			return AnimationProperty->ContainerPtrToValuePtr<T>(StaticInfo);
		}
	}

	return nullptr;
}
