#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSettings>
#include <QPushButton>
#include <QSpinBox>
#include <QFontComboBox>
#include <QDialogButtonBox>

class StickyNoteSettings: public QDialog
{
    Q_OBJECT

public:
    explicit StickyNoteSettings(QWidget *parent = nullptr);
    ~StickyNoteSettings();

signals:
    void settingChanged(QColor color, QString fontFamily, int fontSize);

private slots:
    void onPushButtonColorClicked();
    void onDialogButtonBoxAccepted();
    void onDialogButtonBoxRejected();

private:
    void saveFontColor(QColor color);
    QString loadFontColor();

    void saveFontFamily(QString fontFamily);
    QString loadFontFamily();

    void saveFontSize(int fontSize);
    int loadFontSize();

private:
    void initializeButtons();
    void initializeSettings();

private:
    QVBoxLayout *m_mainLayout;
    QFontComboBox *m_fontComboBox;
    QSpinBox *m_spinBoxSize;
    QPushButton *m_pushButtonColor;

    QSettings m_setting;
    QColor m_color;
};
