#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FTextBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FTextBrickProperty);

private:
	// ~Variables
	const int32 MaxTextLength;
	const bool bIsPassword;
	const bool bAllowMultiLine;
	const bool bIsUserText;
	// ~Variables

public:
	// ~Constructor
	FTextBrickProperty(int32 InMaxTextLength = 0, bool bInIsPassword = false, bool bInAllowMultiLine = false, bool bInIsUserText = false)
		: MaxTextLength(InMaxTextLength), bIsPassword(bInIsPassword), bAllowMultiLine(bInAllowMultiLine), bIsUserText(bInIsUserText)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		// NOTE: Can't use the == operator since it ignores case
		const auto& StrA = *static_cast<const FString*>(A);
		const auto& StrB = *static_cast<const FString*>(B);
		return StrA.Equals(StrB);
	}

	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override
	{
		auto Value = FString();
		if (GetValue(Container, Value))
		{
			OutValue = FText::AsCultureInvariant(Value);
			return true;
		}

		return false;
	}

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return SetValue(Container, NewValue.ToString());
	}

	virtual bool IsUserText() const override
	{
		return bIsUserText;
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = FString();
		GetValue(Container, Value);
		return Value;
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override;

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FString& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, const FString& NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}

	int32 GetMaxTextLength() const
	{
		return MaxTextLength;
	}

	bool GetAllowMultiLine() const
	{
		return bAllowMultiLine;
	}

	bool IsPassword() const
	{
		return bIsPassword;
	}
};

DECLARE_BRICK_PROP_TYPE(FTextBrickProperty, FString);
