#include <SFML/Graphics.hpp>
#include "cell.h"
#include "field.h"
#include <unistd.h>

void setSize(int& x, int& y, int& bombs)
{
    sf::Font font;
    font.loadFromFile("MesloLGS NF Regular.ttf");

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Set size");
    sf::RectangleShape box[3];
    sf::Text text[3];

    for (int i = 0; i < 3; i++)
    {
        box[i].setSize({300, 100});
        box[i].setFillColor(sf::Color::White);
        box[i].setOutlineColor(sf::Color::Black);
        box[i].setOutlineThickness(3);

        text[i].setFont(font);
        text[i].setCharacterSize(40);
        text[i].setFillColor(sf::Color::Black);
    }

    box[0].setPosition({250, 50});
    box[1].setPosition({250, 250});
    box[2].setPosition({250, 450});

    text[0].setString("Size: 8x8\nMines: 10");
    text[0].setPosition({250, 50});
    text[1].setString("Size: 16x16\nMines: 40");
    text[1].setPosition({250, 250});
    text[2].setString("Size: 22x22\nMines: 99");
    text[2].setPosition({250, 450});

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                int mx = sf::Mouse::getPosition(window).x, my = sf::Mouse::getPosition(window).y;

                if (mx >= 250 && mx <= 550)
                {
                    if (my >= 50 && my <= 150)
                    {
                        x = 8; y = 8; bombs = 10;
                        window.close();
                    }
                    else if (my >= 250 && my <= 350)
                    {
                        x = 16; y = 16; bombs = 40;
                        window.close();
                    }
                    else if (my >= 450 && my <= 550)
                    {
                        x = 22; y = 22; bombs = 99;
                    }
                }
            }
        }

        window.clear();

        for (int i = 0; i < 3; i++)
        {
            window.draw(box[i]);
            window.draw(text[i]);
        }

        window.display();
    }
}

int main()
{
    int x, y, bombs, flags = 0;

    setSize(x, y, bombs);

    sf::RenderWindow window(sf::VideoMode({x*40, y*40}), "Minesweeper");
    sf::RenderWindow* windowPointer = &window;

    sf::Font font;
    font.loadFromFile("MesloLGS NF Regular.ttf");
    sf::Font* fontPointer = &font;
    
    auto gameOver {[windowPointer, fontPointer]()
    {
        sf::RenderWindow goScreen(sf::VideoMode({280, 100}), "Game Over");
        sf::Text message("Game Over", (*fontPointer));
        message.setCharacterSize(50);
        message.setFillColor(sf::Color::Red);
        goScreen.draw(message);
        goScreen.display();
        sleep(1);
        goScreen.close();
        (*windowPointer).close();
    }};

    Field field(x, y, bombs);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonReleased:
                int mx = sf::Mouse::getPosition(window).x / 40, my = sf::Mouse::getPosition(window).y / 40;
                switch (event.key.code)
                {
                case sf::Mouse::Left:
                    if (field.field[mx][my].isMine) gameOver();
                    else if (field.field[mx][my].stat == 1 || field.field[mx][my].num == 0)
                    {
                        int flagsAround = 0, minesAround = 0, flagedMines = 0;
                        for (int a = mx - 1; a < mx + 2; a++)
                        {
                            for (int b = my - 1; b < my + 2; b++)
                            {
                                if (a > -1 && a < x && b > -1 && b < y)
                                {
                                    if (field.field[a][b].isMine) minesAround++;
                                    if (field.field[a][b].stat == 2) flagsAround++;
                                    if (field.field[a][b].isMine && field.field[a][b].stat == 2) flagedMines++;
                                }
                            }
                        }

                        if (flagsAround == minesAround)
                        {
                            for (int a = mx - 1; a < mx + 2; a++)
                            {
                                for (int b = my - 1; b < my + 2; b++)
                                {
                                    if (a > -1 && a < x && b > -1 && b < y)
                                    {
                                        if (!field.field[a][b].isMine) field.field[a][b].stat = 1;
                                    }
                                }
                            }

                            if (minesAround != flagedMines) gameOver();
                        }
                    }
                    else field.field[mx][my].stat = 1;
                    break;

                case sf::Mouse::Right:
                    if (field.field[mx][my].stat == 2) field.field[mx][my].stat = 0;
                    else
                    {
                        field.field[mx][my].stat = 2;
                        if (field.field[mx][my].isMine) flags++;
                    }
                    break;
                }
                break;
            }
        }

        window.clear();

        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                sf::Texture img;
                if (field.field[i][j].stat == 0)
                {
                    img.loadFromFile("Sprites/cellClosedSprite.png");
                    field.field[i][j].sprite.setTexture(img);
                }
                else if (field.field[i][j].stat == 1)
                {
                    std::string addr = "Sprites/";
                    addr += char('0' + field.field[i][j].num);
                    addr += ".png";

                    img.loadFromFile(addr);
                    field.field[i][j].sprite.setTexture(img);
                }
                else
                {
                    img.loadFromFile("Sprites/flagSprite.png");
                    field.field[i][j].sprite.setTexture(img);
                }

                field.field[i][j].sprite.setPosition(i * 40, j * 40);
                window.draw(field.field[i][j].sprite);
            }
        }

        window.display();

        if (flags == bombs)
        {
            sf::RenderWindow winScreen(sf::VideoMode({280, 100}), "WinScreen");
            sf::Text message("You Won!", font);
            message.setCharacterSize(50);
            message.setFillColor(sf::Color::Green);
            winScreen.draw(message);
            winScreen.display();
            sleep(1);
            winScreen.close();
            window.close();
        }
    }

    return 0;
}