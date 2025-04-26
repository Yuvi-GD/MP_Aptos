// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_Aptos.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "AptosWrapper.h"

#define LOCTEXT_NAMESPACE "FMP_AptosModule"

void FMP_AptosModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("MP_Aptos")->GetBaseDir();

	// Add the relative location of the third party DLLs and load them
	FString AptosLibraryPath;
	FString AptosUILogicLibraryPath;

#if PLATFORM_WINDOWS
	AptosLibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AptosSDK/Binaries/Aptos.dll"));
	AptosUILogicLibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AptosSDK/Binaries/AptosUILogic.dll"));
#endif // PLATFORM_WINDOWS

	// Load Aptos.dll
	AptosLibraryHandle = FPlatformProcess::GetDllHandle(*AptosLibraryPath);
	if (AptosLibraryHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("MP_Aptos Successfully loaded Aptos.dll"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MP_Aptos AptosLibraryError", "Failed to load Aptos.dll"));
	}

	// Load AptosUILogic.dll
	AptosUILogicLibraryHandle = FPlatformProcess::GetDllHandle(*AptosUILogicLibraryPath);
	if (AptosUILogicLibraryHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("MP_Aptos Successfully loaded AptosUILogic.dll"));
		
		// Initialize SDK immediately after loading DLLs
		if (AptosWrapper::InitSDK())
		{
			UE_LOG(LogTemp, Log, TEXT("MP_Aptos Initialize SDK functions is working from module class"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to initialize SDK functions"));
		}
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MP_Aptos AptosUILogicLibraryError", "Failed to load AptosUILogic.dll"));
	}

	/*
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/MP_AptosLibrary/Win64/ExampleLibrary.dll"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
		ExampleLibraryFunction();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}*/
}

void FMP_AptosModule::ShutdownModule()
{
	/*
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;*/

	// Free the DLL handles
	FPlatformProcess::FreeDllHandle(AptosLibraryHandle);
	AptosLibraryHandle = nullptr;

	FPlatformProcess::FreeDllHandle(AptosUILogicLibraryHandle);
	AptosUILogicLibraryHandle = nullptr;
}

FMP_AptosModule& FMP_AptosModule::Get()
{
	return *FModuleManager::LoadModulePtr<FMP_AptosModule>("MP_Aptos");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMP_AptosModule, MP_Aptos)
//IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, AptosUI, "AptosUI");