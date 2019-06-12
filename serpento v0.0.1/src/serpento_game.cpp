#include "estado_del_juego.h"
#include "ofMain.h"

using namespace snake;

class ofApp : public ofBaseApp
{
public:
	Estado estado;
	Accion accion;
	UCT<Estado, Accion> uct;

	//--------------------------------------------------------------
	void setup()
	{

	}

private:

};
//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
