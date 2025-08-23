// Copyright 2025 Władysław Witkowski. All Rights Reserved.

#include "GoogleAdMobGameInstance.h"
#include "GoogleAdMob.h"
#include "GoogleAdMobAdState.h"
#include "GoogleAdMobAppOpenAd.h"
#include "GoogleAdMobResponseInfo.h"
#include "GoogleAdMobAdError.h"
#include "GoogleAdMobAdValue.h"
#include "GoogleAdMobBannerAd.h"
#include "GoogleAdMobInterstitialAd.h"
#include "GoogleAdMobRewardedAd.h"
#include "GoogleAdMobRewardedInterstitialAd.h"
#include "GoogleAdMobMenu.h"

void UGoogleAdMobGameInstance::Init()
{
	Super::Init();

	UClass* WBPClass = LoadObject<UClass>(
		nullptr,
		TEXT("/Game/WBP_GoogleAdMobMenu.WBP_GoogleAdMobMenu_C")
	);
	GoogleAdMobMenu = CreateWidget<UGoogleAdMobMenu>(this, WBPClass);
	GoogleAdMobMenu->GameInstance = this;

	TSharedRef<bool> bInitFlag = MakeShared<bool>(false);

	UGoogleAdMob::OnRequestConsentInfoUpdateSuccess.AddLambda([this, bInitFlag]
		{
			GoogleAdMobMenu->AddLogEntry(TEXT("CONSENT INFO UPDATED SUCCESSFULLY"));
			GoogleAdMobMenu->SetPrivacyOptionsButtonVisibility();

			UGoogleAdMob::OnConsentFormDismissed.AddLambda([this, bInitFlag]
				{
					GoogleAdMobMenu->AddLogEntry(TEXT("CONSENT FORM DISMISSED"));
					if (UGoogleAdMob::CanRequestAds())
					{
						if (!*bInitFlag)
						{
							*bInitFlag = true;
							GoogleAdMobMenu->AddLogEntry(TEXT(
								"UGoogleAdMob::CanRequestAds() returned true. Initializing Google AdMob SDK..."));
							UGoogleAdMob::Initialize();
						}
					}
					else
					{
						GoogleAdMobMenu->AddLogEntry(TEXT(
							"UGoogleAdMob::CanRequestAds() returned false. Cannot initialize Google AdMob SDK until consent from user is obtained"));
					}
				}
			);

			UGoogleAdMob::OnConsentFormDismissedWithError.AddLambda([this]
			(const int32 ErrorCode, const FString& ErrorMessage)
				{
					GoogleAdMobMenu->AddLogEntry(TEXT("CONSENT FORM DISMISSED WITH ERROR:\n"), ErrorCode, ErrorMessage);
				}
			);

			UGoogleAdMob::LoadAndShowConsentFormIfRequired();
		}
	);
	UGoogleAdMob::OnRequestConsentInfoUpdateFailure.AddLambda([this]
	(const int32 ErrorCode, const FString& ErrorMessage)
		{
			GoogleAdMobMenu->AddLogEntry(TEXT("CONSENT INFO UPDATE FAILED WITH ERROR:\n"), ErrorCode, ErrorMessage);
		}
	);
	UGoogleAdMob::RequestConsentInfoUpdate();

	if (UGoogleAdMob::CanRequestAds() && !*bInitFlag)
	{
		*bInitFlag = true;
		UGoogleAdMob::Initialize();
	}

	ApplicationHasEnteredForegroundDelegate.AddDynamic(this, &UGoogleAdMobGameInstance::ShowAppOpenAd);

	BannerAd = NewObject<UGoogleAdMobBannerAd>(this);
	BannerAd->OnLoaded.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdLoaded);
	BannerAd->OnFailedToLoad.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdFailedToLoad);
	BannerAd->OnClicked.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdClicked);
	BannerAd->OnImpression.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdImpression);
	BannerAd->OnOpened.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdOpened);
	BannerAd->OnPaidEvent.AddDynamic(this, &UGoogleAdMobGameInstance::BannerAdPaidEvent);
}

void UGoogleAdMobGameInstance::OnStart()
{
	Super::OnStart();

	GoogleAdMobMenu->AddToViewport();
}

void UGoogleAdMobGameInstance::LoadAppOpenAd()
{
	if (!UGoogleAdMob::CanRequestAds())
	{
		GoogleAdMobMenu->AddLogEntry(
			TEXT("UGoogleAdMob::CanRequestAds() RETURNED FALSE. DID YOU OBTAIN CONSENT FROM THE USER?"));
		return;
	}

	UGoogleAdMobAppOpenAd* AppOpenAd = NewObject<UGoogleAdMobAppOpenAd>();

	AppOpenAd->OnLoaded.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdLoaded);
	AppOpenAd->OnFailedToLoad.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdFailedToLoad);
	AppOpenAd->OnClicked.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdClicked);
	AppOpenAd->OnImpression.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdImpression);
	AppOpenAd->OnPaidEvent.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdPaidEvent);
	AppOpenAd->OnDismissed.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdDismissed);
	AppOpenAd->OnFailedToShow.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdFailedToShow);
	AppOpenAd->OnShown.AddDynamic(this, &UGoogleAdMobGameInstance::AppOpenAdShown);

	AppOpenAds.Add(AppOpenAd);

#if PLATFORM_ANDROID
	AppOpenAd->Load(TEXT("ca-app-pub-3940256099942544/9257395921"));
#elif PLATFORM_IOS
	AppOpenAd->Load(TEXT("ca-app-pub-3940256099942544/5575463023"));
#endif

	GoogleAdMobMenu->AddLogEntry(TEXT("LOADING APP OPEN AD..."));
	GoogleAdMobMenu->UpdateAppOpenAdsNumber();
}

void UGoogleAdMobGameInstance::CleanUpAppOpenAds()
{
	AppOpenAds.RemoveAll([this](auto AppOpenAd)
	{
		if (AppOpenAd->GetState() == EGoogleAdMobAdState::UNKNOWN
			|| AppOpenAd->GetState() == EGoogleAdMobAdState::EXPIRED)
			return true;
		return false;
	});
	GoogleAdMobMenu->UpdateAppOpenAdsNumber();
}

void UGoogleAdMobGameInstance::IsAppOpenAdReady()
{
	for (auto AppOpenAd : AppOpenAds)
	{
		if (AppOpenAd->IsReady())
		{
			GoogleAdMobMenu->AddLogEntry(TEXT(
				"APP OPEN AD IS READY TO BE SHOWN. YOU CAN MINIMIZE THE APP AND RE-OPEN IT AGAIN TO SEE THE AD"));
			return;
		}
	}
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD IS NOT LOADED OR HAS EXPIRED"));
}

void UGoogleAdMobGameInstance::ShowAppOpenAd()
{
	CleanUpAppOpenAds();
	for (auto AppOpenAd : AppOpenAds)
	{
		if (AppOpenAd->GetState() == EGoogleAdMobAdState::LOADED)
		{
			AppOpenAd->Show();
			return;
		}
	}
}

void UGoogleAdMobGameInstance::AppOpenAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(
		TEXT("APP OPEN AD LOADED WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse +
		TEXT("\nYOU CAN NOW MINIMIZE THE APP AND RE-OPEN IT AGAIN TO SEE THE AD"));
}

void UGoogleAdMobGameInstance::AppOpenAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError,
                                                     const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD FAILED TO LOAD WITH ERROR:\n") + LoadAdError.FullError);
	GoogleAdMobMenu->AddLogEntry(
		TEXT("APP OPEN AD FAILED TO LOAD WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
	CleanUpAppOpenAds();
}

void UGoogleAdMobGameInstance::AppOpenAdClicked()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD CLICKED"));
}

void UGoogleAdMobGameInstance::AppOpenAdImpression()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD IMPRESSION"));
}

void UGoogleAdMobGameInstance::AppOpenAdPaidEvent(const FGoogleAdMobAdValue& AdValue)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT(
			"APP OPEN AD PAID WITH AD VALUE:\nVALUE MICROS: %lld\nPRECISION TYPE: %hhd\nDOMAIN (CURRENCY CODE): %s"),
		AdValue.ValueMicros, AdValue.PrecisionType, *AdValue.CurrencyCode));
}

void UGoogleAdMobGameInstance::AppOpenAdDismissed()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD DISMISSED"));
	CleanUpAppOpenAds();
}

void UGoogleAdMobGameInstance::AppOpenAdFailedToShow(const FGoogleAdMobAdError& AdError)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD FAILED TO SHOW WITH ERROR:\n") + AdError.FullError);
	CleanUpAppOpenAds();
}

void UGoogleAdMobGameInstance::AppOpenAdShown()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("APP OPEN AD SHOWN"));
}

void UGoogleAdMobGameInstance::LoadBannerAd(EGoogleAdMobBannerSize BannerSize,
                                            EGoogleAdMobCollapsibleBannerPlacement CollapsibleBannerPlacement) const
{
	if (!UGoogleAdMob::CanRequestAds())
	{
		GoogleAdMobMenu->AddLogEntry(
			TEXT("UGoogleAdMob::CanRequestAds() RETURNED FALSE. DID YOU OBTAIN CONSENT FROM THE USER?"));
		return;
	}

#if PLATFORM_ANDROID
	if (BannerSize == EGoogleAdMobBannerSize::ADAPTIVE)
	{
		BannerAd->Load(TEXT("ca-app-pub-3940256099942544/9214589741"), BannerSize, CollapsibleBannerPlacement);
	}
	else
	{
		BannerAd->Load(TEXT("ca-app-pub-3940256099942544/6300978111"), BannerSize, CollapsibleBannerPlacement);
	}
#elif PLATFORM_IOS
	if (BannerSize == EGoogleAdMobBannerSize::ADAPTIVE)
	{
		BannerAd->Load(TEXT("ca-app-pub-3940256099942544/2435281174"), BannerSize, CollapsibleBannerPlacement);
	}
	else
	{
		BannerAd->Load(TEXT("ca-app-pub-3940256099942544/2934735716"), BannerSize, CollapsibleBannerPlacement);
	}
#endif

	GoogleAdMobMenu->AddLogEntry(TEXT("LOADING BANNER AD..."));
}

void UGoogleAdMobGameInstance::ShowBannerAd(EGoogleAdMobBannerPosition BannerPosition) const
{
	BannerAd->Show(BannerPosition);
}

void UGoogleAdMobGameInstance::RemoveBannerAd() const
{
	BannerAd->Remove();
}

void UGoogleAdMobGameInstance::BannerAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(
		TEXT("BANNER AD LOADED WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
}

void UGoogleAdMobGameInstance::BannerAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError,
                                                    const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("BANNER AD FAILED TO LOAD WITH ERROR:\n") + LoadAdError.FullError);
	GoogleAdMobMenu->AddLogEntry(
		TEXT("BANNER AD FAILED TO LOAD WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
}

void UGoogleAdMobGameInstance::BannerAdClicked()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("BANNER AD CLICKED"));
}

void UGoogleAdMobGameInstance::BannerAdImpression()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("BANNER AD IMPRESSION"));
}

void UGoogleAdMobGameInstance::BannerAdOpened()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("BANNER AD OPENED"));
}

void UGoogleAdMobGameInstance::BannerAdPaidEvent(const FGoogleAdMobAdValue& AdValue)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT(
			"APP OPEN AD PAID WITH AD VALUE:\nVALUE MICROS: %lld\nPRECISION TYPE: %hhd\nDOMAIN (CURRENCY CODE): %s"),
		AdValue.ValueMicros, AdValue.PrecisionType, *AdValue.CurrencyCode));
}

void UGoogleAdMobGameInstance::LoadInterstitialAd()
{
	if (!UGoogleAdMob::CanRequestAds())
	{
		GoogleAdMobMenu->AddLogEntry(
			TEXT("UGoogleAdMob::CanRequestAds() RETURNED FALSE. DID YOU OBTAIN CONSENT FROM THE USER?"));
		return;
	}

	UGoogleAdMobInterstitialAd* InterstitialAd = NewObject<UGoogleAdMobInterstitialAd>();

	InterstitialAd->OnLoaded.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdLoaded);
	InterstitialAd->OnFailedToLoad.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdFailedToLoad);
	InterstitialAd->OnClicked.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdClicked);
	InterstitialAd->OnImpression.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdImpression);
	InterstitialAd->OnPaidEvent.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdPaidEvent);
	InterstitialAd->OnDismissed.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdDismissed);
	InterstitialAd->OnFailedToShow.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdFailedToShow);
	InterstitialAd->OnShown.AddDynamic(this, &UGoogleAdMobGameInstance::InterstitialAdShown);

	InterstitialAds.Add(InterstitialAd);

#if PLATFORM_ANDROID
	InterstitialAd->Load(TEXT("ca-app-pub-3940256099942544/1033173712"));
#elif PLATFORM_IOS
	InterstitialAd->Load(TEXT("ca-app-pub-3940256099942544/4411468910"));
#endif

	GoogleAdMobMenu->AddLogEntry(TEXT("LOADING INTERSTITIAL AD..."));
	GoogleAdMobMenu->UpdateInterstitialAdsNumber();
}

void UGoogleAdMobGameInstance::CleanUpInterstitialAds()
{
	InterstitialAds.RemoveAll([this](auto InterstitialAd)
	{
		if (InterstitialAd->GetState() == EGoogleAdMobAdState::UNKNOWN
			|| InterstitialAd->GetState() == EGoogleAdMobAdState::EXPIRED)
			return true;
		return false;
	});
	GoogleAdMobMenu->UpdateInterstitialAdsNumber();
}

void UGoogleAdMobGameInstance::IsInterstitialAdReady()
{
	for (auto InterstitialAd : InterstitialAds)
	{
		if (InterstitialAd->IsReady())
		{
			GoogleAdMobMenu->AddLogEntry(TEXT(
				"INTERSTITIAL AD IS READY TO BE SHOWN"));
			return;
		}
	}
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD IS NOT LOADED OR HAS EXPIRED"));
}

void UGoogleAdMobGameInstance::ShowInterstitialAd()
{
	CleanUpInterstitialAds();
	for (auto InterstitialAd : InterstitialAds)
	{
		if (InterstitialAd->IsReady())
		{
			InterstitialAd->Show();
			return;
		}
	}
}

void UGoogleAdMobGameInstance::InterstitialAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(
		TEXT("INTERSTITIAL AD LOADED WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
}

void UGoogleAdMobGameInstance::InterstitialAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError,
                                                          const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD FAILED TO LOAD WITH ERROR:\n") + LoadAdError.FullError);
	GoogleAdMobMenu->AddLogEntry(
		TEXT("INTERSTITIAL AD FAILED TO LOAD WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::InterstitialAdClicked()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD CLICKED"));
}

void UGoogleAdMobGameInstance::InterstitialAdImpression()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD IMPRESSION"));
}

void UGoogleAdMobGameInstance::InterstitialAdPaidEvent(const FGoogleAdMobAdValue& AdValue)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT(
			"INTERSTITIAL AD PAID WITH AD VALUE:\nVALUE MICROS: %lld\nPRECISION TYPE: %hhd\nDOMAIN (CURRENCY CODE): %s"),
		AdValue.ValueMicros, AdValue.PrecisionType, *AdValue.CurrencyCode));
}

void UGoogleAdMobGameInstance::InterstitialAdDismissed()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD DISMISSED"));
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::InterstitialAdFailedToShow(const FGoogleAdMobAdError& AdError)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD FAILED TO SHOW WITH ERROR:\n") + AdError.FullError);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::InterstitialAdShown()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("INTERSTITIAL AD SHOWN"));
}

void UGoogleAdMobGameInstance::LoadRewardedAd()
{
	if (!UGoogleAdMob::CanRequestAds())
	{
		GoogleAdMobMenu->AddLogEntry(
			TEXT("UGoogleAdMob::CanRequestAds() RETURNED FALSE. DID YOU OBTAIN CONSENT FROM THE USER?"));
		return;
	}

	UGoogleAdMobRewardedAd* RewardedAd = NewObject<UGoogleAdMobRewardedAd>();

	RewardedAd->OnLoaded.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdLoaded);
	RewardedAd->OnFailedToLoad.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdFailedToLoad);
	RewardedAd->OnClicked.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdClicked);
	RewardedAd->OnImpression.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdImpression);
	RewardedAd->OnPaidEvent.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdPaidEvent);
	RewardedAd->OnDismissed.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdDismissed);
	RewardedAd->OnFailedToShow.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdFailedToShow);
	RewardedAd->OnShown.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedAdShown);
	RewardedAd->OnUserEarnedReward.AddDynamic(this, &UGoogleAdMobGameInstance::UserEarnedReward);

	RewardedAds.Add(RewardedAd);

#if PLATFORM_ANDROID
	RewardedAd->Load(TEXT("ca-app-pub-3940256099942544/5224354917"));
#elif PLATFORM_IOS
	RewardedAd->Load(TEXT("ca-app-pub-3940256099942544/1712485313"));
#endif

	GoogleAdMobMenu->AddLogEntry(TEXT("LOADING REWARDED AD..."));
	GoogleAdMobMenu->UpdateRewardedAdsNumber();
}

void UGoogleAdMobGameInstance::CleanUpRewardedAds()
{
	RewardedAds.RemoveAll([this](auto RewardedAd)
	{
		if (RewardedAd->GetState() == EGoogleAdMobAdState::UNKNOWN
			|| RewardedAd->GetState() == EGoogleAdMobAdState::EXPIRED)
			return true;
		return false;
	});
	GoogleAdMobMenu->UpdateRewardedAdsNumber();
}

void UGoogleAdMobGameInstance::IsRewardedAdReady()
{
	for (auto RewardedAd : RewardedAds)
	{
		if (RewardedAd->IsReady())
		{
			GoogleAdMobMenu->AddLogEntry(TEXT(
				"REWARDED AD IS READY TO BE SHOWN"));
			return;
		}
	}
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD IS NOT LOADED OR HAS EXPIRED"));
}

void UGoogleAdMobGameInstance::ShowRewardedAd()
{
	CleanUpRewardedAds();
	for (auto RewardedAd : RewardedAds)
	{
		if (RewardedAd->GetState() == EGoogleAdMobAdState::LOADED)
		{
			RewardedAd->Show();
			return;
		}
	}
}

void UGoogleAdMobGameInstance::RewardedAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(
		TEXT("REWARDED AD LOADED WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
}

void UGoogleAdMobGameInstance::RewardedAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError,
                                                      const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD FAILED TO LOAD WITH ERROR:\n") + LoadAdError.FullError);
	GoogleAdMobMenu->AddLogEntry(
		TEXT("REWARDED AD FAILED TO LOAD WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedAdClicked()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD CLICKED"));
}

void UGoogleAdMobGameInstance::RewardedAdImpression()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD IMPRESSION"));
}

void UGoogleAdMobGameInstance::RewardedAdPaidEvent(const FGoogleAdMobAdValue& AdValue)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT(
			"REWARDED AD PAID WITH AD VALUE:\nVALUE MICROS: %lld\nPRECISION TYPE: %hhd\nDOMAIN (CURRENCY CODE): %s"),
		AdValue.ValueMicros, AdValue.PrecisionType, *AdValue.CurrencyCode));
}

void UGoogleAdMobGameInstance::RewardedAdDismissed()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD DISMISSED"));
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedAdFailedToShow(const FGoogleAdMobAdError& AdError)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD FAILED TO SHOW WITH ERROR:\n") + AdError.FullError);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedAdShown()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED AD SHOWN"));
}

void UGoogleAdMobGameInstance::UserEarnedReward(int32 RewardAmount, const FString& RewardType)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT("USER EARNED REWARD: %i %s"), RewardAmount, *RewardType));
}

void UGoogleAdMobGameInstance::LoadRewardedInterstitialAd()
{
	if (!UGoogleAdMob::CanRequestAds())
	{
		GoogleAdMobMenu->AddLogEntry(
			TEXT("UGoogleAdMob::CanRequestAds() RETURNED FALSE. DID YOU OBTAIN CONSENT FROM THE USER?"));
		return;
	}

	UGoogleAdMobRewardedInterstitialAd* RewardedInterstitialAd = NewObject<UGoogleAdMobRewardedInterstitialAd>();

	RewardedInterstitialAd->OnLoaded.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdLoaded);
	RewardedInterstitialAd->OnFailedToLoad.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdFailedToLoad);
	RewardedInterstitialAd->OnClicked.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdClicked);
	RewardedInterstitialAd->OnImpression.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdImpression);
	RewardedInterstitialAd->OnPaidEvent.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdPaidEvent);
	RewardedInterstitialAd->OnDismissed.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdDismissed);
	RewardedInterstitialAd->OnFailedToShow.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdFailedToShow);
	RewardedInterstitialAd->OnShown.AddDynamic(this, &UGoogleAdMobGameInstance::RewardedInterstitialAdShown);
	RewardedInterstitialAd->OnUserEarnedReward.AddDynamic(this, &UGoogleAdMobGameInstance::UserEarnedInterstitialReward);

	RewardedInterstitialAds.Add(RewardedInterstitialAd);

#if PLATFORM_ANDROID
	RewardedInterstitialAd->Load(TEXT("ca-app-pub-3940256099942544/5354046379"));
#elif PLATFORM_IOS
	RewardedInterstitialAd->Load(TEXT("ca-app-pub-3940256099942544/6978759866"));
#endif

	GoogleAdMobMenu->AddLogEntry(TEXT("LOADING REWARDED INTERSTITIAL AD..."));
	GoogleAdMobMenu->UpdateRewardedInterstitialAdsNumber();
}

void UGoogleAdMobGameInstance::CleanUpRewardedInterstitialAds()
{
	RewardedInterstitialAds.RemoveAll([this](auto RewardedInterstitialAd)
	{
		if (RewardedInterstitialAd->GetState() == EGoogleAdMobAdState::UNKNOWN
			|| RewardedInterstitialAd->GetState() == EGoogleAdMobAdState::EXPIRED)
			return true;
		return false;
	});
	GoogleAdMobMenu->UpdateRewardedInterstitialAdsNumber();
}

void UGoogleAdMobGameInstance::IsRewardedInterstitialAdReady()
{
	for (auto RewardedInterstitialAd : RewardedInterstitialAds)
	{
		if (RewardedInterstitialAd->IsReady())
		{
			GoogleAdMobMenu->AddLogEntry(TEXT(
				"REWARDED INTERSTITIAL AD IS READY TO BE SHOWN"));
			return;
		}
	}
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD IS NOT LOADED OR HAS EXPIRED"));
}

void UGoogleAdMobGameInstance::ShowRewardedInterstitialAd()
{
	CleanUpRewardedInterstitialAds();
	for (auto RewardedInterstitialAd : RewardedInterstitialAds)
	{
		if (RewardedInterstitialAd->IsReady())
		{
			RewardedInterstitialAd->Show();
			return;
		}
	}
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(
		TEXT("REWARDED INTERSTITIAL AD LOADED WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError,
                                                      const FGoogleAdMobResponseInfo& ResponseInfo)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD FAILED TO LOAD WITH ERROR:\n") + LoadAdError.FullError);
	GoogleAdMobMenu->AddLogEntry(
		TEXT("REWARDED INTERSTITIAL AD FAILED TO LOAD WITH RESPONSE INFO:\n") + ResponseInfo.FullResponse);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdClicked()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD CLICKED"));
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdImpression()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD IMPRESSION"));
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdPaidEvent(const FGoogleAdMobAdValue& AdValue)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT(
			"REWARDED INTERSTITIAL AD PAID WITH AD VALUE:\nVALUE MICROS: %lld\nPRECISION TYPE: %hhd\nDOMAIN (CURRENCY CODE): %s"),
		AdValue.ValueMicros, AdValue.PrecisionType, *AdValue.CurrencyCode));
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdDismissed()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD DISMISSED"));
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdFailedToShow(const FGoogleAdMobAdError& AdError)
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD FAILED TO SHOW WITH ERROR:\n") + AdError.FullError);
	CleanUpInterstitialAds();
}

void UGoogleAdMobGameInstance::RewardedInterstitialAdShown()
{
	GoogleAdMobMenu->AddLogEntry(TEXT("REWARDED INTERSTITIAL AD SHOWN"));
}

void UGoogleAdMobGameInstance::UserEarnedInterstitialReward(int32 RewardAmount, const FString& RewardType)
{
	GoogleAdMobMenu->AddLogEntry(FString::Printf(
		TEXT("USER EARNED INTERSTITIAL REWARD: %i %s"), RewardAmount, *RewardType));
}
