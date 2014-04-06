#include "Viewer.h"
#include "misc.h"
#include <iostream>
#include <QImage>
#include <QColor>
#include <QPainter>

class TestView : public IView
{

public:

    TestView() {
        m_qimage = QImage( 100, 100, QImage::Format_RGB888);
        m_qimage.fill( QColor( "blue"));

        m_viewName = "view1";
        m_connector= nullptr;
    }

    virtual void registration(IConnector *connector)
    {
        m_connector = connector;
    }
    virtual const QString & name() const
    {
        return m_viewName;
    }
    virtual QSize size()
    {
        return m_qimage.size();
    }
    virtual const QImage & getBuffer()
    {
        return m_qimage;
    }
    virtual void handleResizeRequest(const QSize & /*size*/)
    {
    }
    virtual void handleMouseEvent(const QMouseEvent & ev)
    {
        std::cerr << "Mouse " << ev.x() << "," << ev.y() << "\n";

        m_lastMouse = QPointF( ev.x(), ev.y());


        QPointF center = m_qimage.rect().center();
        QPointF diff = m_lastMouse - center;
        double angle = atan2( diff.x(), diff.y());
        angle *= - 180 / M_PI;

        m_qimage.fill( QColor( "blue"));
        QPainter p( & m_qimage);
        p.translate( m_qimage.rect().center());
        p.rotate( angle);
        p.translate( - m_qimage.rect().center());
        p.setFont( QFont( "Arial", 20));
        p.setPen( QColor( "white"));
        p.drawText( m_qimage.rect(), Qt::AlignCenter, m_viewName);

        m_connector-> refreshView( this);
    }
    virtual void handleKeyEvent(const QKeyEvent & /*event*/)
    {
    }


protected:
    IConnector * m_connector;
    QImage m_qimage;
    QString m_viewName;
    int m_timerId;
    QPointF m_lastMouse;
};


Viewer::Viewer( IPlatform * platform) :
    QObject( nullptr)
{
    m_platform = platform;
}

void Viewer::start()
{

    // setup connector
    auto connector = m_platform-> createConnector();

    if( ! connector || ! connector-> initialize()) {
        std::cerr << "Could not initialize connector.\n";
        exit( -1);
    }

    std::cerr << "Viewer::start: connector initialized\n";

    // associate a callback for a command
    connector->addCommandCallback( "debug", [] (const QString & cmd, const QString & params, const QString & sessionId) -> QString {
        std::cerr << "lambda command cb:\n"
                  << " " << cmd << "\n"
                  << " " << params << "\n"
                  << " " << sessionId << "\n";
        return "1";
    });

    connector-> addStateCallback( "/xyz", [] ( const QString & path, const QString & val) {
        std::cerr << "lambda state cb:\n"
                  << "  path: " << path << "\n"
                  << "  val:  " << val << "\n";
    });

    static const QString varPrefix = "/myVars";
    static int pongCount = 0;
    connector-> addStateCallback(
                varPrefix + "/ping",
                [=] ( const QString & path, const QString & val) {
        std::cerr << "lcb: " << path << "=" << val << "\n";
        QString nv = QString::number( pongCount ++);
        connector-> setState( varPrefix + "/pong", nv);
    });


    connector-> setState( "/xya", "hola");
    connector-> setState( "/xyz", "8");

    // create a view to be rendered on the client side
    TestView * testView = new TestView();

    connector-> registerView( testView);
}