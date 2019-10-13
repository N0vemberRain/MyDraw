#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>

#include "types.h"

class InputWgt : public QWidget {
    Q_OBJECT
public:
    explicit InputWgt(QWidget *wgt = nullptr) : QWidget(wgt) {
        mOkButton = new QPushButton("&Создать");
        connect(mOkButton, SIGNAL(clicked()), this, SLOT(slotOk()));
        mOkButton->setStyleSheet("background-color: white;\ncolor: black;\nfont: 14px;");

        mCancelButton = new QPushButton("&Стоп");
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
        mCancelButton->setStyleSheet("background-color: white;\ncolor: black;\nfont: 14px;");

        mLayout = new QGridLayout;
        mLayout->addWidget(mOkButton, 0, 0);
        mLayout->addWidget(mCancelButton, 0, 1);

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

    virtual QStringList getData() const = 0;
    virtual void setData(const QStringList &data) = 0;
    virtual void addStyleSheet() = 0;
private slots:
    virtual void slotOk() = 0;
    virtual void slotCancel() = 0;

private:
    QPushButton *mOkButton;
    QPushButton *mCancelButton;
    QGridLayout *mLayout;

signals:
   // virtual void okSignal(const QStringList &data);
    void okSignal();
    void cancelSignal();
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

    virtual QStringList getData() const = 0;
    virtual void setData(const QStringList &data) = 0;
    virtual void addStyleSheet();
protected:
    QString getPosX() const { return mPosXLine->text(); }
    QString getPosY() const { return mPosYLine->text(); }

    void setPosX(const QString &pos) { mPosXLine->setText(pos); }
    void setPosY(const QString &pos) { mPosYLine->setText(pos); }

private slots:
    virtual void slotOk();
    virtual void slotCancel();
private:
    QLabel *mPosLabel;
    QLineEdit *mPosXLine;
    QLineEdit *mPosYLine;

signals:
    //void okSignal(const QStringList &data);
    //void okSignal();
    //void cancelSignal();
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

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();
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
    explicit RectInputWgt();
    virtual ~RectInputWgt() {}

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();
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

class CircleInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit CircleInputWgt();
    virtual ~CircleInputWgt() {}

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();
private slots:
    virtual void slotOk();
    virtual void slotCancel();
private:
    QLabel *mRPLabel;
    QLineEdit *mRPXLine;
    QLineEdit *mRPYLine;
    QLabel *mRLabel;
    QLineEdit *mRLine;
};

class PointInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit PointInputWgt();
    ~PointInputWgt() {}

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();

private slots:
    virtual void slotOk();
    virtual void slotCancel();

private:


};

class PolylineInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit PolylineInputWgt();
    ~PolylineInputWgt() {}

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();

private slots:
    virtual void slotOk();
    virtual void slotCancel();

private:

};

class TextInputWgt : public FormInputWgt {
    Q_OBJECT
public:
    explicit TextInputWgt();
    ~TextInputWgt() {}

    virtual QStringList getData() const;
    virtual void setData(const QStringList &data);
    virtual void addStyleSheet();

private slots:
    virtual void slotOk();
    virtual void slotCancel();

private:
    QTextEdit *mTextEdit;
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

class CircleInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit CircleInputFactory() : InputFactory () {}
    ~CircleInputFactory() {}

    FormInputWgt* factoryMethod() {
        return new CircleInputWgt;
    }
};

class PointInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit PointInputFactory() : InputFactory () {}
    ~PointInputFactory() {}

    FormInputWgt* factoryMethod() {
        return new PointInputWgt;
    }
};

class PolylineInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit PolylineInputFactory() : InputFactory () {}
    ~PolylineInputFactory() {}

    FormInputWgt* factoryMethod() {
        return new PolylineInputWgt;
    }
};

class TextInputFactory : public InputFactory {
    Q_OBJECT
public:
    explicit TextInputFactory() : InputFactory () {}
    ~TextInputFactory() {}

    FormInputWgt* factoryMethod() {
        return new TextInputWgt;
    }
};

class WorkWidget {

};

#endif //WORKWIDGET_H
