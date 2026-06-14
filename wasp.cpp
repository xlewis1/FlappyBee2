#include "PygamePP.h"

class Wasp {
private:
   PG::Sprite wingsUp;
   PG::Sprite wingsDown;
   float x, y;
   float width, height;
   float velocity;

   const float gravity = 1500.0f;
   const float jump_strength = -460.0f;

   float anim_timer;
   bool use_wings_up;

public:
    Wasp(PG::Window& app) 
        : wingsUp(app, "Wasp_1.png"), wingsDown(app, "wasp_2.png"),
          x(150.0f), y(200.0f), velocity(0.0f), anim_timer(0.0f), use_wings_up(true) {
        width = 80.0f;
        height = 75.0f;
    }

    void play_theme() {
       static PG::Music wasp_bgm("wasptheme.wav");
       extern std::string current_track;

        if (current_track != "wasp") {
            PG::Music::stop();
            wasp_bgm.play(-1);
            current_track = "wasp";
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
        if (anim_timer > 0.04f) {
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