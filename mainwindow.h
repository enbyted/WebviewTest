#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class TransportObject : public QObject
{
    friend class MainWindow;
    Q_OBJECT
private:
    void textChanged(QString str)
    {
        emit textUpdated(str);
    }

public slots:
    void updateText(QString str)
    {
        emit textUpdatedByRemote(str);
    }

signals:
    void textUpdated(QString str);
    void textUpdatedByRemote(QString str);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    TransportObject* m_transport;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
     void pageLoaded(bool ok);

private slots:
     void on_btnReload_clicked();
     void on_textChangedByRemote(QString text);
     void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
