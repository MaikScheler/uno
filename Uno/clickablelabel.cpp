#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f, int cardId)
    : QLabel(parent) {
    this->cardId = cardId;
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked(this->cardId, this);
}

void ClickableLabel::enterEvent(QEvent *ev)
{
    //QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    //animation->setDuration(200); // Adjust duration as needed

    // Start from the current geometry and animate to a new geometry with y position adjusted
    //animation->setStartValue(geometry());
    //animation->setEndValue(QRect(geometry().x(), geometry().y() - 50, geometry().width(), geometry().height()));

    // Set easing curve (optional)
    //animation->setEasingCurve(QEasingCurve::OutCubic);

    // Start the animation
    //animation->start();

    this->move(x(), 0);
}

void ClickableLabel::leaveEvent(QEvent *ev)
{
    this->move(x(), 50);
}
