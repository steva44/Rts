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

#include "RCheckBoxTableDelegate.h"

#include <QtGui>
#include <QCheckBox>

RCheckBoxTableDelegate::RCheckBoxTableDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}


QWidget *RCheckBoxTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QCheckBox *editor = new QCheckBox(parent);
	editor->setObjectName("RDelegate1");
	editor->installEventFilter(const_cast<RCheckBoxTableDelegate *>(this));
	return editor;
}


void RCheckBoxTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	bool value = index.model()->data(index, Qt::EditRole).toBool();

	QCheckBox *checkBox = static_cast<QCheckBox *>(editor);
	checkBox->setChecked(value);
}


void RCheckBoxTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QCheckBox *checkBox = static_cast<QCheckBox *>(editor);
	//checkBox->interpretText();
	bool value = checkBox->isChecked();

	model->setData(index, value, Qt::EditRole);
}


void RCheckBoxTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}


void RCheckBoxTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//paint(painter, option, index);
	QItemDelegate::paint(painter, option, index);
}
