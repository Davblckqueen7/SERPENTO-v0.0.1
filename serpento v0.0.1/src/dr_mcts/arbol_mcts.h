/*
Un sencillo arbol MCTS (Monte Carlo Tree Search) en C++11.
El codigo MCTS esta basado proyecto en  Java (Simon Lucas - University of Essex) y el proyecto en Python (Peter Cowling, Ed Powley, Daniel Whitehouse - University of York) implementados aqui: http://mcts.ai/code/index.html
*/

#pragma once

#include "nodo_arbol.h"
#include "temporalizador_dr.h"
#include <cfloat>

namespace dr {
	namespace mcts {

		// El estado debe cumplir con la interfaz del estado (ver Estado_Ejemplo.h)
		// La acción puede ser cualquier cosa (lo que tu clase Estado sabe cómo manejar)
		template <class Estado, typename Accion>
		class UCT {
			typedef Nodo<Estado, Accion> NodoArbol;

		private:
			BucleTemp temporalizador;
			int iteraciones;

		public:
			float uct_k;					// k es el valor en la funcion UTC. por defecto = sqrt(2)
			unsigned int max_iteraciones;	// el maximo de iteraciones (0 to run till end)
			unsigned int max_milisegundos;		// maximo de milisegundos que durara la depuracion (0 to run till end)
			unsigned int profundidad_simulacion;	// cuantas capas de profundidad sera el limite para las simulaciones

			//--------------------------------------------------------------
			UCT() :
				iteraciones(0),
				uct_k(sqrt(2)),
				max_iteraciones(100),
				max_milisegundos(0),
				profundidad_simulacion(10)
			{
			}


			//--------------------------------------------------------------
			const BucleTemp & get_temp() const
			{
				return temporalizador;
			}

			const int get_iteraciones() const
			{
				return iteraciones;
			}

			//--------------------------------------------------------------
			// obtenga el mejor hijo (inmediato) para un Nodo determinado en función de la puntuación de uct
			NodoArbol* get_mejor_hijo_uct(NodoArbol* nodo, float uct_k) const
			{
				// comprovación de valides
				if (!nodo->esta_expandido()) return NULL;

				float mejor_utc_score = -std::numeric_limits<float>::max();
				NodoArbol* mejor_nodo = NULL;

				// iterar a todos los niños inmediatos y encontrar la mejor puntuación UTC
				int num_hijos = nodo->get_num_hijos();
				for (int i = 0; i < num_hijos; i++) {
					NodoArbol* hijo = nodo->get_hijo(i);
					float uct_explotacion = (float)hijo->get_valor() / (hijo->get_num_visitas() + FLT_EPSILON);
					float uct_exploracion = sqrt(log((float)nodo->get_num_visitas() + 1) / (hijo->get_num_visitas() + FLT_EPSILON));
					float uct_puntuacion = uct_explotacion + uct_k * uct_exploracion;

					if (uct_puntuacion > mejor_utc_score) {
						mejor_utc_score = uct_puntuacion;
						mejor_nodo = hijo;
					}
				}

				return mejor_nodo;
			}


			//--------------------------------------------------------------
			NodoArbol* get_hijo_mas_visitado(NodoArbol* nodo) const
			{
				int contador_visitas = -1;
				NodoArbol* mejor_nodo = NULL;

				// iterar a todos los niños inmediatos y encontrar el más visitado
				int num_hijos = nodo->get_num_hijos();
				for (int i = 0; i < num_hijos; i++) {
					NodoArbol* hijo = nodo->get_hijo(i);
					if (hijo->get_num_visitas() > contador_visitas) {
						contador_visitas = hijo->get_num_visitas();
						mejor_nodo = hijo;
					}
				}

				return mejor_nodo;
			}



			//--------------------------------------------------------------
			Accion depurar(const Estado& estado_actual, unsigned int semilla = 1, vector<Estado>* estados_explorados = nullptr)
			{
				// inicializar el temporalizador
				temporalizador.iniciar();

				// inicializar nodo raiz con el estado actual
				NodoArbol nodo_raiz(estado_actual);

				NodoArbol* mejor_nodo = NULL;

				// titeramos
				iteraciones = 0;
				while (true) {
					// indicamos el inicio del ciclo
					temporalizador.iniciar_ciclo();

					// 1. SELECCIÓN. comienza en la raíz, profundiza en el árbol utilizando UCT en todos los nodos completamente expandidos
					NodoArbol* nodo = &nodo_raiz;
					while (!nodo->es_el_final() && nodo->esta_expandido()) {
						nodo = get_mejor_hijo_uct(nodo, uct_k);
					}

					// 2. EXPANCIÓN. Exapandimos agregando un solo hijo (si no es terminal o no está completamente expandido)
					if (!nodo->esta_expandido() && !nodo->es_el_final()) nodo = nodo->expandir();

					Estado estado(nodo->get_estado());

					// 3. SIMULACIÓN. (si el nodo actual no es la meta del juego)
					if (!nodo->es_el_final()) {
						Accion accion;
						for (int t = 0; t < profundidad_simulacion; t++) {
							if (estado.is_terminal()) break;

							if (estado.get_random_action(accion))
								estado.apply_action(accion);
							else
								break;
						}
					}

					// obtener premios de todos los agentes
					const std::vector<float> premios = estado.evaluate();

					// añadimos a la historia
					if (estados_explorados) estados_explorados->push_back(estado);

					// 4. BACK PROPAGATION
					while (nodo) {
						nodo->actualizar(premios);
						nodo = nodo->get_padre();
					}

					// encontramos el hijo más valorado
					mejor_nodo = get_hijo_mas_visitado(&nodo_raiz);

					// indicamos que ya se acabo el ciclo
					temporalizador.finalizar_ciclo();

					// sale del ciclo si el tiempo de depuracion supera el limite maximo de milisegundos
					if (max_milisegundos > 0 && temporalizador.comprobar_duracion(max_milisegundos)) break;

					// sale del ciclo si se superan el maximo de iteraciones posibles
					if (max_iteraciones > 0 && iteraciones > max_iteraciones) break;
					iteraciones++;
				}

				// retorna la la mejor accion para el nodo actual
				if (mejor_nodo) return mejor_nodo->get_accion();

				// esto no deberia retornarce nunca
				return Accion();
			}


		};
	}
}