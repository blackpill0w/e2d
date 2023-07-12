#include "./spritesheets_manager.hpp"

#include <ranges>

using std::optional;
using std::vector;

namespace Engine {

Spritesheet::Spritesheet(const sf::Texture &txtr_) : entity{}, txtr{std::move(txtr_)} {}

SpritesheetsManager::SpritesheetsManager()
    : entity{}, m_spritesheets{}, m_sprites{}, m_bound_entities{} {
  m_spritesheets.reserve(32);
}

bool SpritesheetsManager::is_valid_spritesheet_id(const Entity::Id id) const {
  using std::ranges::any_of;
  return any_of(m_spritesheets, [id](const Spritesheet &x) { return x.entity.id == id; });
}

bool SpritesheetsManager::is_valid_animation_state(const Entity::Id id,
                                                   const size_t ani_state) const {
  optional<size_t> ani_state_num = number_of_animation_states(id);
  return ani_state_num.has_value() and ani_state < ani_state_num;
}

bool SpritesheetsManager::is_valid_sprite(const Entity::Id id, const size_t ani_state,
                                          const size_t idx) const {
  return is_valid_animation_state(id, ani_state) and idx < m_sprites.at(id).at(ani_state).size();
}

optional<Entity::Id> SpritesheetsManager::load_spritesheet(const std::string &filename,
                                                           const sf::Vector2u sprite_size) {
  if (not add_spritesheet(filename).has_value())
    return std::nullopt;

  auto ss_sprites = split_spritesheet(m_spritesheets.back(), sprite_size);
  if (not ss_sprites.has_value()) {
    m_spritesheets.pop_back();
    return std::nullopt;
  }
  m_sprites[m_spritesheets.back().entity.id] = std::move(ss_sprites.value());

  return m_spritesheets.back().entity.id;
}

bool SpritesheetsManager::bind_entity(const Entity::Id entity_id, const Entity::Id ss_id) {
  if (not is_valid_spritesheet_id(ss_id))
    return false;
  m_bound_entities[ss_id].push_back(entity_id);
  return true;
}

optional<size_t> SpritesheetsManager::number_of_animation_states(const Entity::Id id) const {
  if (not is_valid_spritesheet_id(id))
    return std::nullopt;
  return m_sprites.at(id).size();
}

optional<size_t> SpritesheetsManager::animation_state_sprites_number(const Entity::Id id,
                                                                     const size_t ani_state) const {
  if (not is_valid_animation_state(id, ani_state))
    return std::nullopt;
  return m_sprites.at(id).at(ani_state).size();
}

std::optional<SpriteCoordinates> SpritesheetsManager::get_sprite_coordinates(
    const Entity::Id id, const size_t ani_state, const size_t idx) const {
  if (not is_valid_spritesheet_id(id))
    return std::nullopt;
  return m_sprites.at(id).at(ani_state).at(idx);
}

optional<Entity::Id> SpritesheetsManager::add_spritesheet(const std::string &filename) {
  sf::Texture txtr{};
  if (not txtr.loadFromFile(filename))
    return std::nullopt;
  m_spritesheets.emplace_back(Spritesheet(txtr));

  return m_spritesheets.back().entity.id;
}

optional<vector<vector<SpriteCoordinates>>> SpritesheetsManager::split_spritesheet(
    const Spritesheet &ss, const sf::Vector2u size) const {
  if (size.x <= 0 or size.y <= 0 or size.x > ss.txtr.getSize().x or size.y > ss.txtr.getSize().y)
    return std::nullopt;

  vector<vector<SpriteCoordinates>> res{};
  res.reserve(32);
  for (unsigned y = 0; y < ss.txtr.getSize().y; y += size.y) {
    res.emplace_back();
    res.back().reserve(ss.txtr.getSize().x);
    for (unsigned x = 0; x < ss.txtr.getSize().x; x += size.x)
      res.back().emplace_back(SpriteCoordinates(x, y, size.x, size.y));
  }
  return res;
}

}  // namespace Engine