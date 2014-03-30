/**
    @name:     mmgk_painter_gui
    @author:   Marek Jenda
    @revision: 1.0
    @date:     29.03.2014
    @file:     window.cpp
    @depends:  window.h
**/

#include <QtGui>

#include "window.h"
#include "renderArea.h"



Window::Window() {
    renderArea = new RenderArea;

    // WYBOR KSZTALTU
    shapeComboBox = new QComboBox;
    shapeComboBox->addItem(tr("Prolate cycloid"), RenderArea::Cykloida);
    shapeComboBox->addItem(tr("Sinus"), RenderArea::Sinus);

    shapeLabel = new QLabel(tr("&Shape:"));
    shapeLabel->setBuddy(shapeComboBox);

    //WYBOR PIORA

    penWidthSpinBox = new QSpinBox;
    penWidthSpinBox->setRange(0, 20);
    penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

    penWidthLabel = new QLabel(tr("Pen &Width:"));
    penWidthLabel->setBuddy(penWidthSpinBox);

    penStyleComboBox = new QComboBox;
    penStyleComboBox->addItem(tr("Solid"), Qt::SolidLine);
    penStyleComboBox->addItem(tr("Dash"), Qt::DashLine);
    penStyleComboBox->addItem(tr("Dot"), Qt::DotLine);
    penStyleComboBox->addItem(tr("Dash Dot"), Qt::DashDotLine);
    penStyleComboBox->addItem(tr("Dash Dot Dot"), Qt::DashDotDotLine);
    penStyleComboBox->addItem(tr("None"), Qt::NoPen);

    penStyleLabel = new QLabel(tr("&Pen Style:"));
    penStyleLabel->setBuddy(penStyleComboBox);


    //INNE OPCJE
    antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));

    //SYGNALY I SLOTY
    connect(shapeComboBox,            SIGNAL(activated(int)),
            this         ,            SLOT(  shapeChanged()));
    connect(penWidthSpinBox,          SIGNAL(valueChanged(int)),
            this           ,          SLOT(  penChanged()));
    connect(penStyleComboBox,         SIGNAL(activated(int)),
            this            ,         SLOT(  penChanged()));
    connect(antialiasingCheckBox   ,  SIGNAL(toggled(bool)),
            renderArea             ,  SLOT(  setAntialiased(bool)));

    //ROZMIESZCZANIE WIDGETOW
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 0.1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(shapeComboBox, 2, 1);
    mainLayout->addWidget(penWidthLabel, 2, 2, Qt::AlignRight);
    mainLayout->addWidget(penWidthSpinBox, 2, 3);
    mainLayout->addWidget(penStyleLabel, 3, 2, Qt::AlignRight);
    mainLayout->addWidget(penStyleComboBox, 3, 3);
    mainLayout->addWidget(antialiasingCheckBox, 3, 1, 1, 1, Qt::AlignLeft);
    setLayout(mainLayout);

    //ODPALANE SA SLOTY W CELU INICJALIZACJI
    shapeChanged();
    penChanged();
    antialiasingCheckBox->setChecked(true);

    setWindowTitle(tr("MMGK P1"));

}

void Window::shapeChanged() {
    RenderArea::Shape shape = RenderArea::Shape(shapeComboBox->itemData(
                shapeComboBox->currentIndex()).toInt());
    renderArea->setShape(shape);
}

void Window::penChanged()
{
    int width = penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
            penStyleComboBox->currentIndex()).toInt());

    renderArea->setPen(QPen(Qt::blue, width, style, Qt::FlatCap, Qt::MiterJoin));
}

