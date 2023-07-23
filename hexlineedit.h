#ifndef HEXLINEEDIT_H
#define HEXLINEEDIT_H


#include <QKeyEvent>
#include <iostream>
#include <qlineedit.h>
#include <qnamespace.h>

class HexLineEdit : public QLineEdit {
public:
    explicit HexLineEdit(QWidget *parent = nullptr)
        : QLineEdit(parent)
    { }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        std::cout << event->key() << std::endl;
        if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
         || (event->key() >= Qt::Key_A && event->key() <= Qt::Key_F)
         || (event->key() == Qt::Key_Backspace)
         || (event->key() == Qt::Key_Enter)
         || (event->key() == Qt::Key_X)
        ) {
            QLineEdit::keyPressEvent(event);
        }
        else {
            event->ignore();
        }
    }
};

#endif
