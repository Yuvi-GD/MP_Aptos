// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define BOOST_NO_CXX98_FUNCTION_BASE
#ifdef __GNUC__ 
	#pragma GCC diagnostic ignored "-Wall"
	#pragma GCC diagnostic ignored "-Wdeprecated-builtins"
	#pragma GCC diagnostic ignored "-Wshadow"
	#pragma GCC diagnostic ignored "-Wenum-constexpr-conversion"
#endif
#ifdef check
#undef check
#endif
#ifdef verify
#undef verify
#endif

#ifdef __GNUC__ 
	#pragma GCC diagnostic pop
#endif

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <string>
#include "AptosController.generated.h"

/**
 *
 */
UCLASS()
class MP_APTOS_API  UAptosController : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CopyCurrentWalletAdress"), Category = "AptosController")
	static void CopyCurrentWalletAdress(FString text);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateWallet Clicked"), Category = "AptosController")
	static void OnCreateWalletClicked(class UWidget *TargetComboBox, FString netWork, FString &balance_return, FString &mnemonic_key_return, bool &IsCreateOk);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnImportWallet Clicked"), Category = "AptosController")
	static void OnImportWalletClicked(class UWidget *TargetComboBox, FString mnemonic_key_import, FString &balance_return, bool &IsImportOk);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WalletListDropdownValueChanged"), Category = "AptosController")
	static void OnWalletListDropdownValueChanged(int index, FString &balance_return);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCopyMnemonicWords Clicked"), Category = "AptosController")
	static void CopyMnemonicWords();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCopyPrivateKey Clicked"), Category = "AptosController")
	static void CopyPrivateKey();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnAirdrop Clicked"), Category = "AptosController")
	static void Airdrop(FString &balance_return, bool &IsAirdropOk, FString &Notification);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnLogout Clicked"), Category = "AptosController")
	static void Logout();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On SendTransaction Clicked"), Category = "AptosController")
	static void SendToken(FString targetAddress, float amount, bool &IsSendTokenOk, FString &Notification, FString &balance_return);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateCollection Clicked"), Category = "AptosController")
	static void CreateCollection(FString collectionName, FString collectionDescription, FString collectionUrl, bool &IsCreateCollectionOk, FString &Notification);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateNFT Clicked"), Category = "AptosController")
	static void CreateNFT(FString collectionName, FString tokenName, FString tokenDescription, int supply, int max, FString url, int royaltyPointsPerMillion, bool &IsCreateNFTOk, FString &Notification);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCurrentWalletBalance"), Category = "AptosController")
	static void GetCurrentWalletBalance(FString &Balance);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNetwork"), Category = "AptosController")
	static void SetNetwork(FString Network);

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnInitApp"), Category = "AptosController")
	static void OnInitApp(FString &mnemonic_key);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnQuitApp"), Category = "AptosController")
	static void OnQuitApp();

private:
	static void *m_controller;
};
