#pragma once

#include <QWidget>
#include <QTextCharFormat>
#include <QVariant>
#include <QSettings>
#include <QTimer>
#include <QObject>
#include "stickynotesettings.h"

namespace Ui {
class StickyNote;
}

class StickyNote : public QWidget
{
    Q_OBJECT

public:
    explicit StickyNote(QWidget *parent = nullptr);
    ~StickyNote();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pb_close_clicked();
    void on_pb_minimize_clicked();

    void on_tb_Setting_clicked();
    void on_tb_bold_clicked(bool checked);
    void on_tb_underline_clicked(bool underline);
    void on_tb_StrikeOut_clicked();

    void on_tb_align_left_clicked(bool checked);
    void on_tb_align_center_clicked(bool checked);
    void on_tb_align_right_clicked(bool checked);

    void on_tb_save_clicked();
    void on_tb_load_clicked();

    void on_textEdit_cursorPositionChanged();
    void on_textEdit_selectionChanged();
    void onTextEditTextChanged();

    void onSettingChanged(QColor color, QString fontFamily, int fontSize);
    void onSaveTimerTimeout();

private:
    void saveNote(QString text);
    QString loadNote();

    void saveWindowPosition(const QVariant &value);
    QPoint loadWindowPosition();

private:
    Ui::StickyNote *ui;
    QPoint              m_dragPosition;
    StickyNoteSettings *m_settingPage;
    QString             m_currentFile;
    QString             m_windowTitle;
    QSettings           m_settings;
    QTimer              m_saveTimer;
    bool                m_isTextChanged;
};
