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

#include "RAggregationObjectWidget.h"
#include "RDomain.h"
#include "RModel.h"
#include "RResponse.h"

#include <QHeaderView>
#include <QMessageBox>

RAggregationObjectWidget::RAggregationObjectWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

    theDomain = nullptr;

	treeWidget->headerItem()->setText(0, "Name");
	treeWidget->sortItems(0, Qt::AscendingOrder);

	bool ok = connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(changeCurrent(QTreeWidgetItem *, QTreeWidgetItem *)));
	ok = connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(currentClicked(QTreeWidgetItem *)));
}


RAggregationObjectWidget::~RAggregationObjectWidget()
{

}


void RAggregationObjectWidget::setDomain(RDomain *passedDomain)
{
	theDomain = passedDomain;
}


void RAggregationObjectWidget::createHeader(QString name, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *item;

	if (parent == 0) {
		item = new QTreeWidgetItem(treeWidget);
	} else {
		item = new QTreeWidgetItem(parent);
	}
	item->setText(0, name);

	QObject *object = theDomain->findChild<QObject *>(name);
	QObjectList tempObjectList = object->findChildren<QObject *>();

	QStringList objectNameList;
	for (int i = 0; i < tempObjectList.count(); i++) {
		RResponse *response = qobject_cast<RResponse *>(tempObjectList[i]);
		if ((!response) && (tempObjectList[i]->parent() == object)) {
			objectNameList << tempObjectList[i]->objectName();
		}
	}
	
	for (int i = 0; i < objectNameList.count(); i++) {
		createHeader(objectNameList[i], item);
	}
	//fitColumns();
}


void RAggregationObjectWidget::updateView()
{
	//////Temporary
	//QList<RModel *> modelList = theDomain->findChildren<RModel *>();
	//for (int i = 0; i < modelList.count(); i++) {
	//	createHeader(modelList[i]->objectName());
	//}
}


void RAggregationObjectWidget::deleteItems()
{
	treeWidget->clear();
}


void RAggregationObjectWidget::selectItem(QString itemName)
{
	treeWidget->setFocus();
	treeWidget->clearSelection();
	QList<QTreeWidgetItem *> itemList = treeWidget->findItems(itemName, Qt::MatchExactly, 0);
	if (!itemList.isEmpty()) {
		itemList[0]->setSelected(true);
	}
}


void RAggregationObjectWidget::changeCurrent(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (current) {
		QString objectName = current->text(0);
		emit currentObjectChanged(objectName);
	}	
}


void RAggregationObjectWidget::currentClicked(QTreeWidgetItem *item)
{
	changeCurrent(item, 0);
}


void RAggregationObjectWidget::expandAll()
{
	treeWidget->expandAll();
	//fitColumns();
}


void RAggregationObjectWidget::collapseAll()
{
	treeWidget->collapseAll();
	//fitColumns();
}
