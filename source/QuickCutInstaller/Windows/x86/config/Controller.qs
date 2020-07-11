
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

function Controller() {
    console.log("********** Controller **********");
    if (installer.isUninstaller()) {
        installer.uninstallationStarted.connect(onUninstallStarted);
    }
}

Controller.prototype.IntroductionPageCallback = function () {
    console.log("********** IntroductionPageCallback **********");
    if (installer.isInstaller()) {
        var widget = gui.currentPageWidget();
        assert(typeof widget === 'object');
        widget.title = "Welcome to QuickCut Installer - Version " + installer.value("Version");
        var text = "This wizard will install QuickCut on your computer. " +
            "If you have any problems during the installation, please create an issue on the GitHub page at:\n" +
            "https://github.com/giladreich/QuickCut/issues";
        widget.MessageLabel.setText(text);
    }
}

Controller.prototype.FinishedPageCallback = function () {
    console.log("********** FinishedPageCallback **********");
    if (installer.isInstaller()) {
        try {
            installer.execute(Dir.toNative("@TargetDir@/scripts/UninstallService.bat"));
            installer.execute(Dir.toNative("@TargetDir@/QuickCutService.exe"), "-i"); // Install
            installer.execute(Dir.toNative("@TargetDir@/scripts/ConfigureService.bat"));
        } catch (e) {
            console.log(e);
        }
    }
}

onUninstallStarted = function () {
    console.log("********** onUninstallStarted **********");
    try {
        installer.execute(Dir.toNative("@TargetDir@/scripts/UninstallService.bat"));
    } catch (e) {
        console.log(e);
    }
}
