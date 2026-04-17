// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "ModHook.h"
#include "Subsystems/WorldSubsystem.h"
#include "ModHookSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class BRICKRIGS_API UModHookSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY(Transient)
	TArray<UModHook*> ModHooks;
	// ~Variables

	// ~Super Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void BeginDestroy() override;
	// ~Super Interface

	// Returns the mod hook subsystem instance
	static UModHookSubsystem* Get(const UObject* WorldContextObject);

	// Creates the mod hook instances if necessary
	void CreateModHooks();

public:
	// Called when the game instance is initialized
	void OnGameInstanceInit();

	// Returns the first mod hook of the given class
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = WorldContextObject, DeterminesOutputType = "ModHookClass"))
	static UModHook* GetModHookByClass(const UObject* WorldContextObject, const TSubclassOf<UModHook> ModHookClass);

	// Templated version
	template <typename T>
	static T* GetModHookByClass()
	{
		return Cast<T>(GetModHookByClass(T::StaticClass()));
	}

	// Executes a function on all mod hooks
	static void ForEachModHook(const UObject* WorldContextObject, const TFunction<void(UModHook*)>& Func);
};
