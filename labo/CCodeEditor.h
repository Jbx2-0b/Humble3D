#ifndef CCODEEDITOR_H
#define CCODEEDITOR_H

#include <QObject>

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class CLineNumberArea;


class CCodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CCodeEditor(QWidget *parent = 0);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:

	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);

private:

	QWidget* m_pLineNumberArea;
};


class CLineNumberArea : public QWidget
{
public:
	CLineNumberArea(CCodeEditor *editor) : QWidget(editor)
	{
		m_pCodeEditor = editor;
	}

	QSize sizeHint() const
	{
		return QSize(m_pCodeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) {
		m_pCodeEditor->lineNumberAreaPaintEvent(event);
	}

private:
	CCodeEditor* m_pCodeEditor;
};

#endif // CCODEEDITOR_H
