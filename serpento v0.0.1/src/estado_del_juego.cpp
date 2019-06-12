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
	//TODO Realizar acción
	// Verificar que la accion este dentro del rango de acciones (0-2)

	// Mover a la serpiente

	// Verificar si la serpiente esta comiendo al realizar esta acción
}

void snake::Estado::ObtenerAcciones(std::vector<Accion>& acciones)
{
	acciones.clear();

	//TODO Obtener acciones posibles (pushback)
	// si cualquiera de las casillas vecinas a la cabeza de la serpiente esta vacia o es comida es una accion valida
	
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
	return 0.0f;
}

//----------------------------------------------------------------------------
// ESPECIFICOS DEL JUEGO:
//----------------------------------------------------------------------------

void snake::Estado::reubicar_comida()
{
}

void snake::Estado::reiniciar()
{
	datos.es_el_final = false;
	datos.comio_fruta = false;
	datos.longitud = 1;
	datos.dir = DERECHA;
	datos.puntaje = 0;
	datos.num_movi = 0;
	reubicar_comida();
}

void snake::Estado::dibujar()
{
}

void snake::Estado::dibujar_serpiente()
{
}

void snake::Estado::dibujar_alimento()
{
}
