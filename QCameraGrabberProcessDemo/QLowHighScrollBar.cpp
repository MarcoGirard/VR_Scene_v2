#include "QLowHighScrollBar.h"


#include <QLabel>
#include <QScrollBar>
#include <QVBoxLayout>

QLowHighScrollBar::QLowHighScrollBar(const QString &prefix, QWidget *parent)
	: QWidget(parent),
	prefix_(prefix)
{
	title_ = new QLabel;
	lowScrollBar_ = new QScrollBar;
	highScrollBar_ = new QScrollBar;

	lowScrollBar_->setOrientation(Qt::Horizontal);
	highScrollBar_->setOrientation(Qt::Horizontal);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(title_);
	layout->addWidget(lowScrollBar_);
	layout->addWidget(highScrollBar_);

	setLayout(layout);

	connect(lowScrollBar_, &QScrollBar::valueChanged, this, &QLowHighScrollBar::actionLow);
	connect(highScrollBar_, &QScrollBar::valueChanged, this, &QLowHighScrollBar::actionHigh);
}
QLowHighScrollBar::~QLowHighScrollBar()
{
}

void QLowHighScrollBar::setRange(int min, int max)
{
	if (max <= min) {
		max = min + 1;
	}

	lowScrollBar_->setMinimum(min);
	lowScrollBar_->setMaximum(max - 1);
	highScrollBar_->setMinimum(min + 1);
	highScrollBar_->setMaximum(max);

	lowScrollBar_->setValue(min);
	highScrollBar_->setValue(max);
}

void QLowHighScrollBar::setLow(int low)
{
	lowScrollBar_->setValue(low);
}

void QLowHighScrollBar::setHigh(int high)
{
	highScrollBar_->setValue(high);
}

void QLowHighScrollBar::set(int low, int high)
{
	setLow(low);
	setHigh(high);
}

int QLowHighScrollBar::lowValue()
{
	return lowScrollBar_->value();
}

int QLowHighScrollBar::highValue()
{
	return highScrollBar_->value();
}

void QLowHighScrollBar::actionLow()
{
	if (highScrollBar_->value() <= lowScrollBar_->value()) {
		highScrollBar_->setValue(lowScrollBar_->value() + 1);
	}

	action();
}
void QLowHighScrollBar::actionHigh()
{
	if (lowScrollBar_->value() >= highScrollBar_->value()) {
		lowScrollBar_->setValue(highScrollBar_->value() - 1);
	}

	action();
}
void QLowHighScrollBar::updateText()
{
	title_->setText(QString("%1 : [%2, %3]").arg(prefix_).arg(lowScrollBar_->value()).arg(highScrollBar_->value()));
}
void QLowHighScrollBar::action()
{
	updateText();
	emit valueUpdated();
}
void QLowHighScrollBar::reset()
{
	lowScrollBar_->setValue(lowScrollBar_->minimum());
	highScrollBar_->setValue(highScrollBar_->maximum());
}



