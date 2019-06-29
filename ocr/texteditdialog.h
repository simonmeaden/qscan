#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QButtonGroup>
#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QGridLayout>
#include <QFrame>
#include <QPixmapCache>

#include "style.h"

class TextEditDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TextEditDialog(QWidget* parent = nullptr);
  //  ~TextEditDialog() override;

  void setText(const StyledString& text);
  void setText(const QString& text);
  StyledString text();

protected:
  StyledString m_text;
  QTextEdit* m_editor;
  QPushButton* m_bold_btn;
  QPushButton* m_italic_btn;
  QPushButton* m_underline_btn;
  QPushButton* m_normal_btn;
  QPushButton* m_match_quotes_btn;
  QComboBox* m_font_size_box;
  QPushButton* m_help_btn;
  QPushButton* m_accept_btn;
  QPushButton* m_reject_btn;

  void initGui();
  void textSelectionChanged();

  void setTextStyle(int start, int length, StyleData::Type type);

  void setBoldClicked();
  void setUnderlineClicked();
  void setItalicClicked();
  void setNormalClicked();

  void setFontSizeSelected(int index);

  void matchQuotes();

  void storeStyle(int start, int length, StyleData::Type style);
};

#endif  // TEXTEDITDIALOG_H
