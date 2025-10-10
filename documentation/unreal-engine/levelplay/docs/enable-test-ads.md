[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/804df971aef3){ .md-button .md-button--primary .full-width }

# Enable test ads

This guide explains how to enable test ads in your ads integration. It's important to enable test ads during development so that you can click them without charging Google advertisers. If you click too many ads without being in test mode, you risk your account being flagged for invalid activity.

There are two ways to get test ads:

1.  Use one of Google's [demo ad units](#demo-ad-units).
2.  Use your own ad unit and [enable test devices](#enable-test-devices).

## Demo ad units

The quickest way to enable testing is to use Google-provided demo ad units. These ad units are not associated with your AdMob account, so there's no risk of your account generating invalid traffic when using these ad units.

!!! warning

    Make sure you replace these IDs with your own ad unit ID before publishing your game.

Here are demo ad units that point to specific test creatives for each format:

| Ad format             | Android demo ad unit ID                | iOS demo ad unit ID                    |
| --------------------- | -------------------------------------- | -------------------------------------- |
| App Open              | ca-app-pub-3940256099942544/9257395921 | ca-app-pub-3940256099942544/5575463023 |
| Adaptive Banner       | ca-app-pub-3940256099942544/9214589741 | ca-app-pub-3940256099942544/2435281174 |
| Fixed Size Banner     | ca-app-pub-3940256099942544/6300978111 | ca-app-pub-3940256099942544/2934735716 |
| Interstitial          | ca-app-pub-3940256099942544/1033173712 | ca-app-pub-3940256099942544/4411468910 |
| Rewarded Ads          | ca-app-pub-3940256099942544/5224354917 | ca-app-pub-3940256099942544/1712485313 |
| Rewarded Interstitial | ca-app-pub-3940256099942544/5354046379 | ca-app-pub-3940256099942544/6978759866 |
| Native                | ca-app-pub-3940256099942544/2247696110 | ca-app-pub-3940256099942544/3986624511 |
| Native Video          | ca-app-pub-3940256099942544/1044960115 | ca-app-pub-3940256099942544/2521693316 |

## Enable test devices

If you want to do more rigorous testing with production-looking ads, you can configure your device as a test device and use your own ad unit IDs that you've created in the AdMob UI. Test devices can either be added in the AdMob UI or programmatically using the plugin settings.

Follow the steps below to add your device as a test device.

!!! note

    Android emulators and iOS simulators are automatically configured as test devices.

### Add your test device in the AdMob UI

For a non-programmatic way to add a test device and test new or existing app builds, use the AdMob UI. [Learn how](https://support.google.com/admob/answer/9691433).

!!! note

    New test devices typically start serving test ads in your app within 15 minutes, but it can also take up to 24 hours.

### Add your test device programmatically

To register your test device, complete the following steps:

1.  Run your game on a device of your choice (iOS or Android), and make an ad request.
2.  Check the log output of your device for a message similar to the following examples, which shows your device ID:

    -   __iOS:__ 

        ```
        <Google> To get test ads on this device, set:
        GADMobileAds.sharedInstance.requestConfiguration.testDeviceIdentifiers =
        @[ @"2077ef9a63d2b398840261c8221a0c9b" ];
        ```

    -   __Android:__ 

        ```
        I/Ads: Use RequestConfiguration.Builder.setTestDeviceIds(Arrays.asList("33BE2250B43518CCDA7DE426D04EE231")) 
        to get test ads on this device.
        ```

3.  Copy your test device ID to your clipboard.
4.  Go to __Project Settings > Plugins > Google AdMob__ and paste the copied device ID into the appropriate Test Device IDs array:

    ![](assets/TestDeviceIDs.png)

    !!! warning

        Be sure to disable "Enable Test Devices" fields before you release your game.

5.  Rebuild your project and run it again. If the ad is a Google ad, you'll see a __Test mode__ label centered at the top of the ad (banner, interstitial, or rewarded ad):

    ![](https://developers.google.com/static/admob/images/ios-testad-0-admob.png)
    /// caption
    ///

## Testing with mediation

Google's demo ad units only show Google ads. To test your mediation configuration, you must use the [enable test devices](#enable-test-devices) approach.

Mediated ads do not render a __Test mode__ label. You are responsible for ensuring that test mode is enabled for each of your mediation networks so that these networks don't flag your account for invalid activity. See each network's respective [mediation guide]() for more information.

If you aren't sure whether a mediation ad network adapter provides a test mode, it is safest to avoid clicking on ads from that network during development. You can use the __`FGoogleAdMobResponseInfo::MediationAdapterClassName`__ property on any of the ad formats to figure out which ad network served the current ad.