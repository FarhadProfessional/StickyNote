#include "stickynotesettings.h"
#include <QColorDialog>
#include <QDebug>
#include <QLabel>
#include <QSizePolicy>

StickyNoteSettings::StickyNoteSettings(QWidget *parent):
    QDialog(parent)
{
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAutoFillBackground(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(250, 125);
    setWindowTitle("Settings");

    initializeButtons();
    initializeSettings();
}

StickyNoteSettings::~StickyNoteSettings()
{
}

void StickyNoteSettings::onPushButtonColorClicked()
{
    m_color = QColorDialog::getColor(m_color, this);

    m_pushButtonColor->setStyleSheet(QString("* {background-color:rgba(%1,%2,%3,%4);}").
                                      arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_color.alpha()));
}

void StickyNoteSettings::onDialogButtonBoxAccepted()
{
    QString fontFamily = m_fontComboBox->currentFont().family();
    int fontSize = m_spinBoxSize->value();

    saveFontColor(m_color);
    saveFontFamily(fontFamily);
    saveFontSize(fontSize);

    emit settingChanged(m_color, fontFamily, fontSize);
    close();
}


void StickyNoteSettings::onDialogButtonBoxRejected()
{
    close();
}

void StickyNoteSettings::saveFontColor(QColor color)
{
    m_setting.setValue("settings/StickyNote/fontColor", color);
}

QString StickyNoteSettings::loadFontColor()
{
    return m_setting.value("settings/StickyNote/fontColor", QColor(255,255,255,255)).toString();
}

void StickyNoteSettings::saveFontFamily(QString fontFamily)
{
    m_setting.setValue("settings/StickyNote/fontFamily", fontFamily);
}

QString StickyNoteSettings::loadFontFamily()
{
    return m_setting.value("settings/StickyNote/fontFamily", "Arial").toString();
}

void StickyNoteSettings::saveFontSize(int fontSize)
{
    m_setting.setValue("settings/StickyNote/fontSize", fontSize);
}

int StickyNoteSettings::loadFontSize()
{
    return m_setting.value("settings/StickyNote/fontSize", 12).toInt();
}

void StickyNoteSettings::initializeButtons()
{
    m_mainLayout = new QVBoxLayout;

    QHBoxLayout *hBoxLayoutFontFamily = new QHBoxLayout;
    QLabel *labelFontFamilyTitle = new QLabel("Font: ");
    hBoxLayoutFontFamily->addWidget(labelFontFamilyTitle);

    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setWritingSystem(QFontDatabase::Latin);
    //m_fontComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hBoxLayoutFontFamily->addWidget(m_fontComboBox);
    m_mainLayout->addLayout(hBoxLayoutFontFamily);


    QHBoxLayout *hBoxLayoutFontSize = new QHBoxLayout;
    QLabel *labelFontSizeTitle = new QLabel("Size: ");
    hBoxLayoutFontSize->addWidget(labelFontSizeTitle);

    m_spinBoxSize = new QSpinBox;
    m_spinBoxSize->setMinimum(2);
    m_spinBoxSize->setMaximum(100);
    m_spinBoxSize->setMinimumWidth(188);
    hBoxLayoutFontSize->addWidget(m_spinBoxSize);
    m_mainLayout->addLayout(hBoxLayoutFontSize);

    QHBoxLayout *hBoxLayoutFontColor = new QHBoxLayout;
    QLabel *labelFontColorTitle = new QLabel("Color: ");
    hBoxLayoutFontColor->addWidget(labelFontColorTitle);

    m_pushButtonColor = new QPushButton("");
    m_pushButtonColor->setMinimumWidth(188);
    connect(m_pushButtonColor, &QPushButton::clicked, this, &StickyNoteSettings::onPushButtonColorClicked);
    hBoxLayoutFontColor->addWidget(m_pushButtonColor);
    m_mainLayout->addLayout(hBoxLayoutFontColor);

    QHBoxLayout *hBoxLayoutButtons = new QHBoxLayout;
    QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dialogButtonBox->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &StickyNoteSettings::onDialogButtonBoxAccepted);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &StickyNoteSettings::onDialogButtonBoxRejected);
    hBoxLayoutButtons->addWidget(dialogButtonBox);
    m_mainLayout->addLayout(hBoxLayoutButtons);

    setLayout(m_mainLayout);
}

void StickyNoteSettings::initializeSettings()
{
    m_color = loadFontColor();
    QString fontFamily = loadFontFamily();
    int fontSize = loadFontSize();

    m_pushButtonColor->setStyleSheet(QString("* {background-color:rgba(%1,%2,%3,%4);}").
                                      arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_color.alpha()));
    m_fontComboBox->setCurrentFont(QFont(fontFamily));
    m_spinBoxSize->setValue(fontSize);

    emit settingChanged(m_color, fontFamily, fontSize);
}

