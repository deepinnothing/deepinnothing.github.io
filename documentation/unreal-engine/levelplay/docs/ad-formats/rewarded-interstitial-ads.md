[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/804df971aef3){ .md-button .md-button--primary .full-width }

# Rewarded interstitial ads

![](../assets/format-rewarded-interstitial.png){ align=left }

[Rewarded interstitial](https://support.google.com/admob/answer/9884467) is a type of incentivized ad format that lets you offer rewards for ads that appear automatically during natural app transitions. Unlike rewarded ads, users aren't required to opt in to view a rewarded interstitial.

## Prerequisites

Complete the [Get Started guide](../index.md).

## Always test with test ads

When building and testing your apps, make sure you use test ads rather than live, production ads. Failure to do so can lead to suspension of your account.

The easiest way to load test ads is to use our dedicated test ad unit ID for rewarded interstitial ads:

| Android demo ad unit ID                | iOS demo ad unit ID                    |
| -------------------------------------- | -------------------------------------- |
| ca-app-pub-3940256099942544/5354046379 | ca-app-pub-3940256099942544/6978759866 |

It's been specially configured to return test ads for every request, and you're free to use it in your own apps while coding, testing, and debugging. Just make sure you replace it with your own ad unit ID before publishing your app.

For more information about how Google Mobile Ads SDK test ads work, see [Test Ads](../enable-test-ads.md).

## Load an ad

Loading an ad is accomplished using the __`Load()`__ method on the __`UGoogleAdMobRewardedInterstitialAd`__-derived object. The load method requires your ad unit ID.

=== "C++"

    Header:

    ``` c++
    class UGoogleAdMobRewardedInterstitialAd;
    // ...
    UPROPERTY()
    TObjectPtr<UGoogleAdMobRewardedInterstitialAd> RewardedInterstitialAd;
    ```

    Source:

    ``` c++
    #include "GoogleAdMobRewardedInterstitialAd.h"
    // ...
    RewardedAd = NewObject<UGoogleAdMobRewardedInterstitialAd>(this);
    #if PLATFORM_ANDROID
    RewardedInterstitialAd->Load("ca-app-pub-3940256099942544/5354046379");
    #elif PLATFORM_IOS
    RewardedInterstitialAd->Load("ca-app-pub-3940256099942544/6978759866");
    #endif
    ```

=== "Blueprints"

    ![](../assets/LoadRewardedInterstitialAd.png)

!!! tip

    You can use ad load calls to build up a cache of preloaded ads before you intend to show them, so that ads can be shown with zero latency when needed. Since ads expire after an hour, you should clear this cache and reload with new ads every hour. You can see an example of how it can be implemented in the C++ sample project at the bottom of the page.

## Show the ad

!!! note

    Before displaying a rewarded interstitial ad to users, you must present the user with an intro screen that provides clear reward messaging and an option to skip the ad before it starts. [Learn more](https://support.google.com/admob/answer/9884467)

When you show a rewarded interstitial ad, you use a __`OnUserEarnedReward`__ delegate to handle reward events. To show a rewarded interstitial ad, use it's __`Show()`__ method.

=== "C++"

    ``` c++
    RewardedInterstitialAd->OnUserEarnedReward.AddLambda([](int32 RewardAmount, const FString& RewardType)
    {
        UE_LOG(LogTemp, Display, TEXT("User earned reward: %d %s"), RewardAmount, *RewardType);
    });
    if (RewardedInterstitialAd->IsReady())
    {
        RewardedInterstitialAd->Show();
    }
    ```

=== "Blueprints"

    ![](../assets/ShowRewardedInterstitialAd.png)

## Ad events

You can listen for a number of events in the ad's lifecycle, including loading, ad impression and click, as well as show and dismiss events. But for the rewarded interstitial ad the most important is __`OnUserEarnedReward`__. It is recommended to bind to the delegates before loading the ad.

=== "C++"

    ``` c++
    #include "GoogleAdMobRewardedInterstitialAd.h"
    #include "GoogleAdMobResponseInfo.h"
    #include "GoogleAdMobAdError.h"
    #include "GoogleAdMobAdValue.h"
    // ...
    RewardedInterstitialAd->OnLoaded.AddLambda([](const UGoogleAdMobResponseInfo& ResponseInfo){});
    RewardedInterstitialAd->OnFailedToLoad.AddLambda([](const UGoogleAdMobAdError& LoadAdError, const UGoogleAdMobResponseInfo& ResponseInfo){});
    RewardedInterstitialAd->OnClicked.AddLambda([](){});
    RewardedInterstitialAd->OnImpression.AddLambda([](){});
    RewardedInterstitialAd->OnShown.AddLambda([](){});
    RewardedInterstitialAd->OnFailedToShow.AddLambda([](const UGoogleAdMobAdError& AdError){});
    RewardedInterstitialAd->OnDismissed.AddLambda([](){});
    RewardedInterstitialAd->OnPaidEvent.AddLambda([](const UGoogleAdMobAdValue& AdValue){});
    RewardedInterstitialAd->OnUserEarnedReward.AddLambda([](int32 RewardAmount, const FString& RewardType){});
    ```

=== "Blueprints"

    ![](../assets/RewardedAdDelegates.png)

## Validate server-side verification (SSV) callbacks `(Optional)`

Apps that require extra data in [server-side verification](https://developers.google.com/admob/unity/ssv) callbacks should use the custom data feature of rewarded ads. Any string value set on a rewarded ad object is passed to the __`custom_data`__ query parameter of the SSV callback. If no custom data value is set, the __`custom_data`__ query parameter value won't be present in the SSV callback.

=== "C++"

    ``` c++
    #include "GoogleAdMobRewardedInterstitialAd.h"
    // ...
    RewardedInterstitialAd->SetSSVCustomData(TEXT("CUSTOM_DATA_STRING"));
    ```

=== "Blueprints"

    ![](../assets/SetSSVCustomData.png)

If you want to set the custom reward string, you must do so before showing the ad.

!!! note

    The custom reward string is [percent escaped](https://en.wikipedia.org/wiki/Percent-encoding) and might require decoding when parsed from the SSV callback.

## Sample projects

- [Blueprint](https://deepinnothing.github.io/sample-projects/unreal-engine/google-admob/google-admob-bp.zip)
- [C++](https://deepinnothing.github.io/sample-projects/unreal-engine/google-admob/google-admob-cpp.zip) 