#pragma once

#include "UObject/WeakObjectPtr.h"
#include "CoreMinimal.h"

// A smart delegate handle automatically unbinds the delegate as it is destructed, also makes it easier and safer to manually unbind a delegate
struct FSmartDelegateHandle
{
	template <typename T, typename FDelegateType>
	FSmartDelegateHandle(T* InOwner, FDelegateType& Delegate, const typename FDelegateType::FDelegate& NewDelegate)
	{
		Bind<T>(InOwner, Delegate, NewDelegate);
	}

	~FSmartDelegateHandle()
	{
		Unbind();
	}

	bool IsBound() const
	{
		return Owner.IsValid();
	}

	auto* GetObject() const
	{
		return Owner.Get();
	}

	// Binds to a new delegate
	template <typename T, typename FDelegateType>
	void Bind(T* InOwner, FDelegateType& Delegate, const typename FDelegateType::FDelegate& NewDelegate)
	{
		Unbind();

		Owner = InOwner;
		const auto DelegateHandle = Delegate.Add(NewDelegate);
		UnbindFunction = [&Delegate, DelegateHandle]()
		{
			Delegate.Remove(DelegateHandle);
		};
	}

	// Version that binds a weak object lambda delegate
	template <class T, typename FDelegateType>
	void Bind(T* InOwner, FDelegateType& Delegate, UObject* Object, const typename FDelegateType::FDelegate::TFuncType& Lambda)
	{
		Bind(InOwner, Delegate, FDelegateType::FDelegate::CreateWeakLambda(Object, Lambda));
	}

	// Version that binds a member function delegate
	template <class T, class U, typename FDelegateType>
	void Bind(T* InOwner, FDelegateType& Delegate, U* Object, typename TMemFunPtrType<false, U, typename FDelegateType::FDelegate::TFuncType>::Type InFunc)
	{
		Bind(InOwner, Delegate, FDelegateType::FDelegate::CreateUObject(Object, InFunc));
	}

	void Unbind()
	{
		if (Owner.IsValid())
		{
			UnbindFunction();
		}
		Owner.Reset();
	}

private:
	FWeakObjectPtr Owner;
	TFunction<void()> UnbindFunction;
};

// A smart delegate pool contains an array of smart delegate handles and can be used to conveniently and safely unbind some or all delegates
struct FSmartDelegatePool
{
	~FSmartDelegatePool()
	{
		UnbindAll();
	}

	template <typename T, typename FDelegateType>
	void Add(T* InOwner, FDelegateType& Delegate, const typename FDelegateType::FDelegate& NewDelegate)
	{
		Handles.Emplace(InOwner, Delegate, NewDelegate);
	}

	// Version that binds a weak object lambda delegate
	template <class T, typename FDelegateType>
	void Add(T* InOwner, FDelegateType& Delegate, UObject* Object, const typename FDelegateType::FDelegate::TFuncType& Lambda)
	{
		Add(InOwner, Delegate, FDelegateType::FDelegate::CreateWeakLambda(Object, Lambda));
	}

	// Version that binds a member function delegate
	template <class T, class U, typename FDelegateType>
	void Add(T* InOwner, FDelegateType& Delegate, U* Object, typename TMemFunPtrType<false, U, typename FDelegateType::FDelegate::TFuncType>::Type InFunc)
	{
		Add(InOwner, Delegate, FDelegateType::FDelegate::CreateUObject(Object, InFunc));
	}

	void UnbindAll()
	{
		// NOTE: No need to iterate the handles and call Unbind, will be done upon their destruction
		Handles.Empty();
	}

	void UnbindObject(UObject* Object)
	{
		check(Object);
		// Remove handles in reverse order to reduce reordering remaining items
		for (auto i = Handles.Num() - 1; i >= 0; --i)
		{
			auto& Handle = Handles[i];
			if (Handle.GetObject() == Object)
			{
				// NOTE: No need to call Unbind, will be done upon destruction
				Handles.RemoveAtSwap(i, 1, false);
			}
		}
		Handles.Shrink();
	}

private:
	TArray<FSmartDelegateHandle> Handles;
};
