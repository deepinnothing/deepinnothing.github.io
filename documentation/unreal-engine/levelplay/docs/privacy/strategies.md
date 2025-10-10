[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/804df971aef3){ .md-button .md-button--primary .full-width }

# Privacy strategies

For key considerations when preparing your app for Google Play and Android privacy changes, see [Privacy strategies for Android](https://support.google.com/admob/answer/11402075).

For iOS privacy changes, check out [Privacy strategies for iOS](https://support.google.com/admob/answer/9997589).

## Publisher first-party ID

The Google Mobile Ads Unity Plugin introduced [Publisher first-party ID](https://support.google.com/admob/answer/9997589), to help you deliver more relevant and personalized ads by using data collected from your apps.

Publisher first-party ID is enabled by default, but you can disable it using the following function:

=== "C++"

    ``` c++
    #include "GoogleAdMob.h"
    // ...
    UGoogleAdMob::SetPublisherFirstPartyIdEnabled(false);
    ```

=== "Blueprints"

    ![](../assets/SetPublisherFirstPartyIdEnabled.png)