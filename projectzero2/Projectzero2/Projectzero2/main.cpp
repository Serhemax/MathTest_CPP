#include "pch.h"
#include <SFML/Graphics.hpp>
#include "view.h"
#include "Menu.h"
#include "Map.h"
#include <iostream>
#include <sstream>
using namespace sf;
using namespace std;


int main()
{
    setlocale(0, "");

    float dX = 0;//корректировка нажатия по х
    float dY = 0;//по у

    RenderWindow window(VideoMode(1080, 920), "Project Zero");


    view.reset(sf::FloatRect(0, 0, 1000, 840)); 
    int d = 0;
    int Sum = 0;
    int k = 0;
    srand(time(0));

    Image  image;
    image.loadFromFile("img/Map.png");
    Texture texture;
    texture.loadFromImage(image); 
    Sprite spritemap;
    spritemap.setTexture(texture);

    Image platformImage;
    platformImage.loadFromFile("img/Platform.png");

    Image heroImage;
    heroImage.loadFromFile("img/Walk.PNG");

    Image doorImage;
    doorImage.loadFromFile("img/Door.png");

    int array_for_rand[max_size];
    int array_for_k_num[4];
    Text_Draw text_array[max_size + 3];
    Platform platform_array[max_size];   
    Player p;//передаем координаты прямоугольника player из карты в координаты нашего игрока
    Door door; //2272 -68

    Clock clock;    

    Game(platform_array, text_array, array_for_rand, array_for_k_num, d, k, Sum, p, door, heroImage, doorImage, platformImage);

    while (window.isOpen())
    {
        
        text_array[10].setPosition(view.getCenter().x - 500, view.getCenter().y - 430);
        text_array[11].setPosition(view.getCenter().x + 420, view.getCenter().y - 430);
		text_array[12].setPosition(view.getCenter().x - 200, view.getCenter().y - 430);
		text_array[12].Text(L"Esc - Menu");
        text_array[11].ForK(Sum);
        float time = clock.getElapsedTime().asMicroseconds();

        clock.restart();
        time = time / 800;
		
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // Логика Игры
            
            if (event.type == sf::Event::KeyPressed) {
                for (int i = 0; i < max_size; i++) {
                    if (text_array[i].getRect(platform_array[i].x + 54, platform_array[i].y - 80).intersects(p.getRect())) {
                        if (Keyboard::isKeyPressed(Keyboard::Equal)) {
                            text_array[i].Change();
                            Sum += array_for_rand[i];
                            array_for_rand[i] = 0;
                        }
                        if (Keyboard::isKeyPressed(Keyboard::Hyphen)) {
                            text_array[i].Change();
                            Sum -= array_for_rand[i];
                            array_for_rand[i] = 0;
                        }
                        if (Keyboard::isKeyPressed(Keyboard::Num8)) {
                            text_array[i].Change();
                            if (Sum == 0) Sum = 1;
                            Sum *= array_for_rand[i];
                            array_for_rand[i] = 0;
                        }
                        
                    }

                }
                if (Keyboard::isKeyPressed(Keyboard::R)) {
                    Sum = 0; k = 0; d = 0;
                    for (int i = 0; i < 10; i++) { array_for_rand[i] = 0; }
                    Game(platform_array, text_array, array_for_rand, array_for_k_num, d, k, Sum, p, door, heroImage, doorImage, platformImage);
                }  
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					p.dy = 0;
					text_array[12].text.setFillColor(Color::Transparent);
					Menu(window);//вызов меню
			
				}
            }        
        }
        
        
       // Логика игры
        for (int i = 0; i < max_size; i++) {
            if (platform_array[i].getRect().intersects(p.getRect())) {
                if ((p.dy > 0))
                    if (p.y + p.h < platform_array[i].y + platform_array[i].h && platform_array[i].x + platform_array[i].w / 2 + 40> p.x && platform_array[i].x - platform_array[i].w / 2 - 15 < p.x)//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
                    {
                        p.y = platform_array[i].y - p.h; p.dy = 0; p.onGround = true;  // то выталкиваем игрока так, чтобы он как бы стоял на платформе

                    }
                    else p.onGround = false;
            }
        }
        if (Sum == k)  door.Open(true); 
        if (Sum != k) { door.Open(false); door.sprite.setColor(Color::Yellow);}
        if (Sum > 700) { Game(platform_array, text_array, array_for_rand, array_for_k_num, d, k, Sum, p, door, heroImage, doorImage, platformImage); }
        if (door.open == false && door.getRect().intersects(p.getRect()) && p.dx>0)
        {
            p.x = 2128;           
        };
        if (door.open == true && door.getRect().intersects(p.getRect()) && p.dx > 0)
        {
            for (int i = 0; i < 10; i++) { array_for_rand[i] = 0; }
            Sum = 0; k = 0; d = 0; Game(platform_array, text_array, array_for_rand, array_for_k_num, d, k, Sum, p, door, heroImage, doorImage, platformImage);
        }


        p.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться


        window.setView(view);
        window.clear();


        //Карта

        for (int i = 0; i < HMap; i++) {
            for (int j = 0; j < WMap; j++)
            {
                if (Map[i][j] == ' ') spritemap.setTextureRect(IntRect(66, 0, 32, 32));
                if (Map[i][j] == '0') spritemap.setTextureRect(IntRect(66, 33, 32, 32));
                if (Map[i][j] == '1') spritemap.setTextureRect(IntRect(33, 0, 32, 32));
                if (Map[i][j] == '2') spritemap.setTextureRect(IntRect(33, 33, 32, 32));
                if (Map[i][j] == '3') spritemap.setTextureRect(IntRect(0, 0, 32, 32));
                if (Map[i][j] == '4') spritemap.setTextureRect(IntRect(0, 33, 32, 32));
                if (Map[i][j] == 'q') spritemap.setTextureRect(IntRect(0, 99, 32, 32));


                spritemap.setPosition(j * 32, i * 32);

                window.draw(spritemap);
            }
        }
	   
        // Отрисовка

		for (int i = 0; i < max_size; i++) {
			window.draw(platform_array[i].sprite);
		}
        for (int i = 0; i < 13; i++) {
            window.draw(text_array[i].text);
        }
        window.draw(door.sprite);
        window.draw(p.sprite);


        window.display();
    }

    return 0;
}