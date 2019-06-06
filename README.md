
<p align="center"><img src="pictures/logo.png" width=256 height=256></p>

# QuickCut

QuickCut is a shortcut keyboard manager that allows you to map keys and activate specified processes.

QuickCut allows you shorthand access to your most used apps at the convenience of your chosen keyboard keys. It allows you to tailor your keyboard layoutdesign making your time as effecient as possible.


## Getting Started

Download links can be found [here](https://github.com/giladreich/QuickCut/releases).


QuickCut is designed to be as portable as possible by using `Profiles` and `Actions` in a one to many relationship(profile can create multiple actions) that are saved in `Json` format, so you can easily change between each profile to create different working environment depending on the selected profile(which also makes it easier sharing your profiles with others).

### Creating your first `Profile`

When you first starting QuickCut, you'll see the following window:

![Create Profile](/pictures/create_profile.png)

![Create Profile](/pictures/first_main_window.png)

Don't forget that you can create multiple profiles. As soon as you click the `Default` button, all the shortcuts for the selected profile will be activated.

### Creating your first `Action`

There are two type of actions:

* Key Mapping - Fully maps a desired key to the specified key(no restart required).
* Start Application - Maps a desired key to do the specified action, i.e open specific directory or your favorite application.

`Key Mapping` should look as the following:

![Create Profile](/pictures/action_window_map_screenshot.png)

As soon as you click the green record button, the keycode that related to that key you're targeting will be added to the text-box:

![Create Profile](/pictures/action_window_map_screenshot_record.png)

`Start Application` should look as the following:

![Create Profile](/pictures/action_window_map_snippingtool.png)

In this example, snipping tool will start as soon as we hit the F14 key.

We can also also use multiple keys for a shortcut:

![Create Profile](/pictures/action_window_map_keycombo.png)


So we end up with a configuration file under `Config/profiles.json` that can be easily shared with others:

```json
{
    "activeProfile": "{9d146d79-fba5-48bc-9841-aee8bea2826a}",
    "profileCount": 1,
    "profiles": [
        {
            "id": "{9d146d79-fba5-48bc-9841-aee8bea2826a}",
            "name": "Home Environment",
            "lastModified": "2019-06-06T08:16:26",
            "actionsCount": 2,
            "actions": [
                {
                    "id": "{9c6f3bba-fc58-4ebd-98ba-cc70fa503ba7}",
                    "actionName": "F13 Map to Screenshot",
                    "type": "KeyMap",
                    "srcKey": "7c",
                    "dstKey": "2c",
                    "appPath": "",
                    "appArgs": "",
                    "createdDate": "2019-06-06T07:52:24"
                },
                {
                    "id": "{e75e41f5-de4f-422d-952f-a0a91bcaf62b}",
                    "actionName": "CTRL+SHIFT+F14 Start SnippingTool",
                    "type": "AppStart",
                    "srcKey": "a2a07d",
                    "dstKey": "",
                    "appPath": "C:\/WINDOWS\/system32\/SnippingTool.exe",
                    "appArgs": "",
                    "createdDate": "2019-06-06T08:04:59"
                }
            ]
        }
    ]
}
```

### Summary

We covered the important basics of QuickCut, so we know how to interact with profiles and actions, but there are more features added to QuickCut that you can explore around.

Note that as soon as you're done creating your profiles and actions, you don't need to worry about the QuickCut GUI to be opened anymore. You can just use your computer regulary and everything should just work.

### Prerequisites

Project Structure:

* QuickCut - The actual GUI.
* QuickCutConsole - Mapps the actual key strokes and parses the `profiles.json` file.
* QuickCutService - Responsible for making sure that `QuickCutConsole` is running and also future updater is planned.

All QuickCut projects are using the Qt Framework and designed to be portable across different platforms. Currently the code should compile on all platforms, but the the `QuickCutConsole` and `QuickCutService` requires a Unix hook implementation to make this work on different platforms, which I didn't have the time yet to do so(pull requests for this feature are very welcomed!).

Requirements for building the project:

* Qt MSVC41 x64/x86 kits.
* VS MSVC41 compiler.
* Boost 1.69.0 MSVC141_x64/x86.
* VS17 and Qt VS Plugin(if you want to use the VS solution).
* CMake 3.12(will be supported later).


## Motivation

I always connect my laptop to a monitor using external keyboard and mouse.

This is my favorite keyboard that I currently use at home:

![Apple Keyboard](/pictures/apple_keyboard.png)

It has F13 til F19 keys which never worked for me, so I looked up for alternative solutions and I found out about `KeyTweak` and some other programs that all they do, is editing the registry using the default functionality Windows has for mapping keys. So I was excited for a second and I though that this actually did it for me, but unfortunately every time you map a key on the windows registry, that requires a full reboot to the system in order to load the new key mapping layout.
It also doesn't let you map a combination of keys to a key.

Long story short, I though about alternative solutions that brought me to the idea of making a fully interactive application that runs as a background service which does all the key mapping for me with the ability of creating different actions to be as efficient as possible when using the keyboard.

For that reason `QuickCut` is here and free to use :)

## Contributing

Pull-Requests are more than welcome and will be very appreciated. So feel free to contribute if you want to improve the project.

Same goes for opening issues, if you have any suggestions, feedback or you found any bugs, please do not hesitate to open an [issue](https://github.com/giladreich/QtDirect3D/issues).

## Authors

* **Gilad Reich** - *Initial work* - [giladreich](https://github.com/giladreich)

See also the list of [contributors](https://github.com/giladreich/QtDirect3D/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

