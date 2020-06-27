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

#include "RComboBoxTableDelegate.h"

#include <QtGui>
#include <QCompleter>

RComboBoxTableDelegate::RComboBoxTableDelegate(QObject *parent, const QStringList &passedStringList, bool passedEditable)
	: QItemDelegate(parent)
{
	theStringList = passedStringList;
	setObjectName("RDelegate0");
	editable = passedEditable;
}


RComboBoxTableDelegate::~RComboBoxTableDelegate()
{
	
}


QWidget * RComboBoxTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QComboBox *editor = new QComboBox(parent);
	editor->addItems(theStringList);
	editor->setEditable(editable);
	editor->setObjectName("RDelegate1");
	//editor->lineEdit()->selectAll();

	QCompleter *completer = new QCompleter(theStringList, editor);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    editor->setCompleter(completer);

	// Temporaty lines to remember how to implement path delegates
	//QCompleter *completer = new QCompleter(editor);
	//QDirModel *dirModel = new QDirModel(completer);
	//completer->setModel(dirModel);

	// ISNTALL OR REMOVE?
	editor->installEventFilter(const_cast<RComboBoxTableDelegate *>(this));
	return editor;
}


void RComboBoxTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();

	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	if (editable) {
		comboBox->setEditText(value);
	} else {
		comboBox->setCurrentIndex(comboBox->findText(value));
	}
}


void RComboBoxTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	//comboBox->interpretText();
	QString value = comboBox->currentText();

	model->setData(index, value, Qt::EditRole);
}


void RComboBoxTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}


void RComboBoxTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//paint(painter, option, index);
	QItemDelegate::paint(painter, option, index);
}
