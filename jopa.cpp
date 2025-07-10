#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <string>


using namespace std;
using namespace sf;



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


const int amountoftalesonscreen = 22;
string gamemap[amountoftalesonscreen][amountoftalesonscreen], gamemap_save[amountoftalesonscreen][amountoftalesonscreen];
int coinrand = 60;
string direction = "w";
int coinscreated = 0;
int y_hero = 1;
int x_hero = 1;
int y_ghost = 1;
int x_ghost = 1;
int ghost_time = 3;
int time_passed = 0;
float talescale;
int difficulty_index = -2;
bool game_over = false;
bool startbutton_clicked = false;
bool ghost_allowed = false;
Texture field_texture, wall_texture, crimson_wall_texture1, crimson_wall_texture2, crimson_wall_texture3, coin_texture, hero_texture, hero_texture_up1, hero_texture_up2, hero_texture_down1, hero_texture_down2, hero_texture_left, hero_texture_right, ghost_texture ;
Sprite hero,ghost;
Text coinsscore_text,countdown_text;
RenderWindow window;
int plussize, talesize, coinsscore, save_y, timespassed;
bool animationbool;
time_t start_timer;
   


string create_gamemap(int difficulty) {

    srand(time(0)); // <-- делает rand() случайным каждый раз

    gamemap[amountoftalesonscreen][amountoftalesonscreen];
    int amountofwalls = (rand() % amountoftalesonscreen / 2 + amountoftalesonscreen * amountoftalesonscreen / 10) + difficulty*10;
    cout << "amountofwalls" << amountofwalls << endl;

    int y_walls_amount = amountofwalls / 2;
    cout << "y_walls_amount" << y_walls_amount << endl;
    int x_walls_amount = amountofwalls - y_walls_amount;

    int y_wall_coordinate = 1;
    int x_wall_coordinate = 1;
    bool goplus = true;

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
                    //cout << " randmoin" << endl;
                    gamemap[y_wall_coordinate][x_wall_coordinate] = "coin";
                    coinscreated++;
                }
                goplus ? y_wall_coordinate++ : y_wall_coordinate--;
            }
            y_walls_amount--;
        }

        if (x_walls_amount > 0) {
            if (i == 0) {
                y_wall_coordinate = 1;
                x_wall_coordinate = 1;
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
    return "gameMap created";
}

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
            
                else{
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

    //cout << "doin ghost" << endl;

    if (ghost_allowed) {
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
        after_move[0][1]+=1;
        after_move[1][0]+=1;
        after_move[2][1]-=1;
        after_move[3][0]-=1;

       
        for (int i = 0;i < 4;i++) {
            float ysq = pow( (y_hero - after_move[i][0]) , 2);//after_move[i][0] would be ghost's y coordinate after the move, "i" is the direction
            float xsq = pow( (x_hero - after_move[i][1]) , 2);//after_move[i][1] would be ghost's x coordinate after the move
            float resultsq = sqrt(ysq+xsq);
            //cout << "result number "<<i<< ": " << resultsq << endl;
            
            after_move[i][2]= resultsq;
            //cout <<"aftermove"<< after_move[i][2] << endl;
        }
        int leastnum;
        float least=100000000000;
        for (int i = 0; i < 4;i++) {
            int after_move_y = after_move[i][0];
            int after_move_x = after_move[i][1];
            if ((least > after_move[i][2] and gamemap[after_move_y][after_move_x]=="tale") or (least > after_move[i][2] and gamemap[after_move_y][after_move_x] == "coin") ) {
                least = after_move[i][2];
                leastnum = i;
            }
        }
        y_ghost = after_move[leastnum][0];
        x_ghost = after_move[leastnum][1];
        window.draw(ghost);

        //if the ghost is on the same square as the hero is on, hero dies
        if (y_ghost == y_hero and x_ghost == x_hero) {
            game_over = true;
            window.close();
        }
    }
    
    return 1;
}

string countcoins() {
    coinscreated = 0;
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            if (gamemap[i][x] == "coin") {
                coinscreated +=1;

            }
        }
    }
    return "jopa";


}

string print_countdown(int screenpxfr, Font font) {
    countdown_text.setString(to_string( -(time_passed-3) ));
    countdown_text.setFont(font);
    countdown_text.setCharacterSize(100);

    countdown_text.setFillColor(Color(rand() % 56+200, rand() % 206+50, rand() % 206+50));
    countdown_text.setPosition(float(screenpxfr/2)-50, 150.f);
    window.draw(countdown_text);

    return "1";
}



string setupmap_harder(int difficulty) {
    create_gamemap(difficulty);
    cout << "finished" << endl;
    fill_map();
    countcoins();


    gamemap_save[amountoftalesonscreen][amountoftalesonscreen];

    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap_save[i][x] = gamemap[i][x];
        }
    }
    gamemap[1][1] = "1";
    y_hero = 1;
    x_hero = 1;
    y_ghost = 1;
    x_ghost = 1;

    return"lol";
}

string setupmap_new(int difficulty) {
    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap[i][x] = "wall";
        }
    }
    create_gamemap(difficulty);
    cout << "finished" << endl;
    fill_map();
    countcoins();


    gamemap_save[amountoftalesonscreen][amountoftalesonscreen];

    for (int i = 0; i < amountoftalesonscreen; ++i) {
        for (int x = 0; x < amountoftalesonscreen; ++x) {
            gamemap_save[i][x] = gamemap[i][x];
        }
    }
    gamemap[1][1] = "1";
    y_hero = 1;
    x_hero = 1;
    y_ghost = 1;
    x_ghost = 1;

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
        // стоим на месте
        hero.setTexture(hero_texture);
    }
    return 0;
}


int drawmap() {

    int tailsplaced = 0;
    //отрисовка карты
    for (int i = 0; i < amountoftalesonscreen; i++) {
        for (int x = 0; x < amountoftalesonscreen;x++) {
            if (gamemap[i][x] == "tale") {


                Sprite field2(field_texture);
                field2.setPosition(plussize + x * talescale * 4, plussize + i * talescale * 4);
                field2.setScale(talescale * 0.25, talescale * 0.25);
                window.draw(field2);

                tailsplaced++;
            }
            else if (gamemap[i][x] == "wall" or gamemap[i][x] == "wall1" or gamemap[i][x] == "wall2" or gamemap[i][x] == "wall3") {
                Sprite wall;


                if (gamemap[i][x] == "wall") { wall.setTexture(wall_texture); }
                if (gamemap[i][x] == "wall1") { wall.setTexture(crimson_wall_texture1); }
                if (gamemap[i][x] == "wall2") { wall.setTexture(crimson_wall_texture2); }
                if (gamemap[i][x] == "wall3") { wall.setTexture(crimson_wall_texture3); }


                wall.setPosition(plussize + x * talescale * 4, plussize + i * talescale * 4);
                wall.setScale(talescale * 0.25, talescale * 0.25);
                window.draw(wall);

                tailsplaced++;
            }
            else if (gamemap[i][x] == "coin") {


                Sprite field2(field_texture);
                field2.setPosition(plussize + x * talescale * 4, plussize + i * talescale * 4);
                field2.setScale(talescale * 0.25, talescale * 0.25);
                window.draw(field2);
                //cout << "coim" << endl;

                Sprite coin(coin_texture);
                coin.setPosition(plussize + x * talescale * 4, plussize + i * talescale * 4);
                coin.setScale(talescale * 0.25, talescale * 0.25);
                window.draw(coin);
                tailsplaced++;
            }



        }

    }

    //check for coin
    if (gamemap[y_hero][x_hero] == "coin") {
        coinsscore++;
        gamemap[y_hero][x_hero] = "tale";
        gamemap_save[y_hero][x_hero] = "tale";
        cout << "change";
    }

    //bgColor = Color(rand() % 256, rand() % 256, rand() % 256);
    //checking if all coins are picked up, if so then create a new map and reset variables
    if (coinsscore >= coinscreated) {
        coinsscore_text.setString("all got");
        difficulty_index++;
        setupmap_new(difficulty_index);
        start_timer = time(0);  cout << "new start timer" << endl;
        ghost_allowed = false;
        
        coinsscore = 0;
    }
    //if not all picked up then display the score
    else {
        coinsscore_text.setString("score: " + to_string(coinsscore));
    }

    window.draw(coinsscore_text); //cout << "coinscore" << coinsscore << "coinscreated" << coinscreated << endl;
    window.draw(hero);



    return 1;
}


int main() {

    float fps = 5;
    srand(time(0)); // <-- делает rand() случайным каждый раз

    const int screensize = 800;
    plussize = 10;

    int screenpx = screensize - (screensize % amountoftalesonscreen);
    cout << screenpx << "," << screensize % amountoftalesonscreen << endl;
    int screenpxfr = screenpx + plussize * 2;
    window.create(VideoMode(screenpxfr, screenpxfr + 40), "SFML Image Box");


    talescale = screenpx / amountoftalesonscreen / 4.00001;
    float heroscale = screenpx / amountoftalesonscreen / 20.00001;
    if (heroscale < 1) {
        heroscale = 0.8;
    }
    int tailsplaced = 0;

    // Call the create_gamemap function
    setupmap_new(difficulty_index);
    start_timer = time(0);

    Font font;
    if (!font.loadFromFile("opensans.ttf"))
        return EXIT_FAILURE;
    coinsscore = 0;
    // текст        coinsscore_text.setString("balls: " + to_string(coinsscore));

    coinsscore_text.setFont(font);
    coinsscore_text.setCharacterSize(40);
    coinsscore_text.setString("score:" + to_string(coinsscore));

    coinsscore_text.setFillColor(Color::White);
    coinsscore_text.setPosition(10.f, float(screenpxfr - 10));

    // coin
    //Texture coin_texture;
    if (!coin_texture.loadFromFile("images/coin_1.png"))
        return EXIT_FAILURE;

    // текстура и спрайт
    //Texture field_texture;
    if (!field_texture.loadFromFile("images/grass_3.png"))
        return EXIT_FAILURE;



    int crimsonchance = rand() % 4;
    int crimson123 = rand() % 3;

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

    if (!ghost_texture.loadFromFile("images/ghost3.png"))
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

    intromusic.setVolume(20);
    intromusic.play();
    bool musicintro = true;



    while (window.isOpen())
    {
        {
            if (musicintro == true) {
                if (intromusic.getStatus() == sf::SoundSource::Stopped) {
                    loopmusic.setLoop(true);     // чтобы играла по кругу
                    loopmusic.setVolume(20);     // громкость от 0 до 100
                    loopmusic.play();
                    musicintro = false;
                }
            }

            Event event;
            while (window.pollEvent(event))

                if (event.type == Event::Closed)
                    window.close();




            // клик по спрайту
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                /*if (field2.getGlobalBounds().contains(mousePos))
                {
                    bgColor = Color(rand() % 256, rand() % 256, rand() % 256);
                    text.setString("clicked balls!");

                }*/
            }
        }

        window.clear(bgColor);

        save_y = y_hero;

        if (direction != "o") {
            //cout << "CHCKING dirRECTION<<" << endl;
            string direction = checkdirection();
        }
        // переменная для героя, всегда вне условий
        //Sprite hero;

        hero_set_texture();

        // устанавливаем позицию и масштаб — ОБЩИЕ для всех направлений
        hero.setPosition(plussize + x_hero * talescale * 4, plussize + y_hero * talescale * 4);
        hero.setScale(heroscale, heroscale);

        ghost.setTexture(ghost_texture);
        ghost.setPosition(plussize + x_ghost * talescale * 4, plussize + y_ghost * talescale * 4);
        ghost.setScale(heroscale, heroscale);


        for (int i = 0; i < amountoftalesonscreen; ++i) {
            for (int x = 0; x < amountoftalesonscreen; ++x) {
                gamemap[i][x] = gamemap_save[i][x];
            }
        }
        //gamemap[y_hero][x_hero] = "1";



        //draws the entire map and all the othуr sprites
        drawmap();

        do_ghost();

       
        time_t end_timer = time(0);
        time_passed = end_timer - start_timer;
        if (time_passed <= ghost_time-1) {
            print_countdown(screenpxfr, font);
        }
        else {
            ghost_allowed = true;
           
        }
        /*
        hero.setPosition(plussize + x_hero * talescale * 4, plussize + y_hero * talescale * 4);
        hero.setScale(heroscale, heroscale);*/
        
        
        


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
            if (sum_sleep > fpos or direction!="o") {
               
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
