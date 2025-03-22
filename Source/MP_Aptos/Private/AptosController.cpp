// Fill out your copyright notice in the Description page of Project Settings.

#include "AptosController.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Components/ComboBoxString.h"
#include "AptosWrapper.h"
#include <iostream>


void* UAptosController::m_controller = nullptr;
int curindex = 0;

void UAptosController::CopyCurrentWalletAdress(FString text)
{
    FPlatformApplicationMisc::ClipboardCopy(*text);
}
void UAptosController::OnCreateWalletClicked(class UWidget *TargetComboBox, FString netWork, FString &balance_return, FString &mnemonic_key_return, bool &IsCreateOk)
{
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnCreateWalletClicked"));

    if (!m_controller)
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnCreateWalletClicked: m_controller is null"));
        IsCreateOk = false;
        return;
    }

    std::cerr << "UAptosController::OnCreateWalletClicked" << std::endl;
    // Convert FString to const char*
    std::string MyStdString(TCHAR_TO_UTF8(*netWork));
    const char *MyNetWork = MyStdString.c_str();

    AptosWrapper::SetNetwork(m_controller, MyNetWork);

    if(AptosWrapper::CreateNewWallet(m_controller))
    {
        char *mnemonic_key = AptosWrapper::GetMnemonicsKey(m_controller);
        mnemonic_key_return = mnemonic_key;
        std::cout << mnemonic_key << std::endl;

        //char *balance = AptosUILogic_getCurrentWalletBalanceText(m_controller);
        char *balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
        balance_return = balance;
        std::cout << balance << std::endl;

        size_t size = 0;
        //char **addressList = AptosUILogic_getWalletAddress(m_controller, &size);
        char **addressList = AptosWrapper::GetWalletAddress(m_controller, &size);
        UComboBoxString *MyCombobox = Cast<UComboBoxString>(TargetComboBox);
        if (MyCombobox)
        {
            // Clear existing options
            MyCombobox->ClearOptions();
            for (std::size_t i = 0; i < size; ++i)
            {
                MyCombobox->AddOption(FString(addressList[i]));
                std::cout << addressList[i] << std::endl;
            }
            MyCombobox->SetSelectedIndex(0);
        }
        else
        {
            IsCreateOk = false;
        }
        // std::cout << "Address list:" << std::endl;
        AptosWrapper::DeleteStringArray(addressList, size);
        AptosWrapper::DeleteString(mnemonic_key);
        AptosWrapper::DeleteString(balance);
        IsCreateOk = true;
    }
    else
    {
        IsCreateOk = false;
    }
}

void UAptosController::OnImportWalletClicked(class UWidget *TargetComboBox, FString mnemonic_key_import, FString &balance_return, bool &IsImportOk)
{

    // Validate the mnemonic key format (should be 12 or 24 words)
    TArray<FString> Words;
    mnemonic_key_import.ParseIntoArray(Words, TEXT(" "), true);
    int32 WordCount = Words.Num();
    if (WordCount != 12 && WordCount != 24)
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnImportWalletClicked: Invalid mnemonic key - must be 12 or 24 words, got %d words"), WordCount);
        IsImportOk = false;
        return;
    }

    // Ensure the controller is valid
    if (!m_controller)
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnImportWalletClicked: Controller is null"));
        IsImportOk = false;
        return;
    }

    //// Get the selected network from the UI (assuming the dropdown is set elsewhere)
    //FString SelectedNetwork = "Devnet"; // Replace with actual network from UI dropdown
    //std::string NetworkStdString(TCHAR_TO_UTF8(*SelectedNetwork));
    //const char* Network = NetworkStdString.c_str();
    //AptosWrapper::SetNetwork(m_controller, Network);

    std::string MyStdString(TCHAR_TO_UTF8(*mnemonic_key_import));
    const char *mnemonic_key = MyStdString.c_str();
    if (AptosWrapper::RestoreWallet(m_controller, mnemonic_key))
    {
        char* balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
        balance_return = balance;
        std::cout << balance << std::endl;

        size_t size = 0;
        char** addressList = AptosWrapper::GetWalletAddress(m_controller, &size);
        UComboBoxString *MyCombobox = Cast<UComboBoxString>(TargetComboBox);
        if (MyCombobox)
        {
            // Clear existing options
            MyCombobox->ClearOptions();
            for (std::size_t i = 0; i < size; ++i)
            {
                MyCombobox->AddOption(FString(addressList[i]));
                std::cout << addressList[i] << std::endl;
            }
            MyCombobox->SetSelectedIndex(0);
        }
        else
        {
            IsImportOk = false;
        }
        // std::cout << "Address list:" << std::endl;
        AptosWrapper::DeleteStringArray(addressList, size);
        AptosWrapper::DeleteString(balance);
        IsImportOk = true;

        // Update the cached mnemonic key in AptosWrapper
        AptosWrapper::SaveMnemonicsKey(m_controller);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnImportWalletClicked: Failed to restore wallet with mnemonic: %s"), *mnemonic_key_import);
        IsImportOk = false;
    }
}

void UAptosController::OnWalletListDropdownValueChanged(int index, FString &balance_return)
{
    curindex = index;
    char *curAddress = AptosWrapper::OnWalletListDropdownValueChanged(m_controller, index);
    char *balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    AptosWrapper::DeleteString(curAddress);
    AptosWrapper::DeleteString(balance);
}

void UAptosController::CopyMnemonicWords()
{
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnCopyMnemonicWordsClicked"));
    char* mnemonic = AptosWrapper::GetMnemonicsKey(m_controller);
    FString mnemonicUI = mnemonic;
    // TODO copy to clipboard
    FPlatformApplicationMisc::ClipboardCopy(*mnemonicUI);
    AptosWrapper::DeleteString(mnemonic);
}

void UAptosController::CopyPrivateKey()
{
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnCopyPrivateKeyClicked"));
    // char *privateKey = AptosUILogic_onWalletListDropdownValueChanged(m_controller, curindex);
    char *privateKey = AptosWrapper::GetPrivateKey(m_controller);
    FString privateKeyUI = privateKey;
    // TODO copy to clipboard
    FPlatformApplicationMisc::ClipboardCopy(*privateKeyUI);
    AptosWrapper::DeleteString(privateKey);
}

void UAptosController::Airdrop(FString &balance_return, bool &IsAirdropOk, FString &Notification)
{
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnAirdropClicked"));
    int _amount = 100000000;
    AptosWrapper::Airdrop(m_controller, 100000000);
    char *balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    Notification = FString::Printf(TEXT("Successfully Get Airdrop of %d APT"), 1);
    IsAirdropOk = true;

    AptosWrapper::DeleteString(balance);
}

void UAptosController::Logout()
{
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnLogoutClicked"));
    AptosWrapper::DeleteController(m_controller);
    m_controller = AptosWrapper::CreateController();
}

void UAptosController::SendToken(FString targetAddress, float amount, bool &IsSendTokenOk, FString &Notification, FString &balance_return)
{
    std::string _targetAddress(TCHAR_TO_UTF8(*targetAddress));
    long _amount = (long)(amount * 100000000);
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnSendTokenClicked"));
    if (targetAddress.IsEmpty())
    {
        // notification
        IsSendTokenOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {
        bool success = AptosWrapper::SendToken(m_controller, _targetAddress.c_str(), _amount);
        if (success)
        {
            // notification
            IsSendTokenOk = true;
            Notification = FString::Printf(TEXT("Successfully send %f APT to %s...%s "), amount, *targetAddress.Left(10), *targetAddress.Right(10));
        }
        else
        {
            // notification
            IsSendTokenOk = false;
            Notification = FString::Printf(TEXT("Send Token Transaction Failed"));
        }
    }
    char *balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    AptosWrapper::DeleteString(balance);
}

void UAptosController::CreateCollection(FString collectionName, FString collectionDescription, FString collectionUrl, bool &IsCreateCollectionOk, FString &Notification)
{
    std::string _collectionName(TCHAR_TO_UTF8(*collectionName));
    std::string _collectionDescription(TCHAR_TO_UTF8(*collectionDescription));
    std::string _collectionUrl(TCHAR_TO_UTF8(*collectionUrl));
    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnCreateCollectionClicked"));
    if (collectionName.IsEmpty() || collectionDescription.IsEmpty() || collectionUrl.IsEmpty())
    {
        // notification
        IsCreateCollectionOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {

        bool success = AptosWrapper::CreateCollection(m_controller, _collectionName.c_str(), _collectionDescription.c_str(), _collectionUrl.c_str());
        if (success)
        {
            // notification
            IsCreateCollectionOk = true;
            Notification = FString::Printf(TEXT("Successfully Create Collection: %s "), *collectionName);
        }
        else
        {
            // notification
            IsCreateCollectionOk = false;
            Notification = FString::Printf(TEXT("Failed to Create Collection: %s "), *collectionName);
        }
    }
}

void UAptosController::CreateNFT(FString collectionName, FString tokenName, FString tokenDescription, int supply, int max, FString url, int royaltyPointsPerMillion, bool &IsCreateNFTOk, FString &Notification)
{

    UE_LOG(LogTemp, Warning, TEXT("UAptosController::OnCreateNFTClicked"));
    std::string _collectionName(TCHAR_TO_UTF8(*collectionName));
    std::string _tokenName(TCHAR_TO_UTF8(*tokenName));
    std::string _tokenDescription(TCHAR_TO_UTF8(*tokenDescription));
    int _supply = supply;
    int _max = max;
    std::string _url(TCHAR_TO_UTF8(*url));
    int _royaltyPointsPerMillion = royaltyPointsPerMillion;
    if (collectionName.IsEmpty() || tokenName.IsEmpty() || tokenDescription.IsEmpty())
    {
        // notification
        IsCreateNFTOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {
        bool success = AptosWrapper::CreateNFT(m_controller, _collectionName.c_str(), _tokenName.c_str(), _tokenDescription.c_str(), _supply, _max, _url.c_str(), _royaltyPointsPerMillion);
        if (success)
        {
            // notification
            IsCreateNFTOk = true;
            Notification = FString::Printf(TEXT("Successfully Create NFT: %s "), *tokenName);
        }
        else
        {
            // notification
            IsCreateNFTOk = false;
            Notification = FString::Printf(TEXT("Failed to Create NFT: %s "), *tokenName);
        }
    }
}

void UAptosController::GetCurrentWalletBalance(FString& Balance)
{
    char* balance = AptosWrapper::GetCurrentWalletBalanceText(m_controller);
    Balance = balance;
    AptosWrapper::DeleteString(balance);
}

void UAptosController::SetNetwork(FString Network)
{
    if (Network == "Devnet"|| Network == "Testnet"|| Network == "Mainnet")
    {
        std::string StdNetwork = std::string(TCHAR_TO_UTF8(*Network)); // Step 1: Convert to std::string
        const char* CharNetwork = StdNetwork.c_str(); // Step 2: Get const char* pointer
        char* MutableCharNetwork = const_cast<char*>(CharNetwork); // If a mutable char* is required

        // Step 3: Use char* in an array to pass as char**
        char* NetworkArray[] = { MutableCharNetwork };
        AptosWrapper::SetNetwork(m_controller, CharNetwork);
    }
}

void UAptosController::OnInitApp(FString &mnemonic_key)
{
    m_controller = AptosWrapper::GetController();
    if (!m_controller)
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnInitApp: Controller is null"));
        return;
    }

    FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
    FString LoadedData;
    FFileHelper::LoadFileToString(LoadedData, *SavePath);
    mnemonic_key = LoadedData;

    if (!mnemonic_key.IsEmpty())
    {
        std::string _collectionName(TCHAR_TO_UTF8(*mnemonic_key));
        AptosWrapper::SetNetwork(m_controller, "Devnet");
    }
}

void UAptosController::OnQuitApp()
{
    if (!m_controller)
    {
        UE_LOG(LogTemp, Error, TEXT("UAptosController::OnQuitApp: Controller is null"));
        return;
    }

    AptosWrapper::SaveMnemonicsKey(m_controller);
    AptosWrapper::DeleteController(m_controller);
}

