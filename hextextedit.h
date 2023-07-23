#ifndef HEXTEXTEDIT_H
#define HEXTEXTEDIT_H


#include <QKeyEvent>
#include <qtextedit.h>
#include <qnamespace.h>

class HexTextEdit : public QTextEdit {
public:
    explicit HexTextEdit(QWidget *parent = nullptr)
        : QTextEdit(parent)
    { }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
         || (event->key() >= Qt::Key_A && event->key() <= Qt::Key_F)
         || (event->key() == Qt::Key_Backspace)
         || (event->key() == Qt::Key_Enter)
         || (event->key() == Qt::Key_X)
        ) {
            QTextEdit::keyPressEvent(event);
        }
        else {
            event->ignore();
        }
    }
};

#endif
