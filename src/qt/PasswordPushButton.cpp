#include "PasswordPushButton.h"

//-----------------------------------------------------------------------------

PasswordPushButton::PasswordPushButton(QString qsText, QString qsPass, QWidget* pParent) :
    QPushButton(qsText, pParent)
{
    // remember the text for passing with the signal
    m_qsPass = qsPass;
    // connect the signal produced by the QPushButton to the slot, which will raise
    // our signal with the text parameter
    connect(this, SIGNAL(clicked()), this, SLOT(RaiseSignal()));
    // prevent this button to "steal" the focus from the line edit fields!
    setFocusPolicy(Qt::NoFocus);
}

//-----------------------------------------------------------------------------

PasswordPushButton::PasswordPushButton(const QIcon& icon, QString qsText, QString qsPass, QWidget* pParent) :
    QPushButton(icon, qsText, pParent)
{
	// remember the text for passing with the signal
	m_qsPass = qsPass;
	// connect the signal produced by the QPushButton to the slot, which will raise
	// our signal with the text parameter
	connect(this, SIGNAL(clicked()), this, SLOT(RaiseSignal()));
	// prevent this button to "steal" the focus from the line edit fields!
	setFocusPolicy(Qt::NoFocus);
}

//-----------------------------------------------------------------------------

PasswordPushButton::~PasswordPushButton()
{
	// nothing to do here
}

//-----------------------------------------------------------------------------

void PasswordPushButton::RaiseSignal()
{
	// just emit the signal with the remembered text
	emit SignalText(m_qsPass);
}

//-----------------------------------------------------------------------------

