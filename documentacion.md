_Escrita por Andrade. Última vez actualizada el 15 de mayo de 2025._
> [!NOTE]
> La documentación está actualizada para GraficaChurros v0.0.2.

# Documentación
> [!TIP]
> _A pesar de la sección de [Guía de uso](#guía-de-uso) en la documentación, esta no describe todo lo que
> toda la librería es capaz de hacer, especialmente con futuras versiones de la librería.
> La documentación no es un tutorial, es sólo un libro de referencia, programar siempre es
> a base de prueba y error peleando contra la computadora. No olvide eso._
>
> _Si hay alguna duda sobre como se usa alguna característica o función de la librería, puede ponerse en
> contacto conmigo, aunque le recomiendo que primero revise la documentación antes de hacerlo. Es molesto
> estar resolviendo dudas que son fácilmente aclaradas citando la documentación, si son problemas de
> redacción, eso sí será culpa mía._
>
> \- Andrade.

## Índice
1. [Instalación](#instalación)
   1. [Pautas para instalar GraficaChurros.](#pautas-para-instalar-y-usar-graficachurros)
   2. [Agregar a un proyecto de Dev-C++.](#agregar-a-un-proyecto-de-dev-c)
   3. [Agregar a un proyecto de CLion (o construido con CMake).](#agregar-a-un-proyecto-de-clion-o-construido-con-cmake)
2. [Guía de uso.](#guía-de-uso)
   1. [Primeros pasos para usar GraficaChurros.](#primeros-pasos-para-usar-graficachurros)
3. [Referencia de uso.](#referencia-de-uso)
   1. [Funciones globales.](#funciones-globales)
      1. [Función `churro::get_window_height()`.](#función-churroget_window_height)
      2. [Función `churro::get_window_width()`.](#función-churroget_window_width)
      3. [Función `churro::get_mouse_position_x()`.](#función-churroget_window_position_x)
      4. [Función `churro::get_mouse_position_y()`.](#función-churroget_window_position_y)
      5. [Función `churro::mouse_is_on_screen()`.](#función-churromouse_is_on_screen)
   2. [Objeto global `churro::direct_2d`.](#objeto-global-churrodirect_2d)
      1. [Método `set_render_pipeline()`.](#método-set_render_pipeline)
      2. [Método `refresh()`.](#método-refresh)
      3. [Método `new_solid_color_brush()`.](#método-new_solid_color_brush)
      4. [Método `ellipse()`.](#método-ellipse)
      5. [Método `rectangle()`.](#método-rectangle)
      6. [Método `draw_line()`.](#método-draw_line)
      7. [Método `outline_geometry()`.](#método-outline_geometry)
      8. [Método `fill_geometry()`.](#método-fill_geometry)
   3. [Tipos de datos y directivas de librería.](#tipos-de-datos-y-directivas-de-librería)
      1. [Tipo de función `churro::RENDER_PIPELINE`.](#tipo-de-función-churrorender_pipeline)
      2. [Directiva de librería `WINDOW_MIN_X`.](#directiva-de-librería-window_min_x)
      3. [Directiva de librería `WINDOW_MIN_Y`.](#directiva-de-librería-window_min_y)
      4. [Directiva de librería `WINDOW_NAME`.](#directiva-de-librería-window_name)

## Instalación.

### Pautas para instalar y usar GraficaChurros.
GraficaChurros es una librería que funciona gracias a la API de Direct2D, lo que permite un renderizado de imagen rápido
y eficiente para aplicaciones.

Los archivos de la librería se _incluyen en línea_ (_in-line included_), por lo que no deben de incluirse los archivos
de la librería a la compilación del proyecto, en cambio, sólamente use la directiva de compilador `#include` para incluir
el archivo de encabecera. El motivo de usar este método _arcáico_ es para hacer la librería lo más general posible ante distintos
compiladores y flujos de trabajos compiladores.

Además de esto, la librería requiere que el compilador y las librerías estándar estén actualizadas hasta _al menos_ el estándar ISO C++11.


### Agregar a un proyecto de Dev-C++.
1. Descargue los archivos de la librería desde [Lanzamientos](https://github.com/andrade379166/grafica-churros/releases),
   siempre es recomendable descargar la última versión de la librería.
2. Abra el proyecto al que quiera agregar la librería.
3. Agregue los archivos de la librería a la carpeta del proyecto.
4. Agregue los archivos de la librería al proyecto:
   1. Abra **Proyecto > Agregar a proyecto...** y seleccione los archivos `graficachurros.h` y `graficachurros.cpp`.
5. Agregue la siguiente línea al encabezado del archivo `main.cpp`:
   
   ```cpp
   #include "graficachurros.h"
   ```
   
6. Asegure que el compilador use el estándar de C++ 11:
   * Abra **Proyecto  > Opciones del Proyecto > Compilador > Generación de Código > Estándar del Lenguaje**
     y cambie la configuración a _ISO C++11_.
7. Asegure que el compilador no incluya el archivo `graficachurros.cpp` de la librería a través del compilador:
   1. Abra **Proyecto  > Opciones del Proyecto > Archivos**.
   2. Si realizó el paso 4 correctamente, en la lista aparecerán enlistados los archivos `graficachurros.h` y `graficachurros.cpp`.
   3. Seleccione el archivo `graficachurros.cpp` y marque vacías las casillas de _Incluir en compilación_ e _Incluir en vinculación_.



### Agregar a un proyecto de CLion (o construido con CMake).
> [!TIP]
> _[CLion](https://www.jetbrains.com/clion/) es el IDE de mi preferencia para desarrollar con C y C++ dado a su fácil
> instalación y uso, además de incluir análisis de código en tiempo real para encontrar rápidamente problemas en el código._
>
> _De hecho, esta librería fue escrita usando CLion e incluye documentación rápida embedida._
> 
> \- Andrade.

1. Descargue los archivos de la librería desde [Lanzamientos](https://github.com/andrade379166/grafica-churros/releases),
   siempre es recomendable descargar la última versión de la librería.
3. Abra el proyecto al que quiera agregar la librería.
4. Agregue los archivos de la librería a la carpeta del proyecto.
5. Agregue la siguiente línea al encabezado del archivo `main.cpp`:
   
   ```cpp
   #include "graficachurros.h"
   ```

6. Configure el archivo `CMakeList.txt` de su proyecto:
   1. Abra el archivo `CMakeList.txt` de su proyecto.
   2. Asegúrese que sigua una estructura similar a el siguiente esquema:
      
      ```cmake
      cmake_minimum_required(VERSION 3.30)     # Versión de CMake mínima para compilar.
      project(proyecto)                        # Nombre del proyecto.
      set(CMAKE_CXX_STANDARD 11)               # Estándar de C++ a usar, asegúrese que sea C++ 11 o superior.
      add_executable(proyecto main.cpp)        # Asegúrese que no se incluya "graficachurros.cpp" como ejecutable.
      target_link_libraries(proyecto d2d1)     # Agregue la librería d2d1 al proyecto.
      ```

----------------------------------------------------------------------------------------------------------

## Guía de uso.

### Primeros pasos para usar GraficaChurros.
Tras instalar la librería, use el siguiente ejemplo de código en `main.cpp` para probar que la librería esté instalada correctamente:
```cpp

#include "graficachurros.h"

int main() {
    churro::direct_2d.set_render_pipeline(&ejemplo);
    churro::direct_2d.refresh();
    return 0;
}

churro::RENDER_PIPELINE ejemplo() {
    direct_2d.new_solid_color_brush("rojo", "#FF0000");

    const int tamano_ventana[2] = {
        churro::get_window_width(),
        churro::get_window_height()
    };

//  Se declara un rectángulo que tenga el tamaño del 60% del tamaño de la ventana.
    churro::direct_2d.rectangle(
        "rectángulo",
        tamano_ventana[0] * (0.5f - 0.3f),
        tamano_ventana[0] * (0.5f - 0.3f),
        window[0] * 0.6f,      // Fija el ancho del rectángulo al 60% del ancho de la ventana.
        window[1] * 0.6f       // Fija el ancho del rectángulo al 60% de la altura de la ventana.
    );

    churro::direct_2d.fill_geometry(
        "rectángulo",
        "rojo"
    );
}
```

Si la librería se instaló correctamente, el código anterior debería compilar correctamente y debería mostrar una ventana con
un rectángulo que se ajusta al tamaño de la ventana. Intente seguir las descripciones de los comentarios en el código anterior
y usando la sección [Referencia de uso](#referencia-de-uso).

> [!IMPORTANT]
> Próximamente habrá una guía más detallada sobre cómo se usa, esto

----------------------------------------------------------------------------------------------------------

## Referencia de uso.
Aquí se enlistan y describen todas las funciones, métodos y tipos de dados que implementa la librería.
> [!NOTE]
> Todos los métodos y funciones que no se describan a continuación son funciones internas de la librería,
> estas pueden distinguirse fácilmente dado a que inician con el símbolo de la barra baja `_`.

> [!WARNING]
> El uso deliberado de funciones internas de la librería llevará a errores.

----------------------------------------------------------------------------------------------------------

### Funciones globales.

----------------------------------------------------------------------------------------------------------

#### Función `churro::get_window_height()`.
Devuelve la altura en píxeles de la ventana como un número entero (`int`).

```cpp
churro::get_window_height()
```

----------------------------------------------------------------------------------------------------------

#### Función `churro::get_window_width()`.
Devuelve el largo en píxeles de la ventana como un número entero (`int`).

```cpp
churro::get_window_height()
```

----------------------------------------------------------------------------------------------------------

#### Función `churro::get_mouse_position_x()`.
Devuelve la coordenada $x$ en píxeles del ratón en la ventana (`int`).

```cpp
churro::get_mouse_position_x()
```

> [!NOTE]
> Si el ratón no se encuentra en la ventana, devolverá la última posición en la que estaba.

----------------------------------------------------------------------------------------------------------

#### Función `churro::get_mouse_position_y()`.
Devuelve la coordenada $y$ en píxeles del ratón en la ventana (`int`).

```cpp
churro::get_mouse_position_y()
```

> [!NOTE]
> Si el ratón no se encuentra en la ventana, devolverá la última posición en la que estaba.

----------------------------------------------------------------------------------------------------------

#### Función `churro::mouse_is_on_screen()`.
Devuelve `true` si el ratón se encuentra en ventana, en caso contrario, devolve `false`.
```cpp
churro::mouse_is_on_screen()
```

----------------------------------------------------------------------------------------------------------

### Objeto global `churro::direct_2d`.
El objeto global `churro::direct_2d` es la interfaz por la que el programa se comunica con la librería para
renderizar en la ventana.

> [!WARNING]
> * No sobreescriba este objeto manualmente, podría llevar a errores inesperados.
> * No cree otro objeto con la clase `churro:direct2D`. La ventana solamente se comunicará con el objeto
>   llamado `direct_2d`.

----------------------------------------------------------------------------------------------------------

#### Método `set_render_pipeline()`.
Cambia la pipa de renderizado a una nueva función de tipo [`RENDER_PIPELINE`](#churrorender_pipeline).

```cpp
churro::direct_2d.set_render_pipeline(<función>)
```

Parámetros:
  * `<función>`: dirección de memoria a una función de tipo `churro::RENDER_PIPELINE` que no recibe
    parámetro alguno.

> [!NOTE]
> * Solamente cambia la pipa de renderizado, no manda solicitud para redibujar la ventana.

----------------------------------------------------------------------------------------------------------

#### Método `refresh()`.
Manda una solicitud a la ventana para que sea redibujada.

```cpp
churro::direct_2d.refresh()
```

> [!NOTE]
> * Esta función siempre es llamada cuando el tamaño de la ventana cambia.
> * Si se manda una solicitud para redibujar la ventana cuando esta aún no se ha terminado de dibujar, se ignorará la solicitud.

----------------------------------------------------------------------------------------------------------

#### Método `new_solid_color_brush()`.
Crea una brocha con un color sólido usando un código hexadecimal.

```cpp
churro::direct_2d.new_solid_color_brush(<nombre>, <color>)
```

Parámetros:
  * (Cadena) `<nombre>`: identificador de la brocha a crear.
  * (Cadena) `<color>`: código hexadecimal del color de la brocha usando el formato `#RRGGBB` para colores sólidos
    o `#RRGGBBAA` para colores con transparencia.

> [!NOTE]
> * Las brochas son recursos gráficos estáticos, por lo tanto, no se pueden cambiar sus propiedades una vez creadas.

----------------------------------------------------------------------------------------------------------

#### Método `ellipse()`.
Crea o modifica una elipse dado un punto central y su radio horizontal y vertical.

```cpp
churro::direct_2d.ellipse(<nombre>, <x>, <y>, <radio horizontal>, <radio vertical>)
```

Parámetros:
  * (Cadena) `<nombre>`: identificador de la elipse a crear o modificar.
  * (Flotante) `<x>`: coordenada $x$ en píxeles del punto central de la elipse.
  * (Flotante) `<y>`: coordenada $y$ en píxeles del punto central de la elipse.
  * (Flotante) `<radio horizontal>`: radio horizontal en píxeles de la elipse.
  * (Flotante) `<radio vertical>`: radio vertical en píxeles de la elipse.

----------------------------------------------------------------------------------------------------------

#### Método `rectangle()`.
Crea o modifica un rectángulo desde la esquina superior izquierda hacia la derecha y abajo.

```cpp
churro::direct_2d.rectangle(<nombre>, <x>, <y>, <ancho>, <alto>)
```

Parámetros:
  * (Cadena) `<nombre>`: identificador del rectángulo a crear o modificar.
  * (Flotante) `<x>`: coordenada $x$ en píxeles de la esquina superior izquierda del rectángulo.
  * (Flotante) `<y>`: coordenada $y$ en píxeles de la esquina superior izquierda del rectángulo.
  * (Flotante) `<ancho>`: anchura en píxeles del rectángulo.
  * (Flotante) `<alto>`: altura en píxeles del rectángulo.

----------------------------------------------------------------------------------------------------------

#### Método `draw_line()`.
Dibuja una línea recta desde un punto a otro usando una brocha previamente declarada.

```cpp
churro::direct_2d.draw_line(<brocha>, <grosor>, <inicio x>, <inicio y>, <final x>, <final y>)
```

Parámetros:
  * (Cadena) `<nombre>`: identificador de la brocha a usar.
  * (Flotante) `<grosor>`: grosor en píxeles de la línea.
  * (Flotante) `<inicio x>`: coordenada $x$ en píxeles del punto inicial de la línea.
  * (Flotante) `<inicio y>`: coordenada $y$ en píxeles del punto inicial de la línea.
  * (Flotante) `<final x>`: coordenada $x$ en píxeles del punto final de la línea.
  * (Flotante) `<final y>`: coordenada $y$ en píxeles del punto final de la línea.

----------------------------------------------------------------------------------------------------------

### Tipos de datos y directivas de librería.

----------------------------------------------------------------------------------------------------------

#### Tipo de función `churro::RENDER_PIPELINE`.
Tipo de función usada para declarar funciones para renderizar.

Ejemplo dado:
```cpp
churro:RENDER_PIPELINE function() {...}
```

> [!NOTE]
> Aunque internamente no hay diferencia entre funciones de tipo `void` y de tipo `churro::RENDER_PIPELINE`,
> es preferido usar el tipo de función correcto para hacer código más claro.

----------------------------------------------------------------------------------------------------------

#### Directiva de librería `WINDOW_MIN_X`.
Directiva en forma de macro para definir el ancho mínimo de la ventana. Si no es especificada, el valor por
defecto es 300

```cpp
#define WINDOW_MIN_X <ancho>
```

Parámetros:
  * (Entero) `<ancho>`: ancho mínimo en píxeles de la ventana.

>[!WARNING]
> Todas las directivas de librería tienen que declararse antes de incluir el archo cabecera de la librería.

----------------------------------------------------------------------------------------------------------

#### Directiva de librería `WINDOW_MIN_Y`.
Directiva en forma de macro para definir la altura mínima de la ventana. Si no es especificada, el valor por
defecto es 200.

```cpp
#define WINDOW_MIN_Y <altura>
```

Parámetros:
  * (Entero) `<altura>`: altura mínima en píxeles de la ventana.

> [!WARNING]
> Todas las directivas de librería tienen que declararse antes de incluir el archivo encabecera de la librería.

----------------------------------------------------------------------------------------------------------

#### Directiva de librería `WINDOW_NAME`.
Directiva en forma de macro para definir la altura mínima de la ventana. Si no es especificada, el valor por
defecto es 200.

```cpp
#define WINDOW_MIN_Y <nombre>
```

Parámetros:
  * (Cadena) `<nombre>`: nombre a mostrar para la ventana.

> [!WARNING]
> Todas las directivas de librería tienen que declararse antes de incluir el archivo encabecera de la librería.

----------------------------------------------------------------------------------------------------------
