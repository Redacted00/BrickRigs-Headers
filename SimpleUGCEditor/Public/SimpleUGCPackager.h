// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IPlugin;

class FSimpleUGCPackager : public TSharedFromThis<FSimpleUGCPackager>
{
public:
	void OpenPluginPackager(TSharedRef<IPlugin> Plugin);

	void PackagePlugin(const TSharedRef<IPlugin>& Plugin);

	/** Generates submenu content for the plugin packager command */
	void GeneratePackagerMenuContent(FMenuBuilder& MenuBuilder);

	/** Generates the menu content for the plugin packager toolbar button */
	TSharedRef<SWidget> GeneratePackagerComboButtonContent();

private:
	/** Gets all available game mod plugin packages  */
	void FindAvailableGameMods(TArray<TSharedRef<IPlugin>>& OutAvailableGameMods);

	/** Gets all available game mod plugins and registers command info for them */
	void GetAvailableUGCCommands(const TArray<TSharedRef<IPlugin>>& AvailableUGC);

	/** Generates menu content for the supplied set of commands */
	void GeneratePackagerMenuContent_Internal(FMenuBuilder& MenuBuilder, const TArray<TSharedPtr<FUICommandInfo>>& Commands);

	/**
	* Checks if a plugin has any unsaved content
	*
	* @param	Plugin			The plugin to check for unsaved content
	* @return	True if all mod content has been saved, false otherwise
	*/
	bool IsAllContentSaved(const TSharedRef<IPlugin>& Plugin);

	TArray<TSharedPtr<FUICommandInfo>> UGCCommands;
};
