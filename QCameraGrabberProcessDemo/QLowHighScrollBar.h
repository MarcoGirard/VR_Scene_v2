#ifndef QLOWHIGHSCROLLBAR_H
#define QLOWHIGHSCROLLBAR_H

#include <QWidget>
class QLabel;
class QScrollBar;

class QLowHighScrollBar : public QWidget
{
	Q_OBJECT

public:
	QLowHighScrollBar(const QString &prefix = QString(""), QWidget *parent = 0);
	~QLowHighScrollBar();

	void setPrefix(const QString &prefix)					{ prefix_ = prefix; updateText(); }
	QString prefix()										{ return prefix_; }

	void setRange(int min, int max);
	void setLow(int low);
	void setHigh(int high);
	void set(int low, int high);
	void reset();

	int lowValue();
	int highValue();

private:
	QLabel *title_;
	QScrollBar *lowScrollBar_, *highScrollBar_;

	QString prefix_;
	void updateText();
	void action();

signals:
	void actionTriggered(int lowValue, int highValue);
	void valueUpdated();

private slots:
	void actionLow();
	void actionHigh();
};

#endif // QLOWHIGHSCROLLBAR_H
