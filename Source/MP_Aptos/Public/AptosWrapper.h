#pragma once

#include "CoreMinimal.h"

class MP_APTOS_API AptosWrapper
{
public:
    // Static methods to interact with the SDK
    static bool InitSDK();
    static void* GetController();
    static void* CreateController();
    static void DeleteController(void* controller);

    static char* GetMnemonicsKey(void* controller);
    static void SaveMnemonicsKey(void* controller);
    static void SetNetwork(void* controller, const char* target);
    static bool CreateNewWallet(void* controller);
    static bool RestoreWallet(void* controller, const char* mnemonicsKey);
    static char* GetCurrentWalletAddress(void* controller);
    static char** GetWalletAddress(void* controller, size_t* size);
    static char* GetCurrentWalletBalanceText(void* controller);
    static char* OnWalletListDropdownValueChanged(void* controller, int listIndex);
    static void DeleteString(char* str);
    static void DeleteStringArray(char** strArr, int size);
    static char* GetPrivateKey(void* controller);
    static void Airdrop(void* controller, int amount);
    static bool SendToken(void* controller, const char* _targetAddress, long _amount);
    static bool CreateCollection(void* controller, const char* _collectionName, const char* _collectionDescription, const char* _collectionUri);
    static bool CreateNFT(void* controller, const char* _collectionName, const char* _tokenName, const char* _tokenDescription, int _supply, int _max, const char* _uri, int _royaltyPointsPerMillion);

    /*static char* GetMnemonicsKey(); 
    static void SaveMnemonicsKey(const FString& MnemonicKey);
    static void SetNetwork(const char* Network); 
    static bool CreateNewWallet(); 
    static bool RestoreWallet(const char* MnemonicKey);
    static char* GetCurrentWalletAddress();
    static char** GetWalletAddress(size_t& Size); 
    static char* GetCurrentWalletBalanceText(); 
    static char* OnWalletListDropdownValueChanged(int Index); 
    static void DeleteStringArray(char** StringArray, size_t Size);
    static void DeleteString(char* String);
    static char* GetPrivateKey(); 
    static void Airdrop(int Amount); 
    static bool SendToken(const char* TargetAddress, long Amount); 
    static bool CreateCollection(const char* CollectionName, const char* CollectionDescription, const char* CollectionUrl); 
    static bool CreateNFT(const char* CollectionName, const char* TokenName, const char* TokenDescription, int Supply, int Max, const char* Url, int RoyaltyPointsPerMillion); */

private:
    static void* ControllerInstance;
    static FString MnemonicKey;
};
