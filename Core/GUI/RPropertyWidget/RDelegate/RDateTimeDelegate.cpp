/*********************************************************************
*                                                                    *
* This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
* as a result of work performed in the research group of Professor   *
* Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
* Columbia in Vancouver. The code is part of the computer program    *
* Rts, which is an extension of the computer program Rt developed by *
* Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
*                                                                    *
* The content of this file is the product of thesis-related work     *
* performed at the University of British Columbia (UBC). Therefore,  *
* the Rights and Ownership are held by UBC.                          *
*                                                                    *
* Please be cautious when using this code. It is provided “as is”    *
* without warranty of any kind, express or implied.                  *
*                                                                    *
* Contributors to this file:                                         *
*   - Mojtaba Mahsuli                                                *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RDateTimeDelegate.h"
#include "RTime.h"

#include <QtGui>

RDateTimeDelegate::RDateTimeDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	format = RTime::getTimeFormat();
	setObjectName("RDelegate2");
}


QWidget *RDateTimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QDateTimeEdit *editor = new QDateTimeEdit(parent);
	editor->setObjectName("RDelegate1");
	editor->setDisplayFormat(format);
	editor->setCalendarPopup(true);
	editor->installEventFilter(const_cast<RDateTimeDelegate *>(this));
	return editor;
}


void RDateTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();

	QDateTimeEdit *dateTimeEditor = static_cast<QDateTimeEdit *>(editor);
	dateTimeEditor->setDateTime(QDateTime().fromString(value, format));
}


void RDateTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QDateTimeEdit *dateTimeEditor = static_cast<QDateTimeEdit *>(editor);

	QDateTime time = dateTimeEditor->dateTime();
	QString value = time.toString(format);

	model->setData(index, value, Qt::EditRole);
}


void RDateTimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}


void RDateTimeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//paint(painter, option, index);
	QItemDelegate::paint(painter, option, index);
}
