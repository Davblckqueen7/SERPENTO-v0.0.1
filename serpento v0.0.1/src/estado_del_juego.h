#pragma once

#include "dr_mcts/arbol_mcts.h"

#include "ofMain.h"

using namespace dr::mcts;

namespace snake {

	const enum movimiento { MOVER_ADELANTE = 0, MOVER_IZQUIERDA, MOVER_DERECHA};
	struct Accion
	{
	public:
		Accion(movimiento m = MOVER_ADELANTE) :mov(m) {}
		movimiento mov; //Las acciones posibles seran 3
						//	- 0: Mover a Adelante
						//	- 1: Mover a la Izquierda
						//	- 2: Mover a la Derecha
	};

	constexpr auto casilla_Vacia = 0;
	constexpr auto casilla_Serpiente = 1;
	constexpr auto casilla_Alimento = -1;
	const enum direccion { IZQUIERDA = 0, ARRIBA, DERECHA, ABAJO };

	class Estado
	{
	public:
		//-----------------------------------
		// METODOS NECESARIOS

		// Constructor
		Estado();

		// ya sea que el estado este terminado o no (Estaria terminado si no se puede sobrevivir o comio fruta)
		bool es_el_final();

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
			bool es_el_final;		// ya sea que el estado este terminado o no. (Estaria terminado si no se puede sobrevivir)
			bool comio_fruta;		// yas sea que la cabeza este sobre la fruta (come)
			int puntaje;			// un contador del puntaje optenido por la serpiente
			int tablero[10][10];	// el contenedor de las casillas y el espacio de juego
			std::pair<int, int> pos;// posición de la cabeza de la serpiente
			std::pair<int, int> fruta;// posición de la fruta en el tablero
			std::vector<std::pair<int, int>> cola;// posición de los segmentos de cola de la serpiente
			int longitud_cola;		// longitud de la serpiente
			direccion dir;			// direccion a donde apunta la cabeza de la serpiente
		} datos;

		// re-ubicar la fruta
		void reubicar_comida();

		// reinicia la partida
		void reiniciar();

		// Funciones para usar OpenFrameworks:

		void dibujar();
		void dibujar_serpiente();
		void dibujar_alimento();
	};
}

