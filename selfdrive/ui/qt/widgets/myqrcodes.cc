#include "selfdrive/ui/qt/widgets/myqrcodes.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <QrCode.hpp>

#include "selfdrive/ui/qt/request_repeater.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/wifi.h"

using qrcodegen::QrCode;

MyFootageQRWidget::MyFootageQRWidget(QWidget* parent) : QWidget(parent) {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MyFootageQRWidget::refresh);
}

void MyFootageQRWidget::showEvent(QShowEvent *event) {
  refresh();
  timer->start(5 * 60 * 1000);
  device()->setOffroadBrightness(100);
}

void MyFootageQRWidget::hideEvent(QHideEvent *event) {
  timer->stop();
  device()->setOffroadBrightness(BACKLIGHT_OFFROAD);
}

void MyFootageQRWidget::refresh() {
  wifi = new WifiManager(this);
  QString myIp = wifi->getIp4Address();
  QString qrString = "http://" + myIp + ":8082/footage";
  this->updateQrCode(qrString);
  update();
}

void MyFootageQRWidget::updateQrCode(const QString &text) {
  QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::LOW);
  qint32 sz = qr.getSize();
  QImage im(sz, sz, QImage::Format_RGB32);

  QRgb black = qRgb(0, 0, 0);
  QRgb white = qRgb(255, 255, 255);
  for (int y = 0; y < sz; y++) {
    for (int x = 0; x < sz; x++) {
      im.setPixel(x, y, qr.getModule(x, y) ? black : white);
    }
  }

  // Integer division to prevent anti-aliasing
  int final_sz = ((width() / sz) - 1) * sz;
  img = QPixmap::fromImage(im.scaled(final_sz, final_sz, Qt::KeepAspectRatio), Qt::MonoOnly);
}

void MyFootageQRWidget::paintEvent(QPaintEvent *e) {
  QPainter p(this);
  p.fillRect(rect(), Qt::white);

  QSize s = (size() - img.size()) / 2;
  p.drawPixmap(s.width(), s.height(), img);
}


MyFootagePopup::MyFootagePopup(QWidget *parent) : DialogBase(parent) {
  QHBoxLayout *hlayout = new QHBoxLayout(this);
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->setSpacing(0);

  setStyleSheet("MyFootagePopup { background-color: #E0E0E0; }");

  // text
  QVBoxLayout *vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(85, 70, 50, 70);
  vlayout->setSpacing(50);
  hlayout->addLayout(vlayout, 1);
  {
    QPushButton *close = new QPushButton(QIcon(":/icons/close.svg"), "", this);
    close->setIconSize(QSize(80, 80));
    close->setStyleSheet("border: none;");
    vlayout->addWidget(close, 0, Qt::AlignLeft);
    QObject::connect(close, &QPushButton::clicked, this, &QDialog::reject);

    vlayout->addSpacing(30);

    QLabel *title = new QLabel(tr("Watch and/or download recordings from comma device cameras"), this);
    title->setStyleSheet("font-size: 75px; color: black;");
    title->setWordWrap(true);
    vlayout->addWidget(title);

    QLabel *instructions = new QLabel(QString(R"(
      <ol type='1' style='margin-left: 15px;'>
        <li style='margin-bottom: 50px;'>%1</li>
        <li style='margin-bottom: 50px;'>%2</li>
      </ol>
    )").arg(tr("Connect your smartphone and comma device to the same Wi-Fi network and disconnect from the car's CAN signal"))
    .arg(tr("Scan the QR code on the right")), this);

    instructions->setStyleSheet("font-size: 47px; font-weight: bold; color: black;");
    instructions->setWordWrap(true);
    vlayout->addWidget(instructions);

    vlayout->addStretch();
  }

  // QR code
  MyFootageQRWidget *qr = new MyFootageQRWidget(this);
  hlayout->addWidget(qr, 1);
}