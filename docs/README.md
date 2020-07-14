
<p align="center"><img src="/media/logo.png" width=200 height=200></p>

<p align="center">
    <a href="https://github.com/giladreich/QuickCut/actions?query=workflow%3AWindows" alt="Windows Status">
        <img src="https://github.com/giladreich/QuickCut/workflows/Windows/badge.svg" /></a>
    <a href="https://github.com/giladreich/QuickCut/actions?query=workflow%3ALinux" alt="Linux Status">
        <img src="https://github.com/giladreich/QuickCut/workflows/Linux/badge.svg" /></a>
    <a href="https://github.com/giladreich/QuickCut/actions?query=workflow%3AMacOS" alt="MacOS Status">
        <img src="https://github.com/giladreich/QuickCut/workflows/MacOS/badge.svg" /></a>
    <a href="https://www.qt.io/" alt="Qt">
        <img src="https://img.shields.io/badge/CMake-Qt-brightgreen.svg?logo=cmake" /></a>
    <a href="http://makeapullrequest.com" alt="Pull Requests">
        <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg?logo=pre-commit" /></a>
</p>

---

# QuickCut

QuickCut is a shortcut keyboard manager that both facilitates key mapping and activates specified processes.

QuickCut allows for shorthand access to selected apps at the convenience of your chosen keyboard keys. It allows you to tailor your keyboard layout design making your time as efficient as possible.

## Contents

- [Getting Started](#getting-started)
  * [Introduction](#introduction)
  * [Creating your first Profile](#creating-your-first-profile)
  * [Creating your first Action](#creating-your-first-action)
  * [Summary](#summary)
- [Quick Demo](#quick-demo)
  * [Installing QuickCut](#installing-quickcut)
  * [Creating Profile and Action](#creating-profile-and-action)
  * [Launch your favorite directories](#launch-your-favorite-directories)
  * [Custom Themes](#custom-themes)
- [Motivation](#motivation)
- [Contributing](#contributing)
- [Authors](#authors)
- [License](#license)


## Getting Started

Download links can be found the following [link](https://github.com/giladreich/QuickCut/releases).

For building this project from source, please refer to the [BUILD](/docs/BUILD.md) documentation.


#### Introduction

QuickCut is designed to use `Profiles` and `Actions` in a one to many relationship (each profile can create multiple actions). This enables convenient change between multiple profiles creating different working environments depending on the selected profile. The Profiles are saved in a `JSON` format - making it easier to share your profiles with others.


#### Creating your first Profile

When getting started with QuickCut, the following window appears:

![Create Profile](/media/create_profile.png)

![Create Profile](/media/first_main_window.png)

As soon as you click the `Default` button, all shortcuts for the selected profile will be activated.


#### Creating your first Action

There are two types of actions:

* Key Mapping - maps a desired key to the specified key (no restart required).
* Start Application - maps a desired key to the specified action, i.e launch a specific directory or your favorite application.

`Key Mapping` looks as follows:

![Create Profile](/media/action_window_map_screenshot.png)

Clicking the record button will record the desired keycode:

![Create Profile](/media/action_window_map_screenshot_record.png)

`Start Application` looks as follows:

![Create Profile](/media/action_window_map_snippingtool.png)

SnippingTool will start as soon as we hit the F14 key.

It's also possible to use combination of keys for a single shortcut:

![Create Profile](/media/action_window_map_keycombo.png)


This creates a configuration file under `Config/profiles.json` that can be easily shared.

#### Summary

Above is a demonstration of the basics of QuickCut; how to create profiles and diverse actions.

Once these steps are completed, you can use your computer as usual. 

Note that you no longer need to keep QuickCut GUI open unless you want to make any changes.

## Quick Demo

##### Installing QuickCut

![Create Profile](/media/QuickCut_install.gif)


##### Creating Profile and Action

![Create Profile](/media/QuickCut_SnippingTool.gif)


##### Launch your favorite directories

![Create Profile](/media/QuickCut_open_boost_dir.gif)


##### Custom Themes

![Create Profile](/media/QuickCut_themes.gif)


## Motivation

I always connect my laptop to a monitor using external keyboard and mouse.

This is preffered keyboard that I currently use at home:

![Apple Keyboard](/media/apple_keyboard.png)

It has F13 til F19 keys which are non-functioning keys I wanted to make use of. In looking for alternative solutions I came across `KeyTweak`, as well as other programs, that edit the registry using the default functionality Windows has for mapping keys. Unfortunately every time you map a key on the windows registry it requires a full reboot to the system in order to load the new key mapping layout. It also doesn't allow for mapping a combination of keys to a single key.

This brought me to the idea of developing an interactive application that runs as a background service which does all sorts of key mapping invoking different actions to be as efficient as possible when using the keyboard.

For that reason `QuickCut` is here and free to use :).

## Contributing

Pull-Requests are greatly appreciated should you like to contribute to the project. 

Same goes for opening issues; if you have any suggestions, feedback or you found any bugs, please do not hesitate to open an [issue](https://github.com/giladreich/QuickCut/issues).

## Authors

* **Gilad Reich** - *Initial work* - [giladreich](https://github.com/giladreich)

See also the list of [contributors](https://github.com/giladreich/QuickCut/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

