#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

// Reference: https://www.youtube.com/watch?v=RH3iAmMsolo

double safe_acos(double value) {
    if (value <= -1.0)
        return M_PI;
    else if (value >= 1.0)
        return 0;
    else
        return acos(value);
}

double safe_atan(double y, double x) {
    return atan2(y, x);
}

int main()
{
    // create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stick Arm");

    // create arm 1
    const int lengthArm1 = 100;
    const int widthArm1 = 10;
    const int Origin1_x = 200;
    const int Origin1_y = 300;
    sf::RectangleShape arm1(sf::Vector2f(lengthArm1, widthArm1));
    arm1.setOrigin(0, widthArm1/2); // Set the origin to the left end of the rectangle (this is where rotation will occur)
    arm1.setPosition(Origin1_x, Origin1_y); // Position the arm on the window
    arm1.setFillColor(sf::Color::Red);

    // create arm 1
    const int lengthArm2 = 80;
    const int widthArm2 = 10;
    sf::RectangleShape arm2(sf::Vector2f(lengthArm2, widthArm2));
    arm2.setOrigin(0, widthArm2/2); // Set the origin to the left end of the rectangle (this is where rotation will occur)
    arm2.setPosition(Origin1_x + lengthArm1, Origin1_y ); // Position the arm on the window
    arm2.setFillColor(sf::Color::Green);

    
    // initiazing variables
    sf::Vector2i mousePos = {0, 0};
    double q1 = 0, q2 = 0, mouseDis_squared = 0, mouseDis = 0;

    // loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // get the position of the mouse cursor relative to the window
        // if (event.type == sf::Event::MouseButtonPressed) {
            mousePos = sf::Mouse::getPosition(window);
            mousePos.x = mousePos.x - Origin1_x;
            mousePos.y = Origin1_y - mousePos.y;
        // }

        // calculate the angle between the arm and the mouse cursor
        mouseDis_squared  = powf(mousePos.x, 2) + powf(mousePos.y, 2);
        mouseDis = sqrt(mouseDis_squared);
        if (mouseDis >= lengthArm1 + lengthArm1) {
            q1 = atan2(mousePos.y, mousePos.x);
            q2 = 0;
        } else {
            q2 = (mouseDis_squared - pow(lengthArm1, 2) - pow(lengthArm2, 2)) / (2 * lengthArm1 * lengthArm2);
            q2 = safe_acos(q2);
            q1 = safe_atan( mousePos.y,  mousePos.x ) - safe_atan( lengthArm2 * sin(q2), lengthArm1 + lengthArm2 * cos(q2) );
        //     q2 = acos(q2);
        //     q1 = atan2( lengthArm2 * sin(q2), lengthArm1 + lengthArm2 * cos(q2) );
        }            
        std::cout << "x: " << mousePos.x << ", y: " << mousePos.y << ", d: " << mouseDis << ", q1: " << q1 << ", q2: " << q2 <<  std::endl;

        // set the rotation of the arm
        arm1.setRotation(-q1 * 180 / M_PI);
        arm2.setRotation(-(q1 + q2) * 180 / M_PI);
        arm2.setPosition(Origin1_x + (lengthArm1)*cos(q1), Origin1_y  - lengthArm1*sin(q1)) ;


        // clear the window
        window.clear();

        // draw the arm
        window.draw(arm1);
        window.draw(arm2);

        // display the contents of the window
        window.display();
    }

    return 0;
}