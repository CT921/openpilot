#include "selfdrive/ui/qt/offroad/timpilot.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <QComboBox>
#include <QAbstractItemView>
#include <QScroller>
#include <QListView>
#include <QListWidget>
#include <QProcess>
#include <QDir>

#include "common/params.h"
#include "selfdrive/ui/qt/api.h"
#include "selfdrive/ui/qt/widgets/input.h"

#include "selfdrive/ui/ui.h"

static QStringList get_list(const char* path)
{
  QStringList stringList;
  QFile textFile(path);
  if (!textFile.exists()) {
    qDebug() << "Cars file not found, generating...";

    QFile allCarsCheck("/data/openpilot/selfdrive/car/top_tmp/AllCars");
    if (!allCarsCheck.exists()) {
      qDebug() << "AllCars file not found, generating AllCars first...";

      QProcess process;
      process.setWorkingDirectory("/data/openpilot");
      process.start("python3", QStringList() << "opendbc_repo/opendbc/car/fingerprints.py");
      process.waitForFinished(8000);

      if (process.exitCode() == 0) {
        QString output = process.readAllStandardOutput();

        QFile allCarsFile("/data/openpilot/selfdrive/car/top_tmp/AllCars");
        if (allCarsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
          QTextStream out(&allCarsFile);
          out << output;
          allCarsFile.close();
          qDebug() << "AllCars file generated successfully";
        } else {
          qDebug() << "Failed to write AllCars file";
          return stringList;
        }
      } else {
        qDebug() << "fingerprints.py failed with exit code:" << process.exitCode();
        return stringList;
      }
    } else {
      qDebug() << "AllCars file exists, skipping generation";
    }
    qDebug() << "Generating Cars file from AllCars...";
    QProcess forceProcess;
    forceProcess.setWorkingDirectory("/data/openpilot");
    forceProcess.start("python3", QStringList() << "force_car_recognition.py");
    forceProcess.waitForFinished(2000);

    if (forceProcess.exitCode() != 0) {
      qDebug() << "force_car_recognition.py failed";
    } else {
      qDebug() << "Cars file generated successfully";
    }
  }

  if (textFile.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&textFile);
    while (true) {
      QString line = textStream.readLine();
      if (line.isNull())
        break;
      else
        stringList.append(line);
    }
  }

  return stringList;
}

ForceCarRecognition::ForceCarRecognition(QWidget* parent): QWidget(parent) {

  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setMargin(20);
  main_layout->setSpacing(20);

  QPushButton* back = new QPushButton(tr("Back"));
  back->setObjectName("backBtn");
  back->setFixedSize(500, 100);
  connect(back, &QPushButton::clicked, [=]() { emit backPress(); });
  main_layout->addWidget(back, 0, Qt::AlignLeft);

  QListWidget* list = new QListWidget(this);
  list->setStyleSheet("QListView {padding: 40px; background-color: #393939; border-radius: 15px; height: 140px;} QListView::item{height: 100px}");
  QScroller::grabGesture(list->viewport(), QScroller::LeftMouseButtonGesture);
  list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  list->addItem(tr("[-Not selected-]"));

  QStringList items = get_list("/data/openpilot/selfdrive/car/top_tmp/Cars");
  list->addItems(items);
  list->setCurrentRow(0);

  QString set = QString::fromStdString(Params().get("CarModel"));

  int index = 0;
  for (QString item : items) {
    if (set == item) {
        list->setCurrentRow(index + 1);
        break;
    }
    index++;
  }

  QObject::connect(list, QOverload<QListWidgetItem*>::of(&QListWidget::itemClicked),
    [=](QListWidgetItem* item){

    if (list->currentRow() == 0)
        Params().remove("CarModel");
    else
        Params().put("CarModel", list->currentItem()->text().toStdString());

    emit selectedCar();
    });

  main_layout->addWidget(list);
}