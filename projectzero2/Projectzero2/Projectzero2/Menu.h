#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "view.h"
#include "Map.h"
#include <cstdlib>
const int max_size = 10;
using namespace sf;
using namespace std;

class Text_Draw {
public:
    Text text;
    Font font;
    Text_Draw() {
        font.loadFromFile("2006.ttf");
        text.setFillColor(Color::Magenta);
        text.setStyle(sf::Text::Bold);
        text.setFont(font);
        text.setCharacterSize(65);
    }

    void ForK(int k)
    {
        text.setString(to_string(k));//задаем строку тексту и вызываем сформированную выше строку методом .str() 
    }

    void Change() {

        text.setString("");

    }
    void Text(const wchar_t Text[]) {
        text.setString(Text);
    }

    void setPosition(float x, float y) {
        text.setPosition(x, y);
    }

    FloatRect getRect(float x, float y) {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return FloatRect(x, y, 65, 65);//эта ф-ция нужна для проверки столкновений 
    }

};
class Player {
public:
    enum { left, right, stay } state;//добавляем тип перечисления - состояние объекта
    float dx, dy, x, y, speed, CurrentFrame;//добавили переменную таймер для будущих целей
    int w, h;
    bool onGround;
    Texture texture;
    Sprite sprite;

    Player(Image& image, float X, float Y, int W, int H)
    {
        state = stay; //инициализируем.получаем все объекты для взаимодействия персонажа с картой
        sprite.setTextureRect(IntRect(0, 0, w, h));
        x = X; y = Y; w = W; h = H; CurrentFrame = 0;
        speed = 0; dx = 0; dy = 0;
        onGround = false;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
    }
    Player& operator =(const Player& p) {
        this->x = p.x;
        this->y = p.y;
        this->dx = p.dx;
        this->dy = p.dy;
        this->w = p.w;
        this->h = p.h;
        this->state = p.state;
        this->speed = p.speed;
        this->CurrentFrame = p.CurrentFrame;
        this->onGround = p.onGround;
        this->texture.loadFromImage(p.texture.copyToImage());
        this->sprite.setTexture(this->texture);
        this->sprite.setOrigin(this->w / 2, this->h / 2);
        sprite.setTextureRect(IntRect(0, 0, this->w, this->h));
        return *this;
    }
    Player() {};

    FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
    }

    void control() {

            if ((Keyboard::isKeyPressed(Keyboard::A))) {
                state = left; speed = 0.4;
            }
            if ((Keyboard::isKeyPressed(Keyboard::D))) {
                state = right; speed = 0.4;

            }

            if ((Keyboard::isKeyPressed(Keyboard::Space) && (onGround))) {
                dy = -1.2; onGround = false;
            }
            if (speed == 0) { state = stay; }
    }

    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (Map[i][j] == '0' || Map[i][j] == 'q')
                {
                    if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
                    if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
                    if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
                }

            }
    }

    void Animation(float Y, float time) {
        CurrentFrame += 0.005 * time; if (CurrentFrame > 4) CurrentFrame -= 3;
        sprite.setTextureRect(IntRect(189 * int(CurrentFrame), Y, 144, 216));
    }

    void update(float time)
    {
        control();
        switch (state)
        {
        case right: dx = speed;  Animation(576, time); break;
        case left: dx = -speed;  Animation(864, time); break;
        case stay: dx = 0; sprite.setTextureRect(IntRect(0, 0, 144, 216)); break;
        }

        x += dx * time;
        checkCollisionWithMap(dx, 0);
        y += dy * time;
        checkCollisionWithMap(0, dy);

        sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в центр
        speed = 0;
        dy = dy + 0.0015 * time; // притяжение к земле
        setplayercoordinateforview(x, y);
    }
};
class Platform {
public:
    float x, y, w, h;
    Texture texture;
    Sprite sprite;

    Platform(Image image, float X, float Y, int W, int H) {
        x = X; y = Y; w = W; h = H; texture.loadFromImage(image); sprite.setTexture(texture); sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(IntRect(0, 0, w, h));
        sprite.setPosition(x + w / 2, y + h / 2);
        sprite.setColor(Color::Black);
    }
    Platform() {};

    FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
    }
};
class Door {
public:
    float x, y, w, h;
    bool open;
    Texture texture;
    Sprite sprite;

    Door(Image image, float X, float Y, int W, int H) {
        x = X; y = Y; w = W; h = H; texture.loadFromImage(image); sprite.setTexture(texture); sprite.setOrigin(w / 2, h / 2);
        sprite.setPosition(x + w / 2, y + h / 2);
        open = false;


    }
    Door() {};

    FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
    }

    void Open(bool Open) {
        if (Open == false) {
            open = false;
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
        if (Open == true) {
            open = true;
            sprite.setColor(Color::Transparent);
        }
    }
};

void Menu(RenderWindow& window) {
	
    Text_Draw newgame, about, Exit, control;
    control.text.setCharacterSize(32);
    control.text.setFillColor(Color::White);
    control.Text(L"Твоя задача собрать число, которое написано в\n"
				  L"верхннем левом углу.\n"  
                  L"Текущее число написано в правом верхнем углу\n"
                  L"Это можно сделать с помощбю операций + , -, *\n"
                  L"(Кнопки соответствуют '+' , '-' и '8'(которое'*')(все \n"
                  L"они сверху клавиатуры, а не сбоку), Если у вас\n"
                  L"не получается собрать число, то R - рестарт\n"
                  L"A - идти влево, D - идти вправо,\n" 
                  L"Space - прыжок");

    newgame.Text(L"Continue");
    about.Text(L"Control");
    Exit.Text(L"Exit");

    bool isMenu = 1;
    int menuNum = 0;
	newgame.setPosition(view.getCenter().x - 130, view.getCenter().y - 300);
	about.setPosition(view.getCenter().x - 110, view.getCenter().y - 100);
	Exit.setPosition(view.getCenter().x - 70, view.getCenter().y + 100);
    control.setPosition(view.getCenter().x - 496, view.getCenter().y - 420);

    //////////////////////////////МЕНЮ///////////////////
    while (isMenu)
    {
		Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        newgame.text.setFillColor(Color::White);
        about.text.setFillColor(Color::White);
        Exit.text.setFillColor(Color::White);
        menuNum = 0;
        window.clear();


        if (IntRect(404, 150, 279, 57).contains(Mouse::getPosition(window))) { newgame.text.setFillColor(Color::Blue); menuNum = 1; }
        if (IntRect(424, 365, 237, 59).contains(Mouse::getPosition(window))) { about.text.setFillColor(Color::Blue); menuNum = 2; }
        if (IntRect(470, 584, 136, 60).contains(Mouse::getPosition(window))) { Exit.text.setFillColor(Color::Blue); menuNum = 3; }


        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuNum == 1) { isMenu = false; }//если нажали первую кнопку, то выходим из меню 
            if (menuNum == 2) { window.draw(control.text); window.display(); 
            while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); exit(0); }

        }

        window.draw(newgame.text);
        window.draw(about.text);
        window.draw(Exit.text);

        window.display();
    }
    ////////////////////////////////////////////////////   
}
void Game(Platform platform_array[], Text_Draw text_array[], int array_for_rand[], int array_for_k_num[], int& d, int& k, int& Sum, Player& p, Door& door, Image& heroImage, Image& doorImage, Image& platformImage)
{
    d = rand() % 30; while (d < 10 || d == 0)  d = rand() % 30;

    p = Player(heroImage, 100, 1128, 143, 216);
    door = Door(doorImage, 2272, 1120, 32, 224);

    platform_array[0] = Platform(platformImage, 200, 1000, 159, 29);
    platform_array[1] = Platform(platformImage, 600, 900, 159, 29);
    platform_array[2] = Platform(platformImage, 1164, 950, 159, 29);
    platform_array[3] = Platform(platformImage, 2000, 1000, 159, 29);
    platform_array[4] = Platform(platformImage, 1750, 800, 159, 29);
    platform_array[5] = Platform(platformImage, 1300, 649, 159, 29);
    platform_array[6] = Platform(platformImage, 980, 450, 159, 29);
    platform_array[7] = Platform(platformImage, 1600, 300, 159, 29);
    platform_array[8] = Platform(platformImage, 2050, 435, 159, 29);
    platform_array[9] = Platform(platformImage, 465, 500, 159, 29);



    array_for_rand[0] = rand() % (d - 1) + 1;
    for (int i = 1; i < 10; i++) {
        bool flag = false;
        while (flag == false) {
            flag = true;
            array_for_rand[i] = rand() % (d - 1) + 1;
            for (int j = 0; j < i; j++) {
                if (array_for_rand[i] == array_for_rand[j])
                {
                    flag = false;
                    break;
                }

            }
        }
    }
    for (int i = 0; i < 4; i++) {
        bool flag = false;
        while (flag == false) {
            flag = true;
            array_for_k_num[i] = rand() % max_size;
            for (int j = 0; j < i; j++) {
                if (array_for_k_num[i] == array_for_k_num[j])
                {
                    flag = false;
                    break;
                }

            }
        }
    }
    for (int i = 0; i < 10; i++) { text_array[i].ForK(array_for_rand[i]); }



    for (int i = 0; i < 4; i++) {
        k += array_for_rand[array_for_k_num[i]];
    }

    for (int i = 0; i < max_size; i++) {
        text_array[i].setPosition(platform_array[i].x + 54, platform_array[i].y - 80);
    }

    text_array[10].ForK(k);
}