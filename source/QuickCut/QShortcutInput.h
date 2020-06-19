
#pragma once

#include <QLineEdit>

#if defined(Q_OS_WIN)
#    include <Windows.h>
#endif

class QShortcutInput : public QLineEdit
{
    Q_OBJECT

public:
    QShortcutInput(QWidget * parent);
    ~QShortcutInput();

#if defined(Q_OS_WIN)
    static LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam);
#endif

protected:
    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;

public:
    static QShortcutInput * s_Instance;

#if defined(Q_OS_WIN)
    static HHOOK s_Hook;
#endif
};