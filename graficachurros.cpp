


/*
 *  ====================================================================
 *
 *  GraficaChurros: Utilidad para graficar churros (y algunas cosas más)
 *
 *  ====================================================================
 */

#include "graficachurros.h"

D2D1_FACTORY_OPTIONS factory_options = {
    .debugLevel = D2D1_DEBUG_LEVEL_NONE
};


template <class T> void SafeRelease(T *pointer) {
    if (pointer != nullptr) {
        pointer->Release();
        pointer = nullptr;
    }
}







bool churro::mouse_is_on_screen() {
    return mouse_inside;
}

int churro::get_mouse_position_x() {
    return window_properties.mouse.pos[0];
}

int churro::get_mouse_position_y() {
    return window_properties.mouse.pos[1];
}

int churro::get_window_height() {
    return window_properties.height;
}

int churro::get_window_width() {
    return window_properties.width;
}


HRESULT churro::direct2D::_start_render() {
    HRESULT result = S_OK;

    if (factory == nullptr) {
        result = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            __uuidof(ID2D1Factory1),
            &factory_options,
            reinterpret_cast<void**>(&factory)
        );
    }
    if (FAILED(result)) return result;
    return result;
}

void churro::direct2D::refresh() const {
    if (render_pipeline_task_done == false) return;
    InvalidateRect(main_window, nullptr, TRUE);
}

HRESULT churro::direct2D::_resize_canvas() {
    HRESULT result = S_OK;

    if (render_target == nullptr)
        return result;



    if (
            previous_size.width == window_properties.width && previous_size.height == window_properties.height
        )
        return result;

    previous_size.width = window_properties.width;
    previous_size.height = window_properties.height;

    render_pipeline_task_done = false;
    RECT rectangle;
    GetClientRect(main_window, &rectangle);
    const D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);
    result = render_target->Resize(size);
    render_pipeline_task_done = true;
    refresh();

    return result;
}

HRESULT churro::direct2D::_create_render_target() {
    HRESULT result = S_OK;
    if (render_target == nullptr) {
        RECT rc;
        GetClientRect(main_window, &rc);

        const D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );
        result = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(main_window, size),
            &render_target
        );
    }
    return result;
}

HRESULT churro::direct2D::_create_graphic_resources() {
    HRESULT result = S_OK;
    result = _create_render_target();
    if (FAILED(result)) return result;
    return result;
}

void churro::direct2D::_flush_graphic_resources() {
    for (auto &list : graphic_resource_map) {
        const auto element = &list.second;
        switch (element->type) {
            case GRAPHIC_RESOURCE::solid_color_brush: {
                free(element->pointer.solid_color_brush);
                element->pointer.solid_color_brush = nullptr;
            }
            case GRAPHIC_RESOURCE::ellipse: {
                delete element->pointer.ellipse;
                element->pointer.ellipse = nullptr;
                break;
            }
            case GRAPHIC_RESOURCE::bitmap: {
                break;
            }
            case GRAPHIC_RESOURCE::brush: {
                break;
            }
            case GRAPHIC_RESOURCE::linear_brush: {
                break;
            }
            case GRAPHIC_RESOURCE::radial_brush: {
                break;
            }
            case GRAPHIC_RESOURCE::stroke_style: {
                break;
            }
            case GRAPHIC_RESOURCE::rectangle: {
                break;
            }
            case GRAPHIC_RESOURCE::text: {
                break;
            }
            default:{
                break;
            }
        }
    }
    graphic_resource_map.clear();
}

void churro::direct2D::_discard_graphic_resources() {
    _flush_graphic_resources();
    SafeRelease(render_target);
}

void churro::direct2D::set_render_pipeline(RENDER_PIPELINE (*function)()) {
    flush_graphics_resource_pool = true;
    render_pipeline = function;
}

HRESULT churro::direct2D::_draw() {
    HRESULT result = S_OK;

    if (flush_graphics_resource_pool == true)
        _flush_graphic_resources();

    flush_graphics_resource_pool = false;

    result = _create_graphic_resources();
    if (FAILED(result)) {
        render_pipeline_task_done = true;
        return result;
    }

    result = _resize_canvas();
    if (FAILED(result)) {
        render_pipeline_task_done = true;
        return result;
    }


    render_pipeline_task_done = false;

    PAINTSTRUCT paint_structure;
    BeginPaint(main_window, &paint_structure);

    render_target->BeginDraw();
    {
        render_target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        render_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

        if (render_pipeline != nullptr) render_pipeline();
    }
    result = render_target->EndDraw();


    if (FAILED(result) || result == D2DERR_RECREATE_TARGET) {
        _discard_graphic_resources();
    }

    EndPaint(main_window, &paint_structure);
    render_pipeline_task_done = true;

    return result;
}

void churro::direct2D::_discard_factory() const {
    SafeRelease(factory);
}

D2D1::ColorF churro::direct2D::_color_from_hex(const std::string & color) {
    if (!(color.size() == 7 || color.size() == 9)) throw std::invalid_argument("\"" +color + "\" is an invalid hexadecimal color");
    if (color[0] != '#') throw std::invalid_argument("Mising hexadecimal hashtag at start");

    size_t num = 0;
    for (size_t i = 1; i < color.size(); ++i) {
        num <<= 4;
        switch (color[i]) {
            case '0':               break;
            case '1':   num += 1;   break;
            case '2':   num += 2;   break;
            case '3':   num += 3;   break;
            case '4':   num += 4;   break;
            case '5':   num += 5;   break;
            case '6':   num += 6;   break;
            case '7':   num += 7;   break;
            case '8':   num += 8;   break;
            case '9':   num += 9;   break;
            case 'a':
            case 'A':   num += 10;  break;
            case 'b':
            case 'B':   num += 11;  break;
            case 'c':
            case 'C':   num += 12;  break;
            case 'd':
            case 'D':   num += 13;  break;
            case 'e':
            case 'E':   num += 14;  break;
            case 'f':
            case 'F':   num += 15;  break;
            default:    throw std::invalid_argument("Invalid hexadecimal characters");
        }
    }

    if (color.size() == 7) {
        float rgb_color[] = {
            static_cast<float>(num >> 16),
            static_cast<float>((num >> 8) % 256),
            static_cast<float>(num % 256)
        };
        rgb_color[0] /= 255.0f;
        rgb_color[1] /= 255.0f;
        rgb_color[2] /= 255.0f;


        return {rgb_color[0], rgb_color[1], rgb_color[2]};
    }
    float rgb_color[] = {
        static_cast<float>( num >> 24      ),
        static_cast<float>( num >> 16 % 256),
        static_cast<float>((num >> 8) % 256),
        static_cast<float>( num       % 256)
    };
    rgb_color[0] /= 255.0f;
    rgb_color[1] /= 255.0f;
    rgb_color[2] /= 255.0f;
    rgb_color[3] /= 255.0f;


    return {rgb_color[0], rgb_color[1], rgb_color[2], rgb_color[3]};
}

HRESULT churro::direct2D::new_solid_color_brush(
        const std::string &name,
        const std::string &color
    ) {
    HRESULT result = S_OK;

    if (graphic_resource_map.find(name) != graphic_resource_map.end()) return result;


    GRAPHIC_RESOURCE new_brush;
    result = render_target->CreateSolidColorBrush(
        _color_from_hex(color),
        &new_brush.pointer.solid_color_brush
    );

    if (FAILED(result)) return result;

    new_brush.device_independent = false;
    new_brush.type = GRAPHIC_RESOURCE::solid_color_brush;

    graphic_resource_map[name] = new_brush;
    return result;
}

HRESULT churro::direct2D::ellipse(
        const std::string &name,
        const float x,
        const float y,
        const float x_radius,
        const float y_radius
    ) {

    HRESULT result = S_OK;
    if (graphic_resource_map.find(name) == graphic_resource_map.end())
        result = _new_ellipse(name);

    if (graphic_resource_map[name].type != GRAPHIC_RESOURCE::ellipse) return S_FALSE;

    const auto new_ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), x_radius, y_radius);

    *graphic_resource_map[name].pointer.ellipse = new_ellipse;

    return result;

}

HRESULT churro::direct2D::rectangle(
        const std::string &name,
        const float x,
        const float y,
        const float width,
        const float height
    ) {

    HRESULT result = S_OK;
    if (graphic_resource_map.find(name) == graphic_resource_map.end())
        result = _new_rectangle(name);
    if (graphic_resource_map[name].type != GRAPHIC_RESOURCE::rectangle) return S_FALSE;
    const auto new_rectangle = D2D1::RectF(x, y, x + width, y + height);

    *graphic_resource_map[name].pointer.rectangle = new_rectangle;

    return result;

}


ID2D1Brush* churro::direct2D::_brush(const std::string& name) {
    if (graphic_resource_map.find(name) == graphic_resource_map.end())
        throw std::invalid_argument("No graphic resource named \"" + name + "\" was found");
    switch (graphic_resource_map[name].type) {
        case GRAPHIC_RESOURCE::solid_color_brush:{
            return graphic_resource_map[name].pointer.solid_color_brush;
        }
        case GRAPHIC_RESOURCE::linear_brush:{
            return graphic_resource_map[name].pointer.linear_brush;
        }
        case GRAPHIC_RESOURCE::radial_brush:{
            return graphic_resource_map[name].pointer.radial_brush;
        }
        case GRAPHIC_RESOURCE::brush: {
            return graphic_resource_map[name].pointer.brush;
        }
        default: {
            throw std::invalid_argument("The graphic resource \"" + name + "\" is not a brush");
        }
    }
}


void churro::direct2D::_draw_line(
        ID2D1Brush *brush,
        const float stroke_width,
        const float start_x,
        const float start_y,
        const float end_x,
        const float end_y
    ) const {
    render_target->DrawLine(
        D2D1::Point2F(start_x, start_y),
        D2D1::Point2F(end_x,end_y),
        brush, stroke_width
    );
}


void churro::direct2D::outline_geometry(
        const std::string &name,
        const std::string& brush,
        const float stroke_width
    ) {
    if (graphic_resource_map.find(name) == graphic_resource_map.end())
        throw std::runtime_error("Graphic resource \"" + name + "\" not found.");

    ID2D1Brush* brush_to_use = _brush(brush);

    switch (graphic_resource_map[name].type) {
        case GRAPHIC_RESOURCE::rectangle: {
            render_target->DrawRectangle(graphic_resource_map[name].pointer.rectangle, brush_to_use, stroke_width);
            break;
        }
        case GRAPHIC_RESOURCE::ellipse: {
            render_target->DrawEllipse(graphic_resource_map[name].pointer.ellipse, brush_to_use, stroke_width);
            break;
        }
        case GRAPHIC_RESOURCE::bitmap: {}

        case GRAPHIC_RESOURCE::brush:
        case GRAPHIC_RESOURCE::radial_brush:
        case GRAPHIC_RESOURCE::stroke_style:
        case GRAPHIC_RESOURCE::linear_brush:
        case GRAPHIC_RESOURCE::solid_color_brush:
        default: {
            throw std::runtime_error("Brush graphic resource \"" + name + "\" is not a geometric figure.");
        }
    }

}
void churro::direct2D::outline_geometry(
        const std::string &name,
        const std::string& brush
    ) {
    if (graphic_resource_map.find(name) == graphic_resource_map.end())
        throw std::runtime_error("Graphic resource \"" + name + "\" not found.");

    ID2D1Brush* brush_to_use = _brush(brush);

    switch (graphic_resource_map[name].type) {
        case GRAPHIC_RESOURCE::rectangle: {
            render_target->DrawRectangle(graphic_resource_map[name].pointer.rectangle, brush_to_use);
            break;
        }
        case GRAPHIC_RESOURCE::ellipse: {
            render_target->DrawEllipse(graphic_resource_map[name].pointer.ellipse, brush_to_use);
            break;
        }
        case GRAPHIC_RESOURCE::bitmap:
        case GRAPHIC_RESOURCE::brush:
        case GRAPHIC_RESOURCE::radial_brush:
        case GRAPHIC_RESOURCE::stroke_style:
        case GRAPHIC_RESOURCE::linear_brush:
        case GRAPHIC_RESOURCE::solid_color_brush:
        default:
            throw std::runtime_error("The graphic resource \"" + name + "\" is not a geometric figure");
    }
}


void churro::direct2D::fill_geometry(const std::string &name, const std::string& brush) {

    ID2D1Brush* brush_to_use = direct2D::_brush(brush);

    switch (graphic_resource_map[name].type) {
        case GRAPHIC_RESOURCE::rectangle: {
            render_target->FillRectangle(graphic_resource_map[name].pointer.rectangle, brush_to_use);
            break;
        }
        case GRAPHIC_RESOURCE::ellipse: {
            render_target->FillEllipse(graphic_resource_map[name].pointer.ellipse, brush_to_use);
            break;
        }
        case GRAPHIC_RESOURCE::bitmap:
        case GRAPHIC_RESOURCE::brush:
        case GRAPHIC_RESOURCE::radial_brush:
        case GRAPHIC_RESOURCE::stroke_style:
        case GRAPHIC_RESOURCE::linear_brush:
        case GRAPHIC_RESOURCE::solid_color_brush:
        default:
            throw std::runtime_error("The graphic resource \"" + name + "\" used is not a geometric figure");
    }
}

HRESULT churro::direct2D::_new_ellipse(
        const std::string &name
    ) {
    constexpr HRESULT result = S_OK;
    if (graphic_resource_map.find(name) != graphic_resource_map.end()) return result;

    GRAPHIC_RESOURCE ellipse_resource;

    ellipse_resource.type = GRAPHIC_RESOURCE::ellipse;
    ellipse_resource.device_independent = true;
    ellipse_resource.pointer.ellipse = new D2D1_ELLIPSE;

    graphic_resource_map[name] = ellipse_resource;
    return result;
}

HRESULT churro::direct2D::_new_rectangle(
        const std::string &name
    ) {
    constexpr HRESULT result = S_OK;
    if (graphic_resource_map.find(name) != graphic_resource_map.end()) return result;

    GRAPHIC_RESOURCE rectangle_resource;

    rectangle_resource.type = GRAPHIC_RESOURCE::rectangle;
    rectangle_resource.device_independent = true;
    rectangle_resource.pointer.rectangle = new D2D1_RECT_F;

    graphic_resource_map[name] = rectangle_resource;
    return result;
}

void churro::direct2D::draw_line(
        const std::string& brush,
        const float stroke_width,
        const float start_x,
        const float start_y,
        const float end_x,
        const float end_y
    ) {
    render_target->DrawLine(
        D2D1::Point2F(start_x, start_y),
        D2D1::Point2F(end_x,end_y),
        _brush(brush), stroke_width
    );
}


int WINAPI WinMain (
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd
    ) {
    using namespace churro;
    instance = hInstance;

    static WNDCLASSEX /* =========================> */ window_class;

    window_class.hIconSm = LoadIcon (nullptr, IDI_APPLICATION);
    window_class.hIcon = LoadIcon (nullptr, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.hbrBackground = nullptr;
    window_class.lpszClassName = window_class_name;
    window_class.lpfnWndProc = WindowProcedure;
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.hInstance = instance;
    window_class.lpszMenuName = nullptr;
    window_class.style = CS_DBLCLKS;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;

    if (!RegisterClassEx(&window_class)) /* =========Z= */ return 0;

    main_window = CreateWindowEx (
          WS_EX_APPWINDOW,
          window_class_name,
          WINDOW_NAME,
          WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT,
          CW_USEDEFAULT,
          600,
          300,
          HWND_DESKTOP,
          nullptr,
          hInstance,
          nullptr
    );

    ShowWindow (main_window, nShowCmd);

    MSG messages;
    while (GetMessage (&messages, nullptr, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return 0;
}


VOID main_thread(PVOID pvoid) {
    main();
}

bool main_thread_has_started = false;

void start_main_thread() {
    if (main_thread_has_started == true) return;
    _beginthread(main_thread, 0, nullptr);
    main_thread_has_started = true;
}

LRESULT CALLBACK WindowProcedure(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
    using namespace churro;
    switch (message) {
        case WM_GETMINMAXINFO: {
            const auto lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
            lpMMI->ptMinTrackSize.x = WINDOW_MIN_X;
            lpMMI->ptMinTrackSize.y = WINDOW_MIN_Y;
        }
        case WM_CREATE: {
            if (FAILED(churro::direct_2d._start_render())) /* -------------> */ return -1;
            break;
        }
        case WM_SIZE: {
            if (FAILED(churro::direct_2d._create_render_target()))   return -1;

            const WORD width = LOWORD(lParam);
            const WORD height = HIWORD(lParam);

            start_main_thread();

            window_properties.width = width;
            window_properties.height = height;

            if (FAILED(churro::direct_2d._resize_canvas())) return -1;

            break;
        }

        case WM_SIZING: {
            RECT frame;
            GetClientRect(hWnd, &frame);

            auto const height = frame.bottom - frame.top;
            auto const width = frame.right - frame.left;

            window_properties.height = height;
            window_properties.width = width;

            direct_2d.refresh();
            break;
        }

        case WM_MOUSEMOVE: {
            if (mouse_inside == false) {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = hWnd;
                tme.dwFlags = TME_LEAVE;
                tme.dwHoverTime = 0;
                TrackMouseEvent(&tme);
            }

            mouse_inside = true;

            window_properties.mouse.right_click = wParam & MK_LBUTTON;
            window_properties.mouse.left_click = wParam & MK_RBUTTON;
            window_properties.mouse.pos[0] = GET_X_LPARAM(lParam);
            window_properties.mouse.pos[1] = GET_Y_LPARAM(lParam);
            break;
        }
        case WM_PAINT: {
            if (FAILED(churro::direct_2d._draw())) return -1;
            break;
        }
        case WM_MOUSELEAVE: {
            mouse_inside = false;
            break;
        }
        case WM_LBUTTONDOWN: {
            window_properties.mouse.left_click = true;
            break;
        }
        case WM_LBUTTONUP: {
            window_properties.mouse.left_click = false;
            break;
        }
        case WM_RBUTTONDOWN: {
            window_properties.mouse.right_click = true;
            break;
        }
        case WM_RBUTTONUP: {
            window_properties.mouse.right_click = false;
            break;
        }
        case WM_DESTROY: {
            direct_2d._discard_graphic_resources();
            direct_2d._discard_factory();
            PostQuitMessage(0);
            break;
        }
        default: {}
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
