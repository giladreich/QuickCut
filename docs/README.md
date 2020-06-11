
<p align="center"><img src="/media/logo.png" width=200 height=200></p>

# QuickCut

QuickCut is a shortcut keyboard manager that both facilitates key mapping and activates specified processes.

QuickCut allows for shorthand access to selected apps at the convenience of your chosen keyboard keys. It allows you to tailor your keyboard layout design making your time as effecient as possible.

## Getting Started

Download links can be found [here](https://github.com/giladreich/QuickCut/releases).


QuickCut is designed to use `Profiles` and `Actions` in a one to many relationship (each profile can create multiple actions) that are saved in `JSON` format. This enables convenient change between multiple profiles creating different working environments depending on the selected profile (making it easier to share your profiles with others).

### Creating your first `Profile`

When getting started with QuickCut, the following window appears:

![Create Profile](/media/create_profile.png)

![Create Profile](/media/first_main_window.png)

As soon as you click the `Default` button, all shortcuts for the selected profile will be activated.

### Creating your first `Action`

There are two types of actions:

* Key Mapping - maps a desired key to the specified key (no restart required).
* Start Application - maps a desired key to the specified action, i.e open specific directory or your favorite application.

`Key Mapping` looks as follows:

![Create Profile](/media/action_window_map_screenshot.png)

Clicking the record button will record the desired keycode:

![Create Profile](/media/action_window_map_screenshot_record.png)

`Start Application` looks as follows:

![Create Profile](/media/action_window_map_snippingtool.png)

SnippingTool will start as soon as we hit the F14 key.

Multiple keys can be used for a single shortcut:

![Create Profile](/media/action_window_map_keycombo.png)


This creates a configuration file under `Config/profiles.json` that can be easily shared:

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

Above is a demonstration of the basics of QuickCut; how to create profiles and diverse actions.

Once these steps are completed, you can use your computer as usual. 

Note that you no longer need to keep QuickCut GUI open unless you want to make changes.

### Quick Demo

##### Installing QuickCut

![Create Profile](/media/QuickCut_install.gif)

##### Creating your first action

![Create Profile](/media/QuickCut_SnippingTool.gif)

##### Opening your favorite directories

![Create Profile](/media/QuickCut_open_boost_dir.gif)

##### Choose your favorite theme

![Create Profile](/media/QuickCut_themes.gif)

### Prerequisites

Project Structure:

* QuickCut - the actual GUI.
* QuickCutConsole - maps the actual key strokes and parses the `profiles.json` file.
* QuickCutService - responsible for making sure that `QuickCutConsole` is running and also future updater is planned.

All QuickCut projects use Qt Framework and are designed to be portable across different platforms. Currently the code should compile on all platforms, however the `QuickCutConsole` and `QuickCutService` require a Unix hook implementation to make this work on Linux and MacOS which has yet to be developed (pull requests for this feature are welcomed!).

Requirements for building the project:

* Qt MSVC41 x64/x86 kits.
* VS MSVC41 compiler.
* Boost 1.69.0 MSVC141_x64/x86.
* VS17 and Qt VS Plugin(if you want to use the VS solution).
* CMake 3.12(will be supported later).


## Motivation

I always connect my laptop to a monitor using external keyboard and mouse.

This is preffered keyboard that I currently use at home:

![Apple Keyboard](/media/apple_keyboard.png)

It has F13 til F19 keys which are non-functioning keys I wanted to make use of. In looking for alternative solutions I came across `KeyTweak`, as well as other programs, that edit the registry using the default functionality Windows has for mapping keys. Unfortunately every time you map a key on the windows registry it requires a full reboot to the system in order to load the new key mapping layout. It also doesn't allow for mapping a combination of keys to a key.

This brought me to the idea of developing an interactive application that runs as a background service which does all sorts of key mapping invoking different actions to be as efficient as possible when using the keyboard.

For that reason `QuickCut` is here and free to use :).

## Contributing

Pull-Requests are greatly appreciated should you like to contribute to the project. 

Same goes for opening issues; if you have any suggestions, feedback or you found any bugs, please do not hesitate to open an [issue](https://github.com/giladreich/QuickCut/issues).

## Authors

* **Gilad Reich** - *Initial work* - [giladreich](https://github.com/giladreich)

See also the list of [contributors](https://github.com/giladreich/QtDirect3D/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

