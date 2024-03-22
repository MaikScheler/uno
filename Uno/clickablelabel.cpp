#include "clickablelabel.h"

/*
* Weißt jeder Karte eine Card Id zu
*/
ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f, int cardId)
    : QLabel(parent) {
    this->cardId = cardId;
}

ClickableLabel::~ClickableLabel() {}

/*
* Schmeißt das click Event weiter
*/
void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked(this->cardId, this);
}

/*
* Für die Animation beim Hoveren der Karte
*/
void ClickableLabel::enterEvent(QEvent *ev)
{
    this->move(x(), 0);
}

/*
* Für die Animation beim Hoveren der Karte
*/
void ClickableLabel::leaveEvent(QEvent *ev)
{
    this->move(x(), 50);
}
