#include "QImagePlugin.h"
#include <QDebug>

QImagePlugin::QImagePlugin(QObject *parent) :
    QObject(parent)
{
}

bool QImagePlugin::handleHook(BaseHook & hookData)
{
    qDebug() << "QImage Plugin is handling hook #" << hookData.hookId();
    if( hookData.hookId() == Initialize::StaticHookId ) {
        return true;
    }

    if( hookData.hookId() == LoadImage::StaticHookId) {
        LoadImage & hook = static_cast<LoadImage &>( hookData);
        auto fname = hook.paramsPtr->fileName;
        return hook.result.load( fname);
    }

    qWarning() << "Sorrry, dont' know how to handle this hook";
    return false;
}

std::vector<HookId> QImagePlugin::getInitialHookList()
{
    return {
        Initialize::StaticHookId,
        LoadImage::StaticHookId
    };
}
