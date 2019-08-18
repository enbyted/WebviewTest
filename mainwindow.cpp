#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebChannel>

class TestInterceptor : public QWebEngineUrlRequestInterceptor
{
public:
    TestInterceptor(QObject* parent)
        : QWebEngineUrlRequestInterceptor(parent)
    {}

    void interceptRequest(QWebEngineUrlRequestInfo &info) override
    {
        qDebug() << "Request: " << info.requestUrl().toString();
    }
};

class TestPage : public QWebEnginePage
{
public:
    TestPage(QWebEngineProfile *profile, QObject *parent = Q_NULLPTR)
        : QWebEnginePage(profile, parent)
    {
        qDebug() << "TestPage constructed!";
        setWebChannel(new QWebChannel(this));
    }

    ~TestPage()
    {
        qDebug() << "TestPage destroyed!";
    }

protected:
    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
    {
        qDebug() << "Navigation request: " << url.toString();
        return true;
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->hide();

    m_transport = new TransportObject();
    m_transport->setParent(this);

    QWebEngineProfile* profile = new QWebEngineProfile(this);
    TestPage* page = new TestPage(profile, this);
    profile->setParent(page);

    connect(ui->widget, &QWebEngineView::loadFinished, this, &MainWindow::pageLoaded);
    connect(m_transport, &TransportObject::textUpdatedByRemote, this, &MainWindow::on_textChangedByRemote);

    ui->widget->setPage(page);

    page->webChannel()->registerObject("transport", m_transport);
    ui->widget->load(QUrl("https://clickonce.hipernet.info/test.html"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pageLoaded(bool ok)
{
    ui->widget->show();
}

void MainWindow::on_btnReload_clicked()
{
    ui->widget->hide();
    ui->widget->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void MainWindow::on_textEdit_textChanged()
{
    m_transport->textChanged(ui->textEdit->toPlainText());
}

void MainWindow::on_textChangedByRemote(QString text)
{
    ui->textEdit->setPlainText(text);
}
