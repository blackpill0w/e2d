#pragma once

#include <SFML/Graphics.hpp>

#include "./spritesheets_manager.hpp"
#include "./input_manager.hpp"
#include "./world.hpp"

namespace Engine {
class Engine {
 public:
  Engine();
  void run();

 public:
  InputManager input_manager;
  SpritesheetsManager spritesheets_manager;
  World world;

 protected:
  // TODO: improve?
  sf::RenderWindow win;
};
}  // namespace Engine
