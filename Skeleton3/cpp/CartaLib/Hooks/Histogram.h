/**
 * Hook for adding histogram to the system.
 *
 **/

#pragma once

#include "CartaLib/CartaLib.h"
#include "CartaLib/IPlugin.h"
#include <vector>

namespace Carta
{
namespace Lib
{
namespace Hooks
{
class HistogramHook : public BaseHook
{
    CARTA_HOOK_BOILER1( HistogramHook )

public:

    /**
     * @brief Result is histogram data.
     */
    typedef std::vector<std::pair<double,double> > ResultType;

    /**
     * @brief Params
     */
     struct Params {
            Params( QString p_fileName )
            {
                fileName = p_fileName;
            }

            QString fileName;
        };

    /**
     * @brief PreRender
     * @param pptr
     *
     * @todo make hook constructors protected, so that only hook helper can create them
     */
    HistogramHook( Params * pptr ) : BaseHook( staticId ), paramsPtr( pptr )
    {
        qDebug() << "Histogram hook Made";
        qDebug() << "static id ="<<staticId;
        CARTA_ASSERT( is < Me > () );
    }

    ResultType result;
    Params * paramsPtr;
};
}
}
}