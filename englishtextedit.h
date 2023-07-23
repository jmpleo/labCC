#ifndef ENTEXTEDIT_H
#define ENTEXTEDIT_H


#include <QTextEdit>
#include <QKeyEvent>
#include <qnamespace.h>

class EnglishTextEdit : public QTextEdit {
public:
    explicit EnglishTextEdit(QWidget *parent = nullptr)
        : QTextEdit(parent)
        , _addSymbols("!@#$%^&*()_+{}[]\"':;/\\?.,`~")
    { }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if ((event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z)
         || (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
         || (event->key() == Qt::Key_Backspace)
         || (event->key() == Qt::Key_Enter)
         || (event->key() == Qt::Key_Space)
         || (_addSymbols.contains(event->key()))
        ) {
            QTextEdit::keyPressEvent(event);
        }
        else {
            event->ignore();
        }
    }

private:
    QString _addSymbols;
};

#endif
