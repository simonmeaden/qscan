#include "texteditdialog.h"


TextEditDialog::TextEditDialog(QWidget* parent)
  : QDialog(parent)
  , m_text(StyledString())
{
  initGui();
}

/*!
   \brief Sets the text as a StyledString with styling data.

   \param text - the text string with styling data.
*/
void TextEditDialog::setText(const StyledString& text)
{
  m_text = text;
  m_editor->setPlainText(m_text.text());
}

/*!
   \brief Sets the text as a basic QString without styling data.

  \param text - the text string.
*/
void TextEditDialog::setText(const QString& text)
{
  m_text.setText(text);
  m_editor->setText(text);
}

/*! \brief returns the text as a StyledString.

  \return a StyledString.
*/
StyledStringList TextEditDialog::text()
{
  //  QString modified = m_editor->toPlainText();
  //  m_text.setText(modified);
  //  return m_text;
  StyledStringList paras = m_text.splitParagraphs();
  return paras;
}

void TextEditDialog::initGui()
{
  QPixmap bold_icon;
  QPixmap italic_icon;
  QPixmap underline_icon;
  QPixmap normal_icon;
  QPixmap match_quotes_icon;
  QPixmap help_icon;
  QPixmap accept_icon;
  QPixmap reject_icon;
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/bold")), &bold_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/italic")), &italic_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/underline")), &underline_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/clear_edit")), &normal_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/doublequote")), &match_quotes_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/help-context")), &help_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/accept")), &accept_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/reject")), &reject_icon);

  auto* layout = new QGridLayout;
  setLayout(layout);

  m_editor = new QTextEdit(this);
  m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(m_editor, 0, 0);
  connect(m_editor, &QTextEdit::selectionChanged, this, &TextEditDialog::textSelectionChanged);

  QFrame* sub_btn_frame = new QFrame(this);
  sub_btn_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  auto* sub_box_layout = new QVBoxLayout;
  sub_btn_frame->setLayout(sub_box_layout);
  layout->addWidget(sub_btn_frame, 0, 1);

  m_bold_btn = new QPushButton(bold_icon, "", this);
  m_bold_btn->setEnabled(false);
  m_bold_btn->setIconSize(QSize(32, 32));
  m_bold_btn->setToolTip(tr("Bold"));
  connect(m_bold_btn, &QPushButton::clicked, this, &TextEditDialog::setBoldClicked);
  sub_box_layout->addWidget(m_bold_btn);

  m_italic_btn = new QPushButton(italic_icon, "", this);
  m_italic_btn->setEnabled(false);
  m_italic_btn->setIconSize(QSize(32, 32));
  m_italic_btn->setToolTip(tr("Italic"));
  connect(m_italic_btn, &QPushButton::clicked, this, &TextEditDialog::setItalicClicked);
  sub_box_layout->addWidget(m_italic_btn);

  m_underline_btn = new QPushButton(underline_icon, "", this);
  m_underline_btn->setEnabled(false);
  m_underline_btn->setIconSize(QSize(32, 32));
  m_underline_btn->setToolTip(tr("Underline"));
  connect(m_underline_btn, &QPushButton::clicked, this, &TextEditDialog::setUnderlineClicked);
  sub_box_layout->addWidget(m_underline_btn);

  m_normal_btn = new QPushButton(normal_icon, "", this);
  m_normal_btn->setEnabled(false);
  m_normal_btn->setIconSize(QSize(32, 32));
  m_normal_btn->setToolTip(tr("Clear text formatting"));
  connect(m_underline_btn, &QPushButton::clicked, this, &TextEditDialog::setUnderlineClicked);
  sub_box_layout->addWidget(m_normal_btn);

  QList<QString> list;
  list << tr("XX Small")
       << tr("X Small")
       << tr("Small")
       << tr("Medium")
       << tr("Normal")
       << tr("Large")
       << tr("V Large")
       << tr("X Large")
       << tr("XX Large");
  m_font_size_box = new QComboBox(this);
  m_font_size_box->addItems(list);
  m_font_size_box->setCurrentText(tr("Normal"));
  m_font_size_box->setEnabled(false);
  m_font_size_box->setToolTip(tr("Choose font size."));
  connect(m_font_size_box, qOverload<int>(&QComboBox::currentIndexChanged),
          this, &TextEditDialog::setFontSizeSelected);
  sub_box_layout->addWidget(m_font_size_box);

  m_make_para_btn = new QPushButton(tr("Make Paragraph"), this);
  m_make_para_btn->setIconSize(QSize(144, 48));
  m_make_para_btn->setToolTip(tr("Converts several lines of text into a paragraph."));
  connect(m_make_para_btn, &QPushButton::clicked, this, &TextEditDialog::makeParagraph);
  sub_box_layout->addWidget(m_make_para_btn);

  m_match_quotes_btn = new QPushButton(match_quotes_icon, "", this);
  m_match_quotes_btn->setIconSize(QSize(144, 48));
  m_match_quotes_btn->setToolTip(tr("Match Quotes. Shows matching quote in text."));
  connect(m_match_quotes_btn, &QPushButton::clicked, this, &TextEditDialog::matchQuotes);
  sub_box_layout->addWidget(m_match_quotes_btn);

  auto* main_btn_frame = new QFrame(this);
  main_btn_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  auto* main_box_layout = new QHBoxLayout;
  main_btn_frame->setLayout(main_box_layout);
  layout->addWidget(main_btn_frame, 1, 0, 1, 2);

  m_help_btn = new QPushButton(help_icon, "", this);
  m_help_btn->setIconSize(QSize(32, 32));
  m_help_btn->setToolTip(tr("Help"));
  main_box_layout->addWidget(m_help_btn);

  auto* spacer = new QFrame(this);
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  main_box_layout->addWidget(spacer);

  m_reject_btn = new QPushButton(reject_icon, "Reject Changes", this);
  m_reject_btn->setIconSize(QSize(32, 32));
  m_reject_btn->setToolTip(tr("Reject changes"));
  connect(m_reject_btn, &QPushButton::clicked, this, &TextEditDialog::reject);
  main_box_layout->addWidget(m_reject_btn);

  m_accept_btn = new QPushButton(accept_icon, "Accept Changes and Save", this);
  m_accept_btn->setIconSize(QSize(32, 32));
  m_accept_btn->setToolTip(tr("Accept all changes"));
  connect(m_accept_btn, &QPushButton::clicked, this, &TextEditDialog::acceptChanges);
  main_box_layout->addWidget(m_accept_btn);

}

void TextEditDialog::textSelectionChanged()
{
  QTextCursor cursor = m_editor->textCursor();
  QString text = cursor.selectedText();

  if (text.size() > 0) {
    m_bold_btn->setEnabled(true);
    m_italic_btn->setEnabled(true);
    m_underline_btn->setEnabled(true);
    m_normal_btn->setEnabled(true);
    m_font_size_box->setEnabled(true);
    m_make_para_btn->setEnabled(true);
    m_font_size_box->setCurrentText(tr("Normal"));

  } else {
    m_bold_btn->setEnabled(false);
    m_italic_btn->setEnabled(false);
    m_underline_btn->setEnabled(false);
    m_normal_btn->setEnabled(false);
    m_make_para_btn->setEnabled(false);
    m_font_size_box->setEnabled(false);
  }
}

void TextEditDialog::setTextStyle(Style data)
{
  QTextCursor* cursor = new QTextCursor(m_editor->document());
  cursor->setPosition(data->start(), QTextCursor::MoveAnchor);
  cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, data->length());

  QTextCharFormat format;

  switch (data->type()) {
  case StyleData::Bold:
    format.setFontWeight(QFont::Bold);
    break;

  case StyleData::Italic:
    format.setFontItalic(true);
    break;

  case StyleData::Underline:
    format.setFontUnderline(true);
    break;

  case StyleData::Normal:
    format.setFontWeight(QFont::Normal);
    format.setFontItalic(false);
    format.setFontUnderline(false);
    break;

  case StyleData::Font_X_X_Small:
  case StyleData::Font_X_Small:
  case StyleData::Font_Small:
  case StyleData::Font_Medium:
  case StyleData::Font_Normal:
  case StyleData::Font_Large:
  case StyleData::Font_V_Large:
  case StyleData::Font_X_Large:
  case StyleData::Font_X_X_Large:
    format.setFontStretch(data->fontStretch());
    break;

  case StyleData::Paragraph:
    format.setBackground(QBrush(Qt::lightGray));
    break;

  default:
    break;
  }

  cursor->mergeCharFormat(format);
}

void TextEditDialog::setBoldClicked()
{
  QTextCursor cursor = m_editor->textCursor();
  int anchor = cursor.anchor();
  int position = cursor.position();
  int length = qAbs(position - anchor);
  int start = qMin(anchor, position);
  StyleData::Type type = StyleData::Bold;
  Style style = StyleData::getStyle(type, start, length);

  if (length != 0) {
    setTextStyle(style);

    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Bold failed!"),
                           tr("The bold command failed"), QMessageBox::Ok);
    }
  }
}

void TextEditDialog::setItalicClicked()
{
  QTextCursor cursor = m_editor->textCursor();
  int anchor = cursor.anchor();
  int position = cursor.position();
  int length = qAbs(position - anchor);
  int start = qMin(anchor, position);
  StyleData::Type type = StyleData::Italic;
  Style style = StyleData::getStyle(type, start, length);

  if (length != 0) {
    setTextStyle(style);

    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Italic failed!"),
                           tr("The italic command failed"), QMessageBox::Ok);
    }
  }
}

void TextEditDialog::setNormalClicked()
{
  QTextCursor cursor = m_editor->textCursor();
  int anchor = cursor.anchor();
  int position = cursor.position();
  int length = qAbs(position - anchor);
  int start = qMin(anchor, position);
  StyleData::Type type = StyleData::Normal;
  Style style = StyleData::getStyle(type, start, length);

  if (length != 0) {
    setTextStyle(style);

    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Normal failed!"),
                           tr("The command failed"), QMessageBox::Ok);
    }
  }
}

void TextEditDialog::setFontSizeSelected(int index)
{
  QTextCursor cursor = m_editor->textCursor();
  int anchor = cursor.anchor();
  int position = cursor.position();
  int length = qAbs(position - anchor);
  int start = qMin(anchor, position);
  StyleData::Type type = StyleData::Type(index + int(StyleData::Type::Font_X_X_Small));
  Style style = StyleData::getStyle(type, start, length);

  if (length != 0) {
    // show the editor text as bold
    setTextStyle(style);

    // set the QStyle format
    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Font Size failed!"),
                           tr("The font size change failed"), QMessageBox::Ok);
    }
  }
}

void TextEditDialog::matchQuotes()
{
  // TODO
}

void TextEditDialog::makeParagraph()
{
  QTextCursor cursor = m_editor->textCursor();
  QString text = cursor.selectedText();

  if (!text.isEmpty()) {
    int start = cursor.anchor();
    int length = cursor.position() - start;
    Style style = StyleData::getStyle(StyleData::Paragraph, start, length);
    setTextStyle(style);

    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Paragraph failed!"),
                           tr("The paragraph command failed.\n"
                              "Probably caused by an overlapping paragraph."), QMessageBox::Ok);
    }
  }
}

void TextEditDialog::acceptChanges()
{
  StyledStringList text_list;
  QString text = m_editor->toPlainText();

  emit accept();
}

void TextEditDialog::setUnderlineClicked()
{
  QTextCursor cursor = m_editor->textCursor();
  int anchor = cursor.anchor();
  int position = cursor.position();
  int length = qAbs(position - anchor);
  int start = qMin(anchor, position);
  StyleData::Type type = StyleData::Underline;
  Style style = StyleData::getStyle(type, start, length);

  if (length != 0) {
    setTextStyle(style);

    if (!m_text.appendStyle(style)) {
      QMessageBox::warning(this, tr("Underline failed!"),
                           tr("The underline command failed"), QMessageBox::Ok);
    }
  }
}

