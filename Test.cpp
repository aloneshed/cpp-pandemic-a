#include <iostream>
#include "doctest.h"
#include <stdexcept>
#include "Board.hpp"
#include "City.hpp"
#include "Color.hpp"
#include "Player.hpp"

#include "Researcher.hpp"
#include "Scientist.hpp"
#include "FieldDoctor.hpp"
#include "GeneSplicer.hpp"
#include "OperationsExpert.hpp"
#include "Dispatcher.hpp"
#include "Medic.hpp"
#include "Virologist.hpp"

using namespace std;
using namespace pandemic;

TEST_CASE("OperationsExpert Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Kinshasa] = 3;   // put 3 yellow disease cubes in Kinshasa.
    board[City::Kinshasa] = 2;   // change number of disease cubes in Kinshasa to 2.
    board[City::MexicoCity] = 3; // put 3 yellow disease cubes in MexicoCity
    board[City::Miami] = 2;      // put 2 yellow disease cube in Miami
    board[City::Chicago] = 1;    // put 1 blue disease cube in Chicago

    OperationsExpert player{board, City::Atlanta}; // initialize an "operations expert" player on the given board, in Atlanta.
    player.take_card(City::Johannesburg)           //-
        .take_card(City::Khartoum)
        .take_card(City::Kinshasa) //-
        .take_card(City::BuenosAires)
        .take_card(City::HoChiMinhCity);

    // Test Unique action for OperationsExpert player - Build
    CHECK_NOTHROW(player.build());
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Washington)); // legal action: you drive from Atlanta to a connected city.
    CHECK_NOTHROW(player.build());
    CHECK(player.get_num_of_card() == 5);

    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.
    CHECK_NOTHROW(player.drive(City::Chicago)); // legal action: you drive from Atlanta to a connected city.
    CHECK_NOTHROW(player.treat(City::Chicago)); // legal action: you remove 1 disease cube from current city (0 cube remains).
    CHECK_NOTHROW(player.build());
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.
    CHECK_NOTHROW(player.drive(City::Miami));   // legal action: you drive from Atlanta to a connected city.
    CHECK_NOTHROW(player.build());
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.

    CHECK_THROWS(player.drive(City::Algiers)); // disconnected city.
    CHECK_THROWS(player.drive(City::Milan));   // disconnected city.
    CHECK_THROWS(player.drive(City::Madrid));  // disconnected city.
    CHECK_THROWS(player.drive(City::NewYork)); // disconnected city.

    // fly_direct and treat action
    CHECK_NOTHROW(player.fly_direct(City::Kinshasa));
    CHECK(player.get_num_of_card() == 4);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 1);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 0);

    CHECK_THROWS(player.treat(City::Taipei));
    CHECK_THROWS(player.treat(City::Tokyo));
    CHECK_THROWS(player.treat(City::Manila));

    CHECK_NOTHROW(player.drive(City::Johannesburg));
    CHECK_NOTHROW(player.fly_charter(City::StPetersburg));
    CHECK(player.get_num_of_card() == 3);

    CHECK_THROWS(player.fly_shuttle(City::Chicago)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Miami));   // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Atlanta)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Essen));   // illegal action: there is no research station in Johannesburg and in Essen.

    //fly_shuttle action
    CHECK_NOTHROW(player.build());
    CHECK_NOTHROW(player.fly_shuttle(City::Chicago));
    CHECK_NOTHROW(player.fly_shuttle(City::Miami));
    CHECK_NOTHROW(player.fly_shuttle(City::Atlanta));

    player.take_card(City::LosAngeles)
        .take_card(City::Bogota)
        .take_card(City::SaoPaulo)
        .take_card(City::Lima);

    //discover_cure action
    CHECK(player.get_num_of_card() == 7);
    CHECK_NOTHROW(player.discover_cure(Color::Yellow));
    CHECK(player.get_num_of_card() == 2);

    CHECK_NOTHROW(player.drive(City::Miami)); // legal action: you drive from Atlanta to a connected city.
    CHECK(board[City::Miami] == 2);           //BEFORE treat
    CHECK_NOTHROW(player.treat(City::Miami)); // legal action: after discovering cure for yellow disease, the num of disease cube in Miami Decreases from 2 to 0.
    CHECK(board[City::Miami] == 0);           //AFTER treat
    CHECK_THROWS(player.treat(City::Miami));  // illegal action: there is no disease cube on Miami, should throw an exception.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "OperationsExpert");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("Dispatcher Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Seoul] = 2;    // put 2 red disease cubes in Seoul.
    board[City::Seoul] = 4;    // change number of disease cubes in Seoul to 4.
    board[City::Kolkata] = 3;  // put 3 black disease cubes in Kolkata
    board[City::Istanbul] = 5; // put 5 black disease cube in Istanbul
    board[City::London] = 1;   // put 1 blue disease cube in London

    Dispatcher player{board, City::Milan}; // initialize an "Dispatcher" player on the given board, in Milan.
    player.take_card(City::Bogota)         //-
        .take_card(City::Baghdad)          //-
        .take_card(City::Jakarta)
        .take_card(City::Mumbai)
        .take_card(City::Istanbul);

    //build action
    CHECK_THROWS(player.build()); // illegal action. player does not have the card Milan.
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Essen)); // legal action: you drive from Milan to a connected city.
    CHECK_THROWS(player.build());             // illegal action. player does not have the card Essen.

    CHECK_NOTHROW(player.drive(City::Milan));    // legal action: you drive back to Milan.
    CHECK_NOTHROW(player.drive(City::Paris));    // legal action: you drive from Milan to a connected city.
    CHECK_NOTHROW(player.drive(City::Milan));    // legal action: you drive back to Milan.
    CHECK_NOTHROW(player.drive(City::Istanbul)); // legal action: you drive from Milan to a connected city.
    CHECK_NOTHROW(player.build());
    CHECK_NOTHROW(player.treat(City::Istanbul)); // legal action: you remove 1 disease cube from current city (4 cube remains).
    CHECK(board[City::Istanbul] == 4);
    CHECK_NOTHROW(player.treat(City::Istanbul)); // legal action: you remove 1 disease cube from current city (3 cube remains).
    CHECK(board[City::Istanbul] == 3);

    CHECK_THROWS(player.drive(City::Lagos));    // disconnected city.
    CHECK_THROWS(player.drive(City::Riyadh));   // disconnected city.
    CHECK_THROWS(player.drive(City::Kinshasa)); // disconnected city.
    CHECK_THROWS(player.drive(City::NewYork));  // disconnected city.

    // Test Unique action for OperationsExpert player - fly_direct
    CHECK_NOTHROW(player.fly_direct(City::Bogota)); // legal action. there is research station in Istanbul.
    CHECK_NOTHROW(player.build());                  // legal action. player throw the card Bogota.
    CHECK(player.get_num_of_card() == 4);
    CHECK_NOTHROW(player.fly_direct(City::Istanbul)); // legal action. fly direct back to Istanbul.
    CHECK_NOTHROW(player.drive(City::Baghdad));       // connected city.
    CHECK_NOTHROW(player.build());                    // legal action. player throw the card Baghdad.
    CHECK(player.get_num_of_card() == 3);
    CHECK_NOTHROW(player.drive(City::Cairo));        // legal action. connected city.
    CHECK_THROWS(player.fly_direct(City::Istanbul)); // illegal action. there is no research station in Cairo.

    CHECK_THROWS(player.treat(City::Taipei));
    CHECK_THROWS(player.treat(City::Tokyo));
    CHECK_THROWS(player.treat(City::Manila));
    CHECK_THROWS(player.fly_shuttle(City::Chicago)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Miami));   // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Atlanta)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Essen));   // illegal action: there is no research station in Johannesburg and in Essen.

    //fly_charter action
    CHECK_NOTHROW(player.drive(City::Istanbul));
    CHECK_NOTHROW(player.fly_charter(City::Bogota)); // legal action. there is research station in Istanbul and in Bogota.
    CHECK(player.get_num_of_card() == 3);
    CHECK_NOTHROW(player.fly_charter(City::Baghdad)); // legal action. there is research station in Bogota and in Baghdad.
    CHECK_THROWS(player.fly_charter(City::Lagos));    // illegal action. there is no research station in Lagos.

    //fly_shuttle action
    CHECK_NOTHROW(player.fly_shuttle(City::Istanbul));
    CHECK_NOTHROW(player.fly_shuttle(City::Essen));
    CHECK_NOTHROW(player.fly_shuttle(City::Bogota));
    CHECK_NOTHROW(player.fly_shuttle(City::Istanbul));

    CHECK_THROWS(player.fly_shuttle(City::Chicago)); // illegal action: there is no research station in Chicago.
    CHECK_THROWS(player.fly_shuttle(City::Miami));   // illegal action: there is no research station in Miami.
    CHECK_THROWS(player.fly_shuttle(City::Atlanta)); // illegal action: there is no research station in Atlanta.
    CHECK_THROWS(player.fly_shuttle(City::Karachi)); // illegal action: there is no research station in Karachi.

    player.take_card(City::Delhi)
        .take_card(City::Kolkata)
        .take_card(City::Moscow)
        .take_card(City::Bangkok);

    //discover_cure action
    CHECK(player.get_num_of_card() == 7);
    CHECK_NOTHROW(player.discover_cure(Color::Black));
    CHECK(player.get_num_of_card() == 2);
    CHECK_NOTHROW(player.treat(City::Istanbul)); // legal action: after discovering cure for Black disease, the num of disease cube in Istanbul Decreases from 3 to 0.
    CHECK_THROWS(player.treat(City::Istanbul));  // illegal action: there is no disease cube on Istanbul, should throw an exception.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "Dispatcher");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("Scientist Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Kinshasa] = 3;   // put 3 yellow disease cubes in Kinshasa.
    board[City::Kinshasa] = 2;   // change number of disease cubes in Kinshasa to 2.
    board[City::MexicoCity] = 3; // put 3 yellow disease cubes in MexicoCity
    board[City::Miami] = 2;      // put 2 yellow disease cube in Miami
    board[City::Chicago] = 1;    // put 1 blue disease cube in Chicago

    Scientist player{board, City::Atlanta, 2}; // initialize an "Scientist" player on the given board, in Atlanta, Which can "discover_cure" action with 2 cards only.
    player.take_card(City::Johannesburg)       //-
        .take_card(City::Khartoum)
        .take_card(City::Kinshasa) //-
        .take_card(City::BuenosAires)
        .take_card(City::HoChiMinhCity);

    // build action
    CHECK_THROWS(player.build());
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Washington)); // legal action: you drive from Atlanta to a connected city.
    CHECK_THROWS(player.build());
    CHECK(player.get_num_of_card() == 5);

    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.
    CHECK_NOTHROW(player.drive(City::Chicago)); // legal action: you drive from Atlanta to a connected city.
    CHECK(board[City::Chicago] == 1);           // check number of disease cube in Chicago BEFORE treat.
    CHECK_NOTHROW(player.treat(City::Chicago)); // legal action: you remove 1 disease cube from current city (0 cube remains).
    CHECK(board[City::Chicago] == 0);           // check number of disease cube in Chicago AFTER treat.
    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.
    CHECK_NOTHROW(player.drive(City::Miami));   // legal action: you drive from Atlanta to a connected city.
    CHECK_THROWS(player.build());               // player does not have the cared Miami.
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.drive(City::Atlanta)); // legal action: you drive back to Atlanta.

    CHECK_THROWS(player.drive(City::Algiers)); // disconnected city.
    CHECK_THROWS(player.drive(City::Milan));   // disconnected city.
    CHECK_THROWS(player.drive(City::Madrid));  // disconnected city.
    CHECK_THROWS(player.drive(City::NewYork)); // disconnected city.

    // fly_direct and treat action
    CHECK_NOTHROW(player.fly_direct(City::Kinshasa));
    CHECK(player.get_num_of_card() == 4);
    CHECK(board[City::Kinshasa] == 2);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 1);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 0);
    CHECK_NOTHROW(player.build());

    CHECK_THROWS(player.treat(City::Taipei));
    CHECK_THROWS(player.treat(City::Tokyo));
    CHECK_THROWS(player.treat(City::Manila));

    // fly_charter action
    CHECK_NOTHROW(player.drive(City::Johannesburg));
    CHECK_NOTHROW(player.fly_charter(City::StPetersburg));
    CHECK(player.get_num_of_card() == 3);

    player.take_card(City::StPetersburg);
    CHECK_NOTHROW(player.build());

    //fly_shuttle action
    CHECK_NOTHROW(player.fly_shuttle(City::Kinshasa));

    CHECK_THROWS(player.fly_shuttle(City::Chicago)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Miami));   // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Atlanta)); // illegal action: there is no research station in Johannesburg.
    CHECK_THROWS(player.fly_shuttle(City::Essen));   // illegal action: there is no research station in Johannesburg and in Essen.

    player.take_card(City::LosAngeles)
        .take_card(City::Bogota)
        .take_card(City::SaoPaulo)
        .take_card(City::Lima);

    //discover_cure action
    CHECK(player.get_num_of_card() == 7);
    CHECK_NOTHROW(player.discover_cure(Color::Yellow));
    CHECK(player.get_num_of_card() == 5);

    CHECK_NOTHROW(player.drive(City::Miami)); // legal action: you drive from Atlanta to a connected city.
    CHECK(board[City::Miami] == 2);           //BEFORE treat
    CHECK_NOTHROW(player.treat(City::Miami)); // legal action: after discovering cure for yellow disease, the num of disease cube in Miami Decreases from 2 to 0.
    CHECK(board[City::Miami] == 0);           //AFTER treat
    CHECK_THROWS(player.treat(City::Miami));  // illegal action: there is no disease cube on Miami, should throw an exception.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "Scientist");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("Researcher Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Seoul] = 2;    // put 2 red disease cubes in Seoul.
    board[City::Seoul] = 4;    // change number of disease cubes in Seoul to 4.
    board[City::Kolkata] = 3;  // put 3 black disease cubes in Kolkata
    board[City::Istanbul] = 5; // put 5 black disease cube in Istanbul
    board[City::London] = 1;   // put 1 blue disease cube in London

    Researcher player{board, City::HongKong}; // initialize an "Researcher" player on the given board, in HongKong.
    player.take_card(City::Chennai)           //b
        .take_card(City::Baghdad)             //b
        .take_card(City::Jakarta)             //r
        .take_card(City::Mumbai)              //b
        .take_card(City::Istanbul)            //b
        .take_card(City::Delhi)               //b
        .take_card(City::Kolkata)             //b
        .take_card(City::HoChiMinhCity)       //r
        .take_card(City::Bangkok)             //r
        .take_card(City::Beijing)             //r
        .take_card(City::Osaka);              //r

    // Test Unique action for Researcher player - discover_cure
    CHECK(player.get_num_of_card() == 11);
    CHECK_NOTHROW(player.drive(City::Shanghai));     // connected city.
    CHECK_NOTHROW(player.drive(City::Seoul));        // connected city.
    CHECK_NOTHROW(player.discover_cure(Color::Red)); // legal action. no need research station.
    CHECK(player.get_num_of_card() == 6);

    CHECK_NOTHROW(player.fly_direct(City::Istanbul));  // legal action.
    CHECK_NOTHROW(player.discover_cure(Color::Black)); // legal action. no need research station.
    CHECK(board[City::Istanbul] == 5);
    CHECK_NOTHROW(player.treat(City::Istanbul)); // legal action: after discovering cure for Black disease, the num of disease cube in Istanbul Decreases from 5 to 0.
    CHECK(board[City::Istanbul] == 0);

    //test role function for player and is_clean function for board
    CHECK(player.role() == "Researcher");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("Medic Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Seoul] = 2;    // put 2 red disease cubes in Seoul.
    board[City::Seoul] = 4;    // change number of disease cubes in Seoul to 4.
    board[City::Kolkata] = 8;  // put 8 black disease cubes in Kolkata
    board[City::Istanbul] = 5; // put 5 black disease cube in Istanbul
    board[City::London] = 12;  // put 12 blue disease cube in London

    Medic player{board, City::HongKong}; // initialize an "Medic" player on the given board, in HongKong.
    player.take_card(City::Chennai)      //b
        .take_card(City::Baghdad)        //b
        .take_card(City::Jakarta)        //r
        .take_card(City::Mumbai)         //b
        .take_card(City::Istanbul)       //b
        .take_card(City::Delhi)          //b
        .take_card(City::Kolkata)        //b
        .take_card(City::HoChiMinhCity)  //r
        .take_card(City::Bangkok)        //r
        .take_card(City::Beijing)        //r
        .take_card(City::Osaka);         //r

    // Test Unique action for Medic player - treat
    CHECK_NOTHROW(player.drive(City::Shanghai)); // connected city.
    CHECK_NOTHROW(player.drive(City::Seoul));    // connected city.
    CHECK(board[City::Seoul] == 4);              // BEFORE treat
    CHECK_NOTHROW(player.treat(City::Seoul));    // should reduce the disease level to 0
    CHECK(board[City::Seoul] == 4);              // AFTER treat

    CHECK_NOTHROW(player.fly_direct(City::Istanbul));
    player.take_card(City::Istanbul);
    CHECK_NOTHROW(player.build());
    CHECK_NOTHROW(player.discover_cure(Color::Black));
    CHECK(board[City::Istanbul] == 5);
    CHECK_NOTHROW(player.treat(City::Istanbul));
    CHECK(board[City::Istanbul] == 0);
    player.take_card(City::Kolkata);
    CHECK_NOTHROW(player.fly_direct(City::Kolkata));
    CHECK(board[City::Kolkata] == 0); // we discover cure for black disease, so Medic reduce all black disease in every city he is in.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "Medic");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("Virologist Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Seoul] = 2;    // put 2 red disease cubes in Seoul.
    board[City::Seoul] = 4;    // change number of disease cubes in Seoul to 4.
    board[City::Kolkata] = 3;  // put 3 black disease cubes in Kolkata
    board[City::Istanbul] = 5; // put 5 black disease cube in Istanbul
    board[City::London] = 1;   // put 1 blue disease cube in London
    board[City::Osaka] = 1;    // put 1 blue disease cube in Osaka

    Researcher player{board, City::HongKong}; // initialize an "Virologist" player on the given board, in HongKong.
    player.take_card(City::Chennai)           //b
        .take_card(City::Baghdad)             //b
        .take_card(City::Jakarta)             //r
        .take_card(City::Mumbai)              //b
        .take_card(City::Istanbul)            //b
        .take_card(City::London)              //blue
        .take_card(City::Kolkata)             //b
        .take_card(City::HoChiMinhCity)       //r
        .take_card(City::Bangkok)             //r
        .take_card(City::Beijing)             //r
        .take_card(City::Osaka);              //r

    CHECK_NOTHROW(player.treat(City::Kolkata));  // legal action.
    CHECK_NOTHROW(player.treat(City::London));   // legal action.
    CHECK_NOTHROW(player.treat(City::Osaka));    // legal action.
    CHECK_NOTHROW(player.treat(City::Istanbul)); // legal action.

    CHECK_THROWS(player.treat(City::NewYork)); // illegal action. NewYork does not have disease cubes.
    CHECK_THROWS(player.treat(City::Seoul));   // illegal action. player does not have the card Seoul.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "Virologist");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("GeneSplicer Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Kinshasa] = 3;   // put 3 yellow disease cubes in Kinshasa.
    board[City::Kinshasa] = 2;   // change number of disease cubes in Kinshasa to 2.
    board[City::MexicoCity] = 3; // put 3 yellow disease cubes in MexicoCity
    board[City::Miami] = 2;      // put 2 yellow disease cube in Miami
    board[City::Chicago] = 1;    // put 1 blue disease cube in Chicago

    GeneSplicer player{board, City::Atlanta}; // initialize an "GeneSplicer" player on the given board, in Atlanta.
    player.take_card(City::Johannesburg)      //-
        .take_card(City::Khartoum)
        .take_card(City::Kinshasa) //-
        .take_card(City::BuenosAires)
        .take_card(City::HoChiMinhCity)
        .take_card(City::Atlanta);

    // Test Unique action for GeneSplicer player - discover_cure

    CHECK_NOTHROW(player.build());
    CHECK(player.get_num_of_card() == 5);
    CHECK_NOTHROW(player.discover_cure(Color::Yellow)); // legal action.
    CHECK(player.get_num_of_card() == 0);

    //test role function for player and is_clean function for board
    CHECK(player.role() == "GeneSplicer");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}

TEST_CASE("FieldDoctor Test")
{
    Board board; // Initialize an empty board (with 0 disease cubes in any city).
    CHECK(board.is_clean());
    board[City::Bogota] = 3;     // put 3 yellow disease cubes in Bogota.
    board[City::Bogota] = 6;     // change number of disease cubes in Bogota to 6.
    board[City::MexicoCity] = 3; // put 3 yellow disease cubes in MexicoCity
    board[City::Miami] = 2;      // put 2 yellow disease cube in Miami
    board[City::Lima] = 5;       // put 5 yellow disease cube in Lima

    FieldDoctor player{board, City::Bogota}; // initialize an "FieldDoctor" player on the given board, in Bogota.
    player.take_card(City::Johannesburg)     //-
        .take_card(City::Khartoum)
        .take_card(City::Kinshasa) //-
        .take_card(City::BuenosAires)
        .take_card(City::HoChiMinhCity);

    // Test Unique action for FieldDoctor player - treat
    CHECK_NOTHROW(player.treat(City::Bogota)); // legal action.
    CHECK(board[City::Bogota] == 5);           // Reduce the level by 1.

    CHECK_NOTHROW(player.treat(City::MexicoCity)); // legal action.
    CHECK(board[City::MexicoCity] == 2);           // Reduce the level by 1.

    CHECK_NOTHROW(player.treat(City::Miami)); // legal action.
    CHECK(board[City::Miami] == 1);           // Reduce the level by 1.
    CHECK_NOTHROW(player.treat(City::Miami)); // legal action.
    CHECK(board[City::Miami] == 0);           // Reduce the level by 1.
    CHECK_THROWS(player.treat(City::Miami));  // illegal action. disease level already 0.

    CHECK_NOTHROW(player.treat(City::Lima)); // legal action.
    CHECK(board[City::MexicoCity] == 4);     // Reduce the level by 1.

    //test role function for player and is_clean function for board
    CHECK(player.role() == "FieldDoctor");
    CHECK(!board.is_clean());
    board.remove_cures();
    CHECK(board.is_clean());
}
