/***
 * Manages image statistics.
 *
 */

#pragma once

#include "State/ObjectManager.h"
#include "State/StateInterface.h"
#include "Data/ILinkable.h"


class ImageView;

namespace Carta {

namespace Data {

class LinkableImpl;
class Controller;


class Statistics : public QObject, public Carta::State::CartaObject, public ILinkable {

    Q_OBJECT

public:

    //ILinkable
    virtual QString addLink( Carta::State::CartaObject* cartaObject ) Q_DECL_OVERRIDE;
    virtual QString removeLink( Carta::State::CartaObject* cartaObject ) Q_DECL_OVERRIDE;
    virtual QList<QString> getLinks() const Q_DECL_OVERRIDE;

    /**
     * Clear existing state.
     */
    void clear();

    /**
     * Return a string representing the statistics state of a particular type.
     * @param type - the type of state needed.
     * @param sessionId - an identifier for the user's session.
     * @return a QString representing the corresponding statistics state.
     */
    virtual QString getStateString( const QString& sessionId, SnapshotType type ) const Q_DECL_OVERRIDE;

    /**
     * Returns whether or not the object with the given id is already linked to this object.
     * @param linkId - a QString identifier for an object.
     * @return true if this object is already linked to the one identified by the id; false otherwise.
     */
    virtual bool isLinked( const QString& linkId ) const Q_DECL_OVERRIDE;
    virtual ~Statistics();
    const static QString CLASS_NAME;


private:
    void _initializeCallbacks();
    void _initializeState();

    static bool m_registered;

    Statistics( const QString& path, const QString& id );

    class Factory;

    //Link management
    std::unique_ptr<LinkableImpl> m_linkImpl;



	Statistics( const Statistics& other);
	Statistics& operator=( const Statistics& other );
};
}
}
