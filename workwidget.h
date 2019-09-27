#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QComboBox>

#include "types.h"

class ShearInputWgt : public QWidget {
    Q_OBJECT
public:
    explicit ShearInputWgt(QWidget *parent = nullptr)
        : QWidget(parent) {
        okButton = new QPushButton(tr("&Ok"));
        cancelButton = new QPushButton(tr("&Cancel"));

        mHSLabel = new QLabel(tr("&Горизонтальный сдвиг"));
        mVSLabel = new QLabel(tr("&Вертикальный сдвиг"));
        mHSLine = new QLineEdit;
        mVSLine = new QLineEdit;

        mHSLabel->setBuddy(mHSLine);
        mVSLabel->setBuddy(mVSLine);

        QHBoxLayout *l = new QHBoxLayout;
        l->addWidget(mHSLabel);
        l->addWidget(mHSLine);
        l->addWidget(mVSLabel);
        l->addWidget(mVSLine);
        l->addWidget(okButton);
        l->addWidget(cancelButton);

        setLayout(l);

        connect(okButton, SIGNAL(clicked()), this, SLOT(slotOk()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    }

    QPair<int, int> getShear() const {
        return qMakePair(mHSLine->text().toInt(), mVSLine->text().toInt());
    }

private slots:
    void slotOk() {
        int xs = mHSLine->text().toInt();
        int ys = mVSLine->text().toInt();

        emit emitOk(xs, ys);
    }
    void slotCancel() {
        emit emitCancel();
    }
private:
    QLabel *mHSLabel;
    QLabel *mVSLabel;
    QLineEdit *mHSLine;
    QLineEdit *mVSLine;
    int mHorizontalShear;
    int mVerticalShear;
    QPushButton *okButton;
    QPushButton *cancelButton;

signals:
    void emitOk(const int horizontalShear, const int verticalShear);
    void emitCancel();
};

class PointInputWgt : public QWidget {
    Q_OBJECT
public:
    explicit PointInputWgt(QWidget *wgt = nullptr) : QWidget() {
        createBut = new QPushButton("&Создать");
        connect(createBut, SIGNAL(clicked()), this, SLOT(slotCreate()));

        stopBut = new QPushButton("&Стоп");
        connect(stopBut, SIGNAL(clicked()), this, SLOT(slotStop()));

        pointOne = new QLabel("&Точка 1");
        pointOneEditX = new QLineEdit();
        pointOne->setBuddy(pointOneEditX);
        pointOneEditY = new QLineEdit();
        pointOne->setBuddy(pointOneEditY);

        layout = new QHBoxLayout;
        layout->addWidget(createBut);
        layout->addWidget(stopBut);
        layout->addWidget(pointOne);
        layout->addWidget(pointOneEditX);
        layout->addWidget(pointOneEditY);
        setLayout(layout);

    }
    QStringList getData(const ItemType type) {
        QStringList data;
        data << "P" << pointOneEditX->text() << pointOneEditY->text();
        return data;
    }

signals:
    void stop();
    void create();

    void dataChanged();
public slots:
    void slotCreate() { emit create(); }
    void slotStop() { emit stop(); }

private:
    QHBoxLayout *layout;
    QPushButton *createBut;
    QPushButton *stopBut;
    QLabel *pointOne;
    QLineEdit *pointOneEditX;
    QLineEdit *pointOneEditY;
};

class WorkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkWidget(QWidget *parent = nullptr, const ItemType type = ItemType::Point);

    void makePointWgt();
    void makeLineWgt();
    void makeRectWgt();
    void makeCircleWgt();
    void makePolylineWgt();
    void makeTextWgt();

    void checkData();
    void checkRectData();
    void checkCircleData();

    QStringList getData(const ItemType type);
    void setItemData(const QStringList &data);

    void setPoint(const QPointF &p);
    void endInput();

   /* QString getText() {
        while (waitText) {
            ;
        }
        waitText = true;
        return widthEdit->text();
    }*/
private:
    QStringList getPointData();
    QStringList getLineData();
    QStringList getRectData();
    QStringList getCircleData();
    QStringList getPolylineData();
    QStringList getTextData();

    void setPointData(const QStringList &data);
    void setLineData(const QStringList &data);
    void setPolylineData(const QStringList &data);
    void setRectData(const QStringList &data);
    void setCircleData(const QStringList &data);
    void setTextData(const QStringList &data);

    void changeRectWay(const int index);
    void changeCircleWay(const int index);

    QPushButton *createBut;
    QPushButton *stopBut;
    QLabel *pointOne;
    QLabel *pointTwo;
    QLabel *width;
    QLabel *height;
    QLineEdit *pointOneEditX;
    QLineEdit *pointOneEditY;
    QLineEdit *pointTwoEditX;
    QLineEdit *pointTwoEditY;
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QComboBox *changeWay;
    QComboBox *fontHeight;

    QHBoxLayout *layout;

    ItemType type;

    bool waitText;
signals:
    void stop();
    void create();

    void dataChanged();
public slots:
    void slotCreate();
    void slotStop();

private slots:
    void slotChangeData();
    void slotChangeData(const QString &text);
    void slotChangeRectData();
    void slotChangeWay(const int index);
   /* void slotEditingFinished() {
        waitText = false;
    }*/
};


#endif // WORKWIDGET_H
