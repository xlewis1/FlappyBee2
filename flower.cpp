#include "PygamePP.h"
#include <vector>
#include <cstdlib>

class FlowerObstacle {
private:
   float x, y;
   float width, height;
   bool is_tall;
   bool passed;

   bool will_grow;
   bool is_growing;
   float target_height;
   float target_y;

public:
   FlowerObstacle(float start_x, bool tall) : x(start_x), is_tall(tall), passed(false) {
       width = 180.0f;

       will_grow = (!is_tall && (rand() % 100 < 35));
       is_growing = false;

       height = is_tall ? 500.0f : 380.0f;
       float visual_y_offset = is_tall ? 130.0f : 170.0f;
       y = (600.0f - height) + visual_y_offset;

       target_height = 500.0f;
       target_y = (600.0f - target_height) + 130.0f;
    }
    
    void update(float dt, float speed) {
        x -= speed * dt;

        if (will_grow && !is_growing && x < 650.0f) {
            is_growing = true;
        }

        if (is_growing && !is_tall) {
            float growth_speed = 250.0f; 
            height += growth_speed * dt;
            y -= growth_speed * dt; 

            if (height >= target_height) {
                height = target_height;
                y = target_y;
                is_tall = true;
                is_growing = false;
            }
        }
    }

    bool is_offscreen() const {
        return (x + width < 0);
    }

    PG::Rect get_rect() const {
        if (is_tall || is_growing) {
            return PG::Rect(x + 55, y + 50, width - 110, 120.0f);
        } else {
            return PG::Rect(x + 55, y + 150, width - 110, 100.0f);
        }
    }

    bool get_tall_status() const { return is_tall; }
    bool get_growing_status() const { return is_growing; }
    float get_x() const { return x; }
    bool has_been_scored() const { return passed; }
    void mark_scored() { passed = true; }

    float get_width() const { return width; }
    float get_y() const { return y; }
    float get_height() const { return height; }
};

class FlowerManager {
private:
    PG::Sprite short_flower;
    PG::Sprite tall_flower;
    std::vector<FlowerObstacle> active_flowers;
    
    float spawn_timer;
    float current_spawn_interval;
    float game_speed;

public:
    FlowerManager(PG::Window& app) 
        : short_flower(app, "flower_1.png"), tall_flower(app, "Flower_2.png"),
          spawn_timer(0.0f), game_speed(300.0f) {
        current_spawn_interval = 2.3f;
    }

    void reset() {
        active_flowers.clear();
        spawn_timer = 0.0f;
        current_spawn_interval = 2.3f;
    }

    void update(float dt, bool active, float player_x) {
        if (!active) return;

        spawn_timer += dt;
        if (spawn_timer >= current_spawn_interval) {
            bool pick_tall = (rand() % 2 == 0);
            active_flowers.push_back(FlowerObstacle(800.0f + 150, pick_tall));
            
            current_spawn_interval = 2.0f + static_cast<float>(rand() % 50) / 100.0f;
            spawn_timer = 0.0f;
        }

        for (size_t i = 0; i < active_flowers.size(); i++) {
            active_flowers[i].update(dt, game_speed);

            if (!active_flowers[i].has_been_scored() && active_flowers[i].get_x() + 80.0f < player_x) {
                PG::Global::score++;
                if (PG::Global::score > PG::Global::high_score) {
                    PG::Global::high_score = PG::Global::score;
                }
                active_flowers[i].mark_scored();
            }
        }

        if (!active_flowers.empty() && active_flowers.front().is_offscreen()) {
            active_flowers.erase(active_flowers.begin());
        }
    }

    void draw(PG::Window& app) {
        for (const auto& flower : active_flowers) {
            int fx = (int)flower.get_x();
            int fy = (int)flower.get_y();
            int fw = (int)flower.get_width();
            int fh = (int)flower.get_height();

            if (flower.get_tall_status() || flower.get_growing_status()) {
                tall_flower.draw(app, fx, fy, fw, fh);
            } else {
                short_flower.draw(app, fx, fy, fw, fh);
            }
        }
    }

    const std::vector<FlowerObstacle>& get_obstacles() const {
        return active_flowers;
    }
    
    float get_speed() const { return game_speed; }
};