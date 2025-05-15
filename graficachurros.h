

#ifndef graficachurros_h

#define graficachurros_h

#include <fstream>
#include <windowsx.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include <unordered_map>

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
    size_t get_mouse_position_x();
    size_t get_mouse_position_y();
    WORD get_window_height();
    WORD get_window_width();

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
        HRESULT create_graphic_resources();
        void discard_graphic_resources();
        HRESULT create_render_target();
        void flush_graphic_resources();
        HRESULT resize_canvas();
        HRESULT start_render();
        void discard_factory() const;
        void refresh() const;
        HRESULT draw();
        HRESULT new_solid_color_brush(
                const std::string &name,
                const D2D1_COLOR_F color,
                const D2D1_BRUSH_PROPERTIES &properties
            );
        HRESULT new_solid_color_brush(
                const std::string &name,
                D2D1_COLOR_F color
            );
        HRESULT new_solid_color_brush(
                const std::string &name,
                const std::string &color
            );
        HRESULT ellipse(
            const std::string &name,
            float x,
            float y,
            float x_radius,
            float y_radius
        );
        HRESULT rectangle(
                const std::string &name,
                float x,
                float y,
                float width,
                float height
            );
        ID2D1SolidColorBrush* solid_color_brush(
            const std::string& name
        );
        ID2D1Brush* brush(const std::string& name);
        void draw_line(
                ID2D1SolidColorBrush *solid_color_brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            ) const;
        void draw_line(
                ID2D1LinearGradientBrush *linear_gradient_brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            ) const;
        void draw_line(
                ID2D1RadialGradientBrush *radial_gradient_brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            ) const;
        void draw_line(
                ID2D1Brush *brush,
                float stroke_width,
                float start_x,
                float start_y,
                float end_x,
                float end_y
            ) const;
        void outline_geometry(
                const std::string &name,
                ID2D1SolidColorBrush *solid_color_brush,
                float stroke_width
            );

        void outline_geometry(
                const std::string &name,
                const std::string &brush,
                float stroke_width
            );

        void outline_geometry(
                const std::string &name,
                const std::string& brush
            );
        void fill_geometry(
                const std::string &name,
                ID2D1SolidColorBrush *solid_color_brush
            );
        void fill_geometry(
                const std::string &name,
                const std::string& brush
            );
    private:
        static D2D1::ColorF color_from_hex(
            const std::string & color
        );
        HRESULT new_ellipse(
                const std::string &name
            );
        HRESULT new_rectangle(
                const std::string &name
            );

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
}

#include "graficachurros.cpp"

#endif
