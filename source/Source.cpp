
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

#include "Board.h"
#include "Utility.h"

#include <SFML/Graphics.hpp>

using namespace std;

vector<Piece*> initialize_pieces(vector<pair<int**, int>> arr) {
    vector<Piece*> pieces;
    for (int i = 0; i < arr.size(); i++) {
        Piece* p = new Piece(arr[i].second, arr[i].first);
        p->set_color_key(i + 1);
        pieces.push_back(p);
    }
    return pieces;
}

int main() {

    srand(time(0));

    int** s_mat = initialize_matrix(3, 3);
    s_mat[0][1] = 1; s_mat[0][2] = 1; s_mat[1][0] = 1; s_mat[1][1] = 1;

    int** z_mat = initialize_matrix(3, 3);
    z_mat[0][1] = 1; z_mat[0][0] = 1; z_mat[1][2] = 1; z_mat[1][1] = 1;

    int** i_mat = initialize_matrix(4, 4);
    i_mat[1][0] = 1; i_mat[1][1] = 1; i_mat[1][2] = 1; i_mat[1][3] = 1;

    int** o_mat = initialize_matrix(2, 2);
    o_mat[0][0] = 1; o_mat[0][1] = 1; o_mat[1][0] = 1; o_mat[1][1] = 1;

    int** j_mat = initialize_matrix(3, 3);
    j_mat[0][0] = 1; j_mat[1][0] = 1; j_mat[1][1] = 1; j_mat[1][2] = 1;

    int** l_mat = initialize_matrix(3, 3);
    l_mat[0][2] = 1; l_mat[1][0] = 1; l_mat[1][1] = 1; l_mat[1][2] = 1;

    int** t_mat = initialize_matrix(3, 3);
    t_mat[1][0] = 1; t_mat[0][1] = 1; t_mat[1][1] = 1; t_mat[1][2] = 1;

    vector<pair<int**, int>> piece_arr = {
        {s_mat, 3},
        {z_mat, 3},
        {i_mat, 4},
        {o_mat, 2}, 
        {j_mat, 3},
        {l_mat, 3},
        {t_mat, 3}
    };

    vector<Piece*> piece_ptr = initialize_pieces(piece_arr);

    Board board(10, 24);

    board.set_edge_length(30);
    board.set_default_color(sf::Color(255,255,255));
    board.set_position(50, 0);
    board.set_margin(4);

    map<int, sf::Color> colors{
        {1, sf::Color::Blue},
        {2, sf::Color::Red},
        {3, sf::Color::Green},
        {4, sf::Color::Magenta},
        {5, sf::Color::Cyan},
        {6, sf::Color::Yellow},
        {7, sf::Color::Black}
    };

    board.set_colors(colors);
    board.set_pieces(piece_ptr);

    board.randomize_piece();

    sf::RenderWindow window(sf::VideoMode(800, 800), "Tetris!");
    sf::View view;

    sf::RectangleShape background(sf::Vector2f(0, 0));
    background.setFillColor(sf::Color(64, 64, 64));


    sf::Font sansation;
    if (!sansation.loadFromFile("sansation.ttf"))
        cout << "couldn't open sansation.ttf" << endl;

    sf::Text score_label;
    score_label.setFont(sansation);
    score_label.setFillColor(sf::Color::White);
    score_label.setPosition(420, 120);
    score_label.setCharacterSize(50);

    sf::Text inst_label;
    inst_label.setFont(sansation);
    inst_label.setFillColor(sf::Color::White);
    inst_label.setPosition(420, 180);
    inst_label.setCharacterSize(25);
    inst_label.setString("Use AD to move horizontally.\nUse S to speed up drop.\nUse QE to rotate.");

    sf::Text dead_label;
    dead_label.setFont(sansation);
    dead_label.setFillColor(sf::Color::White);
    dead_label.setPosition(420, 300);
    dead_label.setCharacterSize(30);
    dead_label.setString("You lost!\nPress any key to restart.");

    window.setFramerateLimit(5);

    while (window.isOpen())
    {
        view.setSize(sf::Vector2f(window.getSize()));
        view.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

        background.setSize(sf::Vector2f(window.getSize()));

        sf::Event event;
        while (window.pollEvent(event))
        {

            switch (event.type) {
                
            case (sf::Event::Closed): {
                window.close();
                break;
            }
            case (sf::Event::KeyPressed): {
                switch (event.key.code) {
                case (sf::Keyboard::A): {
                    board.move_x(-1);
                    break;
                }
                case (sf::Keyboard::D): {
                    board.move_x(1);
                    break;
                }
                case (sf::Keyboard::Q): {
                    board.rotate_piece(1);
                    break;
                }
                case (sf::Keyboard::E): {
                    board.rotate_piece(-1);
                    break;
                }
                }
                board.restart_if_lost();
                break;
            }

            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            window.setFramerateLimit(15);
        }
        else {
            window.setFramerateLimit(5);
        }

        board.update();

        window.clear();


        window.setView(view);
        window.draw(background);

        window.draw(board);

        score_label.setString("Score: " + to_string(board.score()));
        window.draw(score_label);
        window.draw(inst_label);

        if (board.lost()) window.draw(dead_label);

        window.display();
    }

    return 0;

}