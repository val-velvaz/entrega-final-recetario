#include "ui/PantallaAgregarReceta.hpp"
#include "ui/PantallaMenuPrincipal.hpp"
#include "ui/PantallaExito.hpp" // <--- ¡IMPORTANTE! Incluir la nueva pantalla
#include "Game.hpp"
#include "persistence/RutasAssets.hpp"
#include "utils/RenderizadorTextos.hpp"
#include "utils/ValidadorEntradas.hpp"
#include "entities/Categoria.hpp"
#include <iostream>
#include <cmath>
#include <filesystem> 
#include <SDL3_image/SDL_image.h>

namespace fs = std::filesystem;

PantallaAgregarReceta::PantallaAgregarReceta()
    : fuenteTitulo(nullptr), fuenteLabels(nullptr), fuenteInputs(nullptr), fuenteLista(nullptr), fuenteUI(nullptr),
      texturaTitulo(nullptr),
      inputNombre(nullptr), inputAutor(nullptr), inputTiempo(nullptr), inputProcedimiento(nullptr),
      inputIngNombre(nullptr), inputIngCant(nullptr), inputIngUnidad(nullptr), 
      btnAgregarIngrediente(nullptr), btnQuitarIngrediente(nullptr),
      texPreviewImagen(nullptr), imagenCargada(false),
      categoriaSeleccionada(Categoria::DESAYUNO),
      btnCambiarCategoria(nullptr), texturaCategoriaActual(nullptr),
      btnGuardar(nullptr), btnCancelar(nullptr),
      modoEdicion(false), tempNombre(""), tempAutor(""), tempTiempo(""), tempProc(""), nombreOriginal("") {}

PantallaAgregarReceta::PantallaAgregarReceta(const Receta& r) : PantallaAgregarReceta() {
    modoEdicion = true;
    nombreOriginal = r.obtenerNombrePlatillo();
    
    tempNombre = r.obtenerNombrePlatillo();
    tempAutor = r.obtenerAutor().obtenerNombreCompleto();
    tempTiempo = std::to_string(r.obtenerTiempoPreparacion());
    tempProc = r.obtenerProcedimiento();
    categoriaSeleccionada = r.obtenerCategoria();
    
    rutaImagenTemporal = r.obtenerRutaImagen();
    if (!rutaImagenTemporal.empty()) imagenCargada = true;

    const auto& lista = r.obtenerIngredientesConstante();
    for(int i=0; i<lista.obtenerCantidadElementos(); ++i) {
        listaIngredientesTemp.agregarAlFinal(lista.obtenerEnPosicion(i));
    }
}

PantallaAgregarReceta::~PantallaAgregarReceta() {
    cleanup();
}

void PantallaAgregarReceta::init(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    SDL_Window* window = game.getWindow();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    fuenteTitulo = TTF_OpenFont(RutasAssets::obtenerRutaFuenteNegrita().c_str(), 36);
    fuenteLabels = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 18);
    fuenteInputs = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 20);
    fuenteLista  = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 18);
    fuenteUI     = TTF_OpenFont(RutasAssets::obtenerRutaFuenteRegular().c_str(), 14);

    std::string tit = modoEdicion ? "EDITAR RECETA" : "NUEVA RECETA";
    texturaTitulo = RenderizadorTextos::renderizarTexto(renderer, fuenteTitulo, tit, {50, 50, 70, 255});
    if (texturaTitulo) rectTitulo = { 50.0f, 20.0f, (float)texturaTitulo->w, (float)texturaTitulo->h };

    float yC1 = 90.0f;
    inputNombre = new CajaDeTexto(window, 70, yC1, 400, 35, fuenteInputs);
    inputNombre->establecerPlaceholder("Nombre del platillo");
    if(modoEdicion) inputNombre->establecerTexto(tempNombre);

    inputAutor = new CajaDeTexto(window, 500, yC1, 300, 35, fuenteInputs);
    inputAutor->establecerPlaceholder("Nombre del Autor");
    if(modoEdicion) inputAutor->establecerTexto(tempAutor);

    rectPreviewImagen = { 830.0f, yC1, 130.0f, 130.0f }; 
    if (imagenCargada) {
        texPreviewImagen = IMG_LoadTexture(renderer, rutaImagenTemporal.c_str());
    }

    float yC1_Row2 = 150.0f;
    inputTiempo = new CajaDeTexto(window, 70, yC1_Row2, 100, 35, fuenteInputs);
    inputTiempo->establecerPlaceholder("Min");
    if(modoEdicion) inputTiempo->establecerTexto(tempTiempo);

    std::string iconCat = "assets/images/icons/cat_desayuno.png";
    switch(categoriaSeleccionada) {
         case Categoria::COMIDA: iconCat = "assets/images/icons/cat_comida.png"; break;
         case Categoria::CENA: iconCat = "assets/images/icons/cat_cena.png"; break;
         case Categoria::NAVIDENO: iconCat = "assets/images/icons/cat_navideno.png"; break;
         default: break;
    }
    btnCambiarCategoria = new Boton(renderer, iconCat, 300, yC1_Row2 - 15, 60, 60);
    actualizarTextoCategoria(renderer);

    float yC2 = 240.0f;
    inputIngNombre = new CajaDeTexto(window, 70, yC2, 350, 35, fuenteInputs);
    inputIngNombre->establecerPlaceholder("Ingrediente");
    inputIngCant = new CajaDeTexto(window, 440, yC2, 100, 35, fuenteInputs);
    inputIngCant->establecerPlaceholder("Cant.");
    inputIngUnidad = new CajaDeTexto(window, 560, yC2, 120, 35, fuenteInputs);
    inputIngUnidad->establecerPlaceholder("Unidad");

    btnAgregarIngrediente = new Boton(renderer, "assets/images/icons/boton_agregar.png", 700, yC2, 35, 35);
    btnQuitarIngrediente = new Boton(renderer, "assets/images/icons/boton_menos.png", 745, yC2, 35, 35);

    float yC3 = 470.0f;
    inputProcedimiento = new CajaDeTexto(window, 70, yC3, w - 140, 180, fuenteInputs);
    inputProcedimiento->establecerPlaceholder("Describa el procedimiento...");
    if(modoEdicion) inputProcedimiento->establecerTexto(tempProc);

    btnGuardar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonGuardar(), w - 120, h - 80, 60, 60);
    btnCancelar = new Boton(renderer, RutasAssets::obtenerRutaIconoBotonVolver(), 60, h - 80, 60, 60);

    for(int i=0; i<listaIngredientesTemp.obtenerCantidadElementos(); ++i) {
        Ingrediente ing = listaIngredientesTemp.obtenerEnPosicion(i);
        std::string txt = ing.obtenerNombre() + " | " + ing.obtenerCantidad() + " " + ing.obtenerUnidad();
        SDL_Texture* t = RenderizadorTextos::renderizarTexto(renderer, fuenteLista, txt, {60,60,60,255});
        if(t) {
            float yL = 290.0f + (listaVisualIngredientes.size() * 30.0f);
            if(listaVisualIngredientes.size()<5) {
                ItemIngredienteVisual it; it.textura=t; it.rect={80.0f, yL, (float)t->w, (float)t->h};
                listaVisualIngredientes.push_back(it);
            } else SDL_DestroyTexture(t);
        }
    }
}

void PantallaAgregarReceta::cleanup() {
    if (texturaTitulo) { SDL_DestroyTexture(texturaTitulo); texturaTitulo = nullptr; }
    if (texturaCategoriaActual) { SDL_DestroyTexture(texturaCategoriaActual); texturaCategoriaActual = nullptr; }
    if (texPreviewImagen) { SDL_DestroyTexture(texPreviewImagen); texPreviewImagen = nullptr; }
    
    for(auto& item : listaVisualIngredientes) {
        if (item.textura) SDL_DestroyTexture(item.textura);
    }
    listaVisualIngredientes.clear();
    
    if (fuenteTitulo) { TTF_CloseFont(fuenteTitulo); fuenteTitulo = nullptr; }
    if (fuenteLabels) { TTF_CloseFont(fuenteLabels); fuenteLabels = nullptr; }
    if (fuenteInputs) { TTF_CloseFont(fuenteInputs); fuenteInputs = nullptr; }
    if (fuenteLista) { TTF_CloseFont(fuenteLista); fuenteLista = nullptr; }
    if (fuenteUI) { TTF_CloseFont(fuenteUI); fuenteUI = nullptr; }

    if (btnGuardar) { delete btnGuardar; btnGuardar = nullptr; }
    if (btnCancelar) { delete btnCancelar; btnCancelar = nullptr; }
    if (btnCambiarCategoria) { delete btnCambiarCategoria; btnCambiarCategoria = nullptr; }
    if (btnAgregarIngrediente) { delete btnAgregarIngrediente; btnAgregarIngrediente = nullptr; }
    if (btnQuitarIngrediente) { delete btnQuitarIngrediente; btnQuitarIngrediente = nullptr; }

    if (inputNombre) { delete inputNombre; inputNombre = nullptr; }
    if (inputAutor) { delete inputAutor; inputAutor = nullptr; }
    if (inputTiempo) { delete inputTiempo; inputTiempo = nullptr; }
    if (inputIngNombre) { delete inputIngNombre; inputIngNombre = nullptr; }
    if (inputIngCant) { delete inputIngCant; inputIngCant = nullptr; }
    if (inputIngUnidad) { delete inputIngUnidad; inputIngUnidad = nullptr; }
    if (inputProcedimiento) { delete inputProcedimiento; inputProcedimiento = nullptr; }
}

void PantallaAgregarReceta::actualizarTextoCategoria(SDL_Renderer* renderer) {
    if (texturaCategoriaActual) SDL_DestroyTexture(texturaCategoriaActual);
    std::string textoCat = categoriaATexto(categoriaSeleccionada);
    texturaCategoriaActual = RenderizadorTextos::renderizarTexto(renderer, fuenteLabels, textoCat, {255, 255, 255, 255});
    if (texturaCategoriaActual) rectCategoria = { 380.0f, 155.0f, (float)texturaCategoriaActual->w, (float)texturaCategoriaActual->h };

    if (btnCambiarCategoria) {
        std::string rutaIcono = "";
        switch (categoriaSeleccionada) {
            case Categoria::DESAYUNO: rutaIcono = "assets/images/icons/cat_desayuno.png"; break;
            case Categoria::COMIDA:   rutaIcono = "assets/images/icons/cat_comida.png"; break;
            case Categoria::CENA:     rutaIcono = "assets/images/icons/cat_cena.png"; break;
            case Categoria::NAVIDENO: rutaIcono = "assets/images/icons/cat_navideno.png"; break;
            default:                  rutaIcono = "assets/images/icons/cat_desayuno.png"; break;
        }
        btnCambiarCategoria->cambiarImagen(renderer, rutaIcono);
    }
}

void PantallaAgregarReceta::agregarIngredienteALista(SDL_Renderer* renderer) {
    std::string nom = inputIngNombre->obtenerTexto();
    std::string cant = inputIngCant->obtenerTexto();
    std::string uni = inputIngUnidad->obtenerTexto();

    if (nom.empty() || cant.empty()) return;

    Ingrediente nuevoIng(nom, cant, uni);
    listaIngredientesTemp.agregarAlFinal(nuevoIng);

    std::string textoVisual = nom + "   |   " + cant + " " + uni;
    SDL_Texture* tex = RenderizadorTextos::renderizarTexto(renderer, fuenteLista, textoVisual, {60, 60, 60, 255});
    
    if (tex) {
        float yLista = 290.0f + (listaVisualIngredientes.size() * 30.0f);
        if (listaVisualIngredientes.size() < 5) { 
            ItemIngredienteVisual item;
            item.textura = tex;
            item.rect = { 80.0f, yLista, (float)tex->w, (float)tex->h };
            listaVisualIngredientes.push_back(item);
        } else {
            SDL_DestroyTexture(tex);
        }
    }
    inputIngNombre->establecerTexto("");
    inputIngCant->establecerTexto("");
    inputIngUnidad->establecerTexto("");
}

void PantallaAgregarReceta::eliminarUltimoIngrediente() {
    if (listaIngredientesTemp.estaVacia()) return;
    try {
        listaIngredientesTemp.eliminarDelFinal();
        if (!listaVisualIngredientes.empty()) {
            SDL_Texture* t = listaVisualIngredientes.back().textura;
            if (t) SDL_DestroyTexture(t);
            listaVisualIngredientes.pop_back();
        }
    } catch (...) {}
}

void PantallaAgregarReceta::procesarArchivoDroppeado(SDL_Renderer* renderer, const char* ruta) {
    if (!ruta) return;
    if (texPreviewImagen) { SDL_DestroyTexture(texPreviewImagen); texPreviewImagen = nullptr; }
    
    texPreviewImagen = IMG_LoadTexture(renderer, ruta);
    if (texPreviewImagen) {
        rutaImagenTemporal = std::string(ruta);
        imagenCargada = true;
    }
}

void PantallaAgregarReceta::handleEvents(Game& game) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) game.setEstaCorriendo(false);

        if (evento.type == SDL_EVENT_DROP_FILE) {
            procesarArchivoDroppeado(game.getRenderer(), evento.drop.data);
        }

        if (inputNombre) inputNombre->manejarEvento(evento);
        if (inputAutor) inputAutor->manejarEvento(evento);
        if (inputTiempo) inputTiempo->manejarEvento(evento);
        if (inputIngNombre) inputIngNombre->manejarEvento(evento);
        if (inputIngCant) inputIngCant->manejarEvento(evento);
        if (inputIngUnidad) inputIngUnidad->manejarEvento(evento);
        if (inputProcedimiento) inputProcedimiento->manejarEvento(evento);

        if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT) {
            float mx = evento.button.x;
            float my = evento.button.y;

            if (btnCancelar && btnCancelar->estaPresionado(mx, my)) {
                game.popEstado();
            }
            else if (btnGuardar && btnGuardar->estaPresionado(mx, my)) {
                guardarReceta(game);
            }
            else if (btnCambiarCategoria && btnCambiarCategoria->estaPresionado(mx, my)) {
                int catInt = static_cast<int>(categoriaSeleccionada);
                catInt = (catInt + 1) % 4;
                categoriaSeleccionada = static_cast<Categoria>(catInt);
                actualizarTextoCategoria(game.getRenderer());
            }
            else if (btnAgregarIngrediente && btnAgregarIngrediente->estaPresionado(mx, my)) {
                agregarIngredienteALista(game.getRenderer());
            }
            else if (btnQuitarIngrediente && btnQuitarIngrediente->estaPresionado(mx, my)) {
                eliminarUltimoIngrediente();
            }
        }
    }
}

void PantallaAgregarReceta::update(Game& game) {
    (void)game;
    if (btnGuardar) btnGuardar->actualizar();
    if (btnCancelar) btnCancelar->actualizar();
    if (btnCambiarCategoria) btnCambiarCategoria->actualizar();
    if (btnAgregarIngrediente) btnAgregarIngrediente->actualizar();
    if (btnQuitarIngrediente) btnQuitarIngrediente->actualizar();
}

void PantallaAgregarReceta::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();
    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    
    float tiempo = SDL_GetTicks() / 1000.0f;
    Uint8 baseB = 240 + (Uint8)(std::sin(tiempo) * 10);
    SDL_SetRenderDrawColor(renderer, 235, 235, baseB, 255);
    SDL_FRect bg = {0, 0, (float)w, (float)h};
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);

    SDL_FRect card1 = {40, 70, (float)w-80, 130};
    SDL_RenderFillRect(renderer, &card1);
    SDL_FRect card2 = {40, 210, (float)w-80, 230};
    SDL_RenderFillRect(renderer, &card2);
    SDL_FRect card3 = {40, 450, (float)w-80, 220};
    SDL_RenderFillRect(renderer, &card3);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderFillRect(renderer, &rectPreviewImagen);
    
    if (texPreviewImagen) {
        SDL_RenderTexture(renderer, texPreviewImagen, nullptr, &rectPreviewImagen);
    } else {
        SDL_Color cHint = {150, 150, 160, 255};
        if (fuenteUI) {
             SDL_Texture* tHint = RenderizadorTextos::renderizarTexto(renderer, fuenteUI, "Arrastra Foto", cHint);
             if (tHint) {
                SDL_FRect rHint = { rectPreviewImagen.x + 15, rectPreviewImagen.y + 55, (float)tHint->w, (float)tHint->h };
                SDL_RenderTexture(renderer, tHint, nullptr, &rHint);
                SDL_DestroyTexture(tHint);
             }
        }
    }

    if (texturaTitulo) SDL_RenderTexture(renderer, texturaTitulo, nullptr, &rectTitulo);

    SDL_Color cLabel = {100, 100, 120, 255};
    RenderizadorTextos::renderizarTexto(renderer, fuenteLabels, "Detalles Basicos", cLabel); 

    if (texturaCategoriaActual) {
        SDL_FRect rBadge = { rectCategoria.x - 15, rectCategoria.y - 5, rectCategoria.w + 30, rectCategoria.h + 10 };
        SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255); 
        SDL_RenderFillRect(renderer, &rBadge);
        SDL_RenderTexture(renderer, texturaCategoriaActual, nullptr, &rectCategoria);
    }
    if (btnCambiarCategoria) btnCambiarCategoria->render(renderer);

    float yList = 285.0f;
    for(size_t i=0; i<listaVisualIngredientes.size(); ++i) {
        if (i % 2 == 0) SDL_SetRenderDrawColor(renderer, 240, 245, 255, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect rRow = { 60, yList + (i*30) - 2, (float)w-120, 30 };
        SDL_RenderFillRect(renderer, &rRow);
        SDL_RenderTexture(renderer, listaVisualIngredientes[i].textura, nullptr, &listaVisualIngredientes[i].rect);
    }

    if (inputNombre) inputNombre->render(renderer);
    if (inputAutor) inputAutor->render(renderer);
    if (inputTiempo) inputTiempo->render(renderer);
    if (inputIngNombre) inputIngNombre->render(renderer);
    if (inputIngCant) inputIngCant->render(renderer);
    if (inputIngUnidad) inputIngUnidad->render(renderer);
    if (btnAgregarIngrediente) btnAgregarIngrediente->render(renderer);
    if (btnQuitarIngrediente) btnQuitarIngrediente->render(renderer);
    if (inputProcedimiento) inputProcedimiento->render(renderer);
    if (btnGuardar) btnGuardar->render(renderer);
    if (btnCancelar) btnCancelar->render(renderer);
}

void PantallaAgregarReceta::guardarReceta(Game& game) {
    try {
        std::string nombre = inputNombre->obtenerTexto();
        std::string autorStr = inputAutor->obtenerTexto();
        std::string tiempoStr = inputTiempo->obtenerTexto();
        std::string procedimiento = inputProcedimiento->obtenerTexto();

        if (nombre.empty() || tiempoStr.empty()) {
            std::cout << "Faltan datos!" << std::endl;
            return;
        }
        if (!ValidadorEntradas::verificarSiEsNumeroEnteroPositivo(tiempoStr)) return;

        Nombre autorObj(autorStr, "", ""); 
        int tiempo = std::stoi(tiempoStr);

        Receta nuevaReceta(nombre, autorObj, categoriaSeleccionada, tiempo, procedimiento);
        
        for (int i = 0; i < listaIngredientesTemp.obtenerCantidadElementos(); ++i) {
            nuevaReceta.agregarIngredienteOrdenado(listaIngredientesTemp.obtenerEnPosicion(i));
        }

        if (imagenCargada && !rutaImagenTemporal.empty()) {
            std::string carpetaDestino = "assets/images/platillos/otros"; 
            switch(categoriaSeleccionada) {
                case Categoria::DESAYUNO: carpetaDestino = "assets/images/platillos/desayuno"; break;
                case Categoria::COMIDA: carpetaDestino = "assets/images/platillos/comida"; break;
                case Categoria::CENA: carpetaDestino = "assets/images/platillos/cena"; break;
                case Categoria::NAVIDENO: carpetaDestino = "assets/images/platillos/navideno"; break;
                default: carpetaDestino = "assets/images/platillos/otros"; break;
            }

            fs::create_directories(carpetaDestino);
            std::string extension = fs::path(rutaImagenTemporal).extension().string();
            std::string rutaFinal = carpetaDestino + "/" + nombre + extension;
            try {
                fs::copy_file(rutaImagenTemporal, rutaFinal, fs::copy_options::overwrite_existing);
                nuevaReceta.establecerRutaImagen(rutaFinal);
            } catch (...) {}
        }

        if (modoEdicion) {
            game.getManejadorRecetas().eliminarRecetaPorNombre(nombreOriginal);
        }

        game.getManejadorRecetas().agregarRecetaNueva(nuevaReceta);
        
        // --- AQUÍ ESTÁ LA MAGIA: APILAR LA PANTALLA DE ÉXITO ---
        game.pushEstado(new PantallaExito());
        
        // Nota: No hacemos popEstado() aquí para que el usuario vea el éxito encima
        // y al dar Enter en PantallaExito, esa se cierre y vuelva aquí.
        // Si quieres que al dar Enter salga de TODO, tendrías que manejarlo diferente.
        // Por ahora, esto mostrará el éxito y al cerrar volverás al formulario.
        
        // Opcional: Limpiar formulario si no es edición
        if (!modoEdicion) {
             inputNombre->establecerTexto("");
             inputTiempo->establecerTexto("");
             // ... etc
        }

    } catch (...) {}
}
