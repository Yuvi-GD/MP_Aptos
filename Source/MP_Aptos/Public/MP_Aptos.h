// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FMP_AptosModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	// Getter for DLL handles
	static FMP_AptosModule& Get();
	void* GetAptosLibraryHandle() const { return AptosLibraryHandle; }
	void* GetAptosUILogicLibraryHandle() const { return AptosUILogicLibraryHandle; }

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;

	/** Handles to the DLLs we will load */
	void* AptosLibraryHandle;
	void* AptosUILogicLibraryHandle;
};
