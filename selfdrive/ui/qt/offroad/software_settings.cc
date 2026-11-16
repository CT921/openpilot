#include "selfdrive/ui/qt/offroad/settings.h"

#include <cassert>
#include <cmath>
#include <string>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QProcess>

#include "common/params.h"
#include "common/util.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "system/hardware/hw.h"


void SoftwarePanel::checkForUpdates() {
  std::system("pkill -SIGUSR1 -f system.updated.updated");
}

SoftwarePanel::SoftwarePanel(QWidget* parent) : ListWidget(parent) {
  onroadLbl = new QLabel(tr("Updates are only downloaded while the car is off."));
  onroadLbl->setStyleSheet("font-size: 50px; font-weight: 400; text-align: left; padding-top: 30px; padding-bottom: 30px;");
  addItem(onroadLbl);

  // current version
  versionLbl = new LabelControl(tr("Current Version"), "");
  addItem(versionLbl);

  // dp on/off btn
  onOffBtn = new ButtonControl(tr("Onroad/Offroad Mode"), tr("Go Offroad"));
  connect(onOffBtn, &ButtonControl::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to switch driving mode?"), tr("CONFIRM"), this)) {
      bool val = params.getBool("dp_device_offroad");
      params.putBool("dp_device_offroad", !val);
    }
  });
  addItem(onOffBtn);

  // download update btn
  downloadBtn = new ButtonControl(tr("Download"), tr("CHECK"));
  connect(downloadBtn, &ButtonControl::clicked, [=]() {
    downloadBtn->setEnabled(false);
    if (downloadBtn->text() == tr("CHECK")) {
      checkForUpdates();
    } else {
      std::system("pkill -SIGHUP -f system.updated.updated");
    }
  });
  addItem(downloadBtn);

  // install update btn
  installBtn = new ButtonControl(tr("Install Update"), tr("INSTALL"));
  connect(installBtn, &ButtonControl::clicked, [=]() {
    installBtn->setEnabled(false);
    params.putBool("DoReboot", true);
  });
  addItem(installBtn);

  // branch selecting
  targetBranchBtn = new ButtonControl(tr("Target Branch"), tr("SELECT"));
  connect(targetBranchBtn, &ButtonControl::clicked, [=]() {
    if (Hardware::get_device_type() == cereal::InitData::DeviceType::TICI) {
      // TICI device special handling - only show -c3/-C3 branches
      auto current = params.get("GitBranch");
      QStringList allBranches = QString::fromStdString(params.get("UpdaterAvailableBranches")).split(",");
      QStringList branches;

      // Filter to only show branches ending with -c3 or -C3
      for (const QString &b : allBranches) {
        if (b.endsWith("-c3") || b.endsWith("-C3")) {
          branches.append(b);
        }
      }

      // Prioritize recommended branches for TOP fork
      for (QString b : {current.c_str(), "top-c3", "dev-c3", "release-c3"}) {
        auto i = branches.indexOf(b);
        if (i >= 0) {
          branches.removeAt(i);
          branches.insert(0, b);
        }
      }

      QRegularExpression topPattern("^TOP\\d+-[Cc]3$", QRegularExpression::CaseInsensitiveOption);
      for (const QString &branch : allBranches) {
        if (topPattern.match(branch).hasMatch()) {
          auto i = branches.indexOf(branch);
          if (i >= 0) {
            branches.removeAt(i);
            branches.insert(1, branch);
          }
        }
      }

      QString cur = QString::fromStdString(params.get("UpdaterTargetBranch"));
      QString selection = MultiOptionDialog::getSelection(tr("Select a branch"), branches, cur, this);
      if (!selection.isEmpty()) {
        params.put("UpdaterTargetBranch", selection.toStdString());
        targetBranchBtn->setValue(QString::fromStdString(params.get("UpdaterTargetBranch")));
        checkForUpdates();
      }
    } else {
      // Non-TICI devices - filter out -c3/-C3 branches
      auto current = params.get("GitBranch");
      QStringList allBranches = QString::fromStdString(params.get("UpdaterAvailableBranches")).split(",");
      QStringList branches;

      // Filter out -c3/-C3 branches for non-TICI devices
      for (const QString &b : allBranches) {
        if (!b.endsWith("-c3") && !b.endsWith("-C3")) {
          branches.append(b);
        }
      }

      for (QString b : {current.c_str(), "devel-staging", "devel", "nightly", "nightly-dev", "master"}) {
        auto i = branches.indexOf(b);
        if (i >= 0) {
          branches.removeAt(i);
          branches.insert(0, b);
        }
      }

      QString cur = QString::fromStdString(params.get("UpdaterTargetBranch"));
      QString selection = MultiOptionDialog::getSelection(tr("Select a branch"), branches, cur, this);
      if (!selection.isEmpty()) {
        params.put("UpdaterTargetBranch", selection.toStdString());
        targetBranchBtn->setValue(QString::fromStdString(params.get("UpdaterTargetBranch")));
        checkForUpdates();
      }
    }
  });
  if (!params.getBool("IsTestedBranch")) {
    addItem(targetBranchBtn);
  }

  // uninstall button
  auto uninstallBtn = new ButtonControl(tr("Uninstall %1").arg(getBrand()), tr("UNINSTALL"));
  connect(uninstallBtn, &ButtonControl::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to uninstall?"), tr("Uninstall"), this)) {
      params.putBool("DoUninstall", true);
    }
  });
  addItem(uninstallBtn);

  // error log button
  errorLogBtn = new ButtonControl(tr("Error Log"), tr("VIEW"), tr("View the error log for debugging purposes when openpilot crashes."));
  connect(errorLogBtn, &ButtonControl::clicked, [=]() {
    std::string txt = util::read_file("/data/community/crashes/error.txt");
    ConfirmationDialog::rich(QString::fromStdString(txt), this);
  });
  addItem(errorLogBtn);

  fs_watch = new ParamWatcher(this);
  QObject::connect(fs_watch, &ParamWatcher::paramChanged, [=](const QString &param_name, const QString &param_value) {
    updateLabels();
  });

  //connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
  //  is_onroad = !offroad;
  //  updateLabels();
  //});

  updateLabels();
}

void SoftwarePanel::showEvent(QShowEvent *event) {
  // nice for testing on PC
  installBtn->setEnabled(true);

  updateLabels();
}

void SoftwarePanel::updateLabels() {
  // add these back in case the files got removed
  fs_watch->addParam("LastUpdateTime");
  fs_watch->addParam("UpdateFailedCount");
  fs_watch->addParam("UpdaterState");
  fs_watch->addParam("UpdateAvailable");
  fs_watch->addParam("dp_device_offroad");

  if (!isVisible()) {
    return;
  }

  // dp on/off btn
  if (params.getBool("dp_device_offroad")) {
    onOffBtn->setText(tr("Go Onroad"));
  } else {
    onOffBtn->setText(tr("Go Offroad"));
  }

  // updater only runs offroad
  onroadLbl->setVisible(is_onroad);
  downloadBtn->setVisible(!is_onroad);

  // download update
  QString updater_state = QString::fromStdString(params.get("UpdaterState"));
  bool failed = std::atoi(params.get("UpdateFailedCount").c_str()) > 0;
  if (updater_state != "idle") {
    downloadBtn->setEnabled(false);
    downloadBtn->setValue(updater_state);
  } else {
    if (failed) {
      downloadBtn->setText(tr("CHECK"));
      downloadBtn->setValue(tr("failed to check for update"));
    } else if (params.getBool("UpdaterFetchAvailable")) {
      downloadBtn->setText(tr("DOWNLOAD"));
      downloadBtn->setValue(tr("update available"));
    } else {
      QString lastUpdate = tr("never");
      auto tm = params.get("LastUpdateTime");
      if (!tm.empty()) {
        lastUpdate = timeAgo(QDateTime::fromString(QString::fromStdString(tm + "Z"), Qt::ISODate));
      }
      downloadBtn->setText(tr("CHECK"));
      downloadBtn->setValue(tr("up to date, last checked %1").arg(lastUpdate));
    }
    downloadBtn->setEnabled(true);
  }
  targetBranchBtn->setValue(QString::fromStdString(params.get("UpdaterTargetBranch")));

  // current + new versions
  versionLbl->setText(QString::fromStdString(params.get("UpdaterCurrentDescription")));
  versionLbl->setDescription(QString::fromStdString(params.get("UpdaterCurrentReleaseNotes")));

  installBtn->setVisible(!is_onroad && params.getBool("UpdateAvailable"));
  installBtn->setValue(QString::fromStdString(params.get("UpdaterNewDescription")));
  installBtn->setDescription(QString::fromStdString(params.get("UpdaterNewReleaseNotes")));

  update();
}
