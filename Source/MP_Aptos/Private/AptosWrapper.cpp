#include "AptosWrapper.h"
#include "MP_Aptos.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>

void* AptosWrapper::ControllerInstance = nullptr;
FString AptosWrapper::MnemonicKey;

// Function pointer types for DLL exports
typedef void* (*CreateUiControllerFunc)();
typedef void (*DeleteUiControllerFunc)(void*);
typedef void (*SetNetworkFunc)(void*, const char*);
typedef bool (*CreateNewWalletFunc)(void*);
typedef bool (*InitWalletFromCacheFunc)(void*, const char*, int);
typedef bool (*RestoreWalletFunc)(void*, const char*);
typedef char* (*GetMnemonicsKeyFunc)(void*);
typedef char* (*GetPrivateKeyFunc)(void*);
typedef char* (*GetCurrentWalletBalanceTextFunc)(void*);
typedef void (*DeleteStringFunc)(char*);
typedef void (*DeleteStringArrayFunc)(char**, int);
typedef char** (*GetWalletAddressFunc)(void*, size_t*);
typedef char* (*OnWalletListDropdownValueChangedFunc)(void*, int);
typedef char* (*GetCurrentWalletAddressFunc)(void*);
typedef void (*AirdropFunc)(void*, int);
typedef bool (*SendTokenFunc)(void*, const char*, long);
typedef bool (*CreateCollectionFunc)(void*, const char*, const char*, const char*);
typedef bool (*CreateNTFFunc)(void*, const char*, const char*, const char*, int, int, const char*, int);

// Static function pointers with unique names
static CreateUiControllerFunc CreateUiControllerFuncPtr = nullptr;
static DeleteUiControllerFunc DeleteUiControllerFuncPtr = nullptr;
static SetNetworkFunc SetNetworkFuncPtr = nullptr;
static CreateNewWalletFunc CreateNewWalletFuncPtr = nullptr;
static InitWalletFromCacheFunc InitWalletFromCacheFuncPtr = nullptr;
static RestoreWalletFunc RestoreWalletFuncPtr = nullptr;
static GetMnemonicsKeyFunc GetMnemonicsKeyFuncPtr = nullptr;
static GetPrivateKeyFunc GetPrivateKeyFuncPtr = nullptr;
static GetCurrentWalletBalanceTextFunc GetCurrentWalletBalanceTextFuncPtr = nullptr;
static DeleteStringFunc DeleteStringFuncPtr = nullptr;
static DeleteStringArrayFunc DeleteStringArrayFuncPtr = nullptr;
static GetWalletAddressFunc GetWalletAddressFuncPtr = nullptr;
static OnWalletListDropdownValueChangedFunc OnWalletListDropdownValueChangedFuncPtr = nullptr;
static GetCurrentWalletAddressFunc GetCurrentWalletAddressFuncPtr = nullptr;
static AirdropFunc AirdropFuncPtr = nullptr;
static SendTokenFunc SendTokenFuncPtr = nullptr;
static CreateCollectionFunc CreateCollectionFuncPtr = nullptr;
static CreateNTFFunc CreateNTFFuncPtr = nullptr;

bool AptosWrapper::InitSDK()
{
    // Get the DLL handle from FMP_AptosModule
    void* AptosUILogicLibraryHandle = FMP_AptosModule::Get().GetAptosUILogicLibraryHandle();
    if (!AptosUILogicLibraryHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos AptosUILogic.dll not loaded by module"));
        return false;
    }
    UE_LOG(LogTemp, Log, TEXT("MP_Aptos Using AptosUILogic.dll handle at %p"), AptosUILogicLibraryHandle);

    // Inline DLL function pointer initialization with detailed logging
    CreateUiControllerFuncPtr = (CreateUiControllerFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_createUiController"));
    if (!CreateUiControllerFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_createUiController"));

    DeleteUiControllerFuncPtr = (DeleteUiControllerFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_deleteUiController"));
    if (!DeleteUiControllerFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_deleteUiController"));

    SetNetworkFuncPtr = (SetNetworkFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_setNetwork"));
    if (!SetNetworkFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_setNetwork"));

    CreateNewWalletFuncPtr = (CreateNewWalletFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_createNewWallet"));
    if (!CreateNewWalletFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_createNewWallet"));

    GetMnemonicsKeyFuncPtr = (GetMnemonicsKeyFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_getMnemonicsKey"));
    if (!GetMnemonicsKeyFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_getMnemonicsKey"));

    GetCurrentWalletBalanceTextFuncPtr = (GetCurrentWalletBalanceTextFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_getCurrentWalletBalanceText"));
    if (!GetCurrentWalletBalanceTextFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_getCurrentWalletBalanceText"));

    GetCurrentWalletAddressFuncPtr = (GetCurrentWalletAddressFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_getCurrentWalletAddress"));
    if (!GetCurrentWalletAddressFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_getCurrentWalletAddress"));

    DeleteStringFuncPtr = (DeleteStringFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_deleteString"));
    if (!DeleteStringFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_deleteString"));

    GetWalletAddressFuncPtr = (GetWalletAddressFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_getWalletAddress"));
    if (!GetWalletAddressFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_getWalletAddress"));

    OnWalletListDropdownValueChangedFuncPtr = (OnWalletListDropdownValueChangedFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_onWalletListDropdownValueChanged"));
    if (!OnWalletListDropdownValueChangedFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_onWalletListDropdownValueChanged"));

    GetPrivateKeyFuncPtr = (GetPrivateKeyFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_getPrivateKey"));
    if (!GetPrivateKeyFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_getPrivateKey"));

    AirdropFuncPtr = (AirdropFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_airdrop"));
    if (!AirdropFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_airdrop"));

    SendTokenFuncPtr = (SendTokenFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_sendToken"));
    if (!SendTokenFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_sendToken"));

    CreateCollectionFuncPtr = (CreateCollectionFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_createCollection"));
    if (!CreateCollectionFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_createCollection"));

    CreateNTFFuncPtr = (CreateNTFFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_createNFT"));
    if (!CreateNTFFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_createNFT"));

    RestoreWalletFuncPtr = (RestoreWalletFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_restoreWallet"));
    if (!RestoreWalletFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_restoreWallet"));

    DeleteStringArrayFuncPtr = (DeleteStringArrayFunc)FPlatformProcess::GetDllExport(AptosUILogicLibraryHandle, TEXT("AptosUILogic_deleteStringArray"));
    if (!DeleteStringArrayFuncPtr) UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load AptosUILogic_deleteStringArray"));

    if (!CreateUiControllerFuncPtr || !DeleteUiControllerFuncPtr || !SetNetworkFuncPtr || !CreateNewWalletFuncPtr ||
        !GetMnemonicsKeyFuncPtr || !GetCurrentWalletBalanceTextFuncPtr || !GetCurrentWalletAddressFuncPtr ||
        !DeleteStringFuncPtr || !GetWalletAddressFuncPtr || !OnWalletListDropdownValueChangedFuncPtr ||
        !GetPrivateKeyFuncPtr || !AirdropFuncPtr || !SendTokenFuncPtr || !CreateCollectionFuncPtr ||
        !CreateNTFFuncPtr || !RestoreWalletFuncPtr || !DeleteStringArrayFuncPtr /* || !InitWalletFromCacheFuncPtr */ )
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to load one or more DLL exports"));
        FPlatformProcess::FreeDllHandle(AptosUILogicLibraryHandle);
        AptosUILogicLibraryHandle = nullptr;
        return false;
    }

    ControllerInstance = CreateUiControllerFuncPtr();
    if (!ControllerInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos Failed to create UIController"));
        FPlatformProcess::FreeDllHandle(AptosUILogicLibraryHandle);
        AptosUILogicLibraryHandle = nullptr;
        return false;
    }



    SetNetworkFuncPtr(ControllerInstance, "https://fullnode.devnet.aptoslabs.com/v1");
    UE_LOG(LogTemp, Log, TEXT("MP_Aptos Aptos SDK Initialized"));
    return true;
}

void* AptosWrapper::GetController()
{
    return ControllerInstance;
}

void* AptosWrapper::CreateController()
{
    UE_LOG(LogTemp, Log, TEXT("MP_Aptos CreateController called"));
    if (!CreateUiControllerFuncPtr)
        {
            UE_LOG(LogTemp, Error, TEXT("MP_Aptos CreateController failed: CreateUiControllerFuncPtr is null"));
            return nullptr;
        }
    void* controller = CreateUiControllerFuncPtr();
    UE_LOG(LogTemp, Log, TEXT("MP_Aptos CreateController returned controller: %p"), controller);
    return controller;
}

void AptosWrapper::DeleteController(void* controller)
{
    if (DeleteUiControllerFuncPtr) DeleteUiControllerFuncPtr(controller);
}

void AptosWrapper::SetNetwork(void* controller, const char* target)
{
    if (SetNetworkFuncPtr) SetNetworkFuncPtr(controller, target);
}

bool AptosWrapper::CreateNewWallet(void* controller)
{
    if (!controller)
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos CreateNewWallet failed: Controller is null"));
        return false;
    }

    if (!CreateNewWalletFuncPtr)
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos CreateNewWallet failed: Function pointer is null"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("MP_Aptos Calling CreateNewWallet with controller: %p"), controller);
    bool result = CreateNewWalletFuncPtr(controller);
    if (!result)
    {
        UE_LOG(LogTemp, Error, TEXT("MP_Aptos CreateNewWallet failed: DLL returned false"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("MP_Aptos CreateNewWallet succeeded"));
    }
    return result;
}

bool AptosWrapper::RestoreWallet(void* controller, const char* mnemonicsKey)
{
    if (!RestoreWalletFuncPtr)
    {
        UE_LOG(LogTemp, Error, TEXT("AptosWrapper::RestoreWallet: RestoreWalletFuncPtr is null"));
        return false;
    }
    if (RestoreWalletFuncPtr(controller, mnemonicsKey))
    {
        UE_LOG(LogTemp, Error, TEXT("AptosWrapper::RestoreWallet: is working"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AptosWrapper::RestoreWallet: is not working"));
        return false;
    }
}

char* AptosWrapper::GetMnemonicsKey(void* controller)
{
     //If we already have a cached mnemonic key, return it
    //if (!MnemonicKey.IsEmpty())
    //{
    //    return TCHAR_TO_UTF8(*MnemonicKey);
    //}
    //// Try to load from file first
    //FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
    //FString LoadedData;
    //if (FFileHelper::LoadFileToString(LoadedData, *SavePath))
    //{
    //    MnemonicKey = LoadedData;
    //    UE_LOG(LogTemp, Log, TEXT("AptosWrapper::GetMnemonicsKey: Loaded mnemonic key from file: %s"), *MnemonicKey);
    //    return TCHAR_TO_UTF8(*MnemonicKey);
    //}

     //If not found in file, get from SDK
    if (!GetMnemonicsKeyFuncPtr)
    {
        UE_LOG(LogTemp, Error, TEXT("AptosWrapper::GetMnemonicsKey: GetMnemonicsKeyFuncPtr is null"));
        return nullptr;
    }

    char* Mnemonic = GetMnemonicsKeyFuncPtr(controller);
    if (Mnemonic)
    {
        MnemonicKey = UTF8_TO_TCHAR(Mnemonic);
        UE_LOG(LogTemp, Log, TEXT("AptosWrapper::GetMnemonicsKey: Fetched mnemonic key from SDK: %s"), *MnemonicKey);
    }
    return Mnemonic;

    //return GetMnemonicsKeyFuncPtr ? GetMnemonicsKeyFuncPtr(controller) : nullptr;
}

void AptosWrapper::SaveMnemonicsKey(void* controller)
{
    char* mnemonic = GetMnemonicsKey(controller);
    FString SaveData = mnemonic;

    DeleteString(mnemonic);
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
    FFileHelper::SaveStringToFile(SaveData, *SavePath);
}

char* AptosWrapper::GetPrivateKey(void* controller)
{
    return GetPrivateKeyFuncPtr ? GetPrivateKeyFuncPtr(controller) : nullptr;
}

char* AptosWrapper::GetCurrentWalletBalanceText(void* controller)
{
    return GetCurrentWalletBalanceTextFuncPtr ? GetCurrentWalletBalanceTextFuncPtr(controller) : nullptr;
}

void AptosWrapper::DeleteString(char* str)
{
    if (DeleteStringFuncPtr) DeleteStringFuncPtr(str);
    else if (str) delete[] str;
}

void AptosWrapper::DeleteStringArray(char** strArr, int size)
{
    if (DeleteStringArrayFuncPtr) DeleteStringArrayFuncPtr(strArr, size);
    else {
        for (int i = 0; i < size; ++i) delete[] strArr[i];
        delete[] strArr;
    }
}

char** AptosWrapper::GetWalletAddress(void* controller, size_t* size)
{
    return GetWalletAddressFuncPtr ? GetWalletAddressFuncPtr(controller, size) : nullptr;
}

char* AptosWrapper::OnWalletListDropdownValueChanged(void* controller, int listIndex)
{
    return OnWalletListDropdownValueChangedFuncPtr ? OnWalletListDropdownValueChangedFuncPtr(controller, listIndex) : nullptr;
}

char* AptosWrapper::GetCurrentWalletAddress(void* controller)
{
    return GetCurrentWalletAddressFuncPtr ? GetCurrentWalletAddressFuncPtr(controller) : nullptr;
}

void AptosWrapper::Airdrop(void* controller, int amount)
{
    if (AirdropFuncPtr) AirdropFuncPtr(controller, amount);
}

bool AptosWrapper::SendToken(void* controller, const char* _targetAddress, long _amount)
{
    return SendTokenFuncPtr ? SendTokenFuncPtr(controller, _targetAddress, _amount) : false;
}

bool AptosWrapper::CreateCollection(void* controller, const char* _collectionName, const char* _collectionDescription, const char* _collectionUri)
{
    return CreateCollectionFuncPtr ? CreateCollectionFuncPtr(controller, _collectionName, _collectionDescription, _collectionUri) : false;
}

bool AptosWrapper::CreateNFT(void* controller, const char* _collectionName, const char* _tokenName, const char* _tokenDescription, int _supply, int _max, const char* _uri, int _royaltyPointsPerMillion)
{
    return CreateNTFFuncPtr ? CreateNTFFuncPtr(controller, _collectionName, _tokenName, _tokenDescription, _supply, _max, _uri, _royaltyPointsPerMillion) : false;
}