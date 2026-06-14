#include "PygamePP.h"

class ScrollingBackground {
private:
   PG::Sprite tex;
   float x1, x2;
   float speed;

public:
   ScrollingBackground(PG::Window& app, std::string path, float scroll_speed) 
        : tex(app, path), x1(0.0f), x2(800.0f), speed(scroll_speed) {}

    void update(float dt, bool active) {
        if (!active) return;
        x1 -= speed * dt;
        x2 -= speed * dt;

        if (x1 <= -800.0f) x1 = x2 + 800.0f;
        if (x2 <= -800.0f) x2 = x1 + 800.0f;
    }

    void draw(PG::Window& app) {
        tex.draw(app, (int)x1, 0, 800, 600);
        tex.draw(app, (int)x2, 0, 800, 600);
    }
};

void run_menu_state(PG::Window& app, PG::Font& font, PG::Font& large_font, float dt) {
    static PG::Music menu_bgm("menu.wav");
    extern std::string current_track;

    if (current_track != "menu") {
        menu_bgm.play(-1);
        current_track = "menu";
    }
    
    static PG::Sprite logo(app, "logo.png");
    static ScrollingBackground menu_bg(app, "Background.png", 140.0f);

    auto render_with_outline = [&](PG::Font& target_font, const std::string& text, int x, int y, SDL_Color text_color) {
        SDL_Color black = {0, 0, 0, 255};
        target_font.render(app, text, x - 2, y, black);
        target_font.render(app, text, x + 2, y, black);
        target_font.render(app, text, x, y - 2, black);
        target_font.render(app, text, x, y + 2, black);
        target_font.render(app, text, x, y, text_color);
    };

    menu_bg.update(dt, true);
    
    menu_bg.draw(app);

    logo.draw(app, 100, 60, 600, 300);

    render_with_outline(font, "PRESS SPACE TO START", 300, 420, {255, 255, 255, 255});

    std::string hi_score_msg = "BEST DISTANCE: " + std::to_string(PG::Global::high_score);
    render_with_outline(font, hi_score_msg, 325, 480, {180, 180, 180, 255});
}