#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <iostream>
#include <QPropertyAnimation>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags(), int cardId = 0);
    ~ClickableLabel();

signals:
    void clicked(int cardId, ClickableLabel *cardLabel);

protected:
    void mousePressEvent(QMouseEvent* event);
    void leaveEvent(QEvent *ev) override;
    void enterEvent(QEvent *ev) override;
private:
    int cardId;

};

#endif // CLICKABLELABEL_H
