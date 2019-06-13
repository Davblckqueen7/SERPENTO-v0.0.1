/*
Un Nodo dentro del arbol de deciciones.
Mantuve separado esta clase independiente de la clase princial UCT/MCTS.
Contiene información / métodos relacionados con el Estado, Acción, Padres, Hijos, etc.
*/

#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include <algorithm>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

namespace dr {
	namespace mcts {

		template <class Estado, typename Accion>
		class Nodo {
			typedef std::shared_ptr< Nodo<Estado, Accion> > Ptr;

		public:
			//--------------------------------------------------------------
			Nodo(const Estado& estado, Nodo* padre = NULL) :
				estado(estado),
				accion(),
				padre(padre),
				agente_id(estado.datos.num_movi),
				num_visitas(0),
				valor(0),
				profundidad(padre ? padre->profundidad + 1 : 0)
			{
			}


			//--------------------------------------------------------------
			// expandir agregando un solo hijo
			Nodo* expandir()
			{
				srand(time(0));
				// verificacion para saber que el nodo actual no esta completamente expandido
				if (esta_expandido()) return NULL;

				// si esta es la primera expansión y aún no tenemos todas las acciones posibles
				if (acciones.empty()) {
					// recuperar lista de acciones del estado
					estado.ObtenerAcciones(acciones);

					// aleatorizar el orden
					std::random_shuffle(acciones.begin(), acciones.end());
				}
				// agregar la siguiente acción en el nodo niño
				return anadir_hijo_con_accion(acciones[hijos.size()]);
			}


			//--------------------------------------------------------------
			void actualizar(const float& premio)
			{
				this->valor += premio;
				num_visitas++;
			}


			//--------------------------------------------------------------
			// GETTERS
			// estado del nodo
			const Estado& get_estado() const { return estado; }

			// la acción que llevó a este estado
			const Accion& get_accion() const { return accion; }

			// si todos los hijos fueron expandidos y simulados completamente
			bool esta_expandido() const { return hijos.empty() == false && hijos.size() == acciones.size(); }

			// pregunta si este Nodo termina la búsqueda (es decir, el juego)
			bool es_el_final() const { return estado.datos.es_el_final; }

			// numero de veces que un nodo es visitado
			int get_num_visitas() const { return num_visitas; }

			// valor acumulado (veces ganado*)
			float get_valor() const { return valor; }

			// a que profundidad se haran las simulaciones
			int get_profundidad() const { return profundidad; }

			// numeri de hijos que tiene el nodo
			int get_num_hijos() const { return hijos.size(); }

			// obtener nodo hijo
			Nodo* get_hijo(int i) const { return hijos[i].get(); }

			// obtener nodo padre
			Nodo* get_padre() const { return padre; }

		private:
			Estado estado;			// el estado de este nodo
			Accion accion;			// la accion que llevo a este estado
			Nodo* padre;		// nodo padre del nodo actual
			int agente_id;			// agente que tomara la decición

			int num_visitas;			// numero de veces que un nodo a sido visitado
			float valor;			// valor del Nodo actual
			int profundidad;

			std::vector< Ptr > hijos;	// todos los hijos del nodo actual
			std::vector< Accion > acciones;			// posibles acciones para el estado actual


			//--------------------------------------------------------------
			// crear un clon del estado actual, aplicar la acción y agregarlo como hijo
			Nodo* anadir_hijo_con_accion(const Accion& nueva_accion)
			{
				// cree un nuevo Nodo con el mismo estado (se clonará) como este TreeNode
				Nodo* nodo_hijo = new Nodo(estado, this);

				// establecer la acción del niño para que sea la nueva acción
				nodo_hijo->accion = nueva_accion;

				// aplicar la nueva acción al estado del Nodo hijo
				nodo_hijo->estado.AplicarAccion(nueva_accion);

				// añadir a la lista de hijos
				hijos.push_back(Ptr(nodo_hijo));

				return nodo_hijo;
			}

		};

	}
}