#pragma once

#include "CoreMinimal.h"

class ABrickEditor;

enum class EScopedOperationFlags : uint8
{
	Selection,
	Visibility,
	AddObjects,
	RemoveObjects,
	MoveObjects,
	EditProperties,
};

struct FBrickEditorScopedOperation
{
	// ~Constructor
	FBrickEditorScopedOperation(ABrickEditor* InEditor);
	// ~Destructor
	~FBrickEditorScopedOperation();

	void Modify(EScopedOperationFlags InFlag)
	{
		RootScope->Flags |= 1 << static_cast<int32>(InFlag);
	}

	bool IsModified(EScopedOperationFlags InFlag) const
	{
		return (RootScope->Flags & (1 << static_cast<int32>(InFlag))) != 0;
	}

	bool IsAnythingModified() const
	{
		return RootScope->Flags != 0;
	}

	void SetNoUndo()
	{
		RootScope->bNoUndo = true;
	}

	bool IsNoUndo() const
	{
		return RootScope->bNoUndo;
	}

	void DontMarkDirty()
	{
		RootScope->bDontMarkDirty = true;
	}

	bool ShouldMarkDirty() const
	{
		return !RootScope->bDontMarkDirty;
	}

private:
	// The editor we are associated with
	ABrickEditor* Editor;
	// The root scoped operation, can be this
	FBrickEditorScopedOperation* RootScope;
	// Flags indicate what has been modified
	uint32 Flags;
	// Used to exclude the operation from the undo buffer
	uint8 bNoUndo : 1;
	// Can be used to disable the item from being marked dirty, for example after opening
	uint8 bDontMarkDirty : 1;
};
