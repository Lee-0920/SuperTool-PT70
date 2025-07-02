#ifndef AUXILIARYWIDGET_H
#define AUXILIARYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QThread>
#include <QTimer>

namespace UI
{
class AuxiliaryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuxiliaryWidget(QWidget *parent = 0);

    QPushButton *m_oneKeyBackupButton;
    QPushButton *m_clearBackupButton;
    QPushButton *m_extractBackupButton;
    QPushButton *m_clearDataButton;
    QPushButton *m_clearAllButton;
    QPushButton *m_advancedButton;
    QPushButton *m_startLuipButton;
    QPushButton *m_setDateTimeButton;
    QPushButton *m_copyLogTxtButton;
    QPushButton *m_copyAllLogTxtButton;
signals:

private slots:

   void ClearBackupSlot();
   void SetDateTimeSlot();
   void AdvancedSlot();
   void StartLuipSlot();
   void OneKeyBackupSlot();
   void ExtractBackupSlot();
   void ClearDataSlot();
   void ClearAllSlot();
   void GetLogTxtSlot();
   void GetAllLogTxtSlot();
   void ClearSNSlot();
   void ClearMeasureStatusSlot();
   void ClearRemainSlot();
   void ClearHardwareSlot();
};
}

#endif // AUXILIARYWIDGET_H
