

#ifndef graficachurros_h


#define graficachurros_h

#include <fstream>
#include <windowsx.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include <unordered_map>
#include <utility>

#define main __main__

#ifndef WINDOW_NAME
#define WINDOW_NAME "GraficaChurros"
#endif
#ifndef WINDOW_MIN_X
#define WINDOW_MIN_X 300
#endif
#ifndef WINDOW_MIN_Y
#define WINDOW_MIN_Y 200
#endif

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
template <class T> void SafeRelease(T *pointer);
VOID main_thread(PVOID pvoid);
int main();

namespace churro {
    typedef void RENDER_PIPELINE;
    bool mouse_is_on_screen();
    int get_mouse_position_x();
    int get_mouse_position_y();
    int get_window_height();
    int get_window_width();

    class direct2D {
        typedef struct {
            bool device_independent;
            enum {
                solid_color_brush,
                bitmap,
                brush,
                linear_brush,
                radial_brush,
                stroke_style,
                ellipse,
                rectangle,
                text
            } type;
            union {
                ID2D1SolidColorBrush *solid_color_brush;
                ID2D1Bitmap *bitmap;
                ID2D1Brush *brush;
                ID2D1LinearGradientBrush *linear_brush;
                ID2D1RadialGradientBrush *radial_brush;
                ID2D1StrokeStyle *stroke_style;
                D2D1_ELLIPSE* ellipse;
                D2D1_RECT_F* rectangle;
            } pointer;
        } GRAPHIC_RESOURCE;

    public:
        void set_render_pipeline(RENDER_PIPELINE (*function)());
        void refresh() const;

        /*
        Crea una brocha con un color sólido usando un código hexadecimal.

        ARGUMENTOS:
            [name]:         Identificador de la brocha a crear.
            [color]:        Cadena con el código hexadecimal de un color.
         */
        HRESULT new_solid_color_brush(
                const std::string &name,
                const std::string &color
            );

        /*
        Crea o modifica una elipse dado un punto central y su radio horizontal y vertical

        ARGUMENTOS:
            [name]:         Identificador de la elipse a crear o modificar.
            [x]:            Coordenada X del punto central de la elipse.
            [y]:            Coordenada Y del punto central de la elipse.
            [x_radius]:     Radio vertical de la elipse.
            [y_radius]:     Radio horizontal de la elipse.
         */
        HRESULT ellipse(
            const std::string &name,
            float x,
            float y,
            float x_radius,
            float y_radius
        );

        /*
        Crea o modifica un rectángulo desde la esquina superior izquierda hacia abajo.

        ARGUMENTOS:
            [name]:         Identificador del rectángulo a crear o modificar.
            [x]:            Coordenada X de la esquina inicial del rectángulo.
            [y]:            Coordenada Y de la esquina inicial del rectángulo.
            [width]:        Largo del rectángulo.
            [height]:       Altura del rectángulo.
         */
        HRESULT rectangle(
                const std::string &name,
                float x,
                float y,
                float width,
                float height
            );

//         /*
//         Dibuja una línea recta desde un punto a otro usando una brocha
//         previamente declarada.
//
//         ARGUMENTOS:
//             [brush]:        Identificador de la brocha a usar.
//             [stroke_width]: Grosor de la línea.
//             [start_x]:      Coordenada X del punto inicial de la línea.
//             [start_y]:      Coordenada Y del punto inicial de la línea.
//             [end_x]:        Coordenada X del punto final de la línea.
//             [end_y]:        Coordenada Y del punto final de la línea.
//          */
        void draw_line(
                const std::string& brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            );

        /*
        Delinea una figura geométrica previamente declarada.

        ARGUMENTOS:
            [name]:         Identificador de la figura geométrica.
            [brush]:        Identificador de la brocha.
            [stroke_width]: Anchura de la linea de delineado.
         */
        void outline_geometry(
                const std::string& name,
                const std::string& brush,
                float stroke_width
            );

        /*
        Delinea una figura geométrica previamente declarada usando implicita-
        mente una anchura de 1 pixel.

        ARGUMENTOS:
            [name]:     Identificador de la figura geométrica.
            [brush]:    Identificador de la brocha.
         */
        void outline_geometry(
                const std::string& name,
                const std::string& brush
            );

        /*
        Rellena una figura geométrica previamente declarada usando una brocha
        previamente declarada.

        ARGUMENTOS:
            [name]:     Identificador de la figura geométrica.
            [brush]:    Identificador de la brocha.
        */
        void fill_geometry(
                const std::string& name,
                const std::string& brush
            );

        void _discard_graphic_resources();
        HRESULT _create_render_target();
        void _flush_graphic_resources();
        HRESULT _resize_canvas();
        HRESULT _start_render();
        void _discard_factory() const;
        HRESULT _draw();

    private:
        void _draw_line(
                ID2D1Brush *brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            ) const;
        static D2D1::ColorF _color_from_hex(
            const std::string & color
        );
        HRESULT _new_ellipse(
                const std::string &name
            );
        HRESULT _new_rectangle(
                const std::string &name
            );

        HRESULT _create_graphic_resources();
        ID2D1Brush* _brush(const std::string& name);

        struct {
            WORD width = 0;
            WORD height = 0;
        } previous_size;

        std::unordered_map<std::string, GRAPHIC_RESOURCE> graphic_resource_map;
        RENDER_PIPELINE (*render_pipeline)() = nullptr;
        ID2D1HwndRenderTarget* render_target = nullptr;
        bool flush_graphics_resource_pool = false;
        bool render_pipeline_task_done = true;
        ID2D1Factory1* factory = nullptr;
    } direct_2d;

    class exception final : public std::exception {
    public:
        explicit exception(std::string  message) : message_(std::move(message)) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    struct {
        WORD height;
        WORD width;
        bool mouse_inside;
        struct {
            int pos[2];
            bool right_click;
            bool left_click;
        } mouse;
    } /* -----------------------------------------------------> */ window_properties;


    char                    window_class_name[] = "graficachurros";
    HINSTANCE               instance;
    HWND                    main_window;
    bool                    mouse_inside;
}

#include "graficachurros.cpp"

#endif
