#include "ui/PantallaVisualizarRecetas.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include "ui/PantallaDetalleReceta.hpp"
#include "ui/PantallaEliminarReceta.hpp"
#include "ui/PantallaAgregarReceta.hpp"
#include "ui/PantallaGestionArchivos.hpp"
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <SDL3_image/SDL_image.h>

struct Burbuja { float x, y, speed, size; };
static std::vector<Burbuja> burbujas;

PantallaVisualizarRecetas::PantallaVisualizarRecetas() 
    : fuenteTitulo(nullptr), fuenteLista(nullptr), fuenteUI(nullptr),
      texturaTitulo(nullptr), 
      inputBusqueda(nullptr), btnBuscar(nullptr), btnReset(nullptr),
      btnOrdNombre(nullptr), btnOrdTiempo(nullptr), btnOrdCategoria(nullptr), btnOrdAutor(nullptr),
      btnCargar(nullptr), btnGuardar(nullptr), btnEliminarTodo(nullptr),
      texIconoOjo(nullptr), texIconoBasura(nullptr), texIconoEditar(nullptr),
      texLblNombre(nullptr), texLblTiempo(nullptr), texLblCat(nullptr), texLblAut(nullptr),
      texLblCargar(nullptr), texLblGuardar(nullptr), texLblDelAll(nullptr),
      btnVolver(nullptr), filtroNombre(""), cacheCantidadRecetas(-1) { // Inicializar en -1 para forzar carga
          
    if (burbujas.empty()) {
        for(int i=0; i<20; ++i) {
            burbujas.push_back({(float)(rand()%1024), (float)(rand()%768), (float)(rand()%2 + 1), (float)(rand()%20 + 10)});
        }
    }
}

PantallaVisualizarRecetas::~PantallaVisualizarRecetas() {
    cleanup();
}

void PantallaVisualizarRecetas::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    SDL_Window* window = game.getWindow();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 36);
    fuenteLista = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 20);
    fuenteUI = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 14);

    SDL_Color colorNegro = {0, 0, 0, 255};
    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, "GESTION DE RECETAS", colorNegro);
    if (texturaTitulo) rectTitulo = { (w - (float)texturaTitulo->w) / 2.0f, 20.0f, (float)texturaTitulo->w, (float)texturaTitulo->h };

    texIconoOjo = IMG_LoadTexture(renderer, "assets/images/icons/boton_ojo.png");
    texIconoBasura = IMG_LoadTexture(renderer, "assets/images/icons/boton_papelera_small.png");
    texIconoEditar = IMG_LoadTexture(renderer, "assets/images/icons/boton_editar.png");

    float searchY = 80.0f;
    inputBusqueda = new CajaDeTexto(window, 50, searchY, 300, 40, fuenteLista);
    inputBusqueda->establecerPlaceholder("Buscar por nombre...");
    btnBuscar = new Boton(renderer, "assets/images/icons/boton_lupa.png", 360, searchY, 40, 40);
    btnReset = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 410, searchY, 40, 40);

    // Filtros
    float sortY = 80.0f;
    float startSortX = 500.0f; float gap = 50.0f;
    btnOrdNombre = new Boton(renderer, "assets/images/icons/filtro_nombre.png", startSortX, sortY, 40, 40);
    texLblNombre = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Nombre", colorNegro);
    if(texLblNombre) rLblNombre = {startSortX, sortY+42, (float)texLblNombre->w, (float)texLblNombre->h};

    btnOrdTiempo = new Boton(renderer, "assets/images/icons/filtro_tiempo.png", startSortX + gap, sortY, 40, 40);
    texLblTiempo = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Tiempo", colorNegro);
    if(texLblTiempo) rLblTiempo = {startSortX + gap, sortY+42, (float)texLblTiempo->w, (float)texLblTiempo->h};

    btnOrdCategoria = new Boton(renderer, "assets/images/icons/filtro_categoria.png", startSortX + gap*2, sortY, 40, 40);
    texLblCat = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Categ.", colorNegro);
    if(texLblCat) rLblCat = {startSortX + gap*2, sortY+42, (float)texLblCat->w, (float)texLblCat->h};

    btnOrdAutor = new Boton(renderer, "assets/images/icons/filtro_autor.png", startSortX + gap*3, sortY, 40, 40);
    texLblAut = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Autor", colorNegro);
    if(texLblAut) rLblAut = {startSortX + gap*3, sortY+42, (float)texLblAut->w, (float)texLblAut->h};

    // Gestión
    float dataX = 800.0f; float dataY = 80.0f; float dataGap = 50.0f;
    btnCargar = new Boton(renderer, "assets/images/icons/archivo_cargar.png", dataX, dataY, 40, 40);
    texLblCargar = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Cargar", {0,100,0,255});
    if(texLblCargar) rLblCargar = {dataX, dataY+42, (float)texLblCargar->w, (float)texLblCargar->h};

    btnGuardar = new Boton(renderer, "assets/images/icons/boton_guardar.png", dataX + dataGap, dataY, 40, 40);
    texLblGuardar = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Guardar", {0,0,200,255});
    if(texLblGuardar) rLblGuardar = {dataX + dataGap, dataY+42, (float)texLblGuardar->w, (float)texLblGuardar->h};

    btnEliminarTodo = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonEliminar(), dataX + dataGap*2, dataY, 40, 40);
    texLblDelAll = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Borrar Todo", {200,0,0,255});
    if(texLblDelAll) rLblDelAll = {dataX + dataGap*2, dataY+42, (float)texLblDelAll->w, (float)texLblDelAll->h};

    btnVolver = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 50, h - 80, 60, 60);

    generarCacheListado(game);
}

void PantallaVisualizarRecetas::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    if (texIconoOjo) { SDL_DestroyTexture(texIconoOjo); texIconoOjo = nullptr; }
    if (texIconoBasura) { SDL_DestroyTexture(texIconoBasura); texIconoBasura = nullptr; }
    if (texIconoEditar) { SDL_DestroyTexture(texIconoEditar); texIconoEditar = nullptr; }
    
    if (texLblNombre) { SDL_DestroyTexture(texLblNombre); texLblNombre = nullptr; }
    if (texLblTiempo) { SDL_DestroyTexture(texLblTiempo); texLblTiempo = nullptr; }
    if (texLblCat) { SDL_DestroyTexture(texLblCat); texLblCat = nullptr; }
    if (texLblAut) { SDL_DestroyTexture(texLblAut); texLblAut = nullptr; }
    if (texLblCargar) { SDL_DestroyTexture(texLblCargar); texLblCargar = nullptr; }
    if (texLblGuardar) { SDL_DestroyTexture(texLblGuardar); texLblGuardar = nullptr; }
    if (texLblDelAll) { SDL_DestroyTexture(texLblDelAll); texLblDelAll = nullptr; }

    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteLista) { TTF_CloseFont(fuenteLista); fuenteLista = nullptr; }
    if (fuenteUI) { TTF_CloseFont(fuenteUI); fuenteUI = nullptr; }

    for (auto& item : itemsLista) {
        if (item.textura) SDL_DestroyTexture(item.textura);
        if (item.texMiniatura) SDL_DestroyTexture(item.texMiniatura);
    }
    itemsLista.clear();

    if(inputBusqueda) { delete inputBusqueda; inputBusqueda = nullptr; }
    if(btnBuscar) { delete btnBuscar; btnBuscar = nullptr; }
    if(btnReset) { delete btnReset; btnReset = nullptr; }
    if(btnOrdNombre) { delete btnOrdNombre; btnOrdNombre = nullptr; }
    if(btnOrdTiempo) { delete btnOrdTiempo; btnOrdTiempo = nullptr; }
    if(btnOrdCategoria) { delete btnOrdCategoria; btnOrdCategoria = nullptr; }
    if(btnOrdAutor) { delete btnOrdAutor; btnOrdAutor = nullptr; }
    if(btnCargar) { delete btnCargar; btnCargar = nullptr; }
    if(btnGuardar) { delete btnGuardar; btnGuardar = nullptr; }
    if(btnEliminarTodo) { delete btnEliminarTodo; btnEliminarTodo = nullptr; }
    if(btnVolver) { delete btnVolver; btnVolver = nullptr; }
}

void PantallaVisualizarRecetas::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            game.setEstaCorriendo(false);
        }

        if (inputBusqueda) inputBusqueda->manejarEvento(evento);

        if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT) {
            float mx = evento.button.x;
            float my = evento.button.y;

            for (const auto& item : itemsLista) {
                if (mx >= item.rectOjo.x && mx <= (item.rectOjo.x + item.rectOjo.w) &&
                    my >= item.rectOjo.y && my <= (item.rectOjo.y + item.rectOjo.h)) {
                    Receta r = game.getManejadorRecetas().obtenerRecetaEnPosicion(item.indiceOriginal);
                    game.pushEstado(new PantallaDetalleReceta(r));
                    return;
                }
                if (mx >= item.rectEditar.x && mx <= (item.rectEditar.x + item.rectEditar.w) &&
                    my >= item.rectEditar.y && my <= (item.rectEditar.y + item.rectEditar.h)) {
                    Receta r = game.getManejadorRecetas().obtenerRecetaEnPosicion(item.indiceOriginal);
                    game.pushEstado(new PantallaAgregarReceta(r));
                    return;
                }
                if (mx >= item.rectBasura.x && mx <= (item.rectBasura.x + item.rectBasura.w) &&
                    my >= item.rectBasura.y && my <= (item.rectBasura.y + item.rectBasura.h)) {
                    Receta r = game.getManejadorRecetas().obtenerRecetaEnPosicion(item.indiceOriginal);
                    game.getManejadorRecetas().eliminarRecetaPorNombre(r.obtenerNombrePlatillo());
                    generarCacheListado(game); 
                    return;
                }
            }

            if (btnVolver && btnVolver->estaPresionado(mx, my)) {
                game.popEstado();
            }
            else if (btnOrdNombre && btnOrdNombre->estaPresionado(mx, my)) { game.getManejadorRecetas().ordenarRecetasPorNombre(); generarCacheListado(game); }
            else if (btnOrdTiempo && btnOrdTiempo->estaPresionado(mx, my)) { game.getManejadorRecetas().ordenarRecetasPorTiempoPreparacion(); generarCacheListado(game); }
            else if (btnOrdCategoria && btnOrdCategoria->estaPresionado(mx, my)) { game.getManejadorRecetas().ordenarRecetasPorCategoria(); generarCacheListado(game); }
            else if (btnOrdAutor && btnOrdAutor->estaPresionado(mx, my)) { game.getManejadorRecetas().ordenarRecetasPorAutor(); generarCacheListado(game); }
            else if (btnBuscar && btnBuscar->estaPresionado(mx, my)) { filtroNombre = inputBusqueda->obtenerTexto(); generarCacheListado(game); }
            else if (btnReset && btnReset->estaPresionado(mx, my)) { filtroNombre = ""; inputBusqueda->establecerTexto(""); generarCacheListado(game); }
            else if (btnCargar && btnCargar->estaPresionado(mx, my)) { 
                game.pushEstado(new PantallaGestionArchivos(false)); 
            }
            else if (btnGuardar && btnGuardar->estaPresionado(mx, my)) { 
                game.pushEstado(new PantallaGestionArchivos(true)); 
            }
            else if (btnEliminarTodo && btnEliminarTodo->estaPresionado(mx, my)) { 
                game.getManejadorRecetas().eliminarTodasLasRecetas(); 
                generarCacheListado(game); 
            }
        }
    }
}

void PantallaVisualizarRecetas::update(Game& game) {
    if(btnBuscar) btnBuscar->actualizar();
    if(btnReset) btnReset->actualizar();
    if(btnOrdNombre) btnOrdNombre->actualizar();
    if(btnOrdTiempo) btnOrdTiempo->actualizar();
    if(btnOrdCategoria) btnOrdCategoria->actualizar();
    if(btnOrdAutor) btnOrdAutor->actualizar();
    if(btnCargar) btnCargar->actualizar();
    if(btnGuardar) btnGuardar->actualizar();
    if(btnEliminarTodo) btnEliminarTodo->actualizar();
    if(btnVolver) btnVolver->actualizar();

    // CORRECCIÓN CRÍTICA: Auto-refrescar si los datos cambiaron (por ejemplo, tras cargar archivo)
    int cantidadActual = game.getManejadorRecetas().obtenerCantidadRecetas();
    if (cantidadActual != cacheCantidadRecetas) {
        std::cout << "[UI] Detectado cambio en recetas. Actualizando lista..." << std::endl;
        generarCacheListado(game);
    }

    for(auto& b : burbujas) {
        b.y -= b.speed;
        if(b.y < -50) {
            b.y = 768 + 50;
            b.x = (float)(rand() % 1024);
        }
    }
}

void PantallaVisualizarRecetas::generarCacheListado(Game& game) {
    for (auto& item : itemsLista) {
        if (item.textura) SDL_DestroyTexture(item.textura);
        if (item.texMiniatura) SDL_DestroyTexture(item.texMiniatura);
    }
    itemsLista.clear();

    // Actualizar la caché de cantidad para evitar bucles infinitos de regeneración
    ManejadorRecetas& manejador = game.getManejadorRecetas();
    cacheCantidadRecetas = manejador.obtenerCantidadRecetas();

    SDL_Renderer* renderer = game.getRenderer();
    SDL_Color colorTexto = {50, 50, 50, 255};
    float yActual = 160.0f;
    float xPadding = 50.0f;

    int itemsMostrados = 0;

    for (int i = 0; i < cacheCantidadRecetas; ++i) {
        const Receta& receta = manejador.obtenerRecetaEnPosicion(i);
        
        if (!filtroNombre.empty()) {
            std::string nombreRec = receta.obtenerNombrePlatillo();
            // Búsqueda insensible a mayúsculas sería mejor, pero esto cumple
            if (nombreRec.find(filtroNombre) == std::string::npos) {
                continue;
            }
        }

        std::string textoFila = std::to_string(itemsMostrados + 1) + ". " + 
                                receta.obtenerNombrePlatillo() + 
                                " | " + categoriaATexto(receta.obtenerCategoria()) +
                                " | " + std::to_string(receta.obtenerTiempoPreparacion()) + "min";

        ItemLista nuevoItem;
        nuevoItem.indiceOriginal = i;
        nuevoItem.textura = RenderizadorTextos::renderizarTexto(renderer, fuenteLista, textoFila, colorTexto);
        
        nuevoItem.texMiniatura = nullptr;
        std::string rutaImg = receta.obtenerRutaImagen();
        if (!rutaImg.empty()) {
            nuevoItem.texMiniatura = IMG_LoadTexture(renderer, rutaImg.c_str());
        }

        if (nuevoItem.textura) {
            nuevoItem.rect.x = xPadding + 40; 
            nuevoItem.rect.y = yActual;
            nuevoItem.rect.w = (float)nuevoItem.textura->w;
            nuevoItem.rect.h = (float)nuevoItem.textura->h;
            
            float btnSize = 30.0f;
            float w = 1024.0f;
            nuevoItem.rectOjo = { w - 150.0f, yActual, btnSize, btnSize };
            nuevoItem.rectEditar = { w - 110.0f, yActual, btnSize, btnSize };
            nuevoItem.rectBasura = { w - 70.0f, yActual, btnSize, btnSize };

            itemsLista.push_back(nuevoItem);
            yActual += 35.0f;
            itemsMostrados++;
        }
    }

    if (itemsMostrados == 0) {
        ItemLista itemVacio;
        itemVacio.textura = RenderizadorTextos::renderizarTexto(renderer, fuenteLista, "No se encontraron recetas.", {150, 0, 0, 255});
        if (itemVacio.textura) {
            itemVacio.rect = { xPadding, yActual, (float)itemVacio.textura->w, (float)itemVacio.textura->h };
            // Inicializamos rects vacíos para evitar crashes
            itemVacio.rectOjo = {0,0,0,0};
            itemVacio.rectEditar = {0,0,0,0};
            itemVacio.rectBasura = {0,0,0,0};
            itemVacio.texMiniatura = nullptr;
            itemsLista.push_back(itemVacio);
        }
    }
}

void PantallaVisualizarRecetas::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    
    SDL_SetRenderDrawColor(renderer, 220, 255, 235, 255);
    SDL_FRect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 180, 240, 200, 150);
    for(const auto& b : burbujas) {
        SDL_FRect rB = {b.x, b.y, b.size, b.size};
        SDL_RenderFillRect(renderer, &rB);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    if (texturaTitulo) SDL_RenderTexture(renderer, texturaTitulo, nullptr, &rectTitulo);

    if (inputBusqueda) inputBusqueda->render(renderer);
    if (btnBuscar) btnBuscar->render(renderer);
    if (btnReset) btnReset->render(renderer);

    if (btnOrdNombre) btnOrdNombre->render(renderer);
    if (texLblNombre) SDL_RenderTexture(renderer, texLblNombre, nullptr, &rLblNombre);
    if (btnOrdTiempo) btnOrdTiempo->render(renderer);
    if (texLblTiempo) SDL_RenderTexture(renderer, texLblTiempo, nullptr, &rLblTiempo);
    if (btnOrdCategoria) btnOrdCategoria->render(renderer);
    if (texLblCat) SDL_RenderTexture(renderer, texLblCat, nullptr, &rLblCat);
    if (btnOrdAutor) btnOrdAutor->render(renderer);
    if (texLblAut) SDL_RenderTexture(renderer, texLblAut, nullptr, &rLblAut);

    if (btnCargar) btnCargar->render(renderer);
    if (texLblCargar) SDL_RenderTexture(renderer, texLblCargar, nullptr, &rLblCargar);
    if (btnGuardar) btnGuardar->render(renderer);
    if (texLblGuardar) SDL_RenderTexture(renderer, texLblGuardar, nullptr, &rLblGuardar);
    if (btnEliminarTodo) btnEliminarTodo->render(renderer);
    if (texLblDelAll) SDL_RenderTexture(renderer, texLblDelAll, nullptr, &rLblDelAll);

    for (const auto& item : itemsLista) {
        // Si es un item de mensaje vacío, no dibujar fondo ni botones
        if (item.rectOjo.w == 0) {
             if (item.textura) SDL_RenderTexture(renderer, item.textura, nullptr, &item.rect);
             continue;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
        SDL_FRect rowBg = { 40, item.rect.y - 2, 944, 34 };
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &rowBg);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_FRect rectThumb = { 45, item.rect.y, 30, 30 };
        if (item.texMiniatura) SDL_RenderTexture(renderer, item.texMiniatura, nullptr, &rectThumb);
        else {
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &rectThumb);
        }

        if (item.textura) SDL_RenderTexture(renderer, item.textura, nullptr, &item.rect);
        
        if (texIconoOjo) SDL_RenderTexture(renderer, texIconoOjo, nullptr, &item.rectOjo);
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &item.rectOjo);
        }

        if (texIconoEditar) SDL_RenderTexture(renderer, texIconoEditar, nullptr, &item.rectEditar);
        else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &item.rectEditar);
        }

        if (texIconoBasura) SDL_RenderTexture(renderer, texIconoBasura, nullptr, &item.rectBasura);
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &item.rectBasura);
        }
    }

    if (btnVolver) btnVolver->render(renderer);
}
