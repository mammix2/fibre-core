#include <QGridLayout>
#include <QDebug>
#include <QDateTime>

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include "RandomGridLayout.h"

//-----------------------------------------------------------------------------

RandomGridLayout::RandomGridLayout(int iRows, int iCols, QWidget *pParent) :
	QGridLayout(pParent)
{
	// remember the number of rows and columns in the grid
	m_iRows = iRows;
	m_iCols = iCols;

	// number of rows and columns must both be positive!
	Q_ASSERT(m_iRows > 0);
	Q_ASSERT(m_iCols > 0);


}

//-----------------------------------------------------------------------------

RandomGridLayout::~RandomGridLayout()
{
	// nothing to do here
}

//-----------------------------------------------------------------------------

void RandomGridLayout::AddWidget(QWidget* pW)
{
	// warn if the number of available grid places is exceeded!
	if (m_liWidgets.count() < m_iRows * m_iCols) {
		// just add the widget to the list
		m_liWidgets << pW;
	}	else {
		qWarning() << "RandomGridLayout: maximal number of widgets exceeded!"
					  << m_iRows*m_iCols << pW;
    }
}

//-----------------------------------------------------------------------------

void RandomGridLayout::PlaceWidgets()
{
	// first remove the widgets from current layout
	for (int i = 0; i < m_liWidgets.count(); i++)
		removeWidget(m_liWidgets[i]);

	// Boost random generator. Set the seed to the number of milliseconds passed
	// since midnight January 1st, 1970
	boost::random::mt19937 rng(QDateTime::currentDateTime().toMSecsSinceEpoch());
	// list of available positions on the grid
	QList<int> li;
	// fill the list with all available positions
	// Positions are indicated by their indices
	// For 4x4 grid, indices indicate position as follows:
	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	// 12 13 14 15
	// Row can be retrieved by dividing position index with number of columns
	// Column can be retrieved as a remainder from integer division between index and number
	// of columns
	for (int i = 0; i < m_liWidgets.count(); i++)
		li << i;

	// now go through widget list and randomly select a position for each
	for (int i = 0; i < m_liWidgets.count(); i++) {
		// create a uniform integer distribution to select from remaining free positions
		boost::random::uniform_int_distribution<> distribution(0, li.count() - 1);
		// get the random number from uniform distribution
		int iRN = distribution(rng);
		// get the position from the list
		int iPos = li[iRN];
		// place the widget to the correct row and column
		addWidget(m_liWidgets[i], iPos / m_iCols, iPos % m_iCols);
		// delete the used position index!
		li.removeAt(iRN);
	}


}

//-----------------------------------------------------------------------------

