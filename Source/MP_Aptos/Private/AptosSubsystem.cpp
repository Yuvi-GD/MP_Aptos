// Fill out your copyright notice in the Description page of Project Settings.


#include "AptosSubsystem.h"
#include "AptosWrapper.h"
#include "AptosController.h"

void UAptosSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Ensure SDK is initialized (already called in FMP_AptosModule, but we confirm here)
    if (!AptosWrapper::InitSDK())
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosSubsystem::Initialize: Failed to initialize Aptos SDK"));
        return;
    }

    // Call OnInitApp to load mnemonic key
    //FString LoadedMnemonicKey;
    FString Mnemonic;
    UAptosController::OnInitApp(Mnemonic);
    MnemonicKey= Mnemonic;
    //void* Controller =AptosWrapper::GetController;
    //UAptosController::m_controller = Controller;
    //MnemonicKey = AptosWrapper::GetMnemonicsKey(Controller); // Store in subsystem for now
    //AptosWrapper::SetNetwork(Controller, "Devnet");
    UE_LOG(LogTemp, Log, TEXT("UAptosSubsystem::Initialize: Loaded mnemonic key: %s"), *MnemonicKey);
}

void UAptosSubsystem::Deinitialize()
{
    // Call OnQuitApp to save mnemonic key
    UAptosController::OnQuitApp();

    // Clean up the controller
    //void* Controller = AptosWrapper::GetController();
    //if (Controller)
    //{
    //    AptosWrapper::SaveMnemonicsKey(Controller);
    //    AptosWrapper::DeleteController(Controller);
    //    UE_LOG(LogTemp, Log, TEXT("UAptosSubsystem::Deinitialize: Controller cleaned up"));
    //}

    Super::Deinitialize();
}