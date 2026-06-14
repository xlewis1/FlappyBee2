#include "PygamePP.h"

void run_character_select_state(PG::Window& app, PG::Font& font, PG::Font& large_font) {
    static PG::Music select_bgm("select.wav");
    extern std::string current_track;

    if (current_track != "select") {
        select_bgm.play(-1);
        current_track = "select";
    }

    static PG::Sprite bg_sky(app, "background.png");
    static float bg_x = 0.0f;

    bg_x -= 30.0f * PG::Global::dt; 
    if (bg_x <= -800) {
    }

    bg_sky.draw(app, (int)bg_x, 0, 800, 600);
    bg_sky.draw(app, (int)bg_x + 800, 0, 800, 600);

    SDL_Color panel_black = {0, 0, 0, 180};
    app.draw_rect(PG::Rect(100, 65, 600, 110), panel_black);

    static PG::Sprite bee_icon(app, "bee_icon.png");
    static PG::Sprite wasp_icon(app, "wasp_icon.png");

    const int SCREEN_WIDTH = 800;

    auto render_with_outline = [&](PG::Font& target_font, const std::string& text, int x, int y, SDL_Color text_color) {
        SDL_Color black = {0, 0, 0, 255};
        target_font.render(app, text, x - 2, y, black);
        target_font.render(app, text, x + 2, y, black);
        target_font.render(app, text, x, y - 2, black);
        target_font.render(app, text, x, y + 2, black);
        target_font.render(app, text, x, y, text_color);
    };

    render_with_outline(large_font, "SELECT YOUR FLYER", 240, 80, {255, 255, 255, 255});
    render_with_outline(font, "Use LEFT / RIGHT Arrows & Press SPACE to Lock In", 185, 140, {200, 200, 200, 255});

    int bee_x = 160;
    int wasp_x = 460;
    int box_y = 220;
    int box_size = 180;

    SDL_Color highlight_color = {255, 215, 0, 255};
    SDL_Color dark_color = {50, 50, 50, 255};

    if (PG::Global::score == 0) {
        // We'll let main.cpp handle the actual input modifier state switching
    }

    
    app.fill_rect(PG::Rect(bee_x - 10, box_y - 10, box_size + 20, box_size + 20), (PG::Global::score == 0) ? highlight_color : dark_color);
    app.fill_rect(PG::Rect(wasp_x - 10, box_y - 10, box_size + 20, box_size + 20), (PG::Global::score == 1) ? highlight_color : dark_color);

    bool is_bee_selected = (PG::Global::score == 0);

    bee_icon.draw(app, bee_x, box_y, box_size, box_size);
    wasp_icon.draw(app, wasp_x, box_y, box_size, box_size);

    if (is_bee_selected) {
       render_with_outline(large_font, "BUMBLE BEE", 310, 440, {255, 215, 0, 255});
       render_with_outline(font, "Stats: Smooth handling, floaty gravity.", 240, 500, {255, 255, 255, 255});
    } else {
       render_with_outline(large_font, "RAZOR WASP", 315, 440, {220, 20, 60, 255});
       render_with_outline(font, "Stats: Hyper fast, aggressive dive drops.", 235, 500, {255, 255, 255, 255});
    }
}