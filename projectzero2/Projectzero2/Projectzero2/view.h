#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой
void setplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€

   	if (x < 535) tempX = 535;//убираем из вида левую сторону
	if (x > 1800) tempX = 1800;//убираем из вида правую сторону
	if (y < 460) tempY = 460;//верхнюю сторону
	if (y > 957) tempY = 957;//нижнюю сторону	

	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты. 

}


