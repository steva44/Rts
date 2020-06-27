#ifndef RAggregationObjectWidget_H
#define RAggregationObjectWidget_H
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

#include <QWidget>
#include <QMenu>
#include "ui_RObjectWidget.h"

class RDomain;

class RAggregationObjectWidget : public QWidget, public Ui::RObjectWidgetUi
{
	Q_OBJECT

public:
    RAggregationObjectWidget(QWidget *parent = nullptr);
	~RAggregationObjectWidget();

	// Sets the Domain object
	void setDomain(RDomain *passedDomain);

	// Creates the object tree by reading the tree headers from RDomain
    void createHeader(QString name, QTreeWidgetItem *parent = nullptr);

	// Expands all branches of the object tree
	void expandAll();

	// Collapses all branches of the object tree
	void collapseAll();

public slots:
	// Updates the objects tree
	void updateView();

	// Selects an item in the tree that has the passed name
	void selectItem(QString itemName);

private slots:
	// Emits a signal that indicates that the user has changes the current selection in the objects tree
	void changeCurrent(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	// Emits a signal that indicates that the user has clicked on the current selection in the objects tree
	void currentClicked(QTreeWidgetItem *item);

signals:
	// Signal that is emitted when the current object selection changes
	void currentObjectChanged(QString &name);

private:
	// Removes the object tree items for all objects
	void deleteItems();

	// Pointer to the Domain object
	RDomain *theDomain;

};

#endif // RAggregationObjectWidget_H
