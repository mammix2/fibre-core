#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QIntValidator>
#include <QApplication>

#include "boost/random/uniform_int_distribution.hpp"

#include "guiconstants.h"
#include "walletmodel.h"


#include "RandomGridLayout.h"
#include "PasswordPushButton.h"

#include "askpassphrasedialog.h"

extern bool fWalletUnlockStakingOnly;

#define PIN_CODE_LEN			5

// random generated code min and max length, you can change this values
#define MIN_CODE_LEN			2
#define MAX_CODE_LEN			4

//-----------------------------------------------------------------------------

AskPassphraseDialog::AskPassphraseDialog(Mode mode, QWidget* pParent) : QDialog(pParent)
{
	this->mode = mode;
	model = 0;
	fCapsLock = false;

	// create the random generator with calculated seed
	m_pRNG = new boost::random::mt19937(0);

	// main dialog layout
	QVBoxLayout* pMainLayout = new QVBoxLayout(this);

	// sub layout holding password fields
	QFormLayout* pForm = new QFormLayout;

	// Instructions label
	QLabel* plbIns = new QLabel(tr("Use your keyboard as normal to enter passwords. "
											 "Or use a keypad below to create a pattern. "
											 "This pattern will then be your secure entry into the wallet."));
	plbIns->setWordWrap(true);
	pForm->addRow(plbIns);

	// Caps lock warning label
	m_plbCL = new QLabel;
	pForm->addRow(m_plbCL);

	m_plePin = new QLineEdit;
	m_plePin->setMaxLength(PIN_CODE_LEN);
	// make sure only digits are allowed
	int iPinMax = 1;
	for (int i = 0; i < PIN_CODE_LEN; i++)
		iPinMax = 10*iPinMax;
	m_plePin->setValidator(new QIntValidator(0, iPinMax-1, this));
	// hide the output
//	m_plePin->setEchoMode(QLineEdit::Password);
	// make sure the pin is checked every time the value changes
	connect(m_plePin, SIGNAL(textChanged(QString)), this, SLOT(CheckPin()));

	pForm->addRow(tr("Enter %1 digit PIN code").arg(PIN_CODE_LEN), m_plePin);

	if (this->mode != Encrypt) {
		// create old password field and add it to form layout
		m_pleOld = new QLineEdit;
		m_pleOld->setMaxLength(MAX_PASSPHRASE_SIZE);
		// **** Obfuscate text - just set the echo mode to Password
//        m_pleOld->setEchoMode(QLineEdit::Password);
		// **** End
		// notify the text change, so that we can enable/disable OK button
		connect(m_pleOld, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
		// set the event filter to detect CAPS LOCK status
		m_pleOld->installEventFilter(this);
		pForm->addRow(tr("Enter passphrase"), m_pleOld);

	}	else {
		m_pleOld = 0;
	}

	if (this->mode != Unlock && this->mode != Decrypt && this->mode != UnlockStaking) {
		// create new password field and add it to form layout
		m_pleNew = new QLineEdit;
		m_pleNew->setMaxLength(MAX_PASSPHRASE_SIZE);
		// **** Obfuscate text - just set the echo mode to Password
//        m_pleNew->setEchoMode(QLineEdit::Password);
		// **** End
		// notify the text change, so that we can enable/disable OK button
		connect(m_pleNew, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
		// set the event filter to detect CAPS LOCK status
		m_pleNew->installEventFilter(this);
		pForm->addRow(tr("New passphrase"), m_pleNew);

		// create repeat new password field and add it to form layout
		m_pleRepeated = new QLineEdit;
		m_pleRepeated->setMaxLength(MAX_PASSPHRASE_SIZE);
		// **** Obfuscate text - just set the echo mode to Password
//        m_pleRepeated->setEchoMode(QLineEdit::Password);
		// **** End
		// notify the text change, so that we can enable/disable OK button
		connect(m_pleRepeated, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
		// set the event filter to detect CAPS LOCK status
		m_pleRepeated->installEventFilter(this);
		pForm->addRow(tr("Repeat new passphrase"), m_pleRepeated);
	}	else {
		m_pleNew = 0;
		m_pleRepeated = 0;
	}

	if (this->mode != UnlockStaking) {
		m_pcbStaking = 0;
	}	else {
		// in staking mode, we need to add a checkbox, or so it seems from old .ui file
		m_pcbStaking = new QCheckBox(tr("For staking only"));
		m_pcbStaking->setChecked(true);
		pForm->addRow(m_pcbStaking);
	}

	// Warning label
	QLabel* plbWarn = new QLabel(GetWarningText());
	pForm->addRow(plbWarn);

	// Create the random placing layout object
	RandomGridLayout* pRandGrid = new RandomGridLayout(4, 4);

    // Create the button objects and constraints
    QPixmap FL_ButtonImage1;
    FL_ButtonImage1.load(":/icons/FL_ButtonImage1");
	 m_pFL_Button1 = new PasswordPushButton(FL_ButtonImage1, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button1);
	 connect(m_pFL_Button1, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button1->setObjectName("FL_Button1");
	 m_pFL_Button1->setMinimumSize(75,75);
	 m_pFL_Button1->setMaximumSize(75,75);

	 QPixmap FL_ButtonImage2;
    FL_ButtonImage2.load(":/icons/FL_ButtonImage2");
	 m_pFL_Button2 = new PasswordPushButton(FL_ButtonImage2, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button2);
	 connect(m_pFL_Button2, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button2->setObjectName("FL_Button2");
	 m_pFL_Button2->setMinimumSize(75,75);
	 m_pFL_Button2->setMaximumSize(75,75);

    QPixmap FL_ButtonImage3;
    FL_ButtonImage3.load(":/icons/FL_ButtonImage3");
	 m_pFL_Button3 = new PasswordPushButton(FL_ButtonImage3, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button3);
	 connect(m_pFL_Button3, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button3->setObjectName("FL_Button3");
	 m_pFL_Button3->setMinimumSize(75,75);
	 m_pFL_Button3->setMaximumSize(75,75);

    QPixmap FL_ButtonImage4;
    FL_ButtonImage4.load(":/icons/FL_ButtonImage4");
	 m_pFL_Button4 = new PasswordPushButton(FL_ButtonImage4, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button4);
	 connect(m_pFL_Button4, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button4->setObjectName("FL_Button4");
	 m_pFL_Button4->setMinimumSize(75,75);
	 m_pFL_Button4->setMaximumSize(75,75);

    QPixmap FL_ButtonImage5;
    FL_ButtonImage5.load(":/icons/FL_ButtonImage5");
	 m_pFL_Button5 = new PasswordPushButton(FL_ButtonImage5, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button5);
	 connect(m_pFL_Button5, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button5->setObjectName("FL_Button5");
	 m_pFL_Button5->setMinimumSize(75,75);
	 m_pFL_Button5->setMaximumSize(75,75);

    QPixmap FL_ButtonImage6;
    FL_ButtonImage6.load(":/icons/FL_ButtonImage6");
	 m_pFL_Button6 = new PasswordPushButton(FL_ButtonImage6, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button6);
	 connect(m_pFL_Button6, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button6->setObjectName("FL_Button6");
	 m_pFL_Button6->setMinimumSize(75,75);
	 m_pFL_Button6->setMaximumSize(75,75);

    QPixmap FL_ButtonImage7;
    FL_ButtonImage7.load(":/icons/FL_ButtonImage7");
	 m_pFL_Button7 = new PasswordPushButton(FL_ButtonImage7, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button7);
	 connect(m_pFL_Button7, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button7->setObjectName("FL_Button7");
	 m_pFL_Button7->setMinimumSize(75,75);
	 m_pFL_Button7->setMaximumSize(75,75);

    QPixmap FL_ButtonImage8;
    FL_ButtonImage8.load(":/icons/FL_ButtonImage8");
	 m_pFL_Button8 = new PasswordPushButton(FL_ButtonImage8, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button8);
	 connect(m_pFL_Button8, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button8->setObjectName("FL_Button8");
	 m_pFL_Button8->setMinimumSize(75,75);
	 m_pFL_Button8->setMaximumSize(75,75);

    QPixmap FL_ButtonImage9;
    FL_ButtonImage9.load(":/icons/FL_ButtonImage9");
	 m_pFL_Button9 = new PasswordPushButton(FL_ButtonImage9, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button9);
	 connect(m_pFL_Button9, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button9->setObjectName("FL_Button9");
	 m_pFL_Button9->setMinimumSize(75,75);
	 m_pFL_Button9->setMaximumSize(75,75);

    QPixmap FL_ButtonImage10;
    FL_ButtonImage10.load(":/icons/FL_ButtonImage10");
	 m_pFL_Button10 = new PasswordPushButton(FL_ButtonImage10, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button10);
	 connect(m_pFL_Button10, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button10->setObjectName("FL_Button10");
	 m_pFL_Button10->setMinimumSize(75,75);
	 m_pFL_Button10->setMaximumSize(75,75);

    QPixmap FL_ButtonImage11;
    FL_ButtonImage11.load(":/icons/FL_ButtonImage11");
	 m_pFL_Button11 = new PasswordPushButton(FL_ButtonImage11, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button11);
	 connect(m_pFL_Button11, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button11->setObjectName("FL_Button11");
	 m_pFL_Button11->setMinimumSize(75,75);
	 m_pFL_Button11->setMaximumSize(75,75);

    QPixmap FL_ButtonImage12;
    FL_ButtonImage12.load(":/icons/FL_ButtonImage12");
	 m_pFL_Button12 = new PasswordPushButton(FL_ButtonImage12, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button12);
	 connect(m_pFL_Button12, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button12->setObjectName("FL_Button12");
	 m_pFL_Button12->setMinimumSize(75,75);
	 m_pFL_Button12->setMaximumSize(75,75);

    QPixmap FL_ButtonImage13;
    FL_ButtonImage13.load(":/icons/FL_ButtonImage13");
	 m_pFL_Button13 = new PasswordPushButton(FL_ButtonImage13, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button13);
	 connect(m_pFL_Button13, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button13->setObjectName("FL_Button13");
	 m_pFL_Button13->setMinimumSize(75,75);
	 m_pFL_Button13->setMaximumSize(75,75);

    QPixmap FL_ButtonImage14;
    FL_ButtonImage14.load(":/icons/FL_ButtonImage14");
	 m_pFL_Button14 = new PasswordPushButton(FL_ButtonImage14, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button14);
	 connect(m_pFL_Button14, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button14->setObjectName("FL_Button14");
	 m_pFL_Button14->setMinimumSize(75,75);
	 m_pFL_Button14->setMaximumSize(75,75);

    QPixmap FL_ButtonImage15;
    FL_ButtonImage15.load(":/icons/FL_ButtonImage15");
	 m_pFL_Button15 = new PasswordPushButton(FL_ButtonImage15, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button15);
	 connect(m_pFL_Button15, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button15->setObjectName("FL_Button15");
	 m_pFL_Button15->setMinimumSize(75,75);
	 m_pFL_Button15->setMaximumSize(75,75);

    QPixmap FL_ButtonImage16;
    FL_ButtonImage16.load(":/icons/FL_ButtonImage16");
	 m_pFL_Button16 = new PasswordPushButton(FL_ButtonImage16, "", GetRandomCode());
	 pRandGrid->AddWidget(m_pFL_Button16);
	 connect(m_pFL_Button16, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	 m_pFL_Button16->setObjectName("FL_Button16");
	 m_pFL_Button16->setMinimumSize(75,75);
	 m_pFL_Button16->setMaximumSize(75,75);


	pRandGrid->PlaceWidgets();

	// sub layout holding the OK and Cancel buttons
	QHBoxLayout* pButtonLayout = new QHBoxLayout;
	QPushButton* pPB;
	// first add the stretching empty space
	pButtonLayout->addStretch(1);
	// create Cancel button and connect it to reject slot
	pPB = new QPushButton(tr("Cancel"));
	connect(pPB, SIGNAL(clicked()), this, SLOT(reject()));
	// **** Prevent activating Cancel button on Enter
	pPB->setAutoDefault(false);
	// **** End
	pButtonLayout->addWidget(pPB);
	// create OK button and connect it to accept slot
	m_pbOK = new QPushButton(tr("OK"));
	connect(m_pbOK, SIGNAL(clicked()), this, SLOT(accept()));
	// **** Activate OK button on Enter
	m_pbOK->setAutoDefault(true);
	// **** End
	pButtonLayout->addWidget(m_pbOK);

	// Place the sub layouts into the main one
	pMainLayout->addLayout(pForm);
	pMainLayout->addLayout(pRandGrid);
	pMainLayout->addLayout(pButtonLayout);

	// finally change the window title
	setWindowTitle(GetWindowTitle());
	// sets the OK button to disabled state
	textChanged();
	// sets the text code buttons to disabled state
	CheckPin();

        // **** Load the style sheet
    QFile f(":/styles/res/styles/fl-style.qss");
    f.open(QFile::ReadOnly);
    QString qsAll = f.readAll();
    f.close();
    // assign the style sheet to the entire dialog
    setStyleSheet(qsAll);
    // **** End

    // **** Set minimal and maximal allowed size
    setMinimumSize(420, 620);

    // **** End
}

//-----------------------------------------------------------------------------

AskPassphraseDialog::~AskPassphraseDialog()
{
	// delete the random generator object
	delete m_pRNG;
}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::accept()
{
	SecureString oldpass, newpass1, newpass2;
	if(!model)
		 return;
	oldpass.reserve(MAX_PASSPHRASE_SIZE);
	newpass1.reserve(MAX_PASSPHRASE_SIZE);
	newpass2.reserve(MAX_PASSPHRASE_SIZE);
	// TODO: get rid of this .c_str() by implementing SecureString::operator=(std::string)
	// Alternately, find a way to make this input mlock()'d to begin with.
	if (m_pleOld != 0)
		oldpass.assign(m_pleOld->text().toStdString().c_str());
	if (m_pleNew != 0)
		newpass1.assign(m_pleNew->text().toStdString().c_str());
	if (m_pleRepeated != 0)
		newpass2.assign(m_pleRepeated->text().toStdString().c_str());

	switch(mode)
	{
	case Encrypt: {
		 if(newpass1.empty() || newpass2.empty())
		 {
			  // Cannot encrypt with empty passphrase
			  break;
		 }
		 QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Confirm wallet encryption"),
                     tr("Warning: If you encrypt your wallet and lose your passphrase, you will <b>LOSE ALL OF YOUR FIBRE COINS!</b>!") + "<br><br>" + tr("Are you sure you wish to encrypt your wallet?"),
					 QMessageBox::Yes|QMessageBox::Cancel,
					 QMessageBox::Cancel);
		 if(retval == QMessageBox::Yes)
		 {
			  if(newpass1 == newpass2)
			  {
					if(model->setWalletEncrypted(true, newpass1))
					{
						 QMessageBox::warning(this, tr("Wallet encrypted"),
													 "<qt>" +
                                                     tr("Fibre-Qt will close now to finish the encryption process. "
													 "Remember that encrypting your wallet cannot fully protect "
                                                     "your fibre coins from being stolen by malware infecting your "
                                                     "computer, but FibreLock can help prevent key and mouse loggers") +
													 "<br><br><b>" +
													 tr("IMPORTANT: Any previous backups you have made of your wallet file "
													 "should be replaced with the newly generated, encrypted wallet file. "
													 "For security reasons, previous backups of the unencrypted wallet file "
													 "will become useless as soon as you start using the new, encrypted wallet.") +
													 "</b></qt>");
						 QApplication::quit();
					}
					else
					{
						 QMessageBox::critical(this, tr("Wallet encryption failed"),
													 tr("Wallet encryption failed due to an internal error. Your wallet was not encrypted."));
					}
					QDialog::accept(); // Success
			  }
			  else
			  {
					QMessageBox::critical(this, tr("Wallet encryption failed"),
												tr("The supplied passphrases do not match."));
			  }
		 }
		 else
		 {
			  QDialog::reject(); // Cancelled
		 }
		 } break;
	case UnlockStaking:
	case Unlock:
		 if(!model->setWalletLocked(false, oldpass))
		 {
			  QMessageBox::critical(this, tr("Wallet unlock failed"),
											tr("The passphrase entered for the wallet decryption was incorrect."));
		 }
		 else
		 {
			  if (m_pcbStaking != 0)
				  fWalletUnlockStakingOnly = m_pcbStaking->isChecked();
			  else
				  fWalletUnlockStakingOnly = false;

			  QDialog::accept(); // Success
		 }
		 break;
	case Decrypt:
		 if(!model->setWalletEncrypted(false, oldpass))
		 {
			  QMessageBox::critical(this, tr("Wallet decryption failed"),
											tr("The passphrase entered for the wallet decryption was incorrect."));
		 }
		 else
		 {
			  QDialog::accept(); // Success
		 }
		 break;
	case ChangePass:
		 if(newpass1 == newpass2)
		 {
			  if(model->changePassphrase(oldpass, newpass1))
			  {
					QMessageBox::information(this, tr("Wallet encrypted"),
												tr("Wallet passphrase was successfully changed."));
					QDialog::accept(); // Success
			  }
			  else
			  {
					QMessageBox::critical(this, tr("Wallet encryption failed"),
												tr("The passphrase entered for the wallet decryption was incorrect."));
			  }
		 }
		 else
		 {
			  QMessageBox::critical(this, tr("Wallet encryption failed"),
										  tr("The supplied passphrases do not match."));
		 }
		 break;
	}

}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::setModel(WalletModel* pModel)
{
	this->model = pModel;
}

//-----------------------------------------------------------------------------

bool AskPassphraseDialog::event(QEvent *event)
{
	 // Detect Caps Lock key press.
	 if (event->type() == QEvent::KeyPress) {
		  QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		  if (ke->key() == Qt::Key_CapsLock) {
				fCapsLock = !fCapsLock;
		  }
		  if (fCapsLock) {
				m_plbCL->setText(tr("<b>Warning: The Caps Lock key is on!</b>"));
		  } else {
				m_plbCL->clear();
		  }
	 }
	 return QWidget::event(event);
}

//-----------------------------------------------------------------------------

bool AskPassphraseDialog::eventFilter(QObject *object, QEvent *event)
{
	 /* Detect Caps Lock.
	  * There is no good OS-independent way to check a key state in Qt, but we
	  * can detect Caps Lock by checking for the following condition:
	  * Shift key is down and the result is a lower case character, or
	  * Shift key is not down and the result is an upper case character.
	  */
	 if (event->type() == QEvent::KeyPress) {
		  QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		  QString str = ke->text();
		  if (str.length() != 0) {
				const QChar *psz = str.unicode();
				bool fShift = (ke->modifiers() & Qt::ShiftModifier) != 0;
				if ((fShift && *psz >= 'a' && *psz <= 'z') || (!fShift && *psz >= 'A' && *psz <= 'Z')) {
					 fCapsLock = true;
					 m_plbCL->setText(tr("<b>Warning: The Caps Lock key is on!</b>"));
				} else if (psz->isLetter()) {
					 fCapsLock = false;
					 m_plbCL->clear();
				}
		  }
	 }
	 return QDialog::eventFilter(object, event);
}

//-----------------------------------------------------------------------------

PasswordPushButton* AskPassphraseDialog::CreateButton(QString qsText, QString qsPass) const
{
	// create button
	PasswordPushButton* pPPB = new PasswordPushButton(qsText, qsPass);
	// connect its SignalText signal to the general text appending slot
	connect(pPPB, SIGNAL(SignalText(QString)), this, SLOT(AppendText(QString)));
	return pPPB;
}

//-----------------------------------------------------------------------------

QString AskPassphraseDialog::GetWarningText() const
{
	QString qsWarn;
	switch (mode) {
	case Encrypt:
		qsWarn = tr("Enter the new passphrase to the wallet.<br/>Please use a passphrase of <b>ten or more random characters</b>, or <b>eight or more words</b>.");
		break;
	case UnlockStaking:
	case Unlock:
		qsWarn = tr("This operation needs your wallet passphrase to unlock the wallet.");
		break;
	case Decrypt:
		qsWarn = tr("This operation needs your wallet passphrase to decrypt the wallet.");
		break;
	case ChangePass:
		qsWarn = tr("Enter the old passphrase and new passphrase to the wallet.");
		break;
	}

	return qsWarn;
}

//-----------------------------------------------------------------------------

QString AskPassphraseDialog::GetWindowTitle() const
{
	QString qsTitle;
	switch (mode) {
	case Encrypt:
        qsTitle = tr("Encrypt wallet using Fibre Lock ©");
		break;
	case UnlockStaking:
	case Unlock:
        qsTitle = tr("Unlock wallet using Fibre Lock ©");
		break;
	case Decrypt:
        qsTitle = tr("Decrypt wallet using Fibre Lock ©");
		break;
	case ChangePass:
        qsTitle = tr("Change passphrase using Fibre Lock ©");
		break;
	}

	return qsTitle;

}

//-----------------------------------------------------------------------------

QString AskPassphraseDialog::GetCode(const QList<quint8> &rliCodes) const
{
	QString qsCode;
	for (int i = 0; i < rliCodes.count(); i++)
		qsCode += rliCodes[i];
	return qsCode;
}

//-----------------------------------------------------------------------------

QString AskPassphraseDialog::GetRandomCode()
{
	// add all available code characters here (you can edit the available character as you wish
	// without the need to change anything else)
    QString qsChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWYXZabcdefghijklmnopqrstuvwyz!$";
	using namespace boost::random;
	// distribution used to select random character
	uniform_int_distribution<> distrChar(0, qsChars.length() - 1);
	// distribution used to select the code length
	uniform_int_distribution<> distrLen(MIN_CODE_LEN, MAX_CODE_LEN);
	// get the code length
	int iLen = distrLen(*m_pRNG);
	// now generate the code: select a random character and repeat that iLen times
	QString qsCode;
	int iPos;
	for (int i = 0; i < iLen; i++) {
		// select a random position in qsChars
		iPos = distrChar(*m_pRNG);
		// add the selected character to the code
		qsCode += qsChars[iPos];
	}
	return qsCode;
}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::GenerateCodes()
{
	if (m_pRNG != 0)
		delete m_pRNG;
	// get the pin code
	int iPin = m_plePin->text().toInt();
	m_pRNG = new boost::random::mt19937(iPin);
	// generate codes for all buttons
	m_pFL_Button1->SetSignalText(GetRandomCode());
	m_pFL_Button2->SetSignalText(GetRandomCode());
	m_pFL_Button3->SetSignalText(GetRandomCode());
	m_pFL_Button4->SetSignalText(GetRandomCode());
	m_pFL_Button5->SetSignalText(GetRandomCode());
	m_pFL_Button6->SetSignalText(GetRandomCode());
	m_pFL_Button7->SetSignalText(GetRandomCode());
	m_pFL_Button8->SetSignalText(GetRandomCode());
	m_pFL_Button9->SetSignalText(GetRandomCode());
	m_pFL_Button10->SetSignalText(GetRandomCode());
	m_pFL_Button11->SetSignalText(GetRandomCode());
	m_pFL_Button12->SetSignalText(GetRandomCode());
	m_pFL_Button13->SetSignalText(GetRandomCode());
	m_pFL_Button14->SetSignalText(GetRandomCode());
	m_pFL_Button15->SetSignalText(GetRandomCode());
	m_pFL_Button16->SetSignalText(GetRandomCode());

	// enable all buttons
	m_pFL_Button1->setEnabled(true);
	m_pFL_Button2->setEnabled(true);
	m_pFL_Button3->setEnabled(true);
	m_pFL_Button4->setEnabled(true);
	m_pFL_Button5->setEnabled(true);
	m_pFL_Button6->setEnabled(true);
	m_pFL_Button7->setEnabled(true);
	m_pFL_Button8->setEnabled(true);
	m_pFL_Button9->setEnabled(true);
	m_pFL_Button10->setEnabled(true);
	m_pFL_Button11->setEnabled(true);
	m_pFL_Button12->setEnabled(true);
	m_pFL_Button13->setEnabled(true);
	m_pFL_Button14->setEnabled(true);
	m_pFL_Button15->setEnabled(true);
	m_pFL_Button16->setEnabled(true);

	if (m_pleOld != 0)
		m_pleOld->setEnabled(true);
	if (m_pleNew != 0)
		m_pleNew->setEnabled(true);
	if (m_pleRepeated != 0)
		m_pleRepeated->setEnabled(true);
}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::AppendText(QString qsText)
{
	// check if the widget with focus is a line edit field
	QLineEdit* pLE = dynamic_cast<QLineEdit*>(focusWidget());
	if (pLE != 0) {
		// check if the focus widget is one of the password
		// this condition can be removed, if the buttons could be used
		// to edit also some other text
		if (pLE == m_pleOld || pLE == m_pleNew || pLE == m_pleRepeated) {
			// append the given text to the existing one
			pLE->setText(pLE->text() + qsText);
		}
	}
}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::textChanged()
{
	 // Validate input, set Ok button to enabled when acceptable
	 bool acceptable = false;
	 switch(mode)
	 {
	 case Encrypt: // New passphrase x2
		  acceptable = !m_pleNew->text().isEmpty() && !m_pleRepeated->text().isEmpty();
		  break;
	 case UnlockStaking:
	 case Unlock: // Old passphrase x1
	 case Decrypt:
		  acceptable = !m_pleOld->text().isEmpty();
		  break;
	 case ChangePass: // Old passphrase x1, new passphrase x2
		  acceptable = !m_pleOld->text().isEmpty() && !m_pleNew->text().isEmpty() && !m_pleRepeated->text().isEmpty();
		  break;
	 }
	 m_pbOK->setEnabled(acceptable);
}

//-----------------------------------------------------------------------------

void AskPassphraseDialog::CheckPin()
{
	if (m_plePin->text().length() != PIN_CODE_LEN) {
		// disable all buttons
		m_pFL_Button1->setEnabled(false);
		m_pFL_Button2->setEnabled(false);
		m_pFL_Button3->setEnabled(false);
		m_pFL_Button4->setEnabled(false);
		m_pFL_Button5->setEnabled(false);
		m_pFL_Button6->setEnabled(false);
		m_pFL_Button7->setEnabled(false);
		m_pFL_Button8->setEnabled(false);
		m_pFL_Button9->setEnabled(false);
		m_pFL_Button10->setEnabled(false);
		m_pFL_Button11->setEnabled(false);
		m_pFL_Button12->setEnabled(false);
		m_pFL_Button13->setEnabled(false);
		m_pFL_Button14->setEnabled(false);
		m_pFL_Button15->setEnabled(false);
		m_pFL_Button16->setEnabled(false);

		// also disable edit fields
		if (m_pleOld != 0)
			m_pleOld->setEnabled(false);
		if (m_pleNew != 0)
			m_pleNew->setEnabled(false);
		if (m_pleRepeated != 0)
			m_pleRepeated->setEnabled(false);
	}	else {
		GenerateCodes();
		// disable the PIN edit. You can comment out this line, if the user
		// should have a chance to correct the pin once it is entered
		m_plePin->setEnabled(false);
		// move focus to the next field
		if (m_pleOld != 0)
			m_pleOld->setFocus();
		else if (m_pleNew != 0)
			m_pleNew->setFocus();
	}
}

//-----------------------------------------------------------------------------

