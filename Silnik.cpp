#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <cmath>
#include <time.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h> 
#include <allegro5\allegro_acodec.h> 
#include <stdio.h>

using namespace std;

class Engine
{
public:

    void lib_all_init(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT events, bool fullscreen);
    void lib_all_init(ALLEGRO_DISPLAY* display, bool fullscreen);
    int  wyswietl_blad();
    void destroy(ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* event_queue);

};
void Engine::lib_all_init(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT events, bool fullscreen)
{
    if ((events.type == ALLEGRO_EVENT_KEY_DOWN) && (events.keyboard.keycode == ALLEGRO_KEY_F)) {
        fullscreen = !fullscreen;
        al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);
    }

}
void Engine::lib_all_init(ALLEGRO_DISPLAY* display, bool fullscreen)
{
    al_set_new_display_option(ALLEGRO_COLOR_SIZE, 32, ALLEGRO_REQUIRE);

    fullscreen = false;
    if (fullscreen)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);

    }

}
int Engine::wyswietl_blad()
{
    if (!al_init()) return -1;
}
void Engine::destroy(ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* event_queue)
{
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

int dolzina = 800;
int visina = 600;

int main() {

    Engine engine;
    ALLEGRO_DISPLAY* display;
    const float FPS = 60.0;
    const float frameFPS = 15.0;
    bool quit = false;
    bool fullscreen = false;

    engine.wyswietl_blad();
    display = al_create_display(dolzina, visina);
    if (!display) return -1;
    al_set_window_position(display, 200, 200);
    if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    bool done = false, active = false;
    bool draw = true;
    int x = 0, y = 0, moveSpeed = 5;


    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();


    ALLEGRO_BITMAP* glava = al_load_bitmap("glava.png");
    ALLEGRO_BITMAP* telo = al_load_bitmap("telo.png");
    ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
    ALLEGRO_FONT* font1 = al_load_font("Minecrafter.Alt.ttf", 15, 0);
    ALLEGRO_FONT* mcfont = al_load_font("Minecrafter.Alt.ttf", 30, 0);
    ALLEGRO_BITMAP* coin1 = al_load_bitmap("coin.png");



    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 10);
    ALLEGRO_TIMER* frameTimer = al_create_timer(1.0 / frameFPS);
    ALLEGRO_TIMER* VREME = al_create_timer(1);
    ALLEGRO_KEYBOARD_STATE keyState;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(VREME));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_start_timer(timer);
    al_start_timer(frameTimer);
    al_start_timer(VREME);
    srand(time(NULL));

    engine.lib_all_init(display, fullscreen);

    const int maxF = 8;
    int curF = 0;
    int frameC = 0;
    int frameD = 2;
    int frameW = 40;
    int frameH = 40;
    int timeS = 0;
    int timeF = 0;

    enum Direction { DOWN, LEFT, RIGHT, UP };

    bool sdasda;
    int dir = DOWN;
    int score = 1;
    int lastX;
    int lastY;

    int coinX = 40 * (rand() % 20);
    int coinY = 40 * (rand() % 15);

    int snakeT[50];
    for (int i = 0; i <= 50; i++) {
        snakeT[i] = 0;
    }
    int snakeX[50], snakeY[50];

    bool menu = true;
    bool dead = false;
    while (!done) {



        lastX = x;
        lastY = y;


        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        engine.lib_all_init(display, events, fullscreen);


        if (events.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (events.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_ENTER:
                if (menu) menu = false, score = 1, timeS = 0, x = 0, y = 0;
                break;
            case ALLEGRO_KEY_F:
                /**/
                break;

            }
        }
        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;

        if (events.type == ALLEGRO_EVENT_TIMER) {
            if (events.timer.source == VREME) timeS++;
            if (events.timer.source == timer) {

                al_get_keyboard_state(&keyState);
                if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && dir != LEFT)
                    dir = RIGHT;
                else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && dir != RIGHT)
                    dir = LEFT;
                else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && dir != DOWN)
                    dir = UP;
                else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && dir != UP)
                    dir = DOWN;
                else if (al_key_down(&keyState, ALLEGRO_KEY_A))
                    score++;
                else if (al_key_down(&keyState, ALLEGRO_KEY_ENTER) && menu == true)
                    menu = false, score = 1, timeS = 0, x = 0, y = 0;
                if (menu == false) {
                    if (score != 0) {
                        for (int i = score; i > 0; i--) {
                            snakeX[i] = snakeX[i - 1];
                            snakeY[i] = snakeY[i - 1];
                        }
                        snakeX[0] = lastX;
                        snakeY[0] = lastY;
                    }
                }

                switch (dir) {
                case RIGHT: x = x + 40;
                    break;
                case LEFT: x = x - 40;
                    break;
                case UP: y = y - 40;
                    break;
                case DOWN: y = y + 40;
                    break;
                }

                if (x == coinX && y == coinY) {
                    score++;

                    coinX = 40 * (rand() % 20);
                    coinY = 40 * (rand() % 15);
                    snakeT[score] = 1;
                }

                if (menu == false) {
                    for (int i = 0; i < score; i++) {
                        if (x == snakeX[i] && y == snakeY[i] && menu == false) dead = true;
                    }
                    if (x < 0 || x >= 800 || y < 0 || y >= 600 && menu == false) dead = true;

                }

                draw = true;

            }
        }
        if (++frameC >= frameD) {
            if (++curF >= maxF)
                curF = 0;

            frameC = 0;
        }

        if (dead && menu == false) {
            menu = true;
            timeF = timeS;
            x = 0, y = 0;
            for (int i = 0; i <= 50; i++) {
                snakeT[i] = 0;
            }
            dead = false;
            dir = DOWN;
        }

        al_draw_bitmap(background, 0, 0, 0);

        if (draw == true) {
            draw = false;
            if (menu) {
                x = 0, y = 0;
                for (int i = 0; i <= 50; i++) {
                    snakeT[i] = 0;
                }
                al_draw_text(mcfont, al_map_rgb(221, 0, 0), 200, 100, 60,
                    "Press Enter to Start");
                al_draw_text(mcfont, al_map_rgb(251, 96, 11), 225, 200, 0,
                    "Press Esc to Exit");
                al_draw_textf(mcfont, al_map_rgb(0, 0, 0), 120, 350, 0,
                    "Coins: %i", score - 1);
                al_draw_textf(mcfont, al_map_rgb(0, 0, 0), 470, 350, 0,
                    "Time: %i sec", timeF);
            }
            else {
                //   al_draw_bitmap(coin, coinX, coinY, NULL);
                al_draw_bitmap_region(coin1, curF * frameW, 0, frameW, frameH, coinX, coinY, 0);
                for (int i = 0; i < score; i++) {
                    al_draw_bitmap(telo, snakeX[i], snakeY[i], NULL);
                }

                al_draw_bitmap(glava, x, y, NULL);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 5, 5, 0,
                    "Coins: %i", score - 1);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 705, 5, 0,
                    "Time: %i", timeS);
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }




    }

    engine.destroy(display, timer, event_queue);
    return 0;
}
