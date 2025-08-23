// Copyright 2025 Władysław Witkowski. All Rights Reserved.

#include "GoogleAdMobMenu.h"
#include "GoogleAdMobGameInstance.h"
#include "GoogleAdMob.h"
#include "GoogleAdMobAdError.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "GoogleAdMobBannerAd.h"

void UGoogleAdMobMenu::AddLogEntry(const FString& LogEntry) const
{
	Log->SetText(FText::FromString(
		Log->GetText().ToString() + LogEntry + TEXT("\n\n")
	));
}

void UGoogleAdMobMenu::AddLogEntry(const FString& LogInfo, int ErrorCode, const FString& ErrorMessage) const
{
	AddLogEntry(LogInfo + FString::FromInt(ErrorCode) + TEXT(" | ") + ErrorMessage);
}

void UGoogleAdMobMenu::SetPrivacyOptionsButtonVisibility() const
{
	if (UGoogleAdMob::ArePrivacyOptionsRequired())
	{
		PrivacyOptionsButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PrivacyOptionsButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UGoogleAdMobMenu::UGoogleAdMobMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BannerSize = EGoogleAdMobBannerSize::ADAPTIVE;
	CollapsibleBannerPlacement = EGoogleAdMobCollapsibleBannerPlacement::NONE;
	BannerPosition = EGoogleAdMobBannerPosition::BOTTOM;
}

void UGoogleAdMobMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GetConsentStatusButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::GetConsentStatusButtonClicked);
	ResetConsentStateButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::ResetConsentStateButtonClicked);
	PrivacyOptionsButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::PrivacyOptionsButtonClicked);

	OpenAdInspectorButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::OpenAdInspectorButtonClicked);

	LoadAppOpenAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::LoadAppOpenAdButtonClicked);
	CheckAppOpenAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::CheckAppOpenAdButtonClicked);

	LoadBannerAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::LoadBannerAdButtonClicked);
	ShowBannerAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::ShowBannerAdButtonClicked);
	RemoveBannerAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::RemoveBannerAdButtonClicked);
	BannerSizeComboBox->OnSelectionChanged.AddDynamic(this, &UGoogleAdMobMenu::BannerSizeSelectionChanged);
	CollapsibleBannerPlacementComboBox->OnSelectionChanged.AddDynamic(this, &UGoogleAdMobMenu::CollapsibleBannerPlacementSelectionChanged);
	BannerPositionComboBox->OnSelectionChanged.AddDynamic(this, &UGoogleAdMobMenu::BannerPositionSelectionChanged);

	LoadInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::LoadInterstitialAdButtonClicked);
	CheckInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::CheckInterstitialAdButtonClicked);
	ShowInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::ShowInterstitialAdButtonClicked);

	LoadRewardedAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::LoadRewardedAdButtonClicked);
	CheckRewardedAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::CheckRewardedAdButtonClicked);
	ShowRewardedAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::ShowRewardedAdButtonClicked);

	LoadRewardedInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::LoadRewardedInterstitialAdButtonClicked);
	CheckRewardedInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::CheckRewardedInterstitialAdButtonClicked);
	ShowRewardedInterstitialAdButton->OnClicked.AddDynamic(this, &UGoogleAdMobMenu::ShowRewardedInterstitialAdButtonClicked);

	AppVolumeSlider->OnValueChanged.AddDynamic(this, &UGoogleAdMobMenu::AppVolumeSliderValueChanged);
	AppMutedCheckBox->OnCheckStateChanged.AddDynamic(this, &UGoogleAdMobMenu::AppMutedCheckBoxStateChanged);
}

void UGoogleAdMobMenu::PrivacyOptionsButtonClicked()
{
	UGoogleAdMob::OnConsentFormDismissed.AddLambda([this]
		{
			AddLogEntry(TEXT("PRIVACY OPTIONS FORM DISMISSED"));
		}
	);
	UGoogleAdMob::OnConsentFormDismissedWithError.AddLambda([this]
		(const int32 ErrorCode, const FString& ErrorMessage)
		{
			AddLogEntry(TEXT("PRIVACY OPTIONS FORM DISMISSED WITH ERROR:\n")
				+ FString::FromInt(ErrorCode) + " | " + ErrorMessage);
		}
	);
	UGoogleAdMob::ShowPrivacyOptionsForm();
}

void UGoogleAdMobMenu::OpenAdInspectorButtonClicked()
{
	UGoogleAdMob::OnAdInspectorClosed.AddLambda([this]
		{
			AddLogEntry(TEXT("AD INSPECTOR UI CLOSED"));
		}
	);
	UGoogleAdMob::OnAdInspectorFailure.AddLambda([this]
		(const FGoogleAdMobAdError& AdInspectorFailure)
		{
			AddLogEntry(TEXT("AD INSPECTOR UI CLOSED WITH ERROR:\n")
				+ AdInspectorFailure.FullError);
		}
	);
	UGoogleAdMob::OpenAdInspector();
}

void UGoogleAdMobMenu::ResetConsentStateButtonClicked()
{
	UGoogleAdMob::ResetConsentState();
	AddLogEntry(TEXT("CONSENT STATE RESET. RE-RUN THE APP"));
}

void UGoogleAdMobMenu::GetConsentStatusButtonClicked()
{
	AddLogEntry(TEXT("CONSENT STATUS: ") +
		StaticEnum<EGoogleAdMobConsentStatus>()->
		GetNameStringByValue(static_cast<__underlying_type(EGoogleAdMobConsentStatus)>(
			UGoogleAdMob::GetConsentStatus()))
	);
}

void UGoogleAdMobMenu::LoadAppOpenAdButtonClicked()
{
	GameInstance->LoadAppOpenAd();
}

void UGoogleAdMobMenu::CheckAppOpenAdButtonClicked()
{
	GameInstance->IsAppOpenAdReady();
}

void UGoogleAdMobMenu::LoadBannerAdButtonClicked()
{
	GameInstance->LoadBannerAd(BannerSize, CollapsibleBannerPlacement);
}

void UGoogleAdMobMenu::ShowBannerAdButtonClicked()
{
	GameInstance->ShowBannerAd(BannerPosition);
}

void UGoogleAdMobMenu::RemoveBannerAdButtonClicked()
{
	GameInstance->RemoveBannerAd();
}

void UGoogleAdMobMenu::BannerSizeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	BannerSize = static_cast<EGoogleAdMobBannerSize>(BannerSizeComboBox->GetSelectedIndex());
}

void UGoogleAdMobMenu::CollapsibleBannerPlacementSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	CollapsibleBannerPlacement = static_cast<EGoogleAdMobCollapsibleBannerPlacement>(
		CollapsibleBannerPlacementComboBox->GetSelectedIndex());
}

void UGoogleAdMobMenu::BannerPositionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	BannerPosition = static_cast<EGoogleAdMobBannerPosition>(BannerPositionComboBox->GetSelectedIndex());
}

void UGoogleAdMobMenu::LoadInterstitialAdButtonClicked()
{
	GameInstance->LoadInterstitialAd();
}

void UGoogleAdMobMenu::CheckInterstitialAdButtonClicked()
{
	GameInstance->IsInterstitialAdReady();
}

void UGoogleAdMobMenu::ShowInterstitialAdButtonClicked()
{
	GameInstance->ShowInterstitialAd();
}

void UGoogleAdMobMenu::LoadRewardedAdButtonClicked()
{
	GameInstance->LoadRewardedAd();
}

void UGoogleAdMobMenu::CheckRewardedAdButtonClicked()
{
	GameInstance->IsRewardedAdReady();
}

void UGoogleAdMobMenu::ShowRewardedAdButtonClicked()
{
	GameInstance->ShowRewardedAd();
}

void UGoogleAdMobMenu::LoadRewardedInterstitialAdButtonClicked()
{
	GameInstance->LoadRewardedInterstitialAd();
}

void UGoogleAdMobMenu::CheckRewardedInterstitialAdButtonClicked()
{
	GameInstance->IsRewardedInterstitialAdReady();
}

void UGoogleAdMobMenu::ShowRewardedInterstitialAdButtonClicked()
{
	GameInstance->ShowRewardedInterstitialAd();
}

void UGoogleAdMobMenu::AppVolumeSliderValueChanged(float Value)
{
	UGoogleAdMob::SetAppVolume(Value);
}

void UGoogleAdMobMenu::AppMutedCheckBoxStateChanged(bool bIsChecked)
{
	UGoogleAdMob::SetAppMuted(bIsChecked);
}

void UGoogleAdMobMenu::UpdateAppOpenAdsNumber()
{
	AppOpenAdsNumber->SetText(
		FText::FromString(FString(TEXT("NUMBER OF APP OPEN ADS: ") +
		FString::FromInt(GameInstance->AppOpenAds.Num()))));
}

void UGoogleAdMobMenu::UpdateInterstitialAdsNumber()
{
	InterstitialAdsNumber->SetText(
		FText::FromString(FString(TEXT("NUMBER OF INTERSTITIAL ADS: ") +
		FString::FromInt(GameInstance->InterstitialAds.Num()))));
}

void UGoogleAdMobMenu::UpdateRewardedAdsNumber()
{
	RewardedAdsNumber->SetText(
		FText::FromString(FString(TEXT("NUMBER OF REWARDED ADS: ") +
		FString::FromInt(GameInstance->RewardedAds.Num()))));
}

void UGoogleAdMobMenu::UpdateRewardedInterstitialAdsNumber()
{
	RewardedInterstitialAdsNumber->SetText(
		FText::FromString(FString(TEXT("NUMBER OF REWARDED INTERSTITIAL ADS: ") +
		FString::FromInt(GameInstance->RewardedInterstitialAds.Num()))));
}
