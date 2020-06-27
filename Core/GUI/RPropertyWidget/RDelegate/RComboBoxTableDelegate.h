#ifndef RComboBoxTableDelegate_H
#define RComboBoxTableDelegate_H
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

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

// This file is written based on the generic delegate example of Qt.

class RComboBoxTableDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	RComboBoxTableDelegate(QObject *parent = 0, const QStringList &passedStringList = QStringList() << "", bool passedEditable = true);	
	~RComboBoxTableDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QStringList theStringList;
	bool editable;

};

#endif // RComboBoxTableDelegate_H
