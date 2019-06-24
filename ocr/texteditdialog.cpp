#include "texteditdialog.h"
#include "ui_texteditdialog.h"

TextEditDialog::TextEditDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TextEditDialog)
{
  ui->setupUi(this);
}

TextEditDialog::~TextEditDialog()
{
  delete ui;
}

void TextEditDialog::setText(const QString &text)
{
  ui->editor->setPlainText(text);
}

QString TextEditDialog::text()
{
  return ui->editor->toPlainText();
}

void TextEditDialog::accept() {}

void TextEditDialog::reject() {}
