#ifndef RFileResponse_H
#define RFileResponse_H
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

#include "RResponse.h"

class RFileResponse : public RResponse
{
    Q_OBJECT
    Q_PROPERTY(QString ResponseFile READ getResponseFile WRITE setResponseFile) // Sets the file path of the text file
    Q_PROPERTY(bool Maximum READ isMaximum WRITE setMaximum)                    // Indicates if the response value should be set to the maximum of the values read from the file
    Q_PROPERTY(bool Absolute READ isAbsolute WRITE setAbsolute)                 // Indicates if the response value should be set to the absolute value of what is read from the file
    Q_PROPERTY(QString Tag READ getTag WRITE setTag)                            // Sets a "tag" that is searched for in the file; The "Row" number below is relative to that tag in the text file
    Q_PROPERTY(int Row READ getRow WRITE setRow)                                // Sets the row number in the file that contains the response value; If entered as 0, all rows at the given "Column" are considered
    Q_PROPERTY(int Column READ getColumn WRITE setColumn)                       // Sets the column number in the file that contains the response value; If entered as 0, all columns at the given "Row" are considered

public:
    RFileResponse(QObject *parent, QString name);
    ~RFileResponse();

    // getter/setter methods for meta-properties
    QString getResponseFile() const;
    void setResponseFile(QString value);
    bool isMaximum() const;
    void setMaximum(bool value);
    bool isAbsolute() const;
    void setAbsolute(bool value);
    QString getTag() const;
    void setTag(QString value);
    int getRow() const;
    void setRow(int value);
    int getColumn() const;
    void setColumn(int value);

    // Invokes the operation of updating the response value by reading the file
    int updateValue();

    QStringList getActionList();

private:
    // Data member for meta-properties
    QString theResponseFile;
    bool theMaximum;
    bool theAbsolute;
    QString theTag;
    int theRow;
    int theColumn;

};

#endif // RFileResponse_H
