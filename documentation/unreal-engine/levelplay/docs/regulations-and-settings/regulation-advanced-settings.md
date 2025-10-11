[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/804df971aef3){ .md-button .md-button--primary .full-width }

# Regulation Advanced Settings

## GDPR – Managing Consent

LevelPlay’s mediation platform supports publisher communication of a user’s consent choice to mediated networks (for supported networks).

Starting from ironSource SDK 7.7.0, GDPR consent from the [Google UMP](https://fab.com/s/b1cdf3b0e8c8) (Google’s CMP solution) and CMPs that support Google Additional Consent, is automatically passed to the ironSource SDK. Learn more [here](https://developers.is.com/ironsource-mobile/general/making-sure-youre-compliant-post-gdpr/#step-5).

To use ironSource’s API to update a user’s consent status, use this function:

=== "C++"

    ``` c++
    ULevelPlay::SetConsent(true);
    ```

=== "Blueprints"

    ![](assets/SetConsent.png)

If the user provided consent, please set the __`bHasUserConsent`__ flag to true. If user did not consent, set it to false.

A detailed article regarding ironSource’s approach to GDPR and user consent can be found [here](https://developers.is.com/ironsource-mobile/general/making-sure-youre-compliant-post-gdpr/).

## US Privacy compliance

LevelPlay’s mediation platform (SDK Version 6.14.0 and above) supports publishers to restrict the sale or sharing of end users personal information under U.S. state privacy laws, such as the California Privacy Rights Act (CPRA).

The notification about personal information of specific users located in California, Virginia, Connecticut, Colorado or Utah, should be handled based on a “do not sell” setting by setting its value to “true” or “false”.

The API should be set before initializing the SDK.

If the user has opted out of “sale” or “sharing” of personal information, set __`bDoNotSell`__ to true, otherwise, if “sale” of personal information is permitted, set it to false:

=== "C++"

    ``` c++
    ULevelPlay::SetDoNotSell(false);
    ```

=== "Blueprints"

    ![](assets/SetDoNotSell.png)

A detailed article regarding ironSource’s approach to U.S state privacy laws can be found [here](https://developers.is.com/ironsource-mobile/general/ironsource-ccpa-compliance/).

## User-Level Settings for Child-Directed Apps with Age Gates

LevelPlay’s mediation platform (SDK Version 7.1.0+) enables publishers of child-directed apps to flag specific end-users as children, as may be permitted or required by applicable law (e.g. COPPA). Publishers of child-directed apps are responsible for determining whether an app is permitted to flag at the end-user level or must treat all end-users as children. Publishers should consult with their legal counsel accordingly.

The indication of whether a specific end-user is a child should be done using a “is_child_directed” flag, by setting its value to “true” or “false”.

The API should be set before initializing the SDK.

If the end-user is a child (as defined by applicable regulations), set __`bIsChildDirected`__ to true. If the end-user is not a child, set it to false:

=== "C++"

    ``` c++
    ULevelPlay::SetIsChildDirected(false);
    ```

=== "Blueprints"

    ![](assets/SetIsChildDirected.png)

A detailed article regarding ironSource’s approach to child-directed apps can be found [here](https://developers.is.com/ironsource-mobile/general/ironsource-mobile-child-directed-apps/).

## Google Play Families policy

Follow these steps if any of your apps participate in Google Play’s[ Designed for Families program](https://support.google.com/googleplay/android-developer/answer/9893335), are listed in Google Play’s Family section, or include children as one of the target audiences.

In addition, make sure you flag your apps for COPPA compliance on the LevelPlay platform. You can learn more [here](https://developers.is.com/ironsource-mobile/general/ironsource-mobile-child-directed-apps/#step-1).

### Guidance for apps directed at children

If your apps are primarily directed at children, follow these steps:

1. Use [Unreal Plugin Language](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-the-unreal-plugin-language-for-mobile-projects-in-unreal-engine) to remove __`com.google.android.gms.permission.AD_ID`__ permission and prevent access to the Android Advertising ID (AAID):

    ``` xml
    <removePermission android:name="com.google.android.gms.permission.AD_ID"/>
    ```

2. Use the ironSource Metadata API to prevent access to the AAID for end-users flagged as children. Make sure you do this before initializing the ironSource SDK.

    === "C++"

        ``` c++
        ULevelPlay::SetMetaData("is_deviceid_optout", "true");
        ULevelPlay::SetIsChildDirected(true);
        ```

    === "Blueprints"

        ![](assets/GooglePlayFamilies1.png)

### Guidance for apps directed at a mixed audience

For children or users of unknown age, use the ironSource Metadata API to prevent access to the AAID, allow initialization of only the [Families Self-Certified Ads SDKs](https://support.google.com/googleplay/android-developer/answer/12955712?visit_id=638199973493653372-1399179309&rd=2), and flag those users as children. Make sure you do this before initializing the ironSource SDK.

=== "C++"

    ``` c++
    ULevelPlay::SetMetaData("is_deviceid_optout", "true");
    ULevelPlay::SetIsChildDirected(true);
    ULevelPlay::SetMetaData("Google_Family_Self_Certified_SDKS", "true");
    ```

=== "Blueprints"

    ![](assets/GooglePlayFamilies2.png)

!!! warning

    Preventing access to the GAID (Android Advertising ID) might impact your monetization performance. Make sure these integration changes are made for required apps and users only. 