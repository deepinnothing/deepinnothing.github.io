[If you like this plugin, please, rate it on Fab. Thank you!](https://fab.com/s/b1cdf3b0e8c8){ .md-button .md-button--primary .full-width }

# Get started

This page outlines how to set up [Google Play Games Services](index.md) and then add features to an Android game. The tasks include setting up the service in Google Play Console and adding features to your game, such as sign-in and achievements.

Here are the basic steps to set up Play Games Services for a game and add features:

1.  [Set up](#set-up-play-games-services) Play Games Services.
2.  [Set up your game project](#set-up-your-game-project) and integrate the Sign-in service.
3.  Set up and [add features](#add-additional-features) to your game project.
4.  [Test and Publish](#test-and-publish-play-games-services-updates) any changes you make to your Play Games Services features.

## Before you start

You must have a Google Play Developer account set up in Play Console. For more information, see the [Register for a Google Play Developer account](https://support.google.com/googleplay/android-developer/answer/6112435).

## Set up Play Games Services
To set up Play Games Services in Google Play Console, see the [set up](console/setup.md) guide.

## Set up your game project
You must set up a Play Games Services in your game project and integrate the [Sign-in service](features/sign-in.md) before adding other Play Games Services features. See the [Set up Unreal Engine project](ue-setup.md) for details.

## Add additional features
After you integrate sign-in, you can add additional Play Games Services features to your game. This involves setting up the feature in Play Console and then integrating it in your game project. See the following guides for details:

* [__`Achievements`__](features/achievements.md)
* [__`Leaderboards`__](features/leaderboards.md)
* [__`Events`__](features/events.md)
* [__`Friends`__](features/friends.md)
* [__`Saved Games`__](features/saved-games.md)
* [__`Player Stats`__](features/player-stats.md)

## Test and publish Play Games Services updates
After you add Play Games Services features to your game project, you should test any updates you make to your Play Games Services project using the [test accounts](console/setup.md#enable-testing) that you setup when you configure Play Games Services. When you're satisfied with the changes, you can use Play Console to [publish](https://developer.android.com/games/pgs/console/publish) them. This does not involve publishing your game, and only updates the Play Games Services project and services hosted by Google.