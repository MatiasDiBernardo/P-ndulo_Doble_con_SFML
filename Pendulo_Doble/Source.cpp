#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>
using namespace std;

double g = 0.8;

double f1(double om1, double om2, double v1, double v2, double l1, double l2, double m1, double m2) {
	double num1 = -g * (2 * m1 + m2) * sin(om1) - m2 * g * sin(om1 - 2 * om2);
	double num2 = -2 * sin(om1 - om2) * m2 * (v2 * v2 * l2 + v1 * v1 * l1 * cos(om1 - om2));
	double den = l1 * (2 * m1 + m2 - m2 * cos(2 * om1 - 2 * om2));

	return (num1 + num2) / den;
}

double f2(double om1, double om2, double v1, double v2, double l1, double l2, double m1, double m2) {
	double num3 = 2 * sin(om1 - om2) * (v1 * v1 * l1 * (m1 + m2) + g * (m1 + m2) * cos(v1) + v2 * v2 * l2 * m2 * cos(om1 - om2));
	double den2 = l2 * (2 * m1 + m2 - m2 * cos(2 * om1 - 2 * om2));

	return num3 / den2;
}

int check() {
	double pi = 3.141592;
	double h = 0.1;
	double rta = 0;
	rta = f1(pi / 4 + (h/2), pi / 2 + (h / 2) * -0.02, (h / 2) * -0.408, (h / 2) * -0.740, 1, 1, 0.1, 0.1);
	cout << rta << endl;

	return 0;
}

int main() {
	int width = 800;
	int height = 800;

	sf::RenderWindow window(sf::VideoMode(width, height), "Pendulo Doble");
	window.setFramerateLimit(120);

	//Initial Conditions
	double pi = 3.141592;
	double m1 = 10;
	double m2 = 10;
	double l1 = 100;
	double l2 = 100;
	double om1 = pi/4;
	double om2 = pi/2;
	int x_origin = width / 2;
	int y_origin = height / 4;
	double v1 = 0;
	double v2 = 0;
	double a1 = 0;
	double a2 = 0;
	string type = "RK4";
	double h = 0.1;
	bool state = true;
		
	//Mass position
	float x1 = x_origin + l1 * sin(om1);
	float y1 = y_origin + l1 * cos(om1);

	float x2 = x1 + l2 * sin(om2);
	float y2 = y1 + l2 * cos(om2);
	
	//Create objects
	sf::CircleShape c1;
	sf::CircleShape c2;

	sf::Vertex line1[2];
	sf::Vertex line2[2];

	sf::Vector2f c1_pos(x1, y1);
	sf::Vector2f c2_pos(x2, y2);

	sf::Clock clock;

	line1[0].position = sf::Vector2f((width / 2) + m1, (height / 8) + m1);
	line1[1].position = c1_pos + sf::Vector2f(m1, m1);

	line2[0].position = c1_pos + sf::Vector2f(m1, m1);
	line2[1].position = c2_pos + sf::Vector2f(m2, m2);

	std::vector < sf::Vertex > trace;
	
	c1.setPosition(c1_pos);
	c1.setRadius(m1);
	c1.setFillColor(sf::Color(23, 91, 240));
	c2.setPosition(c2_pos);
	c2.setRadius(m2);
	c2.setFillColor(sf::Color(20, 80, 210));

	while (window.isOpen()) 
	{
		sf::Event e;
		while (window.pollEvent(e)) 
		{
			if (e.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
				state = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
				state = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				v1 = 0;
				v2 = 0;
				trace.clear();
			}
				
			if (!state && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (sf::Mouse::getPosition().y < y_origin + c2_pos.y) om2 += 0.05;
				if (sf::Mouse::getPosition().y > y_origin + c2_pos.y) om2 -= 0.05;
			}

			if (!state && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				if (sf::Mouse::getPosition().y < y_origin + c1_pos.y) om1 += 0.05;
				if (sf::Mouse::getPosition().y > y_origin + c1_pos.y) om1 -= 0.05;
			}
		}

		//Calulations
		if (state)
		{
			if (type == "Euler") 
			{
				a1 = f1(om1, om2, v1, v2, l1, l2, m1, m2);
				a2 = f2(om1, om2, v1, v2, l1, l2, m1, m2);

				v1 += a1; //t=1 per frame
				v2 += a2;

				om1 += v1;
				om2 += v2;
			}

			if (type == "RK4") {
				double c_a1 = v1;
				double c_b1 = v2;
				double c_c1 = f1(om1, om2, v1, v2, l1, l2, m1, m2);
				double c_d1 = f2(om1, om2, v1, v2, l1, l2, m1, m2);

				double c_a2 = v1 + (h / 2) * c_c1;
				double c_b2 = v2 + (h / 2) * c_d1;
				double c_c2 = f1(om1 + (h / 2), om2 + (h / 2) * c_a1, v1 + (h / 2) * c_c1, v2 + (h / 2) * c_d1, l1, l2, m1, m2);
				double c_d2 = f2(om1 + (h / 2), om2 + (h / 2) * c_b1, v1 + (h / 2) * c_c1, v2 + (h / 2) * c_d1, l1, l2, m1, m2);

				double c_a3 = v1 + (h / 2) * c_c2;
				double c_b3 = v2 + (h / 2) * c_d2;
				double c_c3 = f1(om1 + (h / 2), om2 + (h / 2) * c_a2, v1 + (h / 2) * c_c2, v2 + (h / 2) * c_d2, l1, l2, m1, m2);
				double c_d3 = f2(om1 + (h / 2), om2 + (h / 2) * c_b2, v1 + (h / 2) * c_c2, v2 + (h / 2) * c_d2, l1, l2, m1, m2);

				double c_a4 = v1 + (h / 2) * c_c3;
				double c_b4 = v2 + (h / 2) * c_d3;
				double c_c4 = f1(om1 + (h / 2), om2 + (h / 2) * c_a3, v1 + (h / 2) * c_c3, v2 + (h / 2) * c_d3, l1, l2, m1, m2);
				double c_d4 = f2(om1 + (h / 2), om2 + (h / 2) * c_b3, v1 + (h / 2) * c_c3, v2 + (h / 2) * c_d3, l1, l2, m1, m2);

				om1 = om1 + (h / 6) * (c_a1 + 2 * c_a2 + 2 * c_a3 + c_a4);
				om2 = om2 + (h / 6) * (c_b1 + 2 * c_b2 + 2 * c_b3 + c_b4);

				v1 = v1 + (h / 6) * (c_c1 + 2 * c_c2 + 2 * c_c3 + c_c4);
				v2 = v2 + (h / 6) * (c_d1 + 2 * c_d2 + 2 * c_d3 + c_d4);
			}
		}
		//Update
		x1 = x_origin + l1 * sin(om1);
		y1 = y_origin + l1 * cos(om1);

		x2 = x1 + l2 * sin(om2);
		y2 = y1 + l2 * cos(om2);

		c1_pos.x = x1;
		c1_pos.y = y1;

		c2_pos.x = x2;
		c2_pos.y = y2;

		//Draw new positions
		line1[0].position = sf::Vector2f(x_origin + m1, y_origin + m1);
		line1[1].position = c1_pos + sf::Vector2f(m1, m1);

		line2[0].position = c1_pos + sf::Vector2f(m1, m1);
		line2[1].position = c2_pos + sf::Vector2f(m2, m2);

		c1.setPosition(c1_pos);
		c2.setPosition(c2_pos);

		trace.push_back(sf::Vertex(sf::Vector2f(x2 + m2, y2 + m2)));

		//Render
		window.clear();
		window.draw(&trace[0], trace.size(), sf::Points);
		window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.draw(c1);
		window.draw(c2);

		window.display();
	}

	return 0;
}