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
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags(), QString cardName = "");
    ~ClickableLabel();

signals:
    void clicked(QString cardName);

protected:
    void mousePressEvent(QMouseEvent* event);
    void leaveEvent(QEvent *ev) override;
    void enterEvent(QEvent *ev) override;
private:
    QString cardName;

};

#endif // CLICKABLELABEL_H
