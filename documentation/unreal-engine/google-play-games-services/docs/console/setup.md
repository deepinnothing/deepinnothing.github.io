[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/b1cdf3b0e8c8){ .md-button .md-button--primary .full-width }

# Set up Google Play Games Services
This page describes how to set up Google Play Games Services for your Android game using Google Play Console. The Play Console provides a centralized place for you to manage game services and configure the metadata used to authorize and authenticate your game.

To add your game to Play Console, follow these general steps:

* Create a game project for your game and specify details such as the name and description of the game.
* Create and link the necessary credentials to authorize and authenticate your game to Google Play Games Services.

## Before you start
Complete the following steps before you configure Google Play Games Services.

### Create a Google Play Developer account
* You must have a Google Play Developer account set up in Play Console. For more information, see the [Register for a Google Play Developer account](https://support.google.com/googleplay/android-developer/answer/6112435).
* Grant edit permission: To edit Play Games Services settings in Play Console, your team must have permissions to manage Play Games Services. For more information, see [Add developer account users and manage permissions](https://support.google.com/googleplay/android-developer/answer/9844686).

### Create a Google Cloud project
You must have set up a Google Cloud project. For more information, see

* [__`Get started with Google Cloud`__](https://cloud.google.com/docs/get-started).
* [__`Creating a project`__](https://cloud.google.com/resource-manager/docs/creating-managing-projects#creating_a_project).

To use Play Console, you must enable the APIs you plan to use with your project.

=== "Console"
    
    [Enable Google Play Games Services API](https://console.cloud.google.com/apis/api/games.googleapis.com){ .md-button }

=== "gcloud"
    
    ```
    gcloud services enable \
       --project "PROJECT" \
       "games.googleapis.com"
    ```

## Sign in to the Play Console
To sign in, go to [Google Play Console](https://play.google.com/apps/publish/). If you haven't registered for the Play Console before, you will be prompted to do so.

## Add your game to the Play Console

To add your game, follow these steps:

1.  In Play Console, create an app and specify that it's a game. For more information, see [Create and set up your app](https://support.google.com/googleplay/android-developer/answer/9859152).
2.  Go to __Grow users > Play Games Services > Setup and management > Configuration__.
3.  Specify whether your game already uses Google APIs (such as Firebase). It is important that you choose the correct option; otherwise, your game may experience issues when using Google APIs. Here's the options:
    *   __No, my game doesn't use Google APIs__: If you are creating a new game, or you have never set up a Google API for it, choose this option. Enter your game's name and then click __Create__.
    *   __Yes, my game already uses Google APIs__: Choose this option if you have already set up a Google API for the Game. If this is the case, you will see a list of your projects from Google Cloud Console. Select your project from the list and then click __Use__.
    *   __Use an existing Play Games Services project__: If you want to use an existing Play Games Services project, choose this option. You will see a list of existing Play Games Services projects for your account. Select your game project from the list and then click __Use__. This is not a typical choice, but you might do this if you are creating a new game in Play Console in order to change the package name, or if you have free and paid versions of your game with different package names.

    A Play Games Services game project is created, and a corresponding entry is created for you in the [Google Cloud Console](https://developers.google.com/games/services/console/console.developers.google.com/apis/api).

4.  In the __Properties__ section, click __Edit Properties__ to add information, such as the description, category, and graphic assets for your game. Here's some guidelines for coonfiguring the properties:
    *   Only the display name is required for testing. The other fields must be filled out before you can publish your game.
    *   The display name and description for your game should match what you set up in your game's Play Store listing.
    *   For guidelines on creating the graphic assets, see [Add preview assets to showcase your app](https://support.google.com/googleplay/android-developer/answer/9866151) and the [Google Play Featured-Image Guidelines](https://android-developers.blogspot.com/2011/10/android-market-featured-image.html).

## Generate an OAuth 2.0 client ID

Your game must have an OAuth 2.0 client ID in order to be authenticated and authorized to call the Google Play Games Services. To set up a credential for Play Games Services, which is the association between a client ID and your game, use Google Cloud Platform to create the client ID. Then, use Google Play Console to add a credential, linking the client ID to your game.

For more detailed instructions, see the following steps:

### Configure the OAuth consent screen

If you haven't yet configured the OAuth consent screen, the __Credentials__ section will display a message prompting you to configure.
![Configure OAuth Prompt](https://developer.android.com/static/images/games/pgs/pgs-configure-oauth-prompt.png)
Click __Configure__. This opens a dialog with further instructions and a deep link to the Google Cloud Platform.
![Configure OAuth Instructions](https://developer.android.com/static/images/games/pgs/pgs-configure-oauth-instructions.png)
Make sure that the consent screen is available to everyone that the game is available to. The final list of scopes needs to include `games`, `games_lite`, and `drive.appdata`; none of these scopes will require app verification. We recommend publishing the consent screen immediately. If that is not possible, you can make the consent screen available to testers to allow them to sign-in to the game.

If you have completed the setup of the OAuth consent screen, click __Done__. Google Play Console refreshes automatically, and if configuration was successful you will be able to create a credential:
![Credentials](https://developer.android.com/static/images/games/pgs/pgs-credentials.png)

### Create a credential

In order to authorize your game to communicate with Google Play Games Services, you must create a credential with an authorized OAuth2 client ID.

In the __Credentials__ section, click __Add credential__.

In the wizard, choose whether you want to create an Android credential (if your game APK will authenticate the user and use Play Games Services APIs) or a game server credential (if your game server will use Play Games Services APIs). Follow the instructions specific to your desired credential type.

=== "Android"
    
    #### Set up credentials details

    Ensure that the name in the __Name__ field matches the name of your game. Choose whether to enable [Anti-Piracy](https://developer.android.com/games/pgs/android/anti-piracy).

    #### Set up authorization

    Next, choose an OAuth client ID to use for this game project. If you already have OAuth2 client IDs, you can choose one. However, you will usually create a new one. Click __Create OAuth client__. This opens a dialog with deep links and instructions for creating an OAuth Client ID in Google Cloud Platform.
    
    1.  Select __Android__ as the application type.
    2.  Enter your game's name in the __Name__ field.
    3.  Enter your Android application's [package name](https://developer.android.com/guide/topics/manifest/manifest-element#package) in the __Package name__ field.
    4.  This step varies depending on whether you use the App Signing Key provided by Google or your own.

        === "Google App Signing Key"

            Copy the SHA-1 certificate fingerprint from the dialog with the instructions for creating an OAuth Client ID in Google Cloud Platform. If it was closed, you can find the SHA-1 certificate fingerprint in the __Test and release > Setup > App Signing > App signing key certificate__ section.
            
            !!! note
                
                In this case, the OAuth client will only work with the version downloaded from the Google Play Store. Google replaces the key specified in Unreal Engine's Project Settings with their own App Signing Key when uploaded to Play Console. To enable Google Play Games Services to work with Unreal Engine builds instead, you must use SHA-1 certificate fingerprint of the Upload Key.

        === "Own App Signing Key"

            1.  Open a terminal and run the [Keytool utility](https://developer.android.com/studio/publish/app-signing) to get the SHA1 fingerprints of the release and debug certificates.
            2.  To get the certificate fingerprint, run the following command:
                `keytool -list -keystore <path-to-keystore> -v`
            3.  The keytool utility prompts you to enter a password for the keystore. The keytool then prints the fingerprint to the terminal.

            !!! warning
                Make sure to record the package name and signing certificate that you configured in this step. Using a different certificate or package name in your application will cause authentication failures.

    5.  Paste the SHA1 fingerprint into the __Signing certificate fingerprint (SHA1)__ field.
    6.  Click __Create__.

    For more information about OAuth 2.0 on Android, see [Authenticating to OAuth2 Services](https://developer.android.com/training/id-auth/authenticate).

    After you click __Done__ in the dialog, the available Client IDs will refresh. Choose the credential you created from the drop-down menu and then click __Save Changes__. This creates the credential as a draft, enabling you to authenticate to Play Games Services in your game.

=== "Game server"

    #### Set up credentials details

    Ensure that the name in the __Name__ field matches the name of your game.

    #### Set up authorization

    Next, choose an OAuth client ID to use for this game project. If you already have OAuth2 client IDs, you can choose one. However, you will usually create a new one. Click __Create OAuth client__. This opens a dialog with deep links and instructions for creating an OAuth Client ID in Google Cloud Platform.

    1.  Select __Web application__ as the application type.
    2.  Enter your game's name in the __Name__ field.
    3.  Click __Create__.
    
    For more information about OAuth 2.0 on Android, see [Authenticating to OAuth2 Services](https://developer.android.com/training/id-auth/authenticate).

    After you click __Done__ in the dialog, the available Client IDs will refresh. Choose the credential you created from the drop-down menu and then click __Save Changes__. This creates the credential as a draft, enabling you to authenticate to Play Games Services from your game server. For more information about using Play Games Services with your game server, see [Enabling Server-Side Access to Google Play Games Services](https://developer.android.com/games/pgs/android/server-access).

## Enable testing

To ensure that Google Play Games Services is functioning correctly in your game, you should test your game services before publishing your game changes on Google Play.

If your game is in an unpublished state, grant access to your testers by adding their user accounts to the allowlist. Otherwise, your testers will encounter OAuth and 404 errors when attempting to access Play Games Services endpoints, such as the [sign-in](../features/sign-in.md) endpoint.

Users with authorized test accounts will have access to your unpublished Play Games Services game project and can test that your configured Play Games Services are working correctly.

!!! note
    Remember to add yourself as a tester, or the Play Games SDK will not work for your user account.

There are two ways to enable testers to use Play Games Services APIs for your game:

*   At an individual level, by adding individual email addresses.
*   At a group level, by enabling Play Games Services for a Play Console release track.

To add individual testers to your game project:

1.  Open the __Testers__ tab for your game in the Google Play Console (__Grow users > Play Games Services > Setup and management > Testers__).
2.  Click the __Add testers__ button.
3.  In the dialog that appears, enter the email addresses of the Google Accounts that you wish to add as testers (separated with commas or one email address per line).
4.  Click Add to save the users as testers. The tester accounts you added should be able to access Play Games Services within a couple of hours.
![Console Add Testers](https://developer.android.com/static/images/games/pgs/consoleAddTesters.png)

To give testing access to a group, enable a release track to access Play Games Services:

Google Play makes it easy to distribute pre-release versions of your app to controlled groups of trusted users with the release track features. See [Set up an open, closed, or internal test](https://support.google.com/googleplay/android-developer/answer/9845334) on the Google Play Help website.

You can grant access to test your game to all users who have access to test APKs on a given release track. This works the same as if you had added them to the tester list individually. To do this, follow these steps:

1.  Open the __PGS Testers__ section (__Grow users > Play Games Services > Setup and management > Testers__) and select the __Release tracks__ tab. On this page, you can also see the list of tracks that are already enabled for Play Games Services testing.
2.  Click __Add tracks__.
3.  Select one or more tracks to enable for Play Games Services testing.
4.  Click __Add Tracks__.

The selected release tracks will now appear on the list of tracks enabled for Play Games Services testing.

This feature is only available if you have an Android app linked to your game in Google Play Console.

## Avoid common issues

To avoid common setup mistakes, make sure to follow these recommendations when setting up your game to use Google Play Games Services.

1. __Set up your game with the Play Console__
    
    If you created an Oauth 2.0 client ID for your app in the Google Cloud Console, Google Play Games Services will not know about the association between the game’s achievement and leaderboards and the client ID. To create this association, you must create a credential using the Oauth 2.0 client ID as described in [Create a credential](#create-a-credential).

2.  __Use the correct application ID in Android__
    
    The application ID is a required string resource that you must reference in your Android manifest. The application ID string consists only of the digits (typically 12 or more) at the beginning of the client ID provided by the Play Console. The application ID can be found at the top of the Configuration page and is labeled as Project ID below the name of your game.

3.  __Sign your APK with the correct certificate__
    
    When linking your Android app to your game in the Play Console, you must use exactly the same package name and certificate fingerprint that you used to publish your app, i.e. App Signing Key certificate. If there is a mismatch, calls to Google Play Games Services will fail. You can create two client IDs, one with the App Signing Key certificate fingerprint and another with the Upload Key certificate fingerprint, and use the same package name for both. To learn more about how to specify the signing certificate in the Play Console, see [Signing Your Applications](https://developer.android.com/tools/publishing/app-signing).

4.  __Sign in with a tester account during development__
    
    If you have not published your game setting changes in the Play Console, you might encounter errors during testing if you are not signed in with a whitelisted tester account. You should always enable your Play Console publisher account for testing. To learn how to manage tester accounts, see [Enabling accounts for testing](https://developer.android.com/games/pgs/console/publish#enable_accounts_for_testing).

5.  __Publish the consent screen in Google Cloud Platform__
    
    Before publishing the app in the Play Console, publish the consent screen in Google Cloud Platform. Without this step, the public audience won’t be able to use any of the Play Games Services features.

7.  __At release, publish the Play Games Services settings first before you publish your game__
    
    Developers might accidentally publish their app without publishing the corresponding Play Games Services settings for their app. This might cause players who are signing in with non-tester accounts to encounter errors since the app cannot reference the correct game settings. When releasing your game, remember to first publish your game settings by using the Publish Game option in the Play Console. To learn how to publish your changes, see [Publishing your game changes](https://developer.android.com/games/pgs/console/publish#publish_game_changes).

For additional tips, see the [Android troubleshooting guide](https://developer.android.com/games/pgs/android/troubleshooting).

## Next steps

Once you complete the initial setup tasks described above, you can [enable Play Games Services features](https://developer.android.com/games/pgs/console/enable-features) for your game, such as saved games, leaderboards, and achievements.