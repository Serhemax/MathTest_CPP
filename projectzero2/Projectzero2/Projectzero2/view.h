#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������
void setplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����

   	if (x < 535) tempX = 535;//������� �� ���� ����� �������
	if (x > 1800) tempX = 1800;//������� �� ���� ������ �������
	if (y < 460) tempY = 460;//������� �������
	if (y > 957) tempY = 957;//������ �������	

	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 

}


