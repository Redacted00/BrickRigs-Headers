#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FUICommandList;

class FBrickRigsEditorModule : public FDefaultGameModuleImpl
{
	// ~Variables
	TSharedPtr<FUICommandList> PluginCommands;
	// ~Variables

	// ~Super Interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~Super Interface
};
