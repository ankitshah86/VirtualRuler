#include "ruler.h"

Ruler::Ruler()
{
    this->setMinimumHeight(int(QApplication::desktop()->screen()->height() * 0.7));
    leftSide = new Side(Side::left,Side::cm);
    rightSide = new Side(Side::right,Side::inch);
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
   // setAttribute(Qt::WA_NoSystemBackground);
   // setAttribute(Qt::WA_TranslucentBackground);


    leftSide->cmBtn->setChecked(true);
    rightSide->inchBtn->setChecked(true);

    opacitySlider = new QSlider();
    opacitySlider->setRange(1,100);
    opacitySlider->setMaximumHeight(100);
    opacitySlider->setToolTip("Adjust Transperancy");

    closeButton = new QToolButton();
    closeButton->setText("Close");

    QGroupBox *gl = new QGroupBox("Left Side");
    QGroupBox *gr = new QGroupBox("Right Side");
    QGroupBox *go = new QGroupBox("Transperancy");


    QVBoxLayout *vl = new QVBoxLayout();
    vl->addWidget(leftSide->cmBtn);
    vl->addWidget(leftSide->inchBtn);
    vl->addWidget(leftSide->pixelBtn);
    vl->addStretch(1);
    gl->setLayout(vl);

    QVBoxLayout *vr = new QVBoxLayout();
    vr->addWidget(rightSide->cmBtn);
    vr->addWidget(rightSide->inchBtn);
    vr->addWidget(rightSide->pixelBtn);
    vr->addStretch(1);
    gr->setLayout(vr);

    QVBoxLayout *vo = new QVBoxLayout();
    vo->addWidget(opacitySlider,Qt::AlignHCenter);
    vo->setAlignment(opacitySlider,Qt::AlignHCenter);
    go->setLayout(vo);



    QVBoxLayout *v = new QVBoxLayout();
    v->addStretch(1);
    v->addWidget(gl);
    v->addWidget(gr);
    v->addWidget(go);
    v->addWidget(closeButton);
    v->setAlignment(closeButton,Qt::AlignHCenter);
    v->addStretch(1);




    QHBoxLayout *h = new QHBoxLayout(this);

    h->addWidget(leftSide);
    h->addLayout(v, Qt::AlignHCenter);
    h->addWidget(rightSide);
    h->setMargin(0);
    this->setLayout(h);


    connect(opacitySlider,SIGNAL(valueChanged(int)),this,SLOT(resetOpacity(int)));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));

}

void Ruler::resetOpacity(int op)
{
    this->setWindowOpacity(1 - op/120.0);
    //this->setStyleSheet("background-color:rgba(0,0,0,0);");
}


void Ruler::mousePressEvent(QMouseEvent *event)
{
   // qDebug()<<event->pos();
    isMousePressed = true;
    mousePressX = event->x();
    mousePressY = event->y();
}

void Ruler::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    isMousePressed = false;
}

void Ruler::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed){
        move(event->globalX() - mousePressX,event->globalY() -mousePressY);
    }
}

Side::Side(side side, markings markings)
{
    this->mySide = side;
    this->myMarkings = markings;
    this->setMinimumWidth(50);
    cmBtn = new QRadioButton("Cm");
    inchBtn = new QRadioButton("Inch");
    pixelBtn = new QRadioButton("Pixel");
    connect(cmBtn,SIGNAL(toggled(bool)),this,SLOT(updateMarkings()));
    connect(inchBtn,SIGNAL(toggled(bool)),this,SLOT(updateMarkings()));
    connect(pixelBtn,SIGNAL(toggled(bool)),this,SLOT(updateMarkings()));
}

void Side::updateMarkings()
{
    if(cmBtn->isChecked()){
        myMarkings = cm;
    }
    else if(inchBtn->isChecked()){
        myMarkings = inch;
    }
    else if(pixelBtn->isChecked()){
        myMarkings = pixel;
    }

    this->update();
}

void Side::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);

    int majorLineStart = 0;
    int minorLineStart = 0;
    int regularLineStart = 0;
    int majorLineEnd = 20;
    int minorLineEnd = 15;
    int regularLineEnd = 10;
    int textPosition = 25;
    double distBetweenMarkings = 0 ;


    if(myMarkings == cm){
        distBetweenMarkings = 25.4;
    }
    else if(myMarkings == inch){
        distBetweenMarkings = 8;
    }
    else if(myMarkings == pixel) {
        distBetweenMarkings = 1/10.0; //this->physicalDpiY();
    }

    if(this->mySide == right)
    {
         majorLineStart = this->width() - majorLineEnd;
         minorLineStart = this->width() - minorLineEnd;
         regularLineStart = this->width() - regularLineEnd;
         textPosition = this->width() - 35;
         majorLineEnd = this->width();
         minorLineEnd = this->width();
         regularLineEnd = this->width();
    }
    this->setMaximumWidth(this->logicalDpiX()*4);
    QPainter painter(this);

    QPen p;
    p.setWidth(1);

    painter.setPen(p);

    int h = 0;
    int i = 0;

    if(myMarkings == pixel){
        while(h<this->height()){
            h = h+ 2;//draw in the increaments of 2 so that each line is distinctly visible
            if(i%10 == 0)  painter.drawLine(majorLineStart,h,majorLineEnd,h);
            else if(i%5 == 0) painter.drawLine(minorLineStart,h,minorLineEnd,h);
            else painter.drawLine(regularLineStart,h,regularLineEnd,h);

            int additionalTextMargin = 0;
           if( QString::number(i*2).length() > 1 && mySide == right){
               additionalTextMargin = (QString::number(i*2).length() - 1) * 5;
           }
            if(i%10 == 0) painter.drawText(textPosition - additionalTextMargin,h + 5,QString::number(i*2));

            i++;
        }
    }
    else if(myMarkings == cm ){
        while(h<this->height()){
            h = int(i * double(this->physicalDpiY())/distBetweenMarkings);
            if(i%10 == 0)  painter.drawLine(majorLineStart,h,majorLineEnd,h);
            else if(i%5 == 0) painter.drawLine(minorLineStart,h,minorLineEnd,h);
            else painter.drawLine(regularLineStart,h,regularLineEnd,h);

            if(i%10 == 0) painter.drawText(textPosition,h + 5,QString::number(i/10));

            i++;
        }
    }
    else if(myMarkings == inch){
        while(h<this->height()){
            h = int(i * double(this->physicalDpiY())/distBetweenMarkings);
            if(i%8 == 0)  painter.drawLine(majorLineStart,h,majorLineEnd,h);
            else if(i%4 == 0) painter.drawLine(minorLineStart,h,minorLineEnd,h);
            else painter.drawLine(regularLineStart,h,regularLineEnd,h);
            if(i%8 == 0) painter.drawText(textPosition,h + 5,QString::number(i/8));
            i++;
        }

    }


}


