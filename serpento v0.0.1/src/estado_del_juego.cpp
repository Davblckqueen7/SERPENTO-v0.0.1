#include "estado_del_juego.h"

snake::Estado::Estado()
{
	reiniciar();
}

bool snake::Estado::es_el_final()
{
	return datos.es_el_final;
}

int snake::Estado::IDAgente()
{
	return datos.num_movi;
}

void snake::Estado::AplicarAccion(const Accion & accion)
{
	// Verificar que la accion este dentro del rango de acciones (0-2)
	if (accion.mov < 0 || accion.mov > 2) return;

	
		// Agregar segmentos de la cola a la serpiente (si existen)
		int prevX = datos.cola[0].first;
		int prevY = datos.cola[0].second;
		int prev2X, prev2Y;
		datos.cola[0].first = datos.pos.first;
		datos.cola[0].second = datos.pos.second;
		for (int i = 1; i < datos.longitud_cola; i++)
		{
			prev2X = datos.cola[i].first;
			prev2Y = datos.cola[i].second;
			datos.cola[i].first = prevX;
			datos.cola[i].second = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

	// Mover a la serpiente
	switch (datos.dir)
	{
	case IZQUIERDA:
		switch (accion.mov)
		{
		case MOVER_ADELANTE:
			datos.pos.first--; // x--
			break;
		case MOVER_IZQUIERDA:
			datos.pos.second++; // y++
			datos.dir = ABAJO;
			break;
		case MOVER_DERECHA:
			datos.pos.second--; // y--
			datos.dir = ARRIBA;
			break;
		default:
			break;
		}
		break;
	case DERECHA:
		switch (accion.mov)
		{
		case MOVER_ADELANTE:
			datos.pos.first++; // x++
			break;
		case MOVER_IZQUIERDA:
			datos.pos.second--; // y--
			datos.dir = ARRIBA;
			break;
		case MOVER_DERECHA:
			datos.pos.second++; // y++
			datos.dir = ABAJO;
			break;
		default:
			break;
		}
		break;
	case ARRIBA:
		switch (accion.mov)
		{
		case MOVER_ADELANTE:
			datos.pos.second--; // y--
			break;
		case MOVER_IZQUIERDA:
			datos.pos.first--; // x--
			datos.dir = IZQUIERDA;
			break;
		case MOVER_DERECHA:
			datos.pos.first++; // x++
			datos.dir = DERECHA;
			break;
		default:
			break;
		}
		break;
	case ABAJO:
		switch (accion.mov)
		{
		case MOVER_ADELANTE:
			datos.pos.second++; // y++
			break;
		case MOVER_IZQUIERDA:
			datos.pos.first++; // x++
			datos.dir = DERECHA;
			break;
		case MOVER_DERECHA:
			datos.pos.first--; // x--
			datos.dir = IZQUIERDA;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	// Incribimos los datos en el tablero de juego
	limpiar_tablero();
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (i == datos.pos.first && j == datos.pos.second)
			{
				datos.tablero[i][j] = casilla_Serpiente;
			}
			else if (i == datos.fruta.first && j == datos.fruta.second)
			{
				datos.tablero[i][j] = casilla_Alimento;
			}
			else
			{
				for (int k = 0; k < datos.longitud_cola; k++)
				{
					if (datos.cola[k].first == i && datos.cola[k].second == j)
					{
						datos.tablero[i][j] = casilla_Serpiente;
					}
				}
			}
		}
	}

	// Aumentamos el contador de n�mero de movimientos
	datos.num_movi++;

	// Verificar si la casilla en la que esta la cabeza de la serpiente es una pared
	if (datos.pos.first > 10 || datos.pos.first < 0 || datos.pos.second > 10 || datos.pos.second < 0)
		datos.es_el_final = true;

	// Verificar si la casilla en la que esta la cabeza se la serpiente es un segmento de su propio cuerpo
	for (int i = 0; i < datos.longitud_cola; i++)
		if (datos.cola[i].first == datos.pos.first && datos.cola[i].second == datos.pos.second)
			datos.es_el_final = true;

	// Verificar si la serpiente esta comiendo al realizar esta acci�n (aumentar longuitud de la cola de la serpiente)
	if (datos.pos.first == datos.fruta.first && datos.pos.second == datos.fruta.second)
	{
		datos.puntaje += 10;
		reubicar_comida();
		datos.cola.push_back(std::make_pair(datos.pos.first, datos.pos.second));
		datos.longitud_cola++;
		datos.comio_fruta = true;
	}
}

void snake::Estado::ObtenerAcciones(std::vector<Accion>& acciones)
{
	acciones.clear();

	int newPos;

		// Agregar segmentos de la cola a la serpiente (si existen)
		int prevX = datos.cola[0].first;
		int prevY = datos.cola[0].second;
		int prev2X, prev2Y;
		datos.cola[0].first = datos.pos.first;
		datos.cola[0].second = datos.pos.second;
		for (int i = 1; i < datos.longitud_cola; i++)
		{
			prev2X = datos.cola[i].first;
			prev2Y = datos.cola[i].second;
			datos.cola[i].first = prevX;
			datos.cola[i].second = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

	// si cualquiera de las casillas vecinas a la cabeza de la serpiente esta vacia o es comida es una accion valida
	switch (datos.dir)
	{
	case IZQUIERDA:
		newPos = datos.pos.first - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_ADELANTE));
		}
		newPos = datos.pos.second - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_DERECHA));
		}
		newPos = datos.pos.second + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_IZQUIERDA));
		}
		break;
	case ARRIBA:
		newPos = datos.pos.first - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_IZQUIERDA));
		}
		newPos = datos.pos.first + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_DERECHA));
		}
		newPos = datos.pos.second - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_ADELANTE));
		}
		break;
	case DERECHA:
		newPos = datos.pos.first + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_ADELANTE));
		}
		newPos = datos.pos.second - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_IZQUIERDA));
		}
		newPos = datos.pos.second + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_DERECHA));
		}
		break;
	case ABAJO:
		newPos = datos.pos.first + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_IZQUIERDA));
		}
		newPos = datos.pos.first - 1;
		if (newPos >= 0)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].first != newPos && datos.cola[i].second == datos.pos.second)
					acciones.push_back(Accion(MOVER_DERECHA));
		}
		newPos = datos.pos.second + 1;
		if (newPos < 10)
		{
			for (int i = 0; i < datos.longitud_cola; i++)
				if (datos.cola[i].second != newPos && datos.cola[i].first == datos.pos.first)
					acciones.push_back(Accion(MOVER_ADELANTE));
		}
		break;
	}
}

bool snake::Estado::ObtenerAccionAleatoria(Accion & accion)
{
	std::vector<Accion> acciones;
	ObtenerAcciones(acciones);

	// validacion de seguridad
	if (acciones.empty()) return false;

	accion = acciones[floor(ofRandom(acciones.size()))];
	return true;
}

const float snake::Estado::Evaluar() const
{
	float valor;
	valor = std::powf((1 / 3), datos.num_movi);
	if (valor != 0)
	{
		valor = valor;
	}
	return valor;
}

//----------------------------------------------------------------------------
// ESPECIFICOS DEL JUEGO:
//----------------------------------------------------------------------------

void snake::Estado::limpiar_tablero()
{
	datos.tablero.clear();
	std::vector<int> fila = { casilla_Vacia,casilla_Vacia,casilla_Vacia ,casilla_Vacia ,casilla_Vacia ,casilla_Vacia ,casilla_Vacia ,casilla_Vacia ,casilla_Vacia ,casilla_Vacia };
	for (int i = 0; i < 10; i++)
	{
		datos.tablero.push_back(fila);
	}
}

void snake::Estado::reubicar_comida()
{
	datos.fruta.first = floor(ofRandom(9));
	datos.fruta.second = floor(ofRandom(9));
}

void snake::Estado::reiniciar()
{
	datos.es_el_final = false;
	datos.comio_fruta = false;
	
	datos.dir = DERECHA;
	datos.pos.first = 4; datos.pos.second = 4;
	datos.cola.clear();
	datos.cola.push_back(std::make_pair(4, 4));
	datos.longitud_cola = 1;
	datos.puntaje = 0;
	datos.num_movi = 0;
	limpiar_tablero();
	reubicar_comida();
}

void snake::Estado::dibujar()
{
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(15);
	ofBackground(ofColor::gray);
	float w = ofGetWidth();
	float h = ofGetHeight();
	float casilla_w = w / 10;
	float casilla_h = h / 10;

	for (int c_l = 0; c_l < 10; c_l++)
	{
		for (int c_a = 0; c_a < 10; c_a++)
		{
			int i = c_a % 10;
			int j = c_l / 10;
			ofRectangle rect_casilla(i*casilla_w, j*casilla_h, casilla_w, casilla_h);
			float tama�o = 1.0;		
			if (datos.tablero[i][j] == casilla_Serpiente) 
			{
				ofSetColor(ofColor::green);
				ofDrawRectangle(rect_casilla);
			}
			if (datos.tablero[i][j] == casilla_Alimento)
			{
				ofSetColor(ofColor::red);
				ofDrawRectangle(rect_casilla);
			}
		}
	}

	ofPopStyle();
}