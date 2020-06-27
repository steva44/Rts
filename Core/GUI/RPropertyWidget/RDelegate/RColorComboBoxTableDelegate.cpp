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

#include "RColorComboBoxTableDelegate.h"

#include <QtGui>

RColorComboBoxTableDelegate::RColorComboBoxTableDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}


RColorComboBoxTableDelegate::~RColorComboBoxTableDelegate()
{
	
}


QWidget * RColorComboBoxTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QtColorComboBox *editor = new QtColorComboBox(parent);
	editor->setObjectName("RDelegate1");
	editor->setStandardColors();
	editor->setColorDialogEnabled(false);

	editor->installEventFilter(const_cast<RColorComboBoxTableDelegate *>(this));
	return editor;
}


void RColorComboBoxTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QVariant valueVariant = index.model()->data(index, Qt::EditRole);
	QColor value = valueVariant.value<QColor>();

	QtColorComboBox *comboBox = static_cast<QtColorComboBox *>(editor);
	comboBox->setCurrentColor(value);
}


void RColorComboBoxTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QtColorComboBox *comboBox = static_cast<QtColorComboBox *>(editor);
	QColor value = comboBox->currentColor();

	model->setData(index, value, Qt::EditRole);
}


void RColorComboBoxTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}


void RColorComboBoxTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//paint(painter, option, index);
	QItemDelegate::paint(painter, option, index);
}