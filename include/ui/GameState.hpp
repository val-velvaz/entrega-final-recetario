#pragma once

// [SENTINEL FIX] Forward Declaration para romper dependencia circular
class Game;

class GameState {
public:
    virtual ~GameState() {}
    
    // Métodos virtuales puros que DEBEN ser implementados por cualquier pantalla
    virtual void init(Game& game) = 0;
    virtual void cleanup() = 0;
    virtual void handleEvents(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;

protected:
    GameState() {}
};