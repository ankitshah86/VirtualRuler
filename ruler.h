#ifndef RULER_H
#define RULER_H

#include <QWidget>
#include <QLayout>
#include <QtWidgets>
#include <QPainter>
#include <QScreen>


class Side : public QWidget
{


    Q_OBJECT
public:
    enum side {left, right};
    enum markings {cm, inch, pixel};

    QRadioButton *cmBtn, *inchBtn, *pixelBtn;
    Side(side side, markings markings);
    side mySide;
    markings myMarkings;

public slots:
    void updateMarkings();

protected:
    void paintEvent(QPaintEvent *event) override;
};


class Ruler : public QWidget
{
    Q_OBJECT

public:
    Ruler();

    Side *leftSide, *rightSide;
    QSlider *opacitySlider;
    bool isMousePressed = false;
    int mousePressX,mousePressY;
    QToolButton *closeButton;

public slots:
    void resetOpacity(int op);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};


#endif // RULER_H
