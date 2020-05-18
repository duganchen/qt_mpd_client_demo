#include "abstractmpdconnection.h"

AbstractMPDConnection::AbstractMPDConnection(AbstractMPDSettings *settings, QObject *parent) : QObject(parent)
{
    Q_UNUSED(settings)
}
