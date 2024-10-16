#include "stickynote.h"
#include "ui_stickynote.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QTextCursor>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

StickyNote::StickyNote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StickyNote),
    m_isTextChanged(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setWindowIcon(QIcon("StickyNote/Resources/images/sticky-note.ico"));

    ui->widget_base->setStyleSheet("background-color:rgb(8, 78, 113); border-radius:5px;");
    ui->widget_base->setStyleSheet("QWidget{background-color:rgb(9, 71, 105);}");
    ui->textEdit->setStyleSheet("background-color:rgb(5, 96, 136);border-radius:3px; border-color:rgb(85, 195, 183);border-width:1px;border-style:inset;");


    m_settingPage = new StickyNoteSettings(this);
    m_settingPage->hide();

    m_windowTitle = "Sticky Note";
    ui->label_title->setText(m_windowTitle);
    ui->label_title->setStyleSheet("color:rgb(255, 0, 0);font-size:18px;");
    ui->tb_load->setHidden(true);
    QPixmap pixmap = QPixmap(":/StickyNote/Resources/images/sticky-pin.png");
    ui->label_pin->setPixmap(pixmap.scaled(25,40));

    QString tbStyles = "QToolButton::hover{background-color: rgb(5, 96, 136);}"
                       "QToolButton::checked{background-color: rgb(11, 55, 85);}";
    ui->tb_save->setStyleSheet(tbStyles);
    ui->tb_Setting->setStyleSheet(tbStyles);
    ui->tb_load->setStyleSheet(tbStyles);
    ui->pb_close->setStyleSheet(tbStyles);
    ui->pb_minimize->setStyleSheet(tbStyles);
    ui->tb_align_left->setStyleSheet(tbStyles);
    ui->tb_align_right->setStyleSheet(tbStyles);
    ui->tb_align_center->setStyleSheet(tbStyles);
    ui->tb_bold->setStyleSheet(tbStyles);
    ui->tb_StrikeOut->setStyleSheet(tbStyles);
    ui->tb_underline->setStyleSheet(tbStyles);

    move(loadWindowPosition());

    ui->textEdit->setText(loadNote()); /* from previous time */

    connect(ui->textEdit, &QTextEdit::textChanged, this, &StickyNote::onTextEditTextChanged);
    connect(m_settingPage, &StickyNoteSettings::settingChanged, this, &StickyNote::onSettingChanged);
    connect(&m_saveTimer, &QTimer::timeout, this, &StickyNote::onSaveTimerTimeout);

    m_saveTimer.start(10e3);
}

StickyNote::~StickyNote()
{
    delete ui;
}

void  StickyNote::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void  StickyNote::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void StickyNote::closeEvent(QCloseEvent *event)
{
    saveWindowPosition(pos());
    QWidget::closeEvent(event);
}

void  StickyNote::on_pb_close_clicked()
{
    close();
}

void  StickyNote::on_pb_minimize_clicked()
{
    showMinimized();
}

void  StickyNote::on_tb_Setting_clicked()
{
    m_settingPage->show();
}

void  StickyNote::on_tb_bold_clicked(bool checked)
{
    checked ? ui->textEdit->setFontWeight(QFont::Bold)
    : ui->textEdit->setFontWeight(QFont::Normal);
}

void  StickyNote::on_tb_underline_clicked(bool underline)
{
    QTextCursor  cursor = ui->textEdit->textCursor();
    QFont font = ui->textEdit->currentFont();
    if (!cursor.selectedText().isEmpty())
    {
        font.underline() ? ui->textEdit->setFontUnderline(false) : ui->textEdit->setFontUnderline(true);
        //cursor.clearSelection();
        //ui->textEdit->setTextCursor(cursor);
    }
}

void  StickyNote::on_tb_StrikeOut_clicked()
{
    QTextCursor  cursor = ui->textEdit->textCursor();
    QFont font = ui->textEdit->currentFont();
    if (!cursor.selectedText().isEmpty())
    {
        font.strikeOut() ? font.setStrikeOut(false) : font.setStrikeOut(true);
        ui->textEdit->setCurrentFont(font);
        //cursor.clearSelection();
        //ui->textEdit->setTextCursor(cursor);
    }
}

void  StickyNote::on_tb_align_left_clicked(bool checked)
{
    ui->tb_align_right->setChecked(false);
    ui->tb_align_center->setChecked(false);

    if (checked)
    {
        ui->textEdit->setAlignment(Qt::AlignLeft);
    }
}

void  StickyNote::on_tb_align_center_clicked(bool checked)
{
    ui->tb_align_left->setChecked(false);
    ui->tb_align_right->setChecked(false);

    if (checked)
    {
        ui->textEdit->setAlignment(Qt::AlignCenter);
    }
}

void  StickyNote::on_tb_align_right_clicked(bool checked)
{
    ui->tb_align_center->setChecked(false);
    ui->tb_align_left->setChecked(false);

    if (checked)
    {
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void  StickyNote::on_tb_save_clicked()
{
    saveNote(ui->textEdit->toHtml());
    ui->label_message->setText("Saving...");
    QTimer::singleShot(1000, this, [this](){
        ui->label_message->setText("Saved");
        QTimer::singleShot(3000, this, [this](){
            ui->label_message->setText("");
        });
    });
}

void  StickyNote::on_tb_load_clicked()
{
//    QString  fileName = QFileDialog::getOpenFileName(this, "Open the file");
//    QFile    file(fileName);
//    currentFile = fileName;
//    if (!file.open(QIODevice::ReadOnly | QFile::Text))
//    {
//        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
//        return;
//    }
//    setWindowTitle(fileName);
//    QTextStream  in(&file);
//    QString      text = in.readAll();
//    ui->textEdit->setText(text);
//    file.close();
}

void  StickyNote::on_textEdit_cursorPositionChanged()
{
    //    QTextCursor  cursor = ui->textEdit->textCursor();
    //    if (!cursor.selection().isEmpty())
    //    {
    //        return;
    //    }
    //    _font.setStrikeOut(false);
    //    ui->textEdit->setCurrentFont(_font);
}

void  StickyNote::on_textEdit_selectionChanged()
{
    QTextCursor  cursor = ui->textEdit->textCursor();
    auto         text   = cursor.selectedText();
}

void StickyNote::onTextEditTextChanged()
{
    m_isTextChanged = true;
}

void StickyNote::onSettingChanged(QColor color, QString fontFamily, int fontSize)
{
    ui->textEdit->setTextColor(color);
    ui->textEdit->setFontPointSize(fontSize);
    ui->textEdit->setFontFamily(fontFamily);
}

void StickyNote::onSaveTimerTimeout()
{
    if(m_isTextChanged)
    {
        saveNote(ui->textEdit->toHtml());
        m_isTextChanged = false;
        ui->label_message->setText("Saving...");
        QTimer::singleShot(1000, this, [this](){
            ui->label_message->setText("Saved");
            QTimer::singleShot(3000, this, [this](){
                ui->label_message->setText("");
            });
        });
    }
}

void StickyNote::saveNote(QString text)
{
    m_settings.setValue("settings/StickyNote/save", text);
}

QString StickyNote::loadNote()
{
    return m_settings.value("settings/StickyNote/save", "").toString();
}

void StickyNote::saveWindowPosition(const QVariant &value)
{
    m_settings.setValue("settings/StickyNote/windowPosition", value);
}

QPoint StickyNote::loadWindowPosition()
{
    return m_settings.value("settings/StickyNote/windowPosition", "").toPoint();
}

