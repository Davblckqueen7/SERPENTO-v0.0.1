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
		ofSetBackgroundAuto(true);
		ofBackground(0);
		ofSetVerticalSync(true);

		// Inicio de los parametros para el arbol de busquedas UCT *OPCIONAL
		//TODO Revizar que valor conviene m�s
		uct.uct_k = sqrt(2);
		uct.max_milisegundos = 0;
		uct.max_iteraciones = 100;
		uct.profundidad_simulacion = 100;
	}

	//--------------------------------------------------------------
	void update()
	{

	}

	//--------------------------------------------------------------
	void draw()
	{
		estado.dibujar();

		// estadisticas
		stringstream str;
		str << ofGetFrameRate() << " fps" << endl;
		str << "Tiempo total : " << uct.get_temp().get_duracion_depuracion() << " us" << endl;
		str << "Tiempo promedio : " << uct.get_temp().get_promedio_tiempo_duracion_ciclos() << " us" << endl;
		str << "Iteraciones : " << uct.get_iteraciones() << endl;
		str << "--------------------------------------------" << endl;
		//str << estado.to_string();

		if (estado.es_el_final())
		{
			str << endl << endl;
			str << "Juego Terminado. Presiona SPACE para reiniciar." << endl;
		}

		ofSetColor(255);
		ofDrawBitmapString(str.str(), 10, 15);
	}

	//--------------------------------------------------------------
	void keyPressed(int key)
	{
		switch (key) {
		case 'f':
			ofToggleFullscreen();
			break;

		case ' ':
			estado.reiniciar();
			break;
		}
	}
};

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}