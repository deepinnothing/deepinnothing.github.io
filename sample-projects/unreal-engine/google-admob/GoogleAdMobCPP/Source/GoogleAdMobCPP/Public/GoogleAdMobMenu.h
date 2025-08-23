// Copyright 2025 Władysław Witkowski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GoogleAdMobBannerAd.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "GoogleAdMobMenu.generated.h"

class UCheckBox;
class USlider;
enum class EGoogleAdMobBannerPosition : uint8;
enum class EGoogleAdMobCollapsibleBannerPlacement : uint8;
enum class EGoogleAdMobBannerSize : uint8;
class UComboBoxString;
class UGoogleAdMobGameInstance;
class UTextBlock;
class UButton;

UCLASS(Abstract)
class GOOGLEADMOBCPP_API UGoogleAdMobMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddLogEntry(const FString& LogEntry) const;
	void AddLogEntry(const FString& LogInfo, int ErrorCode, const FString& ErrorMessage) const;
	void SetPrivacyOptionsButtonVisibility() const;
	virtual void NativeConstruct() override;
	void UpdateAppOpenAdsNumber();
	void UpdateInterstitialAdsNumber();
	void UpdateRewardedAdsNumber();
	void UpdateRewardedInterstitialAdsNumber();

	explicit UGoogleAdMobMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	TObjectPtr<UGoogleAdMobGameInstance> GameInstance;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Log;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> PrivacyOptionsButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> OpenAdInspectorButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ResetConsentStateButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> GetConsentStatusButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LoadAppOpenAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AppOpenAdsNumber;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CheckAppOpenAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UComboBoxString> BannerSizeComboBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UComboBoxString> CollapsibleBannerPlacementComboBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UComboBoxString> BannerPositionComboBox;

	EGoogleAdMobBannerSize BannerSize;
	EGoogleAdMobCollapsibleBannerPlacement CollapsibleBannerPlacement;
	EGoogleAdMobBannerPosition BannerPosition;
	
	UPROPERTY(meta=(BindWidget))
    TObjectPtr<UButton> LoadBannerAdButton;
	
	UPROPERTY(meta=(BindWidget))
    TObjectPtr<UButton> ShowBannerAdButton;

	UPROPERTY(meta=(BindWidget))
    TObjectPtr<UButton> RemoveBannerAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> InterstitialAdsNumber;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RewardedAdsNumber;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RewardedInterstitialAdsNumber;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LoadInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CheckInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ShowInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LoadRewardedAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CheckRewardedAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ShowRewardedAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LoadRewardedInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CheckRewardedInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ShowRewardedInterstitialAdButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USlider> AppVolumeSlider;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> AppMutedCheckBox;

	UFUNCTION()
	void PrivacyOptionsButtonClicked();

	UFUNCTION()
	void OpenAdInspectorButtonClicked();

	UFUNCTION()
	void ResetConsentStateButtonClicked();

	UFUNCTION()
	void GetConsentStatusButtonClicked();

	UFUNCTION()
	void LoadAppOpenAdButtonClicked();

	UFUNCTION()
	void CheckAppOpenAdButtonClicked();

	UFUNCTION()
	void LoadBannerAdButtonClicked();

	UFUNCTION()
	void ShowBannerAdButtonClicked();

	UFUNCTION()
	void RemoveBannerAdButtonClicked();

	UFUNCTION()
	void BannerSizeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void CollapsibleBannerPlacementSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void BannerPositionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void LoadInterstitialAdButtonClicked();

	UFUNCTION()
	void CheckInterstitialAdButtonClicked();

	UFUNCTION()
	void ShowInterstitialAdButtonClicked();

	UFUNCTION()
	void LoadRewardedAdButtonClicked();

	UFUNCTION()
	void CheckRewardedAdButtonClicked();

	UFUNCTION()
	void ShowRewardedAdButtonClicked();

	UFUNCTION()
	void LoadRewardedInterstitialAdButtonClicked();

	UFUNCTION()
	void CheckRewardedInterstitialAdButtonClicked();

	UFUNCTION()
	void ShowRewardedInterstitialAdButtonClicked();

	UFUNCTION()
	void AppVolumeSliderValueChanged(float Value);

	UFUNCTION()
	void AppMutedCheckBoxStateChanged(bool bIsChecked);
};
