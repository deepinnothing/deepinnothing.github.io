// Copyright 2025 Władysław Witkowski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "GoogleAdMobGameInstance.generated.h"

class UGoogleAdMobRewardedInterstitialAd;
class UGoogleAdMobRewardedAd;
class UGoogleAdMobInterstitialAd;
enum class EGoogleAdMobBannerPosition : uint8;
struct FGoogleAdMobAdError;
struct FGoogleAdMobAdValue;
class UGoogleAdMobMenu;
class UGoogleAdMobAppOpenAd;
struct FGoogleAdMobResponseInfo;
class UGoogleAdMobBannerAd;
enum class EGoogleAdMobBannerSize: uint8;
enum class EGoogleAdMobCollapsibleBannerPlacement: uint8;

UCLASS()
class GOOGLEADMOBCPP_API UGoogleAdMobGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

	friend class UGoogleAdMobMenu;
	
	virtual void Init() override;
	virtual void OnStart() override;

	UPROPERTY()
	TObjectPtr<UGoogleAdMobMenu> GoogleAdMobMenu;

	UPROPERTY()
	TArray<TObjectPtr<UGoogleAdMobAppOpenAd>> AppOpenAds;

	void LoadAppOpenAd();
	void CleanUpAppOpenAds();
	void IsAppOpenAdReady();
	UFUNCTION() void ShowAppOpenAd();
	UFUNCTION() void AppOpenAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void AppOpenAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError, const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void AppOpenAdClicked();
	UFUNCTION() void AppOpenAdImpression();
	UFUNCTION() void AppOpenAdPaidEvent(const FGoogleAdMobAdValue& AdValue);
	UFUNCTION() void AppOpenAdDismissed();
	UFUNCTION() void AppOpenAdFailedToShow(const FGoogleAdMobAdError& AdError);
	UFUNCTION() void AppOpenAdShown();

	UPROPERTY()
	TObjectPtr<UGoogleAdMobBannerAd> BannerAd;

	void LoadBannerAd(EGoogleAdMobBannerSize BannerSize, EGoogleAdMobCollapsibleBannerPlacement CollapsibleBannerPlacement) const;
	void ShowBannerAd(EGoogleAdMobBannerPosition BannerPosition) const;
	void RemoveBannerAd() const;
	UFUNCTION() void BannerAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void BannerAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError, const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void BannerAdClicked();
	UFUNCTION() void BannerAdImpression();
	UFUNCTION() void BannerAdOpened();
	UFUNCTION() void BannerAdPaidEvent(const FGoogleAdMobAdValue& AdValue);

	UPROPERTY()
	TArray<TObjectPtr<UGoogleAdMobInterstitialAd>> InterstitialAds;

	void LoadInterstitialAd();
	void CleanUpInterstitialAds();
	void IsInterstitialAdReady();
	UFUNCTION() void ShowInterstitialAd();
	UFUNCTION() void InterstitialAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void InterstitialAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError, const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void InterstitialAdClicked();
	UFUNCTION() void InterstitialAdImpression();
	UFUNCTION() void InterstitialAdPaidEvent(const FGoogleAdMobAdValue& AdValue);
	UFUNCTION() void InterstitialAdDismissed();
	UFUNCTION() void InterstitialAdFailedToShow(const FGoogleAdMobAdError& AdError);
	UFUNCTION() void InterstitialAdShown();

	UPROPERTY()
	TArray<TObjectPtr<UGoogleAdMobRewardedAd>> RewardedAds;

	void LoadRewardedAd();
	void CleanUpRewardedAds();
	void IsRewardedAdReady();
	UFUNCTION() void ShowRewardedAd();
	UFUNCTION() void RewardedAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void RewardedAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError, const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void RewardedAdClicked();
	UFUNCTION() void RewardedAdImpression();
	UFUNCTION() void RewardedAdPaidEvent(const FGoogleAdMobAdValue& AdValue);
	UFUNCTION() void RewardedAdDismissed();
	UFUNCTION() void RewardedAdFailedToShow(const FGoogleAdMobAdError& AdError);
	UFUNCTION() void RewardedAdShown();
	UFUNCTION() void UserEarnedReward(int32 RewardAmount, const FString& RewardType);

	UPROPERTY()
	TArray<TObjectPtr<UGoogleAdMobRewardedInterstitialAd>> RewardedInterstitialAds;

	void LoadRewardedInterstitialAd();
	void CleanUpRewardedInterstitialAds();
	void IsRewardedInterstitialAdReady();
	UFUNCTION() void ShowRewardedInterstitialAd();
	UFUNCTION() void RewardedInterstitialAdLoaded(const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void RewardedInterstitialAdFailedToLoad(const FGoogleAdMobAdError& LoadAdError, const FGoogleAdMobResponseInfo& ResponseInfo);
	UFUNCTION() void RewardedInterstitialAdClicked();
	UFUNCTION() void RewardedInterstitialAdImpression();
	UFUNCTION() void RewardedInterstitialAdPaidEvent(const FGoogleAdMobAdValue& AdValue);
	UFUNCTION() void RewardedInterstitialAdDismissed();
	UFUNCTION() void RewardedInterstitialAdFailedToShow(const FGoogleAdMobAdError& AdError);
	UFUNCTION() void RewardedInterstitialAdShown();
	UFUNCTION() void UserEarnedInterstitialReward(int32 RewardAmount, const FString& RewardType);
};
