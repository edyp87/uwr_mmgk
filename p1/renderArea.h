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
        Line,
        Points,
        Polyline,
        Polygon,
        Rect,
        RoundedRect,
        Ellipse,
        Arc,
        Chord,
        Pie,
        Path,
        Text,
        Cykloida,
        Sinus
    };
    RenderArea(QWidget *parent = 0);
    QSize minimumSizeHint() const; // metody dziedziczone po QWidget
    QSize sizeHint() const;

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Shape shape;
    QPen pen;
    QBrush brush;
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
