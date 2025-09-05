

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>


using namespace std;
using namespace sf;

//int musicvolume;
//int miscvolume;

string checkdirection() {
    // WASD
    if (GetAsyncKeyState('A') & 0x8000) return "a";
    else if (GetAsyncKeyState('W') & 0x8000) return "w";
    else if (GetAsyncKeyState('S') & 0x8000) return "s";
    else if (GetAsyncKeyState('D') & 0x8000) return "d";

    // Arrow keys
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) return "a";
    else if (GetAsyncKeyState(VK_UP) & 0x8000) return "w";
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) return "s";
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) return "d";

    else return "o";
}

int begin_y = rand() % 10;
int begin_x = rand() % 10;
const int amountoftalesonscreen = 22;
string gamemap[amountoftalesonscreen][amountoftalesonscreen], gamemap_save[amountoftalesonscreen][amountoftalesonscreen], menu[amountoftalesonscreen][amountoftalesonscreen];
int coinrand = 20;
int talewidthinpx;


string direction = "w";
int coinscreated = 0;
int y_hero = begin_y;
int x_hero = begin_x;
int y_ghost = begin_y;
int x_ghost = begin_x;
int ghost_time = 4;
int time_passed = 0;
float talescale, heroscale, fps;
int difficulty_index = -2;
bool startbutton_clicked = false;
bool ghost_allowed = false;
Texture field_texture, wall_texture, crimson_wall_texture1, crimson_wall_texture2, crimson_wall_texture3, coin_texture, hero_texture, hero_texture_up1, hero_texture_up2, hero_texture_down1, hero_texture_down2, hero_texture_left, hero_texture_right, ghost_texture, bg_start_texture, bg_difficulty_texture, black_bg_texture, tick_texture, game_over_bg_texture, settings_button_texture, replay_button_texture, menu_button_texture;
Sprite hero, ghost, replay_button, settings_button, menu_button;
Color bgColor;
Text score_text, countdown_text, start_text, easy_difficulty_text, medium_difficulty_text, hard_difficulty_text, impossible_difficulty_text, quit_text, settings_text, music_text, back_text, score_only_text, score_int_text, score_best_int_text, score_best_new_only_text, score_best_only_text;
Font font;
RenderWindow window;
int plussize, talesize, coinsscore, save_y, timespassed, score, best_score;
bool animationbool;
time_t start_timer;
int countcoins();
int reset_variables();
int talescreated, screenpxfr;
bool startscreen = false;
bool gamescreen = true;
bool difficultyscreen = false;
bool settingsscreen = false;
bool difficulty_animation_should = false;
bool game_over = false;

bool backtogame = false;

int difficulty_level = 1;
int coinscollected;
int levelscompleted;
int music_volume = 100;

int ghostdistance;
int ghostscore;

bool musicon = true;

sf::RectangleShape ticksquare(sf::Vector2f(40, 40));
auto start_score_timer = std::chrono::high_resolution_clock::now();



string create_gamemap(int difficulty) {

    srand(time(0)); // <-- делает rand() случайным каждый раз

    gamemap[amountoftalesonscreen][amountoftalesonscreen];
    int amountofwalls = (rand() % amountoftalesonscreen / 2 + amountoftalesonscreen * amountoftalesonscreen / 10) + difficulty * 2;
    cout << "amountofwalls" << amountofwalls << endl;

    int y_walls_amount = amountofwalls / 2;
    cout << "y_walls_amount" << y_walls_amount << endl;
    int x_walls_amount = amountofwalls - y_walls_amount;

    int y_wall_coordinate = begin_y;
    int x_wall_coordinate = begin_x;
    bool goplus = true;
    int y_save, x_save;


    for (int i = 0; i < amountofwalls; i++) {
        int y_wall_length = rand() % (amountoftalesonscreen / 3 - 1) + amountoftalesonscreen / 6;
        int x_wall_length = rand() % (amountoftalesonscreen / 3 - 1) + amountoftalesonscreen / 6;

        if (y_walls_amount > 0) {

            if ((y_wall_coordinate + y_wall_length < amountoftalesonscreen) and (y_wall_coordinate - y_wall_length >= 0)) {
                goplus = rand() % 2 == 1;
            }
            else if (y_wall_coordinate + y_wall_length < amountoftalesonscreen) {
                goplus = true;
            }
            else {
                goplus = false;
            }


            for (int x = 0; x < y_wall_length; x++) {
                gamemap[y_wall_coordinate][x_wall_coordinate] = "tale";
                if (rand() % coinrand == 0) {
                    gamemap[y_wall_coordinate][x_wall_coordinate] = "coin";
                    coinscreated++;
                }
                goplus ? y_wall_coordinate++ : y_wall_coordinate--;
            }
            y_walls_amount--;
        }

        if (x_walls_amount > 0) {
            if (i == 0) {
                y_save = y_wall_coordinate;
                x_save = x_wall_coordinate;
                y_wall_coordinate = begin_y;
                x_wall_coordinate = begin_x;
            }
            if (i == 1) {
                x_wall_coordinate = x_save;
                x_wall_coordinate = x_save;
                x_wall_length = 3 + rand() % 5;
            }
            if ((x_wall_coordinate + x_wall_length < amountoftalesonscreen) and (x_wall_coordinate - x_wall_length >= 0)) {
                goplus = rand() % 2 == 1;
            }
            else if (x_wall_coordinate + x_wall_length < amountoftalesonscreen) {
                goplus = true;
            }
            else {
                goplus = false;
            }

            for (int x = 0; x < x_wall_length; x++) {
                gamemap[y_wall_coordinate][x_wall_coordinate] = "tale";
                if (rand() % coinrand == 0) {
                    cout << " randmoin" << endl;
                    gamemap[y_wall_coordinate][x_wall_coordinate] = "coin";
                    coinscreated++;
                }

                goplus ? x_wall_coordinate++ : x_wall_coordinate--;
            }

            x_walls_amount--;
        }
    }
    //if no coins are created, the map gets recreated without touching other variables like fps, ghost_allowed, difficulty, etc...
    int countedcoins = countcoins();
    if (countedcoins < 1) {
        srand(time(0) + (rand() % 1000)); // the rand%1000 is meant for the random part (described higher up) :)
        create_gamemap(difficulty);
    }
    return "gameMap created";
}

//fills the map with bloody walls
string fill_map() {
    for (int i = 0; i < amountoftalesonscreen; i++) {
        for (int x = 0; x < amountoftalesonscreen;x++) {
            //int wall_or_field = rand() % 2;
            //cout << wall_or_field;

            if (gamemap[i][x] != "tale" and gamemap[i][x] != "coin") {
                int crimson123 = rand() % 3;
                if (rand() % 10 != 0) {
                    gamemap[i][x] = "wall";
                }

                else {
                    //cout << "pisu=" << endl;
                    if (crimson123 == 0) {
                        gamemap[i][x] = "wall1";
                        //cout << "popo=" << endl;
                    }
                    else if (crimson123 == 1) {
                        gamemap[i][x] = "wall2";
                    }
                    else if (crimson123 == 2) {
                        gamemap[i][x] = "wall3";
                    }

                }
            }



            //cout << gamemap[i][x] << " ";
        }
        cout << endl;

    }
    return "map filled";

}

int do_ghost() {

    //create list with 4 elements, each with a list: in each by-list is going to be 3 elements: y,x,distance
    //sort by distance for (list[1-4][2])
    // the least distance is the move for us, so we move to y[best][0];x[best][1]


    float after_move[4][3]; //after_move(coords and distance)
    for (int i = 0;i < 4;i++) {
        after_move[i][0] = y_ghost;
        after_move[i][1] = x_ghost;
    }
    //now, that we've filled the blank lists, we add the differences:   
    //                         direction
    //0.y  x+     distance     right
    //1.y+ x      distance     down
    //2.y  x-     distance     left
    //3.y- x      distance     up
    after_move[0][1] += 1;
    after_move[1][0] += 1;
    after_move[2][1] -= 1;
    after_move[3][0] -= 1;


    for (int i = 0;i < 4;i++) {
        float ysq = pow((y_hero - after_move[i][0]), 2);//after_move[i][0] would be ghost's y coordinate after the move, "i" is the direction
        float xsq = pow((x_hero - after_move[i][1]), 2);//after_move[i][1] would be ghost's x coordinate after the move
        float resultsq = sqrt(ysq + xsq);
        //cout << "result number "<<i<< ": " << resultsq << endl;

        after_move[i][2] = resultsq;
        //cout <<"aftermove"<< after_move[i][2] << endl;
    }
    int leastnum;
    float least = 100000000000;
    //getting the cheapest path
    for (int i = 0; i < 4;i++) {
        int after_move_y = after_move[i][0];
        int after_move_x = after_move[i][1];
        //to not go through walls
        if ((least > after_move[i][2] and gamemap[after_move_y][after_move_x] == "tale") or (least > after_move[i][2] and gamemap[after_move_y][after_move_x] == "coin")) {
            least = after_move[i][2];
            leastnum = i;
        }
    }

    
    
    y_ghost = after_move[leastnum][0];
    x_ghost = after_move[leastnum][1];


    //putting the distance from ghost to player in a variable that is then used to count score
    ghostdistance = -(after_move[leastnum][2] - 12);
    if (after_move[leastnum][2] > 16) {
        ghostdistance = -50;
    }

    return 1;
}



int check_if_dead() {
    //if the ghost is on the same square as the hero is on, hero dies
    if (y_ghost == y_hero and x_ghost == x_hero) {
        game_over = true;
        gamescreen = false;
    }
    return 1;
}

int countcoins() {
    coinscreated = 0;
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            if (gamemap[i][x] == "coin") {
                coinscreated += 1;

            }
        }
    }
    return coinscreated;


}

string print_countdown(int screenpxfr, Font font) {
    countdown_text.setString(to_string(-(time_passed - 4)));
    countdown_text.setFont(font);
    countdown_text.setCharacterSize(100);

    countdown_text.setFillColor(Color(rand() % 56 + 200, rand() % 206 + 50, rand() % 206 + 50));
    countdown_text.setPosition(325.f, 150.f);
    window.draw(countdown_text);

    return "1";
}


string setupmap_new(int difficulty) {
    //fill the whole map with walls
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap[i][x] = "wall";
        }
    }
    //fill some with tales(blank spaces) and create a few coins
    create_gamemap(difficulty);
    cout << "finished" << endl;
    //fill the walls with random bloody walls
    fill_map();
    //count the coins
    countcoins();

    //save the map so it will reset after every hero move(i know its unoptimized stfu)
    gamemap_save[amountoftalesonscreen][amountoftalesonscreen];
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap_save[i][x] = gamemap[i][x];
        }
    }

    //gamemap[y_hero][x_hero] = "1";


    return"lol";
}

//texture AND position after button press
int hero_set_texture() {
    if (direction == "a") {
        if (x_hero - 1 >= 0 && (gamemap[y_hero][x_hero - 1] == "tale" or gamemap[y_hero][x_hero - 1] == "coin"))
            x_hero--;
        else if (x_hero == 0 && (gamemap[y_hero][amountoftalesonscreen - 1] == "tale" or gamemap[y_hero][x_hero - 1] == "coin"))
            x_hero = amountoftalesonscreen - 1;

        hero.setTexture(hero_texture_left);
    }
    else if (direction == "d") {
        if (x_hero + 1 < amountoftalesonscreen && (gamemap[y_hero][x_hero + 1] == "tale" or gamemap[y_hero][x_hero + 1] == "coin"))
            x_hero++;
        else if (x_hero == amountoftalesonscreen - 1 && (gamemap[y_hero][0] == "tale" or gamemap[y_hero][0] == "coin"))
            x_hero = 0;

        hero.setTexture(hero_texture_right);
    }
    else if (direction == "s") {
        if (y_hero + 1 < amountoftalesonscreen && (gamemap[y_hero + 1][x_hero] == "tale" or gamemap[y_hero + 1][x_hero] == "coin"))
            y_hero++;
        else if (y_hero == amountoftalesonscreen - 1 && (gamemap[0][x_hero] == "tale" or gamemap[0][x_hero] == "coin"))
            y_hero = 0;

        if (animationbool) {
            hero.setTexture(hero_texture_down1);
            animationbool = false;
        }
        else {
            hero.setTexture(hero_texture_down2);
            animationbool = true;
        }
    }
    else if (direction == "w") {
        if (y_hero - 1 >= 0 && (gamemap[y_hero - 1][x_hero] == "tale" or gamemap[y_hero - 1][x_hero] == "coin"))
            y_hero--;
        else if (y_hero == 0 && (gamemap[amountoftalesonscreen - 1][x_hero] == "tale" or gamemap[amountoftalesonscreen - 1][x_hero] == "coin"))
            y_hero = amountoftalesonscreen - 1;

        if (animationbool) {
            hero.setTexture(hero_texture_up1);
            animationbool = false;
        }
        else {
            hero.setTexture(hero_texture_up2);
            animationbool = true;
        }
    }
    else {
        //cout << "idle hero texture" << endl;
        // стоим на месте
        hero.setTexture(hero_texture);
    }
    return 0;
}

int countscore() {
    //endind the score timer and getting the amount of ms that passed since you've started
    auto end_score_timer = std::chrono::high_resolution_clock::now();
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_score_timer - start_score_timer).count();

    //score=(ms/100 + coinscollected*100 + levelscompleted*500 + ghostcount*20) fps;
    ghostscore += ghostdistance;
    score = (coinscollected * 100 + levelscompleted * 500 + ghostscore) * fps;
    if (difficulty_level == 2) {
        score * 1.5;
    }
    else if (difficulty_level == 3) {
        score * 2;
    }
    else if (difficulty_level == 4) {
        score * 5;
    }

    return 1;
}

int drawmap() {

    int tailsplaced = 0;
    //отрисовка карты
    for (int i = 0; i < amountoftalesonscreen; i++) {
        for (int x = 0; x < amountoftalesonscreen;x++) {
            if (gamemap[i][x] == "tale") {


                Sprite field2(field_texture);
                //field2.setColor(sf::Color(100, 100, 100));

                field2.setPosition(plussize + x * talescale * talewidthinpx, plussize + i * talescale * talewidthinpx);
                field2.setScale(talescale, talescale);
                window.draw(field2);

                tailsplaced++;
            }
            else if (gamemap[i][x] == "wall" or gamemap[i][x] == "wall1" or gamemap[i][x] == "wall2" or gamemap[i][x] == "wall3") {
                Sprite wall;


                if (gamemap[i][x] == "wall") { wall.setTexture(wall_texture); }
                if (gamemap[i][x] == "wall1") { wall.setTexture(crimson_wall_texture1); }
                if (gamemap[i][x] == "wall2") { wall.setTexture(crimson_wall_texture2); }
                if (gamemap[i][x] == "wall3") { wall.setTexture(crimson_wall_texture3); }


                wall.setPosition(plussize + x * talescale * talewidthinpx, plussize + i * talescale * talewidthinpx);
                wall.setScale(talescale, talescale);
                window.draw(wall);

                tailsplaced++;
            }
            else if (gamemap[i][x] == "coin") {


                Sprite field2(field_texture);
                field2.setPosition(plussize + x * talescale * talewidthinpx, plussize + i * talescale * talewidthinpx);
                field2.setScale(talescale, talescale);
                window.draw(field2);
                //cout << "coim" << endl;

                Sprite coin(coin_texture);
                coin.setPosition(plussize + x * talescale * talewidthinpx, plussize + i * talescale * talewidthinpx);
                coin.setScale(talescale, talescale);
                window.draw(coin);
                tailsplaced++;
            }



        }

    }

    //check for coin
    if (gamemap[y_hero][x_hero] == "coin") {
        coinsscore++;
        coinscollected++;
        gamemap[y_hero][x_hero] = "tale";
        gamemap_save[y_hero][x_hero] = "tale";
        cout << "change";
    }

    //checking if all coins are picked up, if so then create a new map and reset variables
    if (coinsscore >= coinscreated) {
        reset_variables();
        levelscompleted++;
        cout << "reseted variables" << endl;
    }
    //if not all picked up then display the score
    else {
        //only count score if the game is running
        if (gamescreen) { countscore(); }
        score_text.setString("score: " + to_string(score) + "     fps:" + to_string(int(fps)));
    }

    window.draw (score_text); //cout << "coinscore" << coinsscore << "coinscreated" << coinscreated << endl;

    window.draw(hero);
    return 1;
}


int reset_variables() {
    //resetting the random pos variables
    //random pos now would be a COMPLETELY RANDOM POSITION ON THE MAP
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            if (gamemap[i][x] == "tale") {
                talescreated++;
            }
        }
    }
    int randtale = rand() % talescreated;

    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            randtale--;
            if (randtale == 1) {
                begin_y = i;
                begin_x = x;
            }
        }
    }

    y_hero = begin_y;
    x_hero = begin_x;
    y_ghost = begin_y;
    x_ghost = begin_x;

    start_timer = time(0);


    difficulty_index++;
    setupmap_new(difficulty_index);
    ghost_allowed = false;

    //increasing fps, game getting rapidly harder

    if (fps < 6 + difficulty_level) {
        fps++;
    }
    else if (difficulty_level == 4) {
        if (fps < 6 + difficulty_level * 2) {
            fps++;
        }
    }

    coinsscore = 0;
}

int reset_variables_fully() {

    reset_variables();
    levelscompleted = 0;
    ghostdistance = 0;
    coinscollected = 0;
    ghostscore = 0;
    ghost_allowed = false;

    return 1;
}

int gameoverscreen_do() {
    window.clear(bgColor);

    drawmap();

    RectangleShape darkOverlay(Vector2f(window.getSize()));
    darkOverlay.setFillColor(Color(0, 0, 0, 200)); // 100 = прозрачность


    window.draw(darkOverlay);  // потом затемнение

    Sprite bg(game_over_bg_texture);
    
    float bgscale = 1.25;

    //we recalculate the position for the scale beacause when trying to find the good-looking scale i kept fckin up
    bg.setPosition(400 - (364 / 2 * bgscale), 400 - (381 / 2 * bgscale) );
    bg.setScale(bgscale,bgscale);
    window.draw(bg);


    window.draw(replay_button);

    window.draw(settings_button);

    window.draw(menu_button);

    
    

    score_int_text.setString(to_string(score));

    window.draw(score_int_text);
    window.draw(score_only_text);

    if (score >= best_score) {
        ofstream Infofile;
        Infofile.open("fonts/Data.txt");
        Infofile << to_string(score);
        Infofile.close();
        best_score = score;

        score_best_new_only_text.setFillColor(Color(rand() % 56 + 200, rand() % 206 + 50, rand() % 206 + 50));
        window.draw(score_best_new_only_text);


    }

    window.draw(score_best_only_text);

    score_best_int_text.setString(to_string(score));
    window.draw(score_best_int_text);


    return 1;
}

int gamescreen_do() {

    if (direction != "o") {
        //cout << "CHCKING dirRECTION<<" << endl;
        string direction = checkdirection();
    }

    hero_set_texture();

    // устанавливаем позицию и масштаб — ОБЩИЕ для всех направлений
    hero.setPosition(plussize + x_hero * talescale * talewidthinpx, plussize + y_hero * talescale * talewidthinpx);
    hero.setScale(heroscale, heroscale);



    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap[i][x] = gamemap_save[i][x];
        }
    }
    //gamemap[y_hero][x_hero] = "1";



    //draws the entire map and all the othуr sprites
    drawmap();

    if (ghost_allowed) {
        do_ghost();
        //draw_ghost();
        //i could NOT put the drawing of a ghost in a function
        //😡
        ghost.setTexture(ghost_texture);
        ghost.setPosition(plussize + x_ghost * talescale * talewidthinpx, plussize + y_ghost * talescale * talewidthinpx);
        ghost.setScale(heroscale, heroscale);
        window.draw(ghost);

        check_if_dead();
    }


    time_t end_timer = time(0);
    time_passed = end_timer - start_timer;
    if (time_passed <= ghost_time - 1) {
        print_countdown(screenpxfr, font);
    }
    else {
        ghost_allowed = true;

    }
    /*
    hero.setPosition(plussize + x_hero * talescale * 4, plussize + y_hero * talescale * 4);
    hero.setScale(heroscale, heroscale);*/
    window.draw(hero);


    return 1;
}


int startscreen_do() {

    Sprite bg(bg_start_texture);
    bg.setPosition(0, 0);
    bg.setScale(1, 1);
    window.draw(bg);

    window.draw(start_text);
    window.draw(settings_text);
    window.draw(quit_text);

    return 1;
}

int settingsscreen_do() {

    

    Sprite bg(bg_start_texture);
    bg.setPosition(0, 0);
    bg.setScale(1, 1);
    window.draw(bg);



    ticksquare.setFillColor(sf::Color(100, 100, 100, 200));
    ticksquare.setPosition(230, 327);
    window.draw(ticksquare);

    if (musicon) {
        Sprite tick(tick_texture);
        tick.setPosition(227, 320);
        tick.setScale(1, 1);
        window.draw(tick);
    }

    window.draw(music_text);
    
    back_text.setPosition(20, 410);
    window.draw(back_text);


    return 1;
}

int difficultyscreen_do() {
    Sprite bg(bg_difficulty_texture);


    window.draw(bg);



    window.draw(easy_difficulty_text);
    window.draw(medium_difficulty_text);
    window.draw(hard_difficulty_text);
    window.draw(impossible_difficulty_text);

    back_text.setPosition(20, 560);
    window.draw(back_text);


    return 1;
}

int start_cutscene(int mode) {//1==start   2==difficulty
    Sprite bg;
    cout << "mode:" << mode << endl;
    //deciding textures
    if (mode == 1) {
        bg.setTexture(bg_start_texture);
        cout << "startmode" << endl;
    }
    else if (mode == 2) {
        bg.setTexture(bg_difficulty_texture);

        cout << "difficultymode" << endl;

    }

    Sprite black_bg(black_bg_texture);

    //i cycle, the background zooms up,
    //and
    //the black bg gamma goes up,
    //so basically you get an effect of you walking in the hall
    for (float i = 0; i < 3; i += 0.03) {
        window.clear(bgColor);

        bg.setPosition((-i * 400), (-i * 350));
        bg.setScale(1 + i, 1 + i);


        black_bg.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(0.33f * i * 255)));

        window.draw(bg);
        window.draw(black_bg);


        window.display();

        sf::sleep(sf::seconds(0.012));
    }
    //after the fade in, change bg and fade out
    sf::sleep(sf::seconds(0.5));
    window.clear(bgColor);

    //fade out:
    if (mode == 1) {

        bg.setTexture(bg_difficulty_texture);
        bg.setPosition(1, 1);
        bg.setScale(1, 1);
        for (float i = 3; i > 0; i -= 0.03) {
            window.clear(bgColor);


            int brightness_index = 0.33f * i * 255;

            black_bg.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(brightness_index)));

            difficultyscreen_do();
            window.draw(black_bg);


            window.display();

            sf::sleep(sf::seconds(0.012));
        }
    }

    return 1;
}


int menu_text_initialize(Text& text_sprite, String text) {
    text_sprite.setFont(font);
    text_sprite.setCharacterSize(40);
    text_sprite.setString(text);

    text_sprite.setFillColor(Color(100, 100, 100, 255));

    return 1;
}

int main() {
    //getting the score from a file
    ifstream
        Infofile("fonts/Data.txt");

    string line;
    while (getline(Infofile, line)) {
        best_score = stoi(line);  
        cout << best_score<<"bestscore" << endl;
    }
    
    Infofile.close();


    fps = 6;
    srand(time(0)); // <-- делает rand() случайным каждый раз

    const int screensize = 800;
    plussize = 3;

    int screenpx = screensize - (screensize % amountoftalesonscreen);
    cout << screenpx << "," << screensize % amountoftalesonscreen << endl;
    int screenpxfr = screenpx;
    window.create(VideoMode(screenpxfr + plussize * 2, screenpxfr + plussize * 2), "SFML Image Box");


    talescale = screenpx / amountoftalesonscreen / 16.01;
    heroscale = screenpx / amountoftalesonscreen / 20.01;

    if (heroscale < 1) {
        heroscale = 0.8;
    }
    int tailsplaced = 0;

    // Call the create_gamemap function
    setupmap_new(difficulty_index);
    //starting the timer for the countdown in the beginning
    start_timer = time(0);
    
    

    if (!font.loadFromFile("fonts/PressStart2P.ttf"))
        return EXIT_FAILURE;
    coinsscore = 0;

    menu_text_initialize (score_text, to_string(coinsscore));
    score_text.setPosition(10.f, float(screenpxfr - 20));
    score_text.setCharacterSize(20);

    menu_text_initialize(start_text, "START");
    start_text.setPosition(20, 250);

    menu_text_initialize(settings_text, "SETTINGS");
    settings_text.setPosition(20, 330);

    menu_text_initialize(quit_text, "QUIT");
    quit_text.setPosition(20, 410);


    menu_text_initialize(music_text, "MUSIC");
    music_text.setPosition(20, 330);

    menu_text_initialize(back_text, "BACK");
    back_text.setPosition(20, 410);
    back_text.setCharacterSize(30);


    menu_text_initialize(easy_difficulty_text, "EASY");
    easy_difficulty_text.setPosition(20, 250);

    menu_text_initialize(medium_difficulty_text, "MEDIUM");
    medium_difficulty_text.setPosition(20, 330);

    menu_text_initialize(hard_difficulty_text, "HARD");
    hard_difficulty_text.setPosition(20, 410);

    menu_text_initialize(impossible_difficulty_text, "ULTRA");
    impossible_difficulty_text.setPosition(20, 490);

    menu_text_initialize(score_only_text, "SCORE");
    score_only_text.setPosition(330, 230);
    score_only_text.setCharacterSize(30);

    menu_text_initialize(score_int_text, to_string(score));
    score_int_text.setPosition(340, 280);
    score_int_text.setCharacterSize(25);

    menu_text_initialize(score_best_new_only_text, "NEW BEST SCORE!!!");
    score_best_new_only_text.setPosition(230, 330);
    score_best_new_only_text.setCharacterSize(20);

    menu_text_initialize(score_best_int_text, to_string(score));
    score_best_int_text.setPosition(340, 450);
    score_best_int_text.setCharacterSize(25);

    menu_text_initialize(score_best_only_text, "BEST SCORE");
    score_best_only_text.setPosition(280, 400);
    score_best_only_text.setCharacterSize(25);
    



    // coin
    //Texture coin_texture;
    if (!coin_texture.loadFromFile("images/coin_1.png"))
        return EXIT_FAILURE;

    // текстура и спрайт
    //Texture field_texture;
    if (!field_texture.loadFromFile("images/grass_dark_1_BW.png"))
        return EXIT_FAILURE;



    int crimsonchance = rand() % 4;
    int crimson123 = rand() % 3;
    talewidthinpx = 16;

    //Texture wall_texture;
    if (!wall_texture.loadFromFile("images/bricks_3.png"))
        return EXIT_FAILURE;

    //Texture crimson_wall_texture1;
    if (!crimson_wall_texture1.loadFromFile("images/bricks_crimson_1.png"))
        return EXIT_FAILURE;
    //Texture crimson_wall_texture2;
    if (!crimson_wall_texture2.loadFromFile("images/bricks_crimson_2.png"))
        return EXIT_FAILURE;
    //Texture crimson_wall_texture3;
    if (!crimson_wall_texture3.loadFromFile("images/bricks_crimson_3.png"))
        return EXIT_FAILURE;




    if (!hero_texture.loadFromFile("images/hero_white7.png"))
        return EXIT_FAILURE;

    if (!hero_texture_up1.loadFromFile("images/hero_white_up1.png"))
        return EXIT_FAILURE;

    if (!hero_texture_up2.loadFromFile("images/hero_white_up2.png"))
        return EXIT_FAILURE;

    if (!hero_texture_down1.loadFromFile("images/hero_white_down1.png"))
        return EXIT_FAILURE;

    if (!hero_texture_down2.loadFromFile("images/hero_white_down2.png"))
        return EXIT_FAILURE;

    if (!hero_texture_left.loadFromFile("images/hero_white_left.png"))
        return EXIT_FAILURE;

    if (!hero_texture_right.loadFromFile("images/hero_white_right.png"))
        return EXIT_FAILURE;

    if (!ghost_texture.loadFromFile("images/ghost.png"))
        return EXIT_FAILURE;


    if (!replay_button_texture.loadFromFile("images/replay_button.png"))
            return EXIT_FAILURE;
    replay_button.setTexture(replay_button_texture);
    replay_button.setPosition(265, 500);
    replay_button.setScale(1, 1);

    if (!settings_button_texture.loadFromFile("images/settings_button.png"))
        return EXIT_FAILURE;
    settings_button.setTexture(settings_button_texture);
    settings_button.setPosition(367, 500);
    settings_button.setScale(1, 1);

    if (!menu_button_texture.loadFromFile("images/menu_button.png"))
        return EXIT_FAILURE;
    menu_button.setTexture(menu_button_texture);
    menu_button.setPosition(465, 500);
    menu_button.setScale(0.4, 0.4);



    if (!game_over_bg_texture.loadFromFile("images/game_over_bg.png"))
        return EXIT_FAILURE;

    if (!bg_start_texture.loadFromFile("images/start_bg.png"))
        return EXIT_FAILURE;

    if (!bg_difficulty_texture.loadFromFile("images/difficulty_bg.png"))
        return EXIT_FAILURE;

    if (!black_bg_texture.loadFromFile("images/black_800.png"))
        return EXIT_FAILURE;


    if (!tick_texture.loadFromFile("images/tick.png"))
        return EXIT_FAILURE;


    animationbool = true;
    Color bgColor = Color::Black;


    sf::Music intromusic;
    if (!intromusic.openFromFile("images/intro.ogg")) {
        cout << "couldnt load musika" << endl;
        return EXIT_FAILURE;
    }

    sf::Music loopmusic;
    if (!loopmusic.openFromFile("images/loop.ogg")) {
        cout << "couldnt load musika" << endl;
        return EXIT_FAILURE;
    }

    intromusic.setVolume(100);
    intromusic.play();
    bool musicintro = true;



    while (window.isOpen())
    {
        {
            if (musicintro == true) {
                if (intromusic.getStatus() == sf::SoundSource::Stopped) {
                    loopmusic.setLoop(true);     // чтобы играла по кругу
                    loopmusic.setVolume(music_volume);     // громкость от 0 до 100
                    loopmusic.play();
                    musicintro = false;
                }
            }

            Event event;
            while (window.pollEvent(event))

                if (event.type == Event::Closed)
                    window.close();




            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                cout << startscreen << endl;
                if (startscreen) {
                    difficulty_animation_should = false;
                    cout << "startscreen" << endl;
                    //if start button was pressed, go to the difficulty choosing screen thru the cutscene
                    if (start_text.getGlobalBounds().contains(mousePos)) {
                        cout << "containsyooo" << endl;
                        start_cutscene(1);
                        startscreen = false;
                        difficultyscreen = true;

                    }

                    else if (settings_text.getGlobalBounds().contains(mousePos)) {
                        startscreen = false;
                        settingsscreen = true;
                    }

                    else if (quit_text.getGlobalBounds().contains(mousePos)) {

                        window.close();

                    }
                }

                if (game_over) {
                    if (replay_button.getGlobalBounds().contains(mousePos)) {
                        game_over = false;
                        reset_variables_fully();
                        
                        gamescreen = true;

                    }
                
                    else if (settings_button.getGlobalBounds().contains(mousePos)) {
                        game_over = false;
                        settingsscreen = true;
                        backtogame = true;
                        reset_variables_fully();

                    }

                    else if (menu_button.getGlobalBounds().contains(mousePos)) {
                        game_over = false;
                        difficultyscreen = true;
                        reset_variables_fully();

                    }
                
                }

                if (settingsscreen) {
                    if (ticksquare.getGlobalBounds().contains(mousePos)) {

                        if (musicon) {
                            loopmusic.setVolume(0);
                            intromusic.setVolume(0);
                            music_volume = 0;
                            musicon = false;
                        }
                        else if (!musicon) {
                            loopmusic.setVolume(100);
                            intromusic.setVolume(100);
                            music_volume = 100;
                            musicon = true;
                        }
                    }

                    if (back_text.getGlobalBounds().contains(mousePos)) {
                        settingsscreen = false;
                        
                        //if the settings button was clicked after the gameover, when you click back from it you get to the difficulty menu
                        if (backtogame) {
                            difficultyscreen = true;
                        }
                        else {
                            startscreen = true;
                        }
                    }

                }

                if (difficultyscreen) {
                    cout << "bobclick" << endl;
                    if (easy_difficulty_text.getGlobalBounds().contains(mousePos) or medium_difficulty_text.getGlobalBounds().contains(mousePos) or hard_difficulty_text.getGlobalBounds().contains(mousePos) or impossible_difficulty_text.getGlobalBounds().contains(mousePos)) {
                        if (difficulty_animation_should) {
                            start_cutscene(2);


                            if (easy_difficulty_text.getGlobalBounds().contains(mousePos)) {
                                difficulty_level = 1;
                            }

                            else if (medium_difficulty_text.getGlobalBounds().contains(mousePos)) {
                                difficulty_level = 2;
                            }
                            else if (hard_difficulty_text.getGlobalBounds().contains(mousePos)) {
                                difficulty_level = 3;
                            }
                            else if (impossible_difficulty_text.getGlobalBounds().contains(mousePos)) {
                                difficulty_level = 4;
                                cout << "impsbl" << endl;
                            }


                            fps = 4 + difficulty_level * 2;


                            float diffby2 = difficulty_level / 2;
                            coinrand = 30;

                            gamescreen = true;
                            difficultyscreen = false;
                            start_timer = time(0);

                            //starting the timer for the score(you get score for the amount of ms that you are alive)
                            auto start_score_timer = std::chrono::high_resolution_clock::now();
                        }

                        

                        difficulty_animation_should = true;
                        //sf::sleep(sf::seconds(1));

                        cout << "bob" << endl;


                    }

                    if (back_text.getGlobalBounds().contains(mousePos)) {
                            difficultyscreen = false;
                            startscreen = true;
                        }

                }
            }

        }


        window.clear(bgColor);



        //if supposed to show game screen:
        if (gamescreen) {
            gamescreen_do();
        }

        else if (startscreen) {
            startscreen_do();
        }

        else if (difficultyscreen) {
            difficultyscreen_do();
        }

        else if (settingsscreen) {
            settingsscreen_do();
        }

        else if (game_over) {
            gameoverscreen_do();
        }

        window.display();



        float sum_sleep = 0;
        float fpos = 1 / fps;
        float secsleep = 0.005;



        if (direction != "o") {
            sf::sleep(sf::seconds(fpos));
        }

        direction = checkdirection();





        bool waiting = true;
        while (waiting) {
            if (direction == "o") {
                direction = checkdirection();
            }

            //cout << "fpos"<<fpos << "|" <<"sum sli[" << sum_sleep <<"|secslip" <<secsleep<< endl;
            //cout << direction;

            //cout << fpos;

            sf::sleep(sf::seconds(secsleep));
            sum_sleep += secsleep;
            if (sum_sleep > fpos or direction != "o") {
                waiting = false;
                //cout << "fpsfin" << endl;
            }
            //*/
        }


    }

    return EXIT_SUCCESS;
}




/*
green
#4cce0f

dark green
#0f1c08

C:\GLP\jopa


*/
