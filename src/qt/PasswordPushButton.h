#ifndef PASSWORDPUSHBUTTON_H
#define PASSWORDPUSHBUTTON_H

#include <QPushButton>
#include <QToolButton>

/** This class is a push button with the assigned text, which is then passed
 *  in a signal when the button is clicked
 */
class PasswordPushButton : public QPushButton
{
	Q_OBJECT

public:
	/** Text button constructor. qsText is the visible text on button, while qsPass
	 *  is the text that will be passed through SignalText signal
	 */
	PasswordPushButton(QString qsText, QString qsPass, QWidget* pParent = 0);
	/** Icon button constructor. qsText is the visible text on button, while qsPass
	 * is the text that will be passed through SignalText signal
	 */
	PasswordPushButton(const QIcon& icon, QString qsText, QString qsPass, QWidget* pParent = 0);
	//! Destructor
	~PasswordPushButton();
	//! Sets the text to be sent by the signal
	void SetSignalText(QString qsPass)
	{	m_qsPass = qsPass; }

signals:
	//! Emitted when button is clicked
	void SignalText(QString qsText);

private slots:
	//! Raises the signalText signal
	void RaiseSignal();

private:
	//! Text to be sent with SignalText
	QString m_qsPass;

};

#endif // PASSWORDPUSHBUTTON_H
