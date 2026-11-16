/**
 * Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.
 *
 * This file is part of sunnypilot and is licensed under the MIT License.
 * See the LICENSE.md file in the root directory for more details.
 */

#include "selfdrive/ui/qt/offroad/osm_panel.h"

#include <tuple>
#include <vector>
#include <string>

#include "common/swaglog.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/offroad/speed_limit/speed_limit_settings.h"

OsmPanel::OsmPanel(QWidget *parent) : QFrame(parent) {
  main_layout = new QStackedLayout(this);

  const auto list = new ListWidget(this);
  list->addItem(mapdVersion = new LabelControl(tr("Mapd Version"), tr("Loading...")));
  list->addItem(setupOsmDeleteMapsButton(parent));
  list->addItem(offlineMapsETA = new LabelControl(tr("Offline Maps ETA"), ""));
  list->addItem(offlineMapsElapsed = new LabelControl(tr("Time Elapsed"), ""));
  list->addItem(setupOsmUpdateButton(parent));
  list->addItem(setupOsmDownloadButton(parent));
  list->addItem(setupUsStatesButton(parent));

  //Speed limit
  list->addItem(horizontal_line());
  list->addItem(setupSpeedLimitButton());

  connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    updateLabels();
  });

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&OsmPanel::updateLabels));
  timer->start(FAST_REFRESH_INTERVAL); // Time specified in milliseconds.
  updateLabels();

  osmScreen = new QWidget(this);
  auto *vlayout = new QVBoxLayout(osmScreen);
  vlayout->setContentsMargins(50, 20, 50, 20);
  vlayout->addWidget(new ScrollView(list, this), 1);
  main_layout->addWidget(osmScreen);

  speedLimitScreen = new SpeedLimitSettings(this);

  speedLimitWrapper = new QWidget(this);
  QVBoxLayout *wrapperLayout = new QVBoxLayout(speedLimitWrapper);
  wrapperLayout->setContentsMargins(30, 20, 30, 20);
  wrapperLayout->setSpacing(0);

  wrapperLayout->addWidget(speedLimitScreen);

  connect(speedLimitScreen, &SpeedLimitSettings::backPress, [=]() {
    main_layout->setCurrentWidget(osmScreen);
  });
  main_layout->addWidget(speedLimitWrapper);
}

ButtonControl *OsmPanel::setupOsmDeleteMapsButton(QWidget *parent) {
  osmDeleteMapsBtn = new ButtonControl(tr("Downloaded Maps"), tr("DELETE")); // Updated on updateLabels()
  connect(osmDeleteMapsBtn, &ButtonControl::clicked, [=]() {
    if (showConfirmationDialog(parent, tr("This will delete ALL downloaded maps\n\nAre you sure you want to delete all the maps?"), tr("Yes, delete all the maps."))) {
      QtConcurrent::run([=]() {
        QDir dir(MAP_PATH);
        osmDeleteMapsBtn->setEnabled(false);
        osmDeleteMapsBtn->setText("⌛");
        dir.removeRecursively();
        updateMapSize();
        osmDeleteMapsBtn->setEnabled(true);
        osmDeleteMapsBtn->setText(tr("DELETE"));
      });
      updateLabels();
    }
  });
  return osmDeleteMapsBtn;
}

ButtonControl *OsmPanel::setupOsmUpdateButton(QWidget *parent) {
  osmUpdateBtn = new ButtonControl(tr("Database Update"), tr("CHECK")); // Updated on updateLabels()
  connect(osmUpdateBtn, &ButtonControl::clicked, [=]() {
    if (osm_download_in_progress && !download_failed_state) {
      updateLabels();
    } else if (showConfirmationDialog(parent)) {
      osm_download_in_progress = true;
      params.putBool("OsmDbUpdatesCheck", true);
      updateLabels();
    }
  });
  return osmUpdateBtn;
}

ButtonControl *OsmPanel::setupOsmDownloadButton(QWidget *parent) {
  osmDownloadBtn = new ButtonControl(tr("Country"), tr("SELECT"));
  connect(osmDownloadBtn, &ButtonControl::clicked, [=]() {
    osmDownloadBtn->setEnabled(false);
    osmDownloadBtn->setValue(tr("Fetching Country list..."));
    const std::vector<std::tuple<QString, QString, QString, QString> > locations = getOsmLocations();
    osmDownloadBtn->setEnabled(true);
    osmDownloadBtn->setValue("");
    const QString initTitle = QString::fromStdString(params.get("OsmLocationTitle"));
    const QString currentTitle = ((initTitle == "== None ==") || (initTitle.length() == 0)) ? "== None ==" : initTitle;

    QStringList locationTitles;
    for (auto &loc: locations) {
      locationTitles.push_back(std::get<0>(loc));
    }

    InputDialog d(tr("Search Country"), this, tr("Enter search keywords, or leave blank to list all countries."), false);
    d.setMinLength(0);
    const int ret = d.exec();
    if (ret) {
      const QString selection = search(d.text(), locationTitles, tr("Select Country"));
      if (!selection.isEmpty()) {
        params.put("OsmLocal", "1");
        params.put("OsmLocationTitle", selection.toStdString());
        for (auto &loc: locations) {
          if (std::get<0>(loc) == selection) {
            params.put("OsmLocationName", std::get<1>(loc).toStdString());
            break;
          }
        }
        if (params.get("OsmLocationName") == "US") {
          emit usStatesBtn->clicked();
          return;
        }
        if (selection != "== None ==") {
          if (showConfirmationDialog(parent)) {
            osm_download_in_progress = true;
            params.putBool("OsmDbUpdatesCheck", true);
            updateLabels();
          }
        }
      }
    }
    updateLabels();
  });
  return osmDownloadBtn;
}

ButtonControl *OsmPanel::setupUsStatesButton(QWidget *parent) {
  usStatesBtn = new ButtonControl(tr("State"), tr("SELECT"));
  connect(usStatesBtn, &ButtonControl::clicked, [=]() {
    const std::tuple<QString, QString> allStatesOption = std::make_tuple("All States (~4.8 GB)", "All");
    usStatesBtn->setEnabled(false);
    usStatesBtn->setValue(tr("Fetching State list..."));
    const std::vector<std::tuple<QString, QString, QString, QString> > locations =
        getUsStatesLocations(allStatesOption);
    usStatesBtn->setEnabled(true);
    usStatesBtn->setValue("");
    const QString initTitle = QString::fromStdString(params.get("OsmStateTitle"));
    const QString currentTitle = ((initTitle == std::get<0>(allStatesOption)) || (initTitle.length() == 0)) ? tr("All") : initTitle;

    QStringList locationTitles;
    for (auto &loc: locations) {
      locationTitles.push_back(std::get<0>(loc));
    }

    InputDialog d(tr("Search State"), this, tr("Enter search keywords, or leave blank to list all states."), false);
    d.setMinLength(0);
    const int ret = d.exec();
    if (ret) {
      const QString selection = search(d.text(), locationTitles, tr("Select State"));
      if (!selection.isEmpty()) {
        params.put("OsmStateTitle", selection.toStdString());
        for (auto &loc: locations) {
          if (std::get<0>(loc) == selection) {
            params.put("OsmStateName", std::get<1>(loc).toStdString());
            break;
          }
        }
        usStatesBtn->setValue(selection);
        if (showConfirmationDialog(parent)) {
          osm_download_in_progress = true;
          params.putBool("OsmDbUpdatesCheck", true);
          updateLabels();
        }
      }
    }
    updateLabels();
  });
  usStatesBtn->setVisible(false); // initially hidden
  return usStatesBtn;
}

void OsmPanel::showEvent(QShowEvent *event) {
  updateLabels(); // For snappier feeling
  if (!timer->isActive()) {
    timer->start(FAST_REFRESH_INTERVAL);
  }
}

void OsmPanel::hideEvent(QHideEvent *event) {
  if (timer->isActive()) {
    timer->stop();
  }
}


void OsmPanel::updateLabels() {
  if (!isVisible()) {
    return;
  }
  mapd_version = params.get("MapdVersion");
  mapdVersion->setText(mapd_version.c_str());

  updateMapSize();
  osm_download_locations = mem_params.get("OSMDownloadLocations");
  osm_download_in_progress = !osm_download_locations.empty();

  timer->setInterval(osm_download_in_progress ? FAST_REFRESH_INTERVAL : SLOW_REFRESH_INTERVAL);
  LOGT("Timer Interval %d", timer->interval());

  const std::string osmLastDownloadTimeStr = params.get("OsmDownloadedDate");
  if (!lastDownloadedTimePoint.has_value() && !osmLastDownloadTimeStr.empty()) {
    const double osmLastDownloadTime = std::stod(osmLastDownloadTimeStr);
    lastDownloadedTimePoint = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(osmLastDownloadTime));
  }

  osmDownloadBtn->setEnabled(!osm_download_in_progress);
  usStatesBtn->setEnabled(!osm_download_in_progress);

  updateDownloadProgress();

  const QString locationName = QString::fromStdString(params.get("OsmLocationName"));
  const bool isUs = !locationName.isEmpty() && locationName == "US";
  usStatesBtn->setVisible(isUs);

  if (!locationName.isEmpty()) {
    if (!isUs) {
      params.remove("OsmStateName");
      params.remove("OsmStateTitle");
    }
    osmUpdateBtn->setVisible(true);
  } else {
    params.remove("OsmLocal");
    params.remove("OsmLocationName");
    params.remove("OsmLocationTitle");
    params.remove("OsmStateName");
    params.remove("OsmStateTitle");
    osmUpdateBtn->setVisible(false);
    usStatesBtn->setVisible(false);
  }

  osmDownloadBtn->setValue(QString::fromStdString(params.get("OsmLocationTitle")));
  usStatesBtn->setValue(QString::fromStdString(params.get("OsmStateTitle")));
  update();
}

void OsmPanel::updateDownloadProgress() {
  const auto pending_update_check = params.getBool("OsmDbUpdatesCheck");
  const QJsonObject osmDownloadProgress = QJsonDocument::fromJson(params.get("OSMDownloadProgress").c_str()).object();
  if (osm_download_in_progress && lastDownloadedTimePoint.has_value()) {
    offlineMapsETA->setVisible(true);
    offlineMapsElapsed->setVisible(true);
    offlineMapsETA->setText(calculateETA(osmDownloadProgress, lastDownloadedTimePoint.value()));
    offlineMapsElapsed->setText(calculateElapsedTime(osmDownloadProgress, lastDownloadedTimePoint.value()));
  } else {
    offlineMapsETA->setVisible(false);
    offlineMapsElapsed->setVisible(false);
  }

  const int total_files = extractIntFromJson(osmDownloadProgress, "total_files");
  const int downloaded_files = extractIntFromJson(osmDownloadProgress, "downloaded_files");
  download_failed_state = total_files && osm_download_in_progress && !lastDownloadedTimePoint.has_value() && downloaded_files < total_files;

  QString updateButtonText = processUpdateStatus(pending_update_check, total_files, downloaded_files, osmDownloadProgress, download_failed_state);

  osmUpdateBtn->setValue(updateButtonText);
  osmUpdateBtn->setText(osm_download_in_progress && !download_failed_state ? tr("REFRESH") : tr("UPDATE"));
  osmDeleteMapsBtn->setValue(formatSize(mapsDirSize));
}

int OsmPanel::extractIntFromJson(const QJsonObject &json, const QString &key) {
  return (json.contains(key)) ? json[key].toInt() : 0;
}

QString OsmPanel::processUpdateStatus(bool pending_update, int total_files, int downloaded_files, const QJsonObject &json, bool failed_state) {
  if (pending_update && !osm_download_in_progress && !total_files) {
    lastDownloadedTimePoint.reset();
    return tr("Download starting...");
  } else if (failed_state) {
    return tr("Error: Invalid download. Retry.");
  } else if (osm_download_in_progress && total_files > downloaded_files) {
    return formatDownloadStatus(json);
  } else if (osm_download_in_progress && downloaded_files >= total_files) {
    osm_download_in_progress = false;
    lastDownloadedTimePoint.reset();
    return tr("Download complete!");
  }

  if (lastDownloadedTimePoint.has_value()) {
    QDateTime dateTime = QDateTime::fromTime_t(std::chrono::system_clock::to_time_t(lastDownloadedTimePoint.value())); //fromMSecsSinceEpoch(duration);
    dateTime = dateTime.toLocalTime();
    return QString("%1").arg(dateTime.toString("yyyy-MM-dd HH:mm:ss"));
  }

  return "";
}

void OsmPanel::updateMapSize() {
  if (mapSizeFuture.has_value() && mapSizeFuture.value().isFinished()) {
    mapsDirSize = mapSizeFuture.value().result();
  }

  if (!mapSizeFuture.has_value() || !mapSizeFuture.value().isRunning()) {
    mapSizeFuture = QtConcurrent::run(getDirSize, MAP_PATH);
  }
}

QString OsmPanel::search(const QString &query, const QStringList &list, const QString &prompt_text) {
  QStringList lst_results = searchFromList(query, list);
  QString selection;

  if (lst_results.isEmpty()) {
    ConfirmationDialog::alert(tr("No results found for keywords: %1").arg(query), this);
    return selection;
  }
  selection = MultiOptionDialog::getSelection(prompt_text, lst_results, "", this);
  return selection;
}

ButtonControl *OsmPanel::setupSpeedLimitButton() {
  speedLimitBtn = new ButtonControl(tr("Speed Limit"), tr("CONFIGURE"),
                                   tr("Configure speed limit settings including source policy, mode, and offset options."));
  connect(speedLimitBtn, &ButtonControl::clicked, [=]() {
    int screenWidth = this->width();
    if (screenWidth > 0) {
      int margin = qMax(20, qMin(50, screenWidth / 20));
      speedLimitScreen->setMaximumWidth(screenWidth - 2 * margin - 40);

      QVBoxLayout *wrapperLayout = qobject_cast<QVBoxLayout*>(speedLimitWrapper->layout());
      if (wrapperLayout) {
        wrapperLayout->setContentsMargins(margin, 20, margin, 20);
      }
    }
    main_layout->setCurrentWidget(speedLimitWrapper);
  });
  return speedLimitBtn;
}
