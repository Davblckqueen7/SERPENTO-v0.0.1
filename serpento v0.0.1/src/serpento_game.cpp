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
		uct.uct_k = sqrt(2);
		uct.max_milisegundos = 0;
		uct.max_iteraciones = 100;
		uct.profundidad_simulacion = 100;
	}

	//--------------------------------------------------------------
	void update()
	{
		// si el juego no a terminado
		if (!estado.datos.es_el_final)
		{
			// depura el MCTS con el metodo UCT en el estado actual y obtiene la mejor acción para realizar
			accion = uct.depurar(estado);

			// aplica la accion al estado actual
			estado.AplicarAccion(accion);
		}
	}

	//--------------------------------------------------------------
	void draw()
	{
		estado.dibujar();
		//dibujar en consola:
		stringstream str;
		for (int i = -1; i < 10 + 2; i++)
			str << "#";
		str << endl;

		for (int i = -1; i < 10; i++)
		{
			for (int j = -1; j < 10; j++)
			{
				if (j == -1)
					str << "#";
				if (i == estado.datos.pos.second && j == estado.datos.pos.first)
					str << "O";
				else if (i == estado.datos.fruta.second && j == estado.datos.fruta.first)
					str << "F";
				else
				{
					bool print = false;
					for (int k = 0; k < estado.datos.longitud_cola; k++)
					{
						if (estado.datos.cola[k].first == j && estado.datos.cola[k].second == i)
						{
							str << "o";
							print = true;
						}
					}
					if (!print)
						str << " ";
				}


				if (j == 10 - 1)
					str << "#";
			}
			str << endl;
		}

		for (int i = -1; i < 10 + 2; i++)
			str << "#";
		str << endl;
		str << "Score:" << estado.datos.puntaje << endl;

		// estadisticas
		
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
	ofSetupOpenGL(800,800,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}