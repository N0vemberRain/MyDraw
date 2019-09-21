#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QComboBox>

#include "types.h"

class InputWgt : public QWidget {
    Q_OBJECT
public:
    explicit InputWgt() : QWidget() {
        mOkButton = new QPushButton("&Создать");
        connect(mOkButton, SIGNAL(clicked()), this, SLOT(slotCreate()));

        mCancelButton = new QPushButton("&Стоп");
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(slotStop()));

        mPosLabel = new QLabel("&Точка 1");
        mPosXLine = new QLineEdit();

        mPosLabel->setBuddy(mPosXLine);
        mPosYLine = new QLineEdit();
        mPosLabel->setBuddy(mPosYLine);

        auto layout = new QHBoxLayout;
        layout->addWidget(mOkButton);
        layout->addWidget(mCancelButton);
        layout->addWidget(mPosLabel);
        layout->addWidget(mPosXLine);
        layout->addWidget(mPosYLine);
    }
    virtual ~InputWgt() {}
    QHBoxLayout* getLayout() {
        return mLayout;
    }
private:
    QLabel *mPosLabel;
    QLineEdit *mPosXLine;
    QLineEdit *mPosYLine;
    QPushButton *mOkButton;
    QPushButton *mCancelButton;
    QHBoxLayout *mLayout;
};

class LineInputWgt : public InputWgt {
    Q_OBJECT
public:
    explicit LineInputWgt() : InputWgt() {
        mEndLabel = new QLabel("&Точка 2");
        mEndXLine = new QLineEdit();

        mEndLabel->setBuddy(mEndXLine);
        mEndYLine = new QLineEdit();
        mEndLabel->setBuddy(mEndYLine);
        getLayout()->addWidget(mEndLabel);
        getLayout()->addWidget(mEndXLine);
        getLayout()->addWidget(mEndYLine);
    }

    virtual ~LineInputWgt() {}
private:
    QLabel *mEndLabel;
    QLineEdit *mEndXLine;
    QLineEdit *mEndYLine;
};

class RectInputWgt : public InputWgt {
    Q_OBJECT
public:
    explicit RectInputWgt() : InputWgt() {

    }

    virtual ~RectInputWgt() {}
private:
    QLabel *mEndLabel;
    QLineEdit mEndXLine;
    QLineEdit *mEndYLine;
    QLabel *mWidthLabel;
    QLabel *mHeightLabel;
    QLineEdit *mWidthLine;
    QLineEdit *mHeightLine;
};

class InputFactory : public QWidget {
    Q_OBJECT
public:
    explicit InputFactory() : QWidget() {}
    virtual ~InputFactory() {}

    virtual InputWgt* factoryMethod() = 0;

private:

};

class LineInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit LineInputFactory() :InputFactory () {}
    ~LineInputFactory() {}

    InputWgt* factoryMethod() {
        return new LineInputWgt;
    }
private:

};

class RectInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit RectInputFactory() : InputFactory () {}
    ~RectInputFactory() {}

    InputWgt* factoryMethod() {
        return new RectInputWgt;
    }
private:

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
