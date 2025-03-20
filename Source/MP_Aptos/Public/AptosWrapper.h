#pragma once

#include "CoreMinimal.h"

class MP_APTOS_API AptosWrapper
{
public:
    // Static methods to interact with the SDK
    static bool InitSDK();
    static void* GetController();

    // New functions to match uicontrollerlogic.h
    static void* CreateController();
    static void DeleteController(void* controller);
    static void SetNetwork(void* controller, const char* target);
    static bool CreateNewWallet(void* controller);
    //static bool InitWalletFromCache(void* controller, const char* mnemonicsKey, int currentAddressIndexKey);
    static bool RestoreWallet(void* controller, const char* mnemonicsKey);
    static char* GetMnemonicsKey(void* controller);
    static char* GetPrivateKey(void* controller);
    static char* GetCurrentWalletBalanceText(void* controller);
    static void DeleteString(char* str);
    static void DeleteStringArray(char** strArr, int size);
    static char** GetWalletAddress(void* controller, size_t* size);
    static char* OnWalletListDropdownValueChanged(void* controller, int listIndex);
    static char* GetCurrentWalletAddress(void* controller);
    static void Airdrop(void* controller, int amount);
    static bool SendToken(void* controller, const char* _targetAddress, long _amount);
    static bool CreateCollection(void* controller, const char* _collectionName, const char* _collectionDescription, const char* _collectionUri);
    static bool CreateNFT(void* controller, const char* _collectionName, const char* _tokenName, const char* _tokenDescription, int _supply, int _max, const char* _uri, int _royaltyPointsPerMillion);

private:
    //static void* AptosLibraryHandle;
    //static void* AptosUILogicLibraryHandle;
    static void* ControllerInstance;
};
