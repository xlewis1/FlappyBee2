#include "PygamePP.h"
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Bee {
private:
   PG::Sprite wingsUp;
   PG::Sprite wingsDown;
   float x, y;
   float width, height;
   float velocity;

   // Calibrated physics parameters: lower gravity and softer jump for a floatier feel
   const float gravity = 1100.0f;
   const float jump_strength = -370.0f;

   float anim_timer;
   bool use_wings_up;

public:
    Bee(PG::Window& app) 
        : wingsUp(app, "Bumble_1.png"), wingsDown(app, "Bumble_2.png"),
          x(150.0f), y(200.0f), velocity(0.0f), anim_timer(0.0f), use_wings_up(true) {
        width = 80.0f;
        height = 75.0f;
    }

    void play_theme() {
       static PG::Music bee_bgm("beetheme.wav");
       extern std::string current_track;

        if (current_track != "bee") {
            PG::Music::stop();
            bee_bgm.play(-1);
            current_track = "bee";
        }
    }
    
    void reset() {
        y = 200.0f;
        velocity = 0.0f;
    }

    void jump() {
        velocity = jump_strength;
    }

    void update(float dt, bool active) {
        if (!active) {
            anim_timer += dt;
            if (anim_timer > 0.10f) {
                use_wings_up = !use_wings_up;
                anim_timer = 0.0f;
            }
            return;
        }

        velocity += gravity * dt;
        y += velocity * dt;

        if (y < 0) {
            y = 0;
            velocity = 0;
        }

        anim_timer += dt;
        if (anim_timer > 0.15f) {
            use_wings_up = !use_wings_up;
            anim_timer = 0.0f;
        }
    }

    void draw(PG::Window& app) {
        if (use_wings_up) {
            wingsUp.draw(app, (int)x, (int)y, (int)width, (int)height);
        } else {
            wingsDown.draw(app, (int)x, (int)y, (int)width, (int)height);
        }
    }

    PG::Rect get_rect() const {
        return PG::Rect(x + 6, y + 4, width - 12, height - 8);
    }
};