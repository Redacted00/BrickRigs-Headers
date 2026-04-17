// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BrickObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickObjectPool : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject", DisplayName = "Get Object Pool"))
	static UBrickObjectPool* Get(const UObject* WorldContextObject);

private:
	UPROPERTY(Transient)
	TArray<UUserWidget*> WidgetPool;

public:
	// Acquires a widget from the pool or creates a new one
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "WidgetClass"))
	static UUserWidget* GetWidget(const UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass);

	// Templated version
	template <typename T>
	static T* GetWidget(const UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass)
	{
		return CastChecked<T>(GetWidget(WorldContextObject, WidgetClass));
	}

	// Removes a single widget from its parent and adds it back to the pool
	UFUNCTION(BlueprintCallable)
	static void PoolWidget(UUserWidget* Widget);

	// Removes the given widgets from their parent and adds them back to the pool
	template <typename T>
	static void PoolWidgets(TArray<T*>& Widgets, int32 NumToKeep = 0)
	{
		for (auto i = NumToKeep; i < Widgets.Num(); ++i)
		{
			PoolWidget(Widgets[i]);
		}
		const auto NumToRemove = Widgets.Num() - NumToKeep;
		if (NumToRemove > 0)
		{
			Widgets.RemoveAt(NumToKeep, NumToRemove, false);
		}
	}

	// Recycles a widget from a local pool if possible, otherwise gathers one from the global pool
	template <typename T>
	static T* RecycleWidget(const UObject* WorldContextObject, TArray<T*>& Widgets, UClass* WidgetClass, int32 Index, const TFunction<void(T*, bool)>& Initializer)
	{
		if (Index < Widgets.Num())
		{
			if (Widgets[Index])
			{
				Initializer(Widgets[Index], true);
			}
			return Widgets[Index];
		}

		if (auto* Widget = GetWidget<T>(WorldContextObject, WidgetClass))
		{
			Widgets.Add(Widget);
			Initializer(Widget, false);
			return Widget;
		}

		ensure(false);
		return nullptr;
	}

	// Recycles or creates the desired number of widgets
	template <typename T>
	static void RecycleWidgets(const UObject* WorldContextObject, TArray<T*>& Widgets, UClass* WidgetClass, int32 NumWidgets, const TFunction<void(T*, int32, bool)>& Initializer)
	{
		Widgets.Reserve(NumWidgets);

		for (auto i = 0; i < NumWidgets; ++i)
		{
			RecycleWidget<T>(WorldContextObject, Widgets, WidgetClass, i, [&](T* Widget, bool bRecycled)
			{
				Initializer(Widget, i, bRecycled);
			});
		}

		// Remove newly unused widgets
		PoolWidgets(Widgets, NumWidgets);
	}
};
