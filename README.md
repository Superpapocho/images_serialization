# images_serialization
**Integrantes**: Valerie Andrea Heller Ortega y Juan Andrés Russy Cervera

Se utiliza una tabla hash para facilitar la manipulación de grandes cantidades de imágenes. Entre las funcionalidades del código se encuentra añadir texto, figuras, recortar, rotar y serializar la información al interior de una imagen.

# Librerias
- OpenCV: Permite la manipulación de imagenes de manera matricial.
- Otras: String, iostream, vector, fstream.

# Carpetas
- *Resources/*: Contiene las imágenes que se utilizan como prueba de las funciones en el script *Test.cpp*.
- *Output/*: Carpeta que contiene los outputs que van siendo generados como prueba de las funciones en el script *Test.cpp*.

# Estructuras
- Hashmap: Tabla hash que contiene listas enlazadas de la estructura NodeImg.
- NodeImg: Guarda la información matricial de la imagen, el título de la misma y el siguiente objeto NodeImg de la lista enlazada.

# Functions
Todas las funciones que desarrollamos pueden ser implementadas tanto para una sola imagen a través de su título, a todas las imagenes que cumplan cierta regla hash o a todas las imagenes dentro de la estructura.

- hash(string key)
- add_image : Agregar imagenes o cambiar las ya existentes (a través de su título).
- remove: Elimina imagenes a partir de su título.
- Mostrar imágenes.
- Cambiar la resolucion de las imágenes.
- Recortar imágenes.
- Rotar imágenes.
- Anotar sobre las imágenes
