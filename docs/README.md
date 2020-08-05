
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

<p align="center">
    Special thanks to JetBrains for supporting this project.<br>
    <a href="https://www.jetbrains.com/" alt="JetBrains">
        <img src="/media/JetBrains.png" width="60px" /></a>
</p>

---
# QuickCut

QuickCut is a cross-platform keyboard manager that both facilitates key mapping and allows the configuration of global hotkeys triggering user defined actions.

QuickCut offer a rich set of features that allows for shorthand access at the convenience of your chosen keyboard keys. It allows you to tailor your keyboard layout design making your time as efficient as possible.

<img src="/media/MainView.png" alt="Create Profile" width="550" height="450"/>

## Contents

- [Features](#features)
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

## Features

<table width=100%>
    <thead>
        <tr>
            <th>Name</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Key Mapping</td>
            <td>Maps a key to another key, i.e. <code>Ctrl</code> to <code>Caps-Lock</code>.</td>
        </tr>
        <tr>
            <td>Mapping Multiple Keys</td>
            <td>Maps multiple keys (shortcut), i.e. <code>Ctrl+Shift+W</code> to <code>Alt+F4</code>.</td>
        </tr>
        <tr>
            <td>Unicode Characters</td>
            <td>All languages are supported including emojis.</td>
        </tr>
        <tr>
            <td>Auto Text</td>
            <td>Pasting user defined template text, i.e. for emails or messages.</td>
        </tr>
        <tr>
            <td>Portable Configurations</td>
            <td>Formatted json file of the user defined Profiles and Actions.</td>
        </tr>
        <tr>
            <td>Multiple Interchangeable Profiles</td>
            <td>Convenient way to switch between profiles to activate different actions.</td>
        </tr>
        <tr>
            <td>Open File / Application</td>
            <td>Launch a specified File/App by a global hotkey.</td>
        </tr>
        <tr>
            <td>Open Directory</td>
            <td>Launch a specified Directory by a global hotkey.</td>
        </tr>
        <tr>
            <td>Custom Themes</td>
            <td>Various options of themes are available.</td>
        </tr>
    </tbody>
</table>

More detailed information about QuickCut's features can be found at [QuickCut's website](https://quickcut.greich.com).

## Getting Started

Download links can be found at the following [link](https://github.com/giladreich/QuickCut/releases).

For GitHub users, it is also possible to get the latest changes from the [CI/CD - GitHub's Actions](https://github.com/giladreich/QuickCut/actions), as new artifacts, including installers will be deployed every change. However, keep in mind that it might be unstable version during development.

For building this project from source, please refer to the [BUILD](/docs/BUILD.md) documentation.

#### Introduction

QuickCut is designed to use `Profiles` and `Actions` in a one to many relationship (each profile can create multiple actions). This enables convenient change between multiple profiles creating different working environments depending on the selected profile. The Profiles are saved in a `JSON` format - making it easier to share your profiles and actions with others.


#### Creating your first Profile

When getting started with QuickCut, the following window appears:

<img src="/media/MainView_Create_Profile.png" alt="Create Profile" width="150"/>

You can create more profiles if you need to manage multiple keyboard layouts:

<img src="/media/MainView_Fresh.png" alt="Create Profile" width="350"/>

As soon as you click the `Default` button, all shortcuts for the selected profile will be activated.


#### Creating your first Action

There are multiple types of actions available that you can select based on your use case:
* Key Mapping
* Open File
* Open Directory
* Auto Text

Here is an example of creating a very simple Action that maps one key to another:

Select the appropriate `Action Type` from the following list:

<img src="/media/ActionView_Create_KeyMapping.png" alt="Action Key Mapping" width="500"/>

Clicking the record button will record the desired key:

<img src="/media/ActionView_Create_KeyMapping_Record.png" alt="Action Key Mapping" width="500"/>

Click the `Create` button and that's it! Your new key mapping action will now be activated without the need to restart the computer.

It is also possible to map multiple keys to different keys, i.e. `F1 + Q` to trigger `Caps-Lock`:

<img src="/media/ActionView_Create_MultiKeyMapping.png" alt="Action Multiple Key Mapping" width="500"/>

Launching your favorite Apps / Directories is simple as that, i.e. `Ctrl + F13` to open SnippingTool:

<img src="/media/ActionView_Create_LaunchApp.png" alt="Action Launch App" width="500"/>

Another useful feature is the `Auto Text` to write frequent messaging templates for you:

<img src="/media/ActionView_Create_AutoText.png" alt="Action Auto Text" width="500"/>

#### Summary

Above is a brief demonstration of QuickCut's features; how to create profiles and diverse actions.

Once these steps are completed, you can close QuickCut's GUI and use your computer as usual.

## Quick Demo

##### Installing QuickCut

<img src="/media/Demo_QuickCut_Installer.gif" alt="QuickCut Installer" width="100%"/>

##### Creating Actions

<img src="/media/Demo_QuickCut_Actions.gif" alt="QuickCut Actions" width="100%"/>


##### Custom Themes

<img src="/media/Demo_QuickCut_Themes.gif" alt="QuickCut Themes" width="100%"/>


## Motivation

I have my laptop connected to a monitor using external keyboard and mouse.

This is my preferred keyboard that I currently use at home:

<img src="/media/AppleKeyboard.png" alt="Apple Keyboard" width="100%"/>

It has F13 til F19 keys which are non-functioning keys I wanted to make use of. In looking for alternative solutions I came across `KeyTweak`, as well as other programs, that edit the registry using the default functionality Windows has for mapping keys. Unfortunately every time you map a key in the Windows Registry it requires a full reboot to the system in order to load the new key mapping layout. It also doesn't allow for mapping a combination of keys to a single key.

This brought me to the idea of developing an interactive application that runs as a background service which does all sorts of key mapping invoking different actions to be as efficient as possible when using the keyboard.

For that reason `QuickCut` is here and free to use. :)

## Contributing

Pull-Requests are greatly appreciated should you like to contribute to the project.

Same goes for opening issues; if you have any suggestions, feedback or you found any bugs, please do not hesitate to open an [issue](https://github.com/giladreich/QuickCut/issues).

## Authors

* **Gilad Reich** - *Initial work* - [giladreich](https://github.com/giladreich)

See also the list of [contributors](https://github.com/giladreich/QuickCut/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

