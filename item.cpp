#include "PygamePP.h"
#include <vector>
#include <cstdlib>

class HoneyJar {
private: 
   float x, y;
   float width, height;
   bool active;

public:
   HoneyJar(float start_x, float start_y) : x(start_x), y(start_y), active(true) {
        width = 50.0f;
        height = 50.0f;
    }
    
    void update(float dt, float speed) {
        x -= speed * dt;
    }

    bool is_offscreen() const {
        return (x + width < 0);
    }

    bool is_active() const { return active; }
    void collect() { active = false; }

    PG::Rect get_rect() const {
        return PG::Rect(x + 5, y + 5, width - 10, height - 10);
    }
    
    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_width() const { return width; }
    float get_height() const { return height; }
};


class ItemManager {
private:
    PG::Sprite honey_sprite;
    std::vector<HoneyJar> active_jars;
    float spawn_chance;

public:
    ItemManager(PG::Window& app) : honey_sprite(app, "item.png"), spawn_chance(60.0f) {}

    void check_and_spawn(float flower_x, float flower_y, bool is_tall) {
        if ((rand() % 100) < spawn_chance) {
            float jar_y = flower_y - 80.0f;
            active_jars.push_back(HoneyJar(flower_x + 65.0f, jar_y));
        }
    }

    void reset() {
        active_jars.clear();
    }

    void update(float dt, float speed, const PG::Rect& player_box) {
        for (size_t i = 0; i < active_jars.size(); i++) {
            active_jars[i].update(dt, speed);

            if (active_jars[i].is_active() && player_box.colliderect(active_jars[i].get_rect())) {
                active_jars[i].collect();
                PG::Global::score += 5;
                if (PG::Global::score > PG::Global::high_score) {
                    PG::Global::high_score = PG::Global::score;
                }
            }
        }

        if (!active_jars.empty() && (active_jars.front().is_offscreen() || !active_jars.front().is_active())) {
            active_jars.erase(active_jars.begin());
        }
    }
    
    void draw(PG::Window& app) {
        for (const auto& jar : active_jars) {
            if (jar.is_active()) {
                honey_sprite.draw(app, (int)jar.get_x(), (int)jar.get_y(), (int)jar.get_width(), (int)jar.get_height());
            }
        }
    }
};