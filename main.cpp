#include "PygamePP.h"
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "menu.cpp"
#include "select.cpp"
#include "bee.cpp"
#include "wasp.cpp"
#include "flower.cpp"
#include "item.cpp"


#undef main

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GROUND_Y = 595.0f;

int game_state = 0;
int chosen_character = 0;
std::string current_track = "";

int main() {
    srand(static_cast<unsigned int>(time(0)));

    PG::Window app("Flappy Bee II: The Buzz Continues", SCREEN_WIDTH, SCREEN_HEIGHT);
    app.set_icon("icon.jpg");

    ScrollingBackground gameplay_bg(app, "Background.png", 140.0f);
    Bee bee_player(app);
    Wasp wasp_player(app);
    FlowerManager flower_manager(app);
    ItemManager item_manager(app);

    PG::Font font("OpenSans_Condensed-MediumItalic.ttf", 28);
    PG::Font large_font("OpenSans_Condensed-MediumItalic.ttf", 48);

    Uint32 last_ticks = PG::get_ticks();

    while (app.is_running()) {
        Uint32 current_ticks = PG::get_ticks();
        float dt = (current_ticks - last_ticks) / 1000.0f;
        last_ticks = current_ticks;
        if (dt > 0.1f) dt = 0.1f;
        PG::Global::dt = dt;

        PG::Event event;
        while (PG::poll_event(event)) {
            if (event.type == PG::QUIT) {
                app.stop();
            }

            if (event.type == PG::KEYDOWN) {
                if (game_state == 0) {
                   if (event.key == PG::KEY_SPACE) {
                       PG::Music::stop();
                       PG::Global::score = 0;
                       game_state = 1;
                       last_ticks = PG::get_ticks();
                       continue;
                    }
                }
               
                else if (game_state == 1) {
                    if (event.key == PG::KEY_LEFT) PG::Global::score = 0;
                    if (event.key == PG::KEY_RIGHT) PG::Global::score = 1;

                    if (event.key == PG::KEY_SPACE) {
                        PG::Music::stop();
                        chosen_character = PG::Global::score;
                        PG::Global::reset();
                        bee_player.reset();
                        wasp_player.reset();
                        flower_manager.reset();
                        item_manager.reset();
                        game_state = 2;
                        last_ticks = PG::get_ticks();
                        continue;
                    }
                }

                else if (game_state == 2) {
                    if (event.key == PG::KEY_SPACE) {
                        if (chosen_character == 0) bee_player.jump();
                        else wasp_player.jump();
                    }
                }
                else if (game_state == 3) {
                    if (event.key == PG::KEY_SPACE) {
                        PG::Music::stop();
                        game_state = 0;
                        last_ticks = PG::get_ticks();
                        continue;
                    }
                }
            }
        }
    

        if (game_state == 2) {
           gameplay_bg.update(dt, true);

           float current_player_x = 150.0f;
           PG::Rect player_hitbox;

           if (chosen_character == 0) {
              bee_player.play_theme();
              bee_player.update(dt, true);
              player_hitbox = bee_player.get_rect();
            } else {
              wasp_player.play_theme();
              wasp_player.update(dt, true);
              player_hitbox = wasp_player.get_rect();
            }

            size_t old_flower_count = flower_manager.get_obstacles().size();
            flower_manager.update(dt, true, current_player_x);

            if (flower_manager.get_obstacles().size() > old_flower_count) {
               const auto& new_flower = flower_manager.get_obstacles().back();
               item_manager.check_and_spawn(new_flower.get_x(), new_flower.get_y(), new_flower.get_tall_status());
            }

            item_manager.update(dt, flower_manager.get_speed(), player_hitbox);

            PG::Rect floor_box(0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y);
            if (player_hitbox.colliderect(floor_box)) {
                game_state = 3;
            }
            
            for (const auto& flower : flower_manager.get_obstacles()) {
               if (player_hitbox.colliderect(flower.get_rect())) {
                   game_state = 3;
                }
            }
        }

        app.clear();

        if (game_state == 0) {
            run_menu_state(app, font, large_font, dt);
        }
        else if (game_state == 1) {
            run_character_select_state(app, font, large_font);
        }
        else if (game_state == 2 || game_state == 3) {
           gameplay_bg.draw(app);
           flower_manager.draw(app);
           item_manager.draw(app);

           if (chosen_character == 0) bee_player.draw(app);
           else wasp_player.draw(app);
           
           auto render_text = [&](PG::Font& target_font, const std::string& text, int x, int y, SDL_Color color) {
              SDL_Color black = {0, 0, 0, 255};
              target_font.render(app, text, x - 2, y, black);
              target_font.render(app, text, x + 2, y, black);
              target_font.render(app, text, x, y - 2, black);
              target_font.render(app, text, x, y + 2, black);
              target_font.render(app, text, x, y, color);
            };

            if (game_state == 2) {
               std::string score_msg = "Score: " + std::to_string(PG::Global::score);
               render_text(font, score_msg, 25, 20, {255, 255, 255, 255});
            }
            else if (game_state == 3) {
               render_text(large_font, "GAME OVER", 300, 180, {220, 20, 60, 255});
               render_text(font, "FINAL SCORE: " + std::to_string(PG::Global::score), 325, 250, {255, 255, 255, 255});
               render_text(font, "BEST DISTANCE: " + std::to_string(PG::Global::high_score), 315, 290, {255, 215, 0, 255});
               render_text(font, "PRESS SPACE TO RETURN TO MENU", 240, 370, {200, 200, 200, 255}); 
            }
        }

        app.display();

        #ifdef __EMSCRIPTEN__
            emscripten_sleep(0);
        #endif
    }

    return 0;
}