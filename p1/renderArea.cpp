/**
    @name:     mmgk_painter_gui
    @author:   Marek Jenda
    @revision: 1.0
    @date:     29.03.2014
    @file:     renderArea.cpp
    @depends:  renderArea.h
**/

#include <QtGui>
#include <math.h>
#include "renderArea.h"
#include <iostream>

typedef unsigned long ul;

RenderArea::RenderArea(QWidget* parent) : QWidget(parent){
    shape = Cykloida;
    antialiased = false;
    transformed = false;


    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

// miminalny rozmiar widgeta
QSize RenderArea::minimumSizeHint() const {
    return QSize(100, 100);
}

//domyslny rozmiar widgeta
QSize RenderArea::sizeHint() const {
    return QSize(600, 400);
}

void RenderArea::setShape(Shape shape) {
   this->shape = shape;
    update();
}

void RenderArea::setPen(const QPen &pen) {
   this->pen = pen;
    update();
}

void RenderArea::setBrush(const QBrush &brush) {
   this->brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased) {
   this->antialiased = antialiased;
    update();
}

void RenderArea::setTransformed(bool transformed) {
   this->transformed = transformed;
    update();
}

void RenderArea::paintEvent(QPaintEvent  *event = NULL) {

    static QPointF function[700];
    int j = 0;



    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);


    // w koncu tworzymy qpainter i uwstawiamy pedzel itp
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    if(antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    for(int x = 0; x < 100; x += 100) {
        for(int y = 0; y < 100; y += 100) {
            painter.save();
            painter.translate(x, y);
            if(transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.6);
                painter.translate(-50, -50);
            }
            switch(shape) {
            case Cykloida: {
                j = 0;
                std::vector<Point> splinePoints;
                for(float i = 0; i <= 70; i+=0.1, j++) {
                    function[j] = QPointF(30*i - 60*sin(i), 30 - 60*cos(i)+150);
                    Point p = {30*i - 60*sin(i), 30 - 60*cos(i)+150};
                    if(j % 2== 0) splinePoints.push_back(p);
                }
                painter.drawPolyline(function, 700);
                Coef * coef = splineNatural(splinePoints);

                j = 0;
                for(float i = 0; i <= 70; i+=0.1, j++) {
                    for(int k = 0; k < static_cast<int>(splinePoints.size()-1); k++)
                        if((splinePoints[k].x <= (30*i - 60*sin(i))) && ((30*i - 60*sin(i) )<= splinePoints[k+1].x)) {
                            function[j] = generatePoint(30*i - 60*sin(i), splinePoints[k].x, coef[k]);
                            break;
                        }

                }
                painter.drawPolyline(function, 700);

            }
                break;
            case Sinus:
                j = 0;
                for(float i = 0; i < 70; i+=0.1, j++) {
                    function[j] = QPointF(100*i, 100*sin(i)+170);
                }
                painter.drawPolyline(function, 700);
            }
            painter.restore();
        }
    }
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() -1, height() - 1));
}

RenderArea::Coef* RenderArea::splineNatural(std::vector<Point>& f) {
    ul n = f.size() - 1; // f.size() = n+1
    // Step 1
    double * h = new double[n];
    for(ul i = 0; i < n; i++)
        h[i] = f[i+1].x - f[i].x;

    // Step 2
    double * alpha = new double[n];
    for(ul i = 1; i < n; i++)
        alpha[i] = (3 / h[ i ]) * (f[i+1].a - f[ i ].a)
                 - (3 / h[i-1]) * (f[ i ].a - f[i-1].a);

    // Step 3
    double * l  = new double[n];
    double * mi = new double[n];
    double * z  = new double[n];
    l[ 0] = 1;
    mi[0] = 0;
    z[ 0] = 0;

    // Step 4
    for(ul i = 1; i < n; i++) {
        l[i] = 2*(f[i+1].x - f[i-1].x) - h[i-1]*mi[i-1];
        mi[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }

    //Step 5
    double * c  = new double[n];
    l[n] = 1;
    z[n] = 0;
    c[n] = 0;

    // Step 6
    double * b  = new double[n];
    double * d  = new double[n];
    for(long  j = n-1; j >= 0; j--) {
        c[j] = z[j] - mi[j]*c[j+1];
        b[j] = (f[j+1].a - f[j].a)/h[j] - h[j]*(c[j+1] + 2*c[j])/3;
        d[j] = (c[j+1] - c[j])/(3*h[j]);
    }

    // Step 7 (preparing return)
    Coef * coef = new Coef[n];
    for(ul i = 0; i < n; i++) {
        coef[i].a = f[i].a;
        coef[i].b = b[i];
        coef[i].c = c[i];
        coef[i].d = d[i];
    }

    // Step 7 (tiding)
    delete [] b;
    delete [] c;
    delete [] d;
    delete [] l;
    delete [] z;
    delete [] mi;
    delete [] alpha;
    delete [] h;

    // Step 8 (return)
    return coef;
}

QPointF RenderArea::generatePoint(float x, float xj, Coef & coef) {
    return QPointF(x, coef.a
                      + coef.b*(x-xj)
                      + coef.c*(x-xj)*(x-xj)
                      + coef.d*(x-xj)*(x-xj)*(x-xj)
                   );
}
