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
    QTextEdit::

protected:
    void textChanged() override
    {

    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
         || (event->key() >= Qt::Key_A && event->key() <= Qt::Key_F)
         || (event->key() == Qt::Key_Backspace)
         || (event->key() == Qt::Key_Delete)
         || (event->key() == Qt::Key_Up)
         || (event->key() == Qt::Key_Down)
         || (event->key() == Qt::Key_Left)
         || (event->key() == Qt::Key_Right)
         || (event->key() == Qt::Key_Enter)
         || (event->key() == Qt::Key_X)
         || (event->matches(QKeySequence::Paste))
        ) {
            QTextEdit::keyPressEvent(event);
        }
        else {
            event->ignore();
        }
    }
};

#endif
