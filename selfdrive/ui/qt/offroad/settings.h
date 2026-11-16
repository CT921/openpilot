#pragma once

#include <map>
#include <string>

#include <QButtonGroup>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QStackedLayout>

#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/offroad/timpilot.h"
#include "selfdrive/ui/qt/widgets/myqrcodes.h" // QrCode

// ********** settings window + top-level panels **********
class SettingsWindow : public QFrame {
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = 0);
  void setCurrentPanel(int index, const QString &param = "");
  bool keepScreenOn = false;

protected:
  void showEvent(QShowEvent *event) override;

signals:
  void closeSettings();
  void reviewTrainingGuide();
  void showDriverView();
  void expandToggleDescription(const QString &param);
  void scrollToToggle(const QString &param);

private:
  QPushButton *sidebar_alert_widget;
  QWidget *sidebar_widget;
  QButtonGroup *nav_btns;
  QStackedWidget *panel_widget;
};

class DevicePanel : public ListWidget {
  Q_OBJECT
public:
  explicit DevicePanel(SettingsWindow *parent);

signals:
  void reviewTrainingGuide();
  void showDriverView();

private slots:
  void poweroff();
  void reboot();
  void updateCalibDescription();
  void flashPanda();

private:
  Params params;
  ButtonControl *pair_device;
  ButtonControl *resetCalibBtn;
  ButtonControl *flashPandaBtn;
  SettingsWindow *parentWindow;
};

class TogglesPanel : public ListWidget {
  Q_OBJECT
public:
  explicit TogglesPanel(SettingsWindow *parent);
  void showEvent(QShowEvent *event) override;

public slots:
  void expandToggleDescription(const QString &param);
  void scrollToToggle(const QString &param);

private slots:
  void updateState(const UIState &s);

private:
  Params params;
  std::map<std::string, ParamControl*> toggles;
  ButtonParamControl *long_personality_setting;
  ButtonParamControl *accel_personality_setting;

  void updateToggles();
};

class SoftwarePanel : public ListWidget {
  Q_OBJECT
public:
  explicit SoftwarePanel(QWidget* parent = nullptr);

private:
  void showEvent(QShowEvent *event) override;
  void updateLabels();
  void checkForUpdates();

  bool is_onroad = false;

  QLabel *onroadLbl;
  LabelControl *versionLbl;
  ButtonControl *errorLogBtn;
  ButtonControl *installBtn;
  ButtonControl *downloadBtn;
  ButtonControl *targetBranchBtn;
  ButtonControl *onOffBtn;

  Params params;
  ParamWatcher *fs_watch;
};

// Forward declaration
class FirehosePanel;

class TimpilotPanel : public QWidget {
  Q_OBJECT

private:
  QStackedLayout* main_layout = nullptr;
  QWidget* home = nullptr;
  ForceCarRecognition* setCar = nullptr;

  QWidget* home_widget;

public:
  explicit TimpilotPanel(QWidget *parent = nullptr);
};