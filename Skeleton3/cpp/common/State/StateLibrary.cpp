#include "StateLibrary.h"
#include "Globals.h"
#include "IConnector.h"
#include <QDebug>

const QString StateLibrary::SEPARATOR = "/";
const QString StateLibrary::APP_ROOT = "carta";
const QString StateLibrary::ANIMATOR = "animator";
const QString StateLibrary::DATA = "data";
const QString StateLibrary::CHANNEL = "channel";
const QString StateLibrary::IMAGE = "image";
const QString StateLibrary::MOUSE = "mouse";
const QString StateLibrary::PLUGIN_LIST = "pluginList";

StateLibrary* StateLibrary::library = nullptr;


StateLibrary* StateLibrary::instance(){
	if ( library == nullptr ){
		library = new StateLibrary();
	}
	return library;
}

StateLibrary::StateLibrary(){
	_initializeStates();
}

void StateLibrary::_initializeStates(){
	states.insert(StateKey::DATA, State(SEPARATOR + APP_ROOT + SEPARATOR + DATA,"Available data files.",false));
	states.insert(StateKey::MOUSE_X, State(SEPARATOR + APP_ROOT + SEPARATOR + MOUSE +SEPARATOR + "x","X-coordinate of the mouse cursor.",false));
	states.insert(StateKey::MOUSE_Y, State(SEPARATOR +APP_ROOT +SEPARATOR+ MOUSE +SEPARATOR+"y","Y-coordinate of the mouse cursor.", false));
	states.insert(StateKey::PLUGIN_NAME,State( SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST+SEPARATOR+"name","Plugin name.", false ));
	states.insert(StateKey::PLUGIN_DESCRIPTION,State( SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST+SEPARATOR+"desciption","Plugin description", false ));
	states.insert(StateKey::PLUGIN_TYPE, State(SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST+SEPARATOR+"type", "Type of plugin", false ));
	states.insert(StateKey::PLUGIN_VERSION, State(SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST+SEPARATOR+"version","Plugin version", false));
	states.insert(StateKey::PLUGIN_ERRORS, State(SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST+SEPARATOR+"loadErrors","Plugin load errors", false));
	states.insert(StateKey::PLUGIN_STAMP, State(SEPARATOR +APP_ROOT +SEPARATOR+PLUGIN_LIST +SEPARATOR+"stamp","Number of plugins loaded", false));
	states.insert(StateKey::FRAME_CHANNEL, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"frame","Channel index", false ));
	states.insert(StateKey::FRAME_CHANNEL_END, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"frameEnd","Ending channel index", false));
	states.insert(StateKey::FRAME_CHANNEL_START, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"frameStart","Beginning channel index", false));
	states.insert(StateKey::ANIMATOR_CHANNEL_STEP, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"frameStep","Channel increment", true));
	states.insert(StateKey::ANIMATOR_CHANNEL_RATE, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"frameRate","Speed indicator for incrementing/decrementing channels in play or reverse play mode.", true));
	states.insert(StateKey::ANIMATOR_CHANNEL_END_BEHAVIOR, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+CHANNEL+SEPARATOR+"endBehavior","Defines the next index when at the beginning or end of the channel range.  Supported types are wrap, reverse, and jump.", true));
	states.insert(StateKey::FRAME_IMAGE, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"frame","Image index", false ));
	states.insert(StateKey::FRAME_IMAGE_END, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"frameEnd","Ending image index", false));
	states.insert(StateKey::FRAME_IMAGE_START, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"frameStart","Beginning image index", false));
	states.insert(StateKey::ANIMATOR_IMAGE_STEP, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"frameStep","Image increment", true));
	states.insert(StateKey::ANIMATOR_IMAGE_RATE, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"frameRate","Speed indicator for incrementing/decrementing images in play or reverse play mode.", true));
	states.insert(StateKey::ANIMATOR_IMAGE_END_BEHAVIOR, State(SEPARATOR +APP_ROOT +SEPARATOR + ANIMATOR+SEPARATOR+IMAGE+SEPARATOR+"endBehavior","Defines the next index when at the beginning or end of the image range.  Supported types are wrap, reverse, and jump.", true));
}

QString StateLibrary::getPath( StateKey stateKey, const QString& id) const{
	State stateValue = states[stateKey];
	QString path( stateValue.getPath());
	if ( id.trimmed().length() > 0 ){
		path.append( SEPARATOR + id.trimmed() );
	}
	return path;
}

StateKey StateLibrary::findKey( const QString& path ) const {
	StateKey pathKey = StateKey::END_KEY;
	for ( StateKey key : states.keys() ){
		if ( states[key].getPath() == path ){
			pathKey = key;
			break;
		}
	}
	return pathKey;
}

bool StateLibrary::isPersistent( const QString& path ) const {
	//Remove the window identifier from the path.
	int lastSeparationIndex = path.lastIndexOf( SEPARATOR);
	bool persistentState = false;
	if ( lastSeparationIndex >= 0 ){
		QString basePath = path.left( lastSeparationIndex );
		QList<State> stateValues = states.values();
		State lookup( basePath, "", false);
		int lookupIndex = stateValues.indexOf( lookup );
		if ( lookupIndex >= 0 ){
			persistentState = stateValues[lookupIndex].isPersistent();
		}
	}
	return persistentState;
}

