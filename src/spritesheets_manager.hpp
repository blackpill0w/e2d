#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "./entity.hpp"
#include "./texture.hpp"

namespace e2d {

using SpriteCoordinates = sf::Rect<size_t>;

/**
   A class that stores all of the spritesheets, and claasifies their sprites by animation
   state.
 */
class SpritesheetsManager {
 public:
  SpritesheetsManager();
  bool is_valid_spritesheet_id(const Entity::Id id) const;
  bool is_valid_animation_state(const Entity::Id id, const size_t ani_state) const;
  bool is_valid_sprite(const Entity::Id id, const size_t ani_state, const size_t idx) const;
  const Texture *get_spritesheet(const Entity::Id id) const;

  /**
     Get the number of animation states.
     @param `id` id of the spritesheet.
   */
  std::optional<size_t> animation_states_num(const Entity::Id id) const;

  /**
     Get the number of sprites of an animation state.
     @param `id` id of the spritesheet.
     @param `ani_state` number of the animation state.
   */
  std::optional<size_t> animation_state_sprites_num(const Entity::Id id, const size_t ani_state)
      const;

  //** `id` is the id of the desired `Texture`.
  std::optional<SpriteCoordinates> get_sprite_at(
      const Entity::Id id, const size_t row, const size_t col
  ) const;
  //** Alias for get_sprite_at().
  std::optional<SpriteCoordinates> get_sprite_coordinates(
      const Entity::Id id, const size_t ani_state, const size_t idx
  ) const;

  //** Get a sprite given it's id in a Tiled map.
  std::optional<SpriteCoordinates> get_sprite_with_tiled_id(
      const Entity::Id id, const size_t tiled_id
  ) const;

  /**
     Load a spritesheet from an image file and extract sprites from it.
     Each sprite in the sheet *must* have the same size `sprite_size`.
     Each line in the spritesheet defines a separate state (Idle, Walk...) which are
     just unsigned ints starting from 0.
     # Return
     `std::nullopt` if:
     - the file doesn't exist
     - the file is of invalid/unsupported type
     - the `sprite_size` equal to or less than 0 or is bigger than the spritesheet
   */
  std::optional<Entity::Id> load_spritesheet(
      const std::string &filename, const sf::Vector2<size_t> sprite_size
  );

  /**
     Load a tileset made by the <a href="https://www.mapeditor.org">Tiled</a> tile map editor.
     # Return
     `std::nullopt` if the file doesn't exist.
   */
  std::optional<Entity::Id> load_tiled_tileset(const std::string &filename);

 public:
  const Entity entity;

 protected:
  /**
     Load a spritesheet from a file and add it to `m_spritesheets`.
   */
  std::optional<Entity::Id> add_spritesheet(const std::string &filename);

  /**
     Split spritesheets, a vector of sprites for each animation state.
     # Return
     `std::nullopt` if the `size` <= 0 or > than the spritesheet.
   */
  std::optional<std::vector<std::vector<SpriteCoordinates>>> split_spritesheet(
      const Texture &ss, const sf::Vector2<size_t> size
  ) const;

 protected:
  std::vector<Texture> m_spritesheets;
  /**
     Store sprites of a spritesheet, grouped by animation state (which is a horizontal
     line of sprites in the spritesheet).
 */
  std::unordered_map<Entity::Id, std::vector<std::vector<SpriteCoordinates>>> m_sprites;
};
}  // namespace e2d
