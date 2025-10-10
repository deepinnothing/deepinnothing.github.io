[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/804df971aef3){ .md-button .md-button--primary .full-width }

# Get started

This guide will walk you through the steps you need to take to start monetizing with Unity LevelPlay. Begin by learning how to integrate LevelPlay into your app and then move on to setting up your app and mediated networks on the LevelPlay platform.

!!! note

    The SDK download, use, and access are subject to the [Unity Advertising Terms of Service](https://unity.com/legal/one-operate-services-terms-of-service). If you disagree with Unity Advertising Terms of Service, do not download, access, or use the SDK or the underlying services.

## Integration

### 1. Install & configure LevelPlay in your app

Install the LevelPlay package, complete a basic integration and go over some platform-specific configuration.

[Learn how]()

### 2. Integrate your ad units

Register listeners and integrate the relevant APIs in your apps’ code for the type of ads you want to show in your app.

[Learn how]()

### 3. Set user IDs

If you’re using server-to-server callbacks to reward your users, you’re required to define a unique identifier for each user.

[Learn how]()

### 4. Add mediation network adapters and SDKs

Integrate the network adapters and SDKs for the mediated networks you want to use on LevelPlay.

[Learn how]()


## Platform

### 1. Add your apps to your account

Set up the apps you’d like to monetize and promote through Unity LevelPlay. Automate your setup by [cloning app settings](https://developers.is.com/ironsource-mobile/air/adding-app/#step-2) or by using the [Application API](https://developers.is.com/ironsource-mobile/air/application-api/).

[Watch the how-to video](https://developers.is.com/ironsource-mobile/general/start-monetizing-app/)

### 2. Add networks

Activate the networks you want to serve ads from on the platform and integrate their adapters. Automate network setup [via CSV](https://developers.is.com/ironsource-mobile/general/import-export-csv/).

[Learn how](https://developers.is.com/ironsource-mobile/unity/mediation-networks-unity)

### 3. Set up your ad units

Configure the types of ads you want to serve in your apps.

[Learn how](https://developers.is.com/ironsource-mobile/general/how-to-manage-your-ad-units-1/)

### 4. Set up your hybrid waterfalls and instance rates

Create mediation groups and add [instance rates](https://developers.is.com/ironsource-mobile/air/mediation-management-3/) for non-bidding networks.

[Watch the how-to video](https://developers.is.com/ironsource-mobile/general/waterfall-tiers-types/)

### 5. Check your app keys

Confirm that each app key from the LevelPlay platform is correct in your apps’ integration.

[See your app keys](https://platform.ironsrc.com/partners/dashboard)

## Verification

### Confirm integration success and test ad delivery

Use the integration test suite to confirm each ad source you’ve set up on LevelPlay is correctly configured and check ads are loading and displaying successfully.

[Learn how](https://developers.is.com/ironsource-mobile/unity/unity-levelplay-test-suite/)

Register your app as an AdMob app by completing the following steps:

1.  [Sign in to](https://admob.google.com/home/) or [sign up for](https://support.google.com/admob/answer/7356219) an AdMob account.
2.  [Register your app with AdMob](https://support.google.com/admob/answer/2773509). This step creates an AdMob app with a unique [AdMob App ID](https://support.google.com/admob/answer/7356431) that is needed later in this guide.

## Configure your project

### Enable the plugin

The plugin is typically enabled by default upon installation. However, if it's not, follow these steps:

1.  Navigate to __Edit > Plugins__ in Unreal Engine.
2.  Search for `Google AdMob` in the plugin list.
3.  If the plugin is disabled, enable it by checking the corresponding box.

### Disable build-in AdMob support for ads

To avoid any compatibility issues and library conflicts, disable build-in AdMob support. Inside __Project Settings__, go to __Platforms > Android > Google Play Services__ and set __`Include AdMob support for ads`__ to __`false`__.

![Disable build-in AdMob support](assets/DisableBuildInAdMob.png)

### Enter your App IDs in Project Settings

1.  Copy your AdMob app ID, as [identified in the AdMob web interface](https://support.google.com/admob/answer/7356431). You'll need to separate AdMob apps for Android and iOS, i.e. two different App IDs for these platforms.
2.  Open __Project Settings > Plugins > Google AdMob__ in Unreal Engine, and paste the copied values into the corresponding AdMob App ID fields.

![AdMob App ID fields example](assets/AdMobAppIDFields.png)

### Add dependency to your modules `(C++ projects)`

To use the plugin in your C++ code, you must include `GoogleAdMob` as either a public or private dependency in your module's build configuration, for example:
``` c#
PrivateDependencyModuleNames.Add("GoogleAdMob");
```

### Obtain consent from a user

It's highly recommended that you use [Google UMP plugin](https://fab.com/s/b1cdf3b0e8c8) or some other CMP solution to obtain consent from a user before requesting ads, so you comply with GDPR rules in EEA countries and US privacy regulations (e.g. CCPA). If you decide to use a custom solution, you can use the following functions to pass consent choices to the Google AdMob SDK (should be done before initializing it):

=== "C++"

    ``` c++
    UGoogleAdMob::SetConsent(bIsConsentRequired, bHasUserConsent); // For GDPR
    UGoogleAdMob::SetRestrictedDataProcessing(bRestrictedDataProcessing); // For US privacy regulations
    ```

=== "Blueprints"

    ![](assets/SetConsent.png)


### Initialize the SDK

Before loading ads, have your app initialize Google Mobile Ads SDK by calling __`UGoogleAdMob::Initialize()`__. This needs to be done only once, ideally at app launch.

!!! warning

    If you need to obtain consent from users in the European Economic Area (EEA), set any request-specific flags, such as __`SetTagForChildDirectedTreatment()`__ or __`SetTagForUnderAgeOfConsent()`__, or otherwise take action before loading ads, ensure you do so before initializing Google Mobile Ads SDK.

Here's an example of how to call __`UGoogleAdMob::Initialize()`__:

=== "C++"

    ``` c++
    UGoogleAdMob::OnInitializationComplete.AddLambda([]()
    {
        UE_LOG(LogTemp, Display, TEXT("Google Mobile Ads initialization complete."));
    });
    UGoogleAdMob::Initialize();
    ```

=== "Blueprints"

    ![](assets/InitializeTheSDK.png)

If you're using mediation, wait until the callback occurs before loading ads to verify that all mediation adapters are initialized.

!!! note

    It's recommended to use some Consent Management Platform to obrain user consent in EEA and regulated US states before initializing the SDK. You can use [Google UMP plugin](https://fab.com/s/b1cdf3b0e8c8) for that. It is also required by __`Enable automatic SDK initialization on start-up`__ setting in Project Settings, which allows you to skip this step entirely.

## Select an ad format

Google Mobile Ads SDK is now imported and you're ready to implement other ads. AdMob offers a number of different ad formats, so you can choose the one that best fits your app's user experience.

### Banner

![](assets/format-banner.png){ align=left }

Banner ad units display rectangular ads that occupy a portion of an app's layout. They can refresh automatically after a set period of time. This means users view a new ad at regular intervals, even if they stay on the same screen in your app. They're also the simplest ad format to implement.

[Implement banner ads](ad-formats/banner-ads.md){ .md-button .md-button--primary align-left }

### Interstitial

![](assets/format-interstitial.png){ align=left }

Interstitial ad units show full-page ads in your app. Place them at natural breaks and transitions in your app's interface, such as after level completion in a gaming app.

[Implement interstitial ads](ad-formats/interstitial-ads.md){ .md-button .md-button--primary align-left }

### Rewarded

![](assets/format-rewarded.png){ align=left }

Rewarded ad units enable users to play games, take surveys, or watch videos to earn in-app rewards, such as coins, extra lives, or points. You can set different rewards for different ad units, and specify the reward values and items the user received.

[Implement rewarded ads](ad-formats/rewarded-ads.md){ .md-button .md-button--primary align-left }

### Rewarded interstitial

![](assets/format-rewarded-interstitial.png){ align=left }

Rewarded interstitial is a new type of incentivized ad format that lets you offer rewards, such as coins or extra lives, for ads that appear automatically during natural app transitions.

Unlike rewarded ads, users aren't required to opt in to view a rewarded interstitial.

Instead of the opt-in prompt in rewarded ads, rewarded interstitials require an intro screen that announces the reward and gives users a chance to opt out if they want to do so.

[Implement rewarded interstitial ads](ad-formats/rewarded-interstitial-ads.md){ .md-button .md-button--primary align-left }

### App open

![](assets/format-app-open.png){ align=left }

App open is an ad format that appears when users open or switch back to your app. The ad overlays the loading screen.

[Implement app open ads](ad-formats/app-open-ads.md){ .md-button .md-button--primary align-left }

## Sample projects

- [Blueprint](https://deepinnothing.github.io/sample-projects/unreal-engine/google-admob/google-admob-bp.zip)
- [C++](https://deepinnothing.github.io/sample-projects/unreal-engine/google-admob/google-admob-cpp.zip)