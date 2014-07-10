#include "capworddialog.h"
#include "ui_capworddialog.h"
#include "VBookModel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iDict.h"
#include "stdio.h"

CapWordDialog::CapWordDialog(MainWindow *owner) :
    QDialog(NULL),
    ui(new Ui::CapWordDialog),
    m_owner(owner)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    move(QCursor::pos().x()+8, QCursor::pos().y()+16);
    //qApp->installEventFilter(this);
}

CapWordDialog::~CapWordDialog()
{
    delete ui;
}

void CapWordDialog::setText(void *v)
{
    DictItemList* itemList = (DictItemList*) v;

    QTextCursor cursor(ui->textEdit->textCursor());
    QTextCharFormat titleFormat;
    QTextCharFormat bodyFormat;
    QTextBlockFormat itemBlock;
    QString text;

    titleFormat.setFontWeight(QFont::DemiBold);
    text = QString::fromUtf8((*itemList)[0].phonetic.c_str());
    text = text.trimmed();
    if (text != "") {
        cursor.insertBlock();
        cursor.insertText(text, titleFormat);
        cursor.insertBlock();
    }

    bodyFormat.setFontWeight(QFont::Light);
    cursor.insertBlock(itemBlock);
    text = QString::fromUtf8((*itemList)[0].expl.c_str());
    text = text.trimmed();
    cursor.insertText(text,bodyFormat);
    ui->textEdit->moveCursor(QTextCursor::Start);

    delete itemList;
}

void CapWordDialog::on_saveToolButton_clicked()
{
    QString word = m_owner->m_capword;
    if (word != "") {
        if (m_owner->m_vbookModel->add(word)) {
            MainWindow::showToolTip(tr("add to vocabulary book,success"));
        } else {
            MainWindow::showToolTip(tr("add to vocabulary book,failure"));
        }
    }
}

void CapWordDialog::on_dictToolButton_clicked()
{
    close();

    m_owner->ui->inputLineEdit->setText(m_owner->m_capword);
    m_owner->activateWindow();
    m_owner->showNormal();
}

bool CapWordDialog::event(QEvent * event)
{
    if (event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this)
            close();
    }
    return QWidget::event(event);
}

#if 0
bool CapWordDialog::eventFilter( QObject * watched, QEvent * event )
{
    //g_log.e("event type2 %d\n", event->type());
    return false;
}
#endif
