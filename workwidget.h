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
    explicit InputWgt(QWidget *wgt = nullptr) : QWidget(wgt) {
        mOkButton = new QPushButton("&Создать");
        connect(mOkButton, SIGNAL(clicked()), this, SLOT(slotOk()));

        mCancelButton = new QPushButton("&Стоп");
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));

        mLayout = new QGridLayout;
        mLayout->addWidget(mOkButton, 0, 0);
        mLayout->addWidget(mCancelButton, 0, 1);

        setStyleSheet("background: rgb(50, 50, 50); color: white;");
        setMaximumSize(200, 200);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);


        mLayout->setVerticalSpacing(5);
        setLayout(mLayout);
    }
    virtual ~InputWgt() {}

    void createLabel(QLabel *label) {
        label->setMinimumHeight(50);
        label->setMaximumHeight(50);
        label->setMaximumWidth(250);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    }
    QGridLayout* getLayout() {
        return mLayout;
    }

private slots:
    virtual void slotOk() = 0;
    virtual void slotCancel() = 0;

private:
    QPushButton *mOkButton;
    QPushButton *mCancelButton;
    QGridLayout *mLayout;

};

class FormInputWgt : public InputWgt {
    Q_OBJECT
public:
    explicit FormInputWgt() : InputWgt() {

        mPosLabel = new QLabel("&Точка 1");
        createLabel(mPosLabel);
        mPosXLine = new QLineEdit();

        mPosLabel->setBuddy(mPosXLine);
        mPosYLine = new QLineEdit();
        mPosLabel->setBuddy(mPosYLine);

        getLayout()->addWidget(mPosLabel, 1, 0, 1, 2);
        getLayout()->addWidget(mPosXLine, 2, 0);
        getLayout()->addWidget(mPosYLine, 2, 1);
    }

    virtual ~FormInputWgt() {}

protected:
    QString getPosX() const { return mPosXLine->text(); }
    QString getPosY() const { return mPosYLine->text(); }

private slots:
    virtual void slotOk();
    virtual void slotCancel();
private:
    virtual QStringList prepareData() const = 0;
    QLabel *mPosLabel;
    QLineEdit *mPosXLine;
    QLineEdit *mPosYLine;

signals:
    void okSignal(const QStringList &data);
    void cancelSignal();
};

class LineInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit LineInputWgt() : FormInputWgt() {
        mEndLabel = new QLabel("&Точка 2");
        createLabel(mEndLabel);
        mEndXLine = new QLineEdit();

        mEndLabel->setBuddy(mEndXLine);
        mEndYLine = new QLineEdit();
        mEndLabel->setBuddy(mEndYLine);
        getLayout()->addWidget(mEndLabel, 3, 0, 1, 2);
        getLayout()->addWidget(mEndXLine, 4, 0);
        getLayout()->addWidget(mEndYLine, 4, 1);

        setLayout(getLayout());

        for(int i = 0; i < getLayout()->rowCount(); i++) {
            getLayout()->setRowStretch(i, 1);
        }


    }

    virtual ~LineInputWgt() {}

    virtual QStringList prepareData() const;
private slots:
    virtual void slotOk();
    virtual void slotCancel();
private:
    QLabel *mEndLabel;
    QLineEdit *mEndXLine;
    QLineEdit *mEndYLine;
};

class RectInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit RectInputWgt() : FormInputWgt() {
        mEndLabel = new QLabel(tr("&Точка2"));
        createLabel(mEndLabel);
        mEndXLine = new QLineEdit();
        mEndYLine = new QLineEdit();
        mEndLabel->setBuddy(mEndXLine);
        mEndLabel->setBuddy(mEndYLine);

        mWidthLabel = new QLabel(tr("&Ширина"));
        createLabel(mWidthLabel);
        mWidthLine = new QLineEdit();
        mWidthLabel->setBuddy(mWidthLine);

        mHeightLabel = new QLabel(tr("&Высота"));
        createLabel(mHeightLabel);
        mHeightLine = new QLineEdit();
        mHeightLabel->setBuddy(mHeightLine);

        getLayout()->addWidget(mEndLabel, 3, 0, 1, 2);
        getLayout()->addWidget(mEndXLine, 4, 0);
        getLayout()->addWidget(mEndYLine, 4, 1);
        getLayout()->addWidget(mWidthLabel, 5, 0);
        getLayout()->addWidget(mWidthLine, 5, 1);
        getLayout()->addWidget(mHeightLabel, 6, 0);
        getLayout()->addWidget(mHeightLine, 6, 1);

        setLayout(getLayout());

        for(int i = 0; i < getLayout()->rowCount(); i++) {
            getLayout()->setRowStretch(i, 1);
        }
    }

    virtual ~RectInputWgt() {}

    virtual QStringList prepareData() const;

private slots:
    virtual void slotOk();
    virtual void slotCancel();
private:
    QLabel *mEndLabel;
    QLineEdit *mEndXLine;
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

class ShearInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit ShearInputFactory() : InputFactory() {}
    ~ShearInputFactory() {}
    /*
    ShearInputWgt* factoryMethod() {
        return new ShearInputWgt;
    }*/
};

class LineInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit LineInputFactory() : InputFactory () {}
    ~LineInputFactory() {}

    FormInputWgt* factoryMethod() {
        return new LineInputWgt;
    }
private:

};

class RectInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit RectInputFactory() : InputFactory () {}
    ~RectInputFactory() {}

    FormInputWgt* factoryMethod() {
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
