#ifndef ASKPASSPHRASEDIALOG_H
#define ASKPASSPHRASEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "boost/random/mersenne_twister.hpp"

#include "PasswordPushButton.h"

class WalletModel;

//! This class allows user to enter new password
class AskPassphraseDialog : public QDialog
{
	Q_OBJECT

public:
	enum Mode {
		 Encrypt,		/**< Ask passphrase twice and encrypt */
		 UnlockStaking, /**< Ask passphrase and unlock */
		 Unlock,			/**< Ask passphrase and unlock */
		 ChangePass,	/**< Ask old passphrase + new passphrase twice */
		 Decrypt			/**< Ask passphrase and decrypt wallet */
	};

	//! Constructor
	AskPassphraseDialog(Mode mode, QWidget* pParent = 0);
	//! Destructor
	virtual ~AskPassphraseDialog();

	//! Performs the operations required
	void accept();
	//! Sets the pointer to the model
	void setModel(WalletModel* pModel);

	//! Returns the entered old password
	QString GetOldPassword() const
	{	return m_pleOld->text(); }
	//! Returns the entered new password
	QString GetNewPassword() const
	{	return m_pleNew->text(); }
	//! Returns the entered repeated new password
	QString GetRepeatedPassword() const
	{	return m_pleRepeated->text(); }

protected:
	//! Both these even handlers are used to detect CAPS LOCK status
	bool event(QEvent *event);
	bool eventFilter(QObject *object, QEvent *event);


private:
	//! Creates and returns the button, taking care of all the details
	PasswordPushButton* CreateButton(QString qsText, QString qsPass) const;
	//! Returns the warning text depending on starting mode
	QString GetWarningText() const;
	//! Returns the window title text depending on starting mode
	QString GetWindowTitle() const;
	//! Converts the list of ASCII codes from rliCodes into a string and returns it
	QString GetCode(const QList<quint8>& rliCodes) const;
	//! Returns the randomly generated string
	QString GetRandomCode();
	//! Generates the text codes for all buttons
	void GenerateCodes();

private slots:
	//! This slot appends the given text to the existing text in line edit with focus
	void AppendText(QString qsText);
	//! Enables/disables OK button
	void textChanged();
	//! Checks the pin code length, generates and enables the buttons, if necessary
	void CheckPin();
    //! Check the use of the use fibrelock option
    void useFibreLockOption();

private:
	//! Indicates the mode of operation
	Mode mode;
	//! Pointer to the wallet model object
	WalletModel *model;
	//! Indicates whether the CAPS LOCK is switched on
	bool fCapsLock;

	//! Pin field
	QLineEdit* m_plePin;
	//! Old password field
	QLineEdit* m_pleOld;
	//! New password field
	QLineEdit* m_pleNew;
	//! Repeated password field
	QLineEdit* m_pleRepeated;
	//! Label for issuing CAPS LOCK warning
	QLabel* m_plbCL;

	//! OK button
	QPushButton* m_pbOK;
	//! Staking check box
	QCheckBox* m_pcbStaking;
    //! FibreLock usage option
    QCheckBox* m_useFibreLock;
	//! Pointer to the random number generator
	boost::random::mt19937* m_pRNG;
	//! Pointers to password buttons
	PasswordPushButton* m_pFL_Button1;
	PasswordPushButton* m_pFL_Button2;
	PasswordPushButton* m_pFL_Button3;
	PasswordPushButton* m_pFL_Button4;
	PasswordPushButton* m_pFL_Button5;
	PasswordPushButton* m_pFL_Button6;
	PasswordPushButton* m_pFL_Button7;
	PasswordPushButton* m_pFL_Button8;
	PasswordPushButton* m_pFL_Button9;
	PasswordPushButton* m_pFL_Button10;
	PasswordPushButton* m_pFL_Button11;
	PasswordPushButton* m_pFL_Button12;
	PasswordPushButton* m_pFL_Button13;
	PasswordPushButton* m_pFL_Button14;
	PasswordPushButton* m_pFL_Button15;
	PasswordPushButton* m_pFL_Button16;
};

#endif // ASKPASSPHRASEDIALOG_H
