#ifndef FLOWMANAGER_H
#define FLOWMANAGER_H

#include <memory>
#include <QThread>
#include <QString>

namespace Flow
{
class FlowManager : public QThread
{
    Q_OBJECT
public:
    static FlowManager *Instance();
    ~FlowManager();

    void OnUpdater(QString path);
    void OnStartLuip();

private:
    FlowManager();

    static std::unique_ptr<FlowManager> m_instance;

    QThread *m_flowThread;
    bool m_isFlowRunnig;

signals:
    void UpdaterSignal(QString path);
    void StartLuipSignal();
    void UpdaterWindowSignal();

private slots:

   void UpdaterSlot(QString path);
   void StartLuipSlot();


};
}

#endif // FLOWMANAGER_H
