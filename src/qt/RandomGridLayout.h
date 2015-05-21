#ifndef RANDOMGRIDLAYOUT_H
#define RANDOMGRIDLAYOUT_H

#include <QGridLayout>

/** This class extends QGridLayout and offers the possibility to randomly
 *  place widgets on a grid
 */
class RandomGridLayout : public QGridLayout
{
public:
	//! Constructor. Requires the number of rows and columns to be specified
	RandomGridLayout(int iRows, int iCols, QWidget* pParent = 0);
	//! Destructor
	virtual ~RandomGridLayout();

	//! Adds the widget to the list. Layout takes ownership of the widget
	void AddWidget(QWidget* pW);

	//! Places the widgets randomly
	void PlaceWidgets();

private:
	//! Number of grid rows
	int m_iRows;
	//! Number of grid columns
	int m_iCols;
	//! List of widgets, children of this layout class, which can be placed randomly
	QList<QWidget*> m_liWidgets;
};

#endif // RANDOMLAYOUT_H
