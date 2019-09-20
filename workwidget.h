#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QComboBox>

#include "types.h"


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
