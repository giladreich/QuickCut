/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    // default constructor
    installer.setValue("TargetDir", "@HomeDir@/QuickCut");
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);

    if (installer.isInstaller()) {
        component.loaded.connect(this, Component.prototype.installerLoaded);

        // ComponentSelectionPage = gui.pageById(QInstaller.ComponentSelection);

        // installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        // installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        // installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
        // if (systemInfo.productType === "windows")
        //     installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
        // installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    }
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    component.addOperation("CreateShortcut", "@TargetDir@/QuickCut.exe", "@DesktopDir@/QuickCut.lnk");
    component.addOperation("CreateShortcut", "@TargetDir@/QuickCut.exe", "@StartMenuDir@/QuickCut.lnk");

    if (component.uninstallationRequested()) {
        component.addElevatedOperation("Execute", "@TargetDir@/QuickCutService.exe", "-t"); // Terminnate/stop
        component.addElevatedOperation("Execute", "@TargetDir@/QuickCutService.exe", "-u"); // Uninstall
        component.addElevatedOperation("Execute", "sc", "stop", "QuickCut Service"); // test
        component.addElevatedOperation("Execute", "sc", "delete", "QuickCut Service"); // test
        component.addElevatedOperation("Execute", "@TargetDir@/UninstallService.cmd"); // test..
    } 
}

Component.prototype.installerLoaded = function () {

    if (installer.addWizardPage(component, "LicenseWidget", QInstaller.LicenseCheck)) {
        var widget = gui.pageWidgetByObjectName("DynamicLicenseWidget");
        if (widget != null) {
            widget.acceptLicense.toggled.connect(this, Component.prototype.checkAccepted);

            widget.complete = false;
            widget.declineLicense.checked = true;
            widget.windowTitle = "License Agreement";
            // widget.textBrowser.setText("qrc:/LICENSE.txt");
        }
    }

}

Component.prototype.checkAccepted = function (checked) {
    var widget = gui.pageWidgetByObjectName("DynamicLicenseWidget");
    if (widget != null)
        widget.complete = checked;
}

Component.prototype.installationFinished = function()
{
    component.addOperation("Execute", "QuickCutService.exe", "-i");
}