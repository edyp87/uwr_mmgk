/**
    @name:     mmgk_painter_gui
    @author:   Marek Jenda
    @revision: 1.0
    @date:     29.03.2014
    @file:     window.h
    @depends:  none
**/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>


// deklaracje wyprzedzajace.
// Nie ma sensu inkludowac wszystkich klas w tym pliku.
class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;

class RenderArea;

class Window : public QWidget {
    Q_OBJECT
public:
    Window();

private slots:
    void shapeChanged();
    void penChanged();
    void brushChanged();

private:
    // Okreslamy wskazniki do widgetow w glownym oknie
    RenderArea  *renderArea;
    QLabel      *shapeLabel;
    QLabel      *penWidthLabel;
    QLabel      *penStyleLabel;
    QLabel      *penCapLabel;
    QLabel      *penJoinLabel;
    QLabel      *brushStyleLabel;
    QLabel      *otherOptionsLabel;
    QComboBox   *shapeComboBox;
    QSpinBox    *penWidthSpinBox;
    QComboBox   *penStyleComboBox;
    QComboBox   *penCapComboBox;
    QComboBox   *penJoinComboBox;
    QComboBox   *brushStyleComboBox;
    QCheckBox   *antialiasingCheckBox;
    QCheckBox   *transformationsCheckBox;


};

#endif // WINDOW_H
