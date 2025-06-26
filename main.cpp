#include <SFML/Graphics.hpp>
#include "cell.h"
#include "field.h"
#include <unistd.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode({320, 320}), "Minesweeper");
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


    int x = 8, y = 8, bombs = 10, flags = 0;

    Field field(x, y);

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