/**
    @name:     mmgk_painter_gui
    @author:   Marek Jenda
    @revision: 1.0
    @date:     29.03.2014
    @file:     renderArea.h
    @depends:  none
**/

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <vector>

// obszar renderowania -- renderuje wiele kopii odpowiedniego ksztaltu wybranego przrz uzytkownika.

class RenderArea : public QWidget {
    Q_OBJECT

public:
    enum Shape {
        Cykloida,
        Sinus
    };
    RenderArea(QWidget *parent = 0);
    QSize minimumSizeHint() const; // metody dziedziczone po QWidget
    QSize sizeHint() const;

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setAntialiased(bool antialiased);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Shape shape;
    QPen pen;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;
    struct Coef {
        double a, b, c, d;
    };

    struct Point {
        double x, a;
    };
    Coef* splineNatural(std::vector<Point>& f);
    QPointF generatePoint(float x, float xj, Coef & coef);
};

#endif // RENDERAREA_H
