
function assert(condition) {
    if (!condition)
        throw new Error("Assertion failed!");
}

var Dir = new function () {
    this.toNative = function (path) {
        if (systemInfo.productType === "windows")
            return path.replace(/\//g, '\\');
        return path;
    }
};

// Constructs this Component after the next button clicked on the intro page.
function Component() {
    console.log("********** Component **********");
    if (installer.isInstaller()) {
        // connect slots
        component.loaded.connect(this, Component.prototype.installerLoaded);
        installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);

        // set target path
        var destInstallPath = Dir.toNative("@ApplicationsDirX64@\\QuickCut");
        installer.setValue("TargetDir", destInstallPath);

        // skip pages
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    }
}

// NOTE(Gilad): Doesn't really work, since the Introduction page can only be modified from
// the Controller, but the Controller isn't able to set UserInterfaces in the config.xml. -_-
Component.prototype.installerLoaded = function () {
    console.log("********** installerLoaded **********");
    if (installer.addWizardPage(component, "WelcomePage", QInstaller.Introduction)) {
        var widget = gui.pageWidgetByObjectName("DynamicWelcomePage");
        if (widget != null) {
            widget.btnNext.clicked.connect(this, Component.prototype.onNextPage);
            widget.btnCancel.clicked.connect(this, Component.prototype.onExit);
        }
    }
}

Component.prototype.onNextPage = function () {
    console.log("********** onNextPage **********");
}

Component.prototype.onExit = function () {
    console.log("********** onExit **********");
}

Component.prototype.createOperations = function () {
    console.log("********** createOperations **********");
    component.createOperations();

    if (installer.isInstaller()) {
        component.addElevatedOperation("CreateShortcut",
            Dir.toNative("@TargetDir@/QuickCut.exe"),
            Dir.toNative("@DesktopDir@/QuickCut.lnk"));

        component.addElevatedOperation("CreateShortcut",
            Dir.toNative("@TargetDir@/QuickCut.exe"),
            Dir.toNative("@StartMenuDir@/QuickCut.lnk"));

        component.addElevatedOperation("CreateShortcut",
            Dir.toNative("@TargetDir@/QuickCutConsole.exe"),
            Dir.toNative("@AllUsersStartMenuProgramsPath@/Startup/QuickCutConsole.lnk"));
    }
}

Component.prototype.installationFinishedPageIsShown = function () {
    console.log("********** installationFinishedPageIsShown **********");
    try {
        installMissingRedistributables();
    } catch (e) {
        console.log(e);
    }
}

installMissingRedistributables = function () {
    console.log("********** installMissingRedistributables **********");
    var regKey = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64";
    var regValue = installer.execute("reg", new Array("query", regKey, "/v", "Installed"))[0];
    if (!regValue) {
        QMessageBox.information("redist.install", "Install Visual Studio Redistributables",
            "The application requires Visual Studio Redistributables to run and will be installed now.",
            QMessageBox.OK);
        console.log("Starting to install missing redistributables...");
        installer.execute(Dir.toNative("@TargetDir@/vc_redist.x64.exe"), "/passive", "/norestart");
        console.log("Finished installing redistributables.");
    }
}
