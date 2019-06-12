#pragma once

#include "dr_mcts/arbol_mcts.h"

#include "ofMain.h"

using namespace dr::mcts;

namespace snake {
	struct Accion
	{
	public:
		Accion(int m = 0) :movimiento(m) {}
		int movimiento; //Las acciones posibles seran 3
						//	- 0: Mover a Adelante
						//	- 1: Mover a la Izquierda
						//	- 2: Mover a la Derecha
	};

#define casilla_Vacia		-1
#define casilla_Serpiente	1
#define casilla_Alimento	0

	class Estado
	{
	public:
		//-----------------------------------
		// METODOS NECESARIOS

		// Constructor
		Estado();

		// ya sea que el estado este terminado o no (Estaria terminado si no se puede sobrevivir o comio fruta)
		bool SeAcabo();

		// id del agente (por defecto en 0) que tomara la decición
		int IDAgente();

		// Aplicar una acción al estado 
		//* Parametros:
		//**	 referencia a la acción a realizar
		void AplicarAccion(const Accion& accion);

		// Retorna las posibles acciones de este estado
		//* Parametros:
		//**	 referencia a las acciones a realizar
		void ObtenerAcciones(std::vector<Accion>& acciones);

		// Obtiene una acción aleatoria, retorna falso si no se encontraron acciones para realizar
		//* Parametros:
		//**	 referencia a la acción a realizar
		bool ObtenerAccionAleatoria(Accion& accion);

		// Evalua este estado y retorna el premio (para cada agente)
		const float Evaluar() const;

		//-----------------------------------
		// IMPLEMENTACION ESPECIFICA
		struct
		{
			int num_movi;			// un contador de los movimientos realizados por la serpiente
			bool es_el_final;		// ya sea que el estado este terminado o no. (Estaria terminado si no se puede sobrevivir o comio fruta)
			int puntaje;			// un contador del puntaje optenido por la serpiente
			int tablero[10][10];	// el contenedor de las casillas y el espacio de juego
		} datos;

		void reiniciar();

		// Funciones para usar OpenFrameworks:

		void dibujar();
		void dibujar_serpiente();
		void dibujar_alimento();
	};
}

