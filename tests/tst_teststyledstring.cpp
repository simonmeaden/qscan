#include <QtTest>
#include <QStringLiteral>
#include <QtDebug>

// add necessary includes here
#include "style.h"
#include "testlogging.h"

class TestStyledString : public QObject
{
  Q_OBJECT

public:
  TestStyledString();
  ~TestStyledString();

private:
  //  StyledString styled_string;

private slots:
  void initTestCase();
  void cleanupTestCase();

  void testText();
  void testLeft();
  void testRight();
  void testMid();
  void testEqualsStyledString();
  void testEqualsQString();
  void testPlusEqualsStyledString();
  void testSplitParagraphsCase1();
  void testSplitParagraphsCase2();
  void testSplitParagraphsCase3();
  void testSplitParagraphsCase4();

};

TestStyledString::TestStyledString()
{

}

TestStyledString::~TestStyledString()
{
}

void TestStyledString::initTestCase()
{
  //  // create a basic styled string with a test message
  //  styled_string = QStringLiteral("This is just a simple test string.");
  //  // add acouply of styles.
  //  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  //  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));

  //  qCInfo(ssTest) << "StyledString test initialised.";
}

void TestStyledString::cleanupTestCase()
{

}

void TestStyledString::testText()
{
  QString str = QStringLiteral("This is just a simple test string.");;
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  QVERIFY(styled_string.text() == str);
}

void TestStyledString::testLeft()
{
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));

  StyledString test1 = styled_string.left(8);
  QVERIFY(test1.length() == 8);
  QVERIFY(test1.text() == "This is ");
  QVERIFY(test1.styles().length() == 1);

  test1 = styled_string.left(4);
  QVERIFY(test1.length() == 4);
  QVERIFY(test1.text() == "This");
  QVERIFY(test1.styles().isEmpty());
}

void TestStyledString::testRight()
{
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Subscript, 26, 1))); // C & E
  styled_string.appendStyle(Style(new StyleData(StyleData::Superscript, 13, 14))); // C & E

  StyledString test1 = styled_string.right(12);
  QVERIFY(test1.length() == 12);
  QVERIFY(test1.text() == "test string.");
  QVERIFY(test1.styles().length() == 3);

  test1 = styled_string.right(4);
  QVERIFY(test1.length() == 4);
  QVERIFY(test1.text() == "ing.");
  QVERIFY(test1.styles().isEmpty());

  test1 = styled_string.right(8);
  QVERIFY(test1.length() == 8);
  QVERIFY(test1.text() == " string.");
  QVERIFY(test1.styles().length() == 2);

}

void TestStyledString::testMid()
{

}

void TestStyledString::testEqualsStyledString()
{

}

void TestStyledString::testEqualsQString()
{
  //  QString str = QStringLiteral("This is just a simple test string.");
  //  StyledString styled_string = str;
  //  QVERIFY(styled_string.text() == str);
  //  QVERIFY(styled_string.styles().isEmpty());
}

void TestStyledString::testPlusEqualsStyledString()
{

}

void TestStyledString::testSplitParagraphsCase1()
{
  // create a basic styled string with a test message
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  // add acouply of styles.
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));
  QList<StyledString> list = styled_string.splitParagraphs();

  QVERIFY(list.size() == 1);
  QList<Style> styles = list.at(0).styles();
  QVERIFY(styles.size() == 3);
  QVERIFY(styles.at(0)->start() == 5);
  QVERIFY(styles.at(0)->length() == 2);
  QVERIFY(styles.at(0)->type() == StyleData::Bold);
  QVERIFY(styles.at(1)->start() == 15);
  QVERIFY(styles.at(1)->length() == 11);
  QVERIFY(styles.at(1)->type() == StyleData::Italic);
  QVERIFY(styles.at(2)->start() == 0);
  QVERIFY(styles.at(2)->length() == 34);
  QVERIFY(styles.at(2)->type() == StyleData::Paragraph);
}

void TestStyledString::testSplitParagraphsCase2()
{
  // create a basic styled string with a test message
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  // add acouply of styles.
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Paragraph, 0, 34)));
  QList<StyledString> list = styled_string.splitParagraphs();

  QVERIFY(list.size() == 1);
  QList<Style> styles = list.at(0).styles();
  QVERIFY(styles.size() == 3);
  QVERIFY(styles.at(0)->start() == 5);
  QVERIFY(styles.at(0)->length() == 2);
  QVERIFY(styles.at(0)->type() == StyleData::Bold);
  QVERIFY(styles.at(1)->start() == 15);
  QVERIFY(styles.at(1)->length() == 11);
  QVERIFY(styles.at(1)->type() == StyleData::Italic);
  QVERIFY(styles.at(2)->start() == 0);
  QVERIFY(styles.at(2)->length() == 34);
  QVERIFY(styles.at(2)->type() == StyleData::Paragraph);
}

void TestStyledString::testSplitParagraphsCase3()
{
  // create a basic styled string with a test message
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  // add acouply of styles.
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11)));
  styled_string.appendStyle(Style(new StyleData(StyleData::Paragraph, 15, 6)));
  QList<StyledString> list = styled_string.splitParagraphs();

  QVERIFY(list.size() == 3);
  QList<Style> styles = list.at(0).styles();
  QVERIFY(styles.size() == 2);
  QVERIFY(styles.at(0)->start() == 5);
  QVERIFY(styles.at(0)->length() == 2);
  QVERIFY(styles.at(0)->type() == StyleData::Bold);
  QVERIFY(styles.at(1)->start() == 0);
  QVERIFY(styles.at(1)->length() == 15);
  QVERIFY(styles.at(1)->type() == StyleData::Paragraph);

  styles = list.at(1).styles();
  QVERIFY(styles.size() == 2);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 6);
  QVERIFY(styles.at(0)->type() == StyleData::Italic);
  QVERIFY(styles.at(1)->start() == 0);
  QVERIFY(styles.at(1)->length() == 6);
  QVERIFY(styles.at(1)->type() == StyleData::Paragraph);

  styles = list.at(2).styles();
  QVERIFY(styles.size() == 2);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 5);
  QVERIFY(styles.at(0)->type() == StyleData::Italic);
  QVERIFY(styles.at(1)->start() == 0);
  QVERIFY(styles.at(1)->length() == 13);
  QVERIFY(styles.at(1)->type() == StyleData::Paragraph);
}

void TestStyledString::testSplitParagraphsCase4()
{
  // create a basic styled string with a test message
  StyledString styled_string = QStringLiteral("This is just a simple test string.");
  // add acouply of styles.
  styled_string.appendStyle(Style(new StyleData(StyleData::Bold, 5, 2))); // A & C
  styled_string.appendStyle(Style(new StyleData(StyleData::Italic, 15, 11))); // B & D
  styled_string.appendStyle(Style(new StyleData(StyleData::Subscript, 26, 1))); // C & E
  styled_string.appendStyle(Style(new StyleData(StyleData::Superscript, 13, 14))); // C & E

  styled_string.appendStyle(Style(new StyleData(StyleData::Paragraph, 15, 6)));
  QList<StyledString> list = styled_string.splitParagraphs();

  QVERIFY(list.size() == 3);
  QList<Style> styles = list.at(0).styles();
  QVERIFY(styles.size() == 3);
  QVERIFY(styles.at(0)->start() == 5);
  QVERIFY(styles.at(0)->length() == 2);
  QVERIFY(styles.at(0)->type() == StyleData::Bold);
  QVERIFY(styles.at(1)->start() == 13);
  QVERIFY(styles.at(1)->length() == 2);
  QVERIFY(styles.at(1)->type() == StyleData::Superscript);
  QVERIFY(styles.at(2)->start() == 0);
  QVERIFY(styles.at(2)->length() == 15);
  QVERIFY(styles.at(2)->type() == StyleData::Paragraph);

  styles = list.at(1).styles();
  QVERIFY(styles.size() == 3);
  QVERIFY(styles.at(1)->start() == 0);
  QVERIFY(styles.at(1)->length() == 6);
  QVERIFY(styles.at(1)->type() == StyleData::Italic);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 11);
  QVERIFY(styles.at(0)->type() == StyleData::Superscript);
  QVERIFY(styles.at(1)->start() == 0);
  QVERIFY(styles.at(1)->length() == 6);
  QVERIFY(styles.at(1)->type() == StyleData::Paragraph);

  styles = list.at(2).styles();
  QVERIFY(styles.size() == 4);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 5);
  QVERIFY(styles.at(0)->type() == StyleData::Italic);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 6);
  QVERIFY(styles.at(0)->type() == StyleData::Superscript);
  QVERIFY(styles.at(0)->start() == 0);
  QVERIFY(styles.at(0)->length() == 6);
  QVERIFY(styles.at(0)->type() == StyleData::Subscript);
  QVERIFY(styles.at(1)->start() == 6);
  QVERIFY(styles.at(1)->length() == 1);
  QVERIFY(styles.at(1)->type() == StyleData::Paragraph);
}

QTEST_APPLESS_MAIN(TestStyledString)

#include "tst_teststyledstring.moc"
