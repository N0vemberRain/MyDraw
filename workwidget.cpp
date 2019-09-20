#include "workwidget.h"

#include <QComboBox>

WorkWidget::WorkWidget(QWidget *parent, const ItemType type) : QWidget(parent), type(type)
{
    createBut = new QPushButton("&Создать");
    connect(createBut, SIGNAL(clicked()), this, SLOT(slotCreate()));

    stopBut = new QPushButton("&Стоп");
    connect(stopBut, SIGNAL(clicked()), this, SLOT(slotStop()));

    pointOne = new QLabel("&Точка 1");
    pointOneEditX = new QLineEdit();

    pointOne->setBuddy(pointOneEditX);
    pointOneEditY = new QLineEdit();
    pointOne->setBuddy(pointOneEditY);
    changeWay = new QComboBox;
    connect(changeWay, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeWay(int)));

    layout = new QHBoxLayout;
    layout->addWidget(createBut);
    layout->addWidget(stopBut);
    layout->addWidget(changeWay);
    layout->addWidget(pointOne);
    layout->addWidget(pointOneEditX);
    layout->addWidget(pointOneEditY);

    pointTwoEditX = nullptr;
    pointTwoEditY = nullptr;
    widthEdit = nullptr;
    heightEdit = nullptr;

    switch (type) {
    case ItemType::Point: makePointWgt(); break;
    case ItemType::Line: makeLineWgt(); break;
    case ItemType::Rect: makeRectWgt(); break;
    case ItemType::Circle: makeCircleWgt(); break;
    case ItemType::Polyline: makePolylineWgt(); break;
    case ItemType::Text: makeTextWgt(); break;
    }
    setLayout(layout);


}

void WorkWidget::makePointWgt() {

}

void WorkWidget::makeLineWgt() {
    pointTwo = new QLabel("&Точка 2");
    pointTwoEditX = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditX);
    pointTwoEditY = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditY);
    changeWay->setEnabled(false);

    layout->addWidget(pointTwo);
    layout->addWidget(pointTwoEditX);
    layout->addWidget(pointTwoEditY);
}

void WorkWidget::makeRectWgt() {

    pointTwo = new QLabel("&Точка 2");
    pointTwoEditX = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditX);
    pointTwoEditY = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditY);

    width = new QLabel("&Длина");
    widthEdit = new QLineEdit;
    width->setBuddy(width);
    height = new QLabel("&Высота");
    heightEdit = new QLineEdit;
    height->setBuddy(heightEdit);

    layout->addWidget(pointTwo);
    layout->addWidget(pointTwoEditX);
    layout->addWidget(pointTwoEditY);
    layout->addWidget(width);
    layout->addWidget(widthEdit);
    layout->addWidget(height);
    layout->addWidget(heightEdit);

    connect(pointOneEditX, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData()));
    connect(pointOneEditY, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(pointTwoEditX, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(pointTwoEditY, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(widthEdit, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(heightEdit, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    changeWay->addItems(
                QStringList() << "2 точки" << "Длина/Ширина"
                );
}

void WorkWidget::makeCircleWgt() {
    pointTwo = new QLabel("&Точка 2");
    pointTwoEditX = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditX);
    pointTwoEditY = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditY);

    width = new QLabel("&Радиус");
    widthEdit = new QLineEdit;
    width->setBuddy(width);

    layout->addWidget(pointTwo);
    layout->addWidget(pointTwoEditX);
    layout->addWidget(pointTwoEditY);
    layout->addWidget(width);
    layout->addWidget(widthEdit);

    connect(pointOneEditX, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData()));
    connect(pointOneEditY, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(pointTwoEditX, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(pointTwoEditY, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));
    connect(widthEdit, SIGNAL(textEdited(QString)),this , SLOT(slotChangeData(QString)));

    changeWay->addItems(QStringList() << "2 точки" << "Радиус");
}

void WorkWidget::makePolylineWgt() {
    pointTwo = new QLabel("&Следующая точка");
    pointTwoEditX = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditX);
    pointTwoEditY = new QLineEdit();
    pointTwo->setBuddy(pointTwoEditY);

    QComboBox *cBox = new QComboBox;

    layout->addWidget(pointTwo);
    layout->addWidget(pointTwoEditX);
    layout->addWidget(pointTwoEditY);
    layout->addWidget(cBox);
}

void WorkWidget::makeTextWgt() {
    width = new QLabel("&Текст");
    widthEdit = new QLineEdit();
    width->setBuddy(widthEdit);
    waitText = true;
    connect(widthEdit, SIGNAL(editingFinished()), this, SLOT(slotEditingFinished()));

    QComboBox *cBox = new QComboBox;
    cBox->setDisabled(true);

    fontHeight = new QComboBox;
    fontHeight->addItem("5");
    fontHeight->addItem("7");
    fontHeight->addItem("10");
    fontHeight->addItem("11");
    fontHeight->addItem("14");

    layout->addWidget(width);
    layout->addWidget(widthEdit);
    layout->addWidget(fontHeight);
    layout->addWidget(cBox);
}

void WorkWidget::slotCreate() {
    emit create();
}

void WorkWidget::slotStop() {
    emit stop();
}

QStringList WorkWidget::getData(const ItemType type) {
    QStringList tmp;
    switch(type) {
    case ItemType::Point: tmp = getPointData(); break;
    case ItemType::Line: tmp = getLineData(); break;
    case ItemType::Rect: tmp = getRectData(); break;
    case ItemType::Circle: tmp = getCircleData(); break;
    case ItemType::Polyline: tmp = getPolylineData(); break;
    case ItemType::Text: tmp = getTextData(); break;
    }

    return tmp;
}

QStringList WorkWidget::getPointData() {
    QStringList data;
    data << "P" << pointOneEditX->text() << pointOneEditY->text();

    return data;
}

QStringList WorkWidget::getLineData() {
    QStringList data;
    data << "P1" << pointOneEditX->text() << pointOneEditY->text() <<
            "P2" << pointTwoEditX->text() << pointTwoEditY->text();

    return data;
}

QStringList WorkWidget::getRectData() {
    QStringList data;
    switch(changeWay->currentIndex()) {
    case 0: widthEdit->clear();
        heightEdit->clear();
        break;
    case 1: pointTwoEditX->clear();
        pointTwoEditY->clear();
        break;
    }
    data << QString::number(RectType) << "P1" << pointOneEditX->text() << pointOneEditY->text() <<
            "P2" << pointTwoEditX->text() << pointTwoEditY->text() <<
            "W" << widthEdit->text() << "H" << heightEdit->text();

    return data;
}

QStringList WorkWidget::getCircleData() {
    QStringList data;
    data << QString::number(CircleType) << "PO" << pointOneEditX->text() << pointOneEditY->text() <<
            "PR" << pointTwoEditX->text() << pointTwoEditY->text() <<
            "R" << widthEdit->text();

    return data;
}

QStringList WorkWidget::getPolylineData() {

}

QStringList WorkWidget::getTextData() {
    QStringList data;
    data << QString::number(TextType) << "P" << pointOneEditX->text() << pointOneEditY->text() <<
            "T" << widthEdit->text() << "FH" << fontHeight->currentText();

    return data;
}

void WorkWidget::setRectData(const QStringList &data) {
    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            pointOneEditX->setText(data.at(i + 1));
            pointOneEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            pointTwoEditX->setText(data.at(i + 1));
            pointTwoEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "W") {
            widthEdit->setText(data.at(i + 1));
            i++;
            continue;
        }
        if(data.at(i) == "H") {
            heightEdit->setText(data.at(i + 1));
            i++;
            continue;
        }
    }
}

void WorkWidget::setLineData(const QStringList &data) {
    if(data.at(0).toInt() != LineType) {
        return;
    }

    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            pointOneEditX->setText(data.at(i + 1));
            pointOneEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            pointTwoEditX->setText(data.at(i + 1));
            pointTwoEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
      /*  if(data.at(i) == "L") {
            widthEdit->setText(data.at(i + 1));
            i++;
            continue;
        }
        if(data.at(i) == "A") {
            heightEdit->setText(data.at(i + 1));
            i++;
            continue;
        }*/
    }
}

void WorkWidget::setCircleData(const QStringList &data) {
    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "PO") {
            pointOneEditX->setText(data.at(i + 1));
            pointOneEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "PR") {
            pointTwoEditX->setText(data.at(i + 1));
            pointTwoEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "R") {
            widthEdit->setText(data.at(i + 1));
            i++;
            continue;
        }
    }
}

void WorkWidget::setTextData(const QStringList &data) {
    if(data.at(0).toInt() != TextType) {
        return;
    }

    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "P") {
            pointOneEditX->setText(data.at(i + 1));
            pointOneEditY->setText(data.at(i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "T") {
            widthEdit->setText(data.at(i + 1));
            i++;
            continue;
        }
        if(data.at(i) == "FH") {
            fontHeight->setCurrentText(data.at(i + 1));
            i++;
            continue;
        }
    }
}

void WorkWidget::setItemData(const QStringList &data) {
    auto type = data.at(0).toInt();
    switch (type) {
    case RectType: setRectData(data); break;
    case CircleType: setCircleData(data); break;
    case LineType: setLineData(data); break;
    case TextType: setTextData(data); break;
    }
}

void WorkWidget::slotChangeData() {
    switch (type) {
    case ItemType::Rect: slotChangeRectData(); break;
    }
}

void WorkWidget::slotChangeData(const QString &text) {
    switch (type) {
    case ItemType::Rect: slotChangeRectData(); break;
    //case ItemType::Circle: slot+
    }
}

void WorkWidget::slotChangeRectData() {

}

void WorkWidget::changeRectWay(const int index) {
    switch (index) {
    case 0: widthEdit->setDisabled(true);
        widthEdit->clear();
        heightEdit->setDisabled(true);
        heightEdit->clear();
        pointTwoEditX->setEnabled(true);
        pointTwoEditY->setEnabled(true);
        break;
    case 1: widthEdit->setEnabled(true);
        heightEdit->setEnabled(true);
        pointTwoEditX->setDisabled(true);
        pointTwoEditX->clear();
        pointTwoEditY->setDisabled(true);
        pointTwoEditY->clear();
        break;
    }
}

void WorkWidget::changeCircleWay(const int index) {
    switch (index) {
    case 0: widthEdit->setDisabled(true);
        widthEdit->clear();
       // pointOneEditX->setEnabled(true);
       // pointOneEditY->setEnabled(true);
        pointTwoEditX->setEnabled(true);
        pointTwoEditY->setEnabled(true);
        break;
    case 1: widthEdit->setEnabled(true);
        pointTwoEditX->setDisabled(true);
        pointTwoEditX->clear();
        pointTwoEditY->setDisabled(true);
        pointTwoEditY->clear();
        break;
    }
}

void WorkWidget::slotChangeWay(const int index) {
    switch (type) {
    case ItemType::Rect: changeRectWay(index);
        break;
    case ItemType::Circle: changeCircleWay(index);
        break;
    }
}

void WorkWidget::checkRectData() {
    switch (changeWay->currentIndex()) {
    case 0: widthEdit->setDisabled(true);
        widthEdit->clear();
        heightEdit->setDisabled(true);
        heightEdit->clear();
        pointTwoEditX->setEnabled(true);
        pointTwoEditY->setEnabled(true);
        break;
    case 1: widthEdit->setEnabled(true);
        heightEdit->setEnabled(true);
        pointTwoEditX->setDisabled(true);
        pointTwoEditX->clear();
        pointTwoEditY->setDisabled(true);
        pointTwoEditY->clear();
        break;
    }
}

void WorkWidget::checkCircleData() {
    switch (changeWay->currentIndex()) {
    case 0: widthEdit->setDisabled(true);
        widthEdit->clear();
        pointOneEditX->setEnabled(true);
        pointOneEditY->setEnabled(true);
        pointTwoEditX->setEnabled(true);
        pointTwoEditY->setEnabled(true);
        break;
    case 1: widthEdit->setEnabled(true);
        pointTwoEditX->setDisabled(true);
        pointTwoEditX->clear();
        pointTwoEditY->setDisabled(true);
        pointTwoEditY->clear();
        break;
    }
}

void WorkWidget::checkData() {
    switch (type) {
    case ItemType::Rect: checkRectData();
        break;
    case ItemType::Circle: checkCircleData();
        break;
    }
}

void WorkWidget::setPoint(const QPointF &p) {
    if(qFuzzyCompare(p.x(), pointOneEditX->text().toDouble()) &&
            qFuzzyCompare(p.y(), pointOneEditY->text().toDouble())) {
        return;
    }
    if(pointOneEditX->text().isEmpty() && pointOneEditY->text().isEmpty()) {
        pointOneEditX->setText(QString::number(p.x()));
        pointOneEditY->setText(QString::number(p.y()));
    } else if(pointTwoEditX != nullptr && pointTwoEditY != nullptr) {
        if(pointTwoEditX->text().isEmpty() && pointTwoEditY->text().isEmpty()) {
            pointTwoEditX->setText(QString::number(p.x()));
            pointTwoEditY->setText(QString::number(p.y()));
        }
    }
}

void WorkWidget::endInput() {
    if(pointOneEditX != nullptr) {
        pointOneEditX->clear();
    }
    if(pointOneEditY != nullptr) {
        pointOneEditY->clear();
    }
    if(pointTwoEditX != nullptr) {
        pointTwoEditX->clear();
    }
    if(pointTwoEditY != nullptr) {
        pointTwoEditY->clear();
    }
    if(widthEdit != nullptr) {
        widthEdit->clear();
    }
    if(heightEdit != nullptr) {
        heightEdit->clear();
    }
}
