#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include "Tile.h"

void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x/2.0f,
        textRect.position.y + textRect.size.y/2.0f});
        text.setPosition({x, y});
        }

void SetUpGame(int rowTiles, int colTiles, int numMines, std::vector<Tile*> &tileList) {
    for (int i = 0; i < rowTiles; i++) {
        for (int j = 0; j < colTiles; j++) {
            Tile* NewTile = new Tile();
            tileList.push_back(NewTile);
        }
    }

    //Place down mines
    std::vector<Tile*> TempTileList = tileList;
    for (int i = 0; i < numMines; i++) {
        int MineLocation = rand() % TempTileList.size();
        TempTileList[MineLocation]->SetMine(1);
        TempTileList.erase(TempTileList.begin() + MineLocation);
    }

    //Set values for tiles
    for (int i = 0; i < tileList.size(); i++) {
        tileList[i]->CheckSurrounding(i, tileList, rowTiles, colTiles);
        //cout << "Val of " << i << ": " << tileList[i]->GetValue() << endl;
    }
}

void SetUpLeaderBoard(std::ifstream &leaderBoard, std::string &content, std::vector<std::string> &data) {
    leaderBoard.open("../../cmake-build-debug/files/leaderboard.txt");
    if (!leaderBoard.is_open()) {
        std::cerr << "Could not open leaderboard file." << std::endl;
    }
    content = "";
    data.clear();
    std::string Read;
    int Rank = 1;
    while (getline(leaderBoard, Read)) {
        std::stringstream ss(Read);
        content += std::to_string(Rank);
        content += ".\t";
        while (getline(ss, Read, ',')) {
            data.push_back(Read);
            content += Read;
            content += "\t";
        }
        content += "\n\n";
        Rank++;
    }
    leaderBoard.close();
}

void EmptyTiles(std::vector<Tile*> &tileList) {
    for (int i = 0; i < tileList.size(); i++) {
        delete tileList[i];
    }
    tileList.clear();
}

int main()
{
    using namespace std;
    //Prep
    ifstream GameDetails;
    GameDetails.open("../../cmake-build-debug/files/config.cfg");
    if (!GameDetails.is_open()) {
        cerr << "Could not open config file." << endl;
        return 1;
    }
    ifstream LeaderBoardFileREAD;
    ofstream LeaderBoardFileWRITE;
    bool DebugMode = false;
    string Columns;
    string Rows;
    string Mines;
    getline(GameDetails, Columns);
    getline(GameDetails, Rows);
    getline(GameDetails, Mines);
    GameDetails.close();
    string LeaderBoardContent;
    vector<string> LeaderBoardData;
    SetUpLeaderBoard(LeaderBoardFileREAD, LeaderBoardContent, LeaderBoardData);

    /*for (int i = 0; i < LeaderBoardData.size(); i++) {//Print out the data from the leaderboard file
        cout << LeaderBoardData[i] << endl;
    }*/

    int MinesLeft = stoi(Mines);
    int MinesLeftCOUNTER = MinesLeft;
    unsigned int Width = (unsigned int)stoi(Columns)*32;
    unsigned int Height = (unsigned int)stoi(Rows)*32 + 100;
    string NameInput = "|";
    vector<Tile*> Tiles;
    vector<sf::FloatRect> TileBoxes;

    // create the Welcome window
    sf::RenderWindow WelcomeWindow(sf::VideoMode({Width, Height}), "Minesweeper", sf::Style::Close);
    // create other windows
    sf::RenderWindow GameWindow;
    sf::RenderWindow LeaderBoardWindow;

    // run the program as long as the window is open
    while (WelcomeWindow.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = WelcomeWindow.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                WelcomeWindow.close();

            //name being entered
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if ((textEntered->unicode >= 65 && textEntered->unicode <= 90) || (textEntered->unicode >= 97 && textEntered->unicode <= 122)) {
                    if (NameInput.size() < 11) {
                        NameInput.erase(NameInput.size()-1, 1);
                        NameInput += textEntered->unicode;
                        NameInput += "|";
                    }
                } else if (textEntered->unicode == 8 && NameInput.size() >= 2) {
                      NameInput.erase(NameInput.size()-2, 1);
                }
            }

            // go to game window
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
                    if (NameInput.size() <= 1) {
                        cerr << "No name entered." << endl;
                    } else {
                        WelcomeWindow.close();
                        GameWindow.create(sf::VideoMode({Width, Height}), "Minesweeper", sf::Style::Close);
                    }
                }
            }
        }

        // clear the window with color
        WelcomeWindow.clear(sf::Color::Blue);

        // draw everything here...
        // window.draw(...);
        sf::Font font("../../cmake-build-debug/files/font.ttf");
        sf::Text WelcomeText(font);
        sf::Text EnterNameText(font);
        sf::Text Name(font);
        WelcomeText.setString("WELCOME TO MINESWEEPER!");
        EnterNameText.setString("Enter your name:");
        //reformat name
        NameInput[0] = toupper(NameInput[0]);
        for (int i = 1; i < NameInput.size(); i++) {
            NameInput[i] = tolower(NameInput[i]);
        }
        Name.setString(NameInput);

        //set the position of the objects
        WelcomeText.setCharacterSize(24);
        EnterNameText.setCharacterSize(20);
        Name.setCharacterSize(18);
        setText(WelcomeText, Width/2, Height/2 - 150);
        setText(EnterNameText, Width/2, Height/2 - 75);
        setText(Name, Width/2, Height/2 - 45);

        // set the object colors
        WelcomeText.setFillColor(sf::Color::White);
        EnterNameText.setFillColor(sf::Color::White);
        Name.setFillColor(sf::Color::Yellow);

        //set the object styles
        WelcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        EnterNameText.setStyle(sf::Text::Bold);
        Name.setStyle(sf::Text::Bold);

        //draw the shape
        WelcomeWindow.draw(WelcomeText);
        WelcomeWindow.draw(EnterNameText);
        WelcomeWindow.draw(Name);

        // end the current frame
        WelcomeWindow.display();
    }

    int RowTiles = Width/32;
    int ColTiles = (Height-100)/32;
    bool GamePaused = false;
    bool GameWon = false;
    bool GameLost = false;
    SetUpGame(RowTiles, ColTiles, stoi(Mines), Tiles);
    sf::Clock GameClock;
    //Game window game loop
    while (GameWindow.isOpen()) {

        // clear the window with black color
        GameWindow.clear(sf::Color::White);

        //set up time objects
        sf::Time TimeElapsed = GameClock.getElapsedTime();

        //draw everything here...
        // window.draw()...
        //Set textures
        sf::Texture HappyFace("../../cmake-build-debug/files/images/face_happy.png");
        sf::Texture WinFace("../../cmake-build-debug/files/images/face_win.png");
        sf::Texture LoseFace("../../cmake-build-debug/files/images/face_lose.png");
        sf::Texture Debug("../../cmake-build-debug/files/images/debug.png");
        sf::Texture Pause("../../cmake-build-debug/files/images/pause.png");
        sf::Texture Play("../../cmake-build-debug/files/images/play.png");
        sf::Texture LeaderBoard("../../cmake-build-debug/files/images/leaderboard.png");
        sf::Texture Digits("../../cmake-build-debug/files/images/digits.png");
        sf::Texture HiddenTile("../../cmake-build-debug/files/images/tile_hidden.png");
        sf::Texture RevealedTile("../../cmake-build-debug/files/images/tile_revealed.png");
        sf::Texture Num1("../../cmake-build-debug/files/images/number_1.png");
        sf::Texture Num2("../../cmake-build-debug/files/images/number_2.png");
        sf::Texture Num3("../../cmake-build-debug/files/images/number_3.png");
        sf::Texture Num4("../../cmake-build-debug/files/images/number_4.png");
        sf::Texture Num5("../../cmake-build-debug/files/images/number_5.png");
        sf::Texture Num6("../../cmake-build-debug/files/images/number_6.png");
        sf::Texture Num7("../../cmake-build-debug/files/images/number_7.png");
        sf::Texture Num8("../../cmake-build-debug/files/images/number_8.png");
        sf::Texture Mine("../../cmake-build-debug/files/images/mine.png");
        sf::Texture Flag("../../cmake-build-debug/files/images/flag.png");

        //Set sprites
        sf::Sprite FaceSprite(HappyFace);
        if (GameWon) {
            FaceSprite.setTexture(WinFace);
        } else if (GameLost) {
            FaceSprite.setTexture(LoseFace);
        }
        sf::Sprite DebugSprite(Debug);
        sf::Sprite PausePlaySprite(Pause);
        if (GamePaused) {
            PausePlaySprite.setTexture(Play);
        }
        sf::Sprite LeaderBoardSprite(LeaderBoard);
        sf::Sprite NegativeMines(Digits);
        sf::Sprite MineCounter1(Digits);
        sf::Sprite MineCounter2(Digits);
        sf::Sprite MineCounter3(Digits);
        sf::Sprite MinuteTimer1(Digits);
        sf::Sprite MinuteTimer2(Digits);
        sf::Sprite SecondTimer1(Digits);
        sf::Sprite SecondTimer2(Digits);

        //Set position
        float RowNumAndSome = (Height - 100)/32 + 0.5f;
        FaceSprite.setPosition({(float)(Width/2 - 32), 32 * RowNumAndSome});
        DebugSprite.setPosition({(float)(Width - 304), 32 * RowNumAndSome});
        PausePlaySprite.setPosition({(float)(Width - 240), 32 * RowNumAndSome});
        LeaderBoardSprite.setPosition({(float)(Width - 176), 32 * RowNumAndSome});
        if (MinesLeft < 0) {
            MinesLeftCOUNTER = MinesLeft*(-1);
            NegativeMines.setTextureRect(sf::IntRect({210, 0}, {21, 32}));
            NegativeMines.setPosition({12, 32 * RowNumAndSome + 16});
        } else {
            MinesLeftCOUNTER = MinesLeft;
        }
        int MinesLeft1 = (MinesLeftCOUNTER/100)%10;
        MineCounter1.setTextureRect(sf::IntRect({MinesLeft1*21, 0}, {21, 32}));
        MineCounter1.setPosition({33, 32 * RowNumAndSome + 16});
        int MinesLeft2 = (MinesLeftCOUNTER/10)%10;
        MineCounter2.setTextureRect(sf::IntRect({MinesLeft2*21, 0}, {21, 32}));
        MineCounter2.setPosition({54, 32 * RowNumAndSome + 16});
        int MinesLeft3 = MinesLeftCOUNTER%10;
        MineCounter3.setTextureRect(sf::IntRect({MinesLeft3*21, 0}, {21, 32}));
        MineCounter3.setPosition({75, 32 * RowNumAndSome + 16});
        int Minutes = TimeElapsed.asSeconds() / 60;
        int Minutes1 = (Minutes/10)%10;
        int Minutes2 = Minutes%10;
        MinuteTimer1.setTextureRect(sf::IntRect({Minutes1*21, 0}, {21, 32}));
        MinuteTimer1.setPosition({(float)Width - 97, 32 * RowNumAndSome + 16});
        MinuteTimer2.setTextureRect(sf::IntRect({Minutes2*21, 0}, {21, 32}));
        MinuteTimer2.setPosition({(float)Width - 76, 32 * RowNumAndSome + 16});
        int Seconds = (int)TimeElapsed.asSeconds() % 60;
        int Seconds1 = (Seconds/10)%10;
        int Seconds2 = Seconds%10;
        SecondTimer1.setTextureRect(sf::IntRect({Seconds1*21, 0}, {21, 32}));
        SecondTimer1.setPosition({(float)Width - 54, 32 * RowNumAndSome + 16});
        SecondTimer2.setTextureRect(sf::IntRect({Seconds2*21, 0}, {21, 32}));
        SecondTimer2.setPosition({(float)Width - 33, 32 * RowNumAndSome + 16});

        //Draw tiles
        int TilesInRow = Width/32;
        int TilesInColumn = (Height-100)/32;
        for (int i = 0; i < TilesInColumn; i++) {
            for (int j = 0; j < TilesInRow; j++) {
                int index = i*TilesInRow + j;
                if (Tiles[index]->GetRevealed()) { //Sprite is revealed
                    sf::Sprite RevealedSprite(RevealedTile);
                    sf::Sprite TileNum(Num1);
                    RevealedSprite.setPosition({(float)j*32, (float)i*32});
                    GameWindow.draw(RevealedSprite);
                    int TileVal = Tiles[index]->GetValue();
                    if (TileVal == 1) { //Num is 1
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 2) { //Num is 2
                        TileNum.setTexture(Num2);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 3) { //Num is 3
                        TileNum.setTexture(Num3);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 4) { //Num is 4
                        TileNum.setTexture(Num4);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 5) { //Num is 5
                        TileNum.setTexture(Num5);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 6) { //Num is 6
                        TileNum.setTexture(Num6);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 7) { //Num is 7
                        TileNum.setTexture(Num7);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else if (TileVal == 8) { //Num is 8
                        TileNum.setTexture(Num8);
                        TileNum.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(TileNum);
                    } else { //No num
                        //Reveal surrounding tiles here
                    }
                } else { //Sprite is hidden
                    sf::Sprite TileSprite(HiddenTile);
                    TileSprite.setPosition({(float)j*32, (float)i*32});
                    GameWindow.draw(TileSprite);
                    sf::FloatRect TileBox = TileSprite.getGlobalBounds();
                    if (TileBoxes.size() < TilesInRow * TilesInColumn) {
                        TileBoxes.push_back(TileBox);
                    }
                    if (Tiles[index]->GetFlag()) { //Sprite is flagged
                        sf::Sprite FlagSprite(Flag);
                        FlagSprite.setPosition({(float)j*32, (float)i*32});
                        GameWindow.draw(FlagSprite);
                    }
                }
            }
        }

        //Debug mode
        if (DebugMode && !GamePaused) {
            //cout << Tiles.size() << endl;
            for (int i = 0; i < TilesInRow; i++) {
                for (int j = 0; j < TilesInColumn; j++) {
                    int index = j*TilesInRow + i;
                    if (Tiles[index]->GetMine() == 1) {
                        sf::Sprite MineSprite(Mine);
                        MineSprite.setPosition({(float)i*32, (float)j*32});
                        GameWindow.draw(MineSprite);
                    }
                }
            }
        } else if (GamePaused) {
            for (int i = 0; i < TilesInRow; i++) {
                for (int j = 0; j < TilesInColumn; j++) {
                    sf::Sprite RevealedSprite(RevealedTile);
                    RevealedSprite.setPosition({(float)i*32, (float)j*32});
                    GameWindow.draw(RevealedSprite);
                }
            }
        }


        //draw the objects
        GameWindow.draw(FaceSprite);
        GameWindow.draw(DebugSprite);
        GameWindow.draw(PausePlaySprite);
        GameWindow.draw(LeaderBoardSprite);
        if (!(MinesLeft >= 0)) {
            GameWindow.draw(NegativeMines);
        }
        GameWindow.draw(MineCounter1);
        GameWindow.draw(MineCounter2);
        GameWindow.draw(MineCounter3);
        GameWindow.draw(MinuteTimer1);
        GameWindow.draw(MinuteTimer2);
        GameWindow.draw(SecondTimer1);
        GameWindow.draw(SecondTimer2);

        //Get bounding boxes
        sf::FloatRect DebugBox = DebugSprite.getGlobalBounds();
        sf::FloatRect FaceBox = FaceSprite.getGlobalBounds();
        sf::FloatRect LeaderBoardBox = LeaderBoardSprite.getGlobalBounds();
        sf::FloatRect PausePlayBox = PausePlaySprite.getGlobalBounds();

        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = GameWindow.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()) {
                GameWindow.close();
            }

            //Mouse clicked
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                sf::Vector2i MousePosition = sf::Mouse::getPosition(GameWindow);
                if (DebugBox.contains({(float)MousePosition.x, (float)MousePosition.y}) && !GamePaused && !GameWon && !GameLost) { //Debug button
                    if (DebugMode) {
                        DebugMode = false;
                    } else {
                        DebugMode = true;
                    }
                } else if (FaceBox.contains({(float)MousePosition.x, (float)MousePosition.y})) { //Face button
                    EmptyTiles(Tiles);
                    TileBoxes.clear();
                    GameClock.reset();
                    SetUpGame(TilesInRow, TilesInColumn, stoi(Mines), Tiles);
                    MinesLeft = stoi(Mines);
                    DebugMode = false;
                    GamePaused = false;
                    GameWon = false;
                    GameLost = false;
                    GameClock.start();
                } else if (LeaderBoardBox.contains({(float)MousePosition.x, (float)MousePosition.y})) {//Leaderboard button
                    for (int i = 0; i < TilesInRow; i++) { //i have no clue why this works the way it does.
                        for (int j = 0; j < TilesInColumn*2; j++) {
                            sf::Sprite RevealedSprite(RevealedTile);
                            RevealedSprite.setPosition({(float)i*32, (float)j*16 - 16});
                            GameWindow.draw(RevealedSprite);
                            GameWindow.display();
                        }
                    }
                    unsigned int LeaderBoardWidth = (unsigned int)RowTiles*16;
                    unsigned int LeaderBoardHeight = (unsigned int)ColTiles*16+50;
                    GameClock.stop();
                    //Open up the leaderboard
                    LeaderBoardWindow.create(sf::VideoMode({LeaderBoardWidth, LeaderBoardHeight}), "Minesweeper", sf::Style::Close);
                    while (LeaderBoardWindow.isOpen()) {
                        // check all the window's events that were triggered since the last iteration of the loop
                        while (const std::optional event = LeaderBoardWindow.pollEvent())
                        {
                            // "close requested" event: we close the window
                            if (event->is<sf::Event::Closed>())
                                LeaderBoardWindow.close();
                        }

                        // clear the window with color
                        LeaderBoardWindow.clear(sf::Color::Blue);

                        // draw everything here...
                        // window.draw(...);
                        sf::Font font("../../cmake-build-debug/files/font.ttf");
                        sf::Text LeaderBoardText(font);
                        sf::Text ContentText(font);
                        LeaderBoardText.setString("LEADERBOARD");
                        ContentText.setString(LeaderBoardContent);

                        //set the position of the objects
                        LeaderBoardText.setCharacterSize(20);
                        ContentText.setCharacterSize(18);
                        setText(LeaderBoardText, LeaderBoardWidth/2, LeaderBoardHeight/2 - 120);
                        setText(ContentText, LeaderBoardWidth/2, LeaderBoardHeight/2 + 20);

                        // set the object colors
                        LeaderBoardText.setFillColor(sf::Color::White);
                        ContentText.setFillColor(sf::Color::White);

                        //set the object styles
                        LeaderBoardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                        ContentText.setStyle(sf::Text::Bold);

                        //draw the shape
                        LeaderBoardWindow.draw(LeaderBoardText);
                        LeaderBoardWindow.draw(ContentText);

                        // end the current frame
                        LeaderBoardWindow.display();
                    }
                    if (!GamePaused && !GameWon && !GameLost) {
                        GameClock.start();
                    }
                } else if (PausePlayBox.contains({(float)MousePosition.x, (float)MousePosition.y}) && !GameWon && !GameLost) { //Pause/play button
                    GamePaused = !GamePaused;
                    if (GamePaused) {
                        GameClock.stop();
                    } else {
                        GameClock.start();
                    }
                } else {
                    for (int i = 0; i < Tiles.size(); i++) {
                        if (TileBoxes[i].contains({(float)MousePosition.x, (float)MousePosition.y}) && !GamePaused && !GameWon && !GameLost) { //Individual tile
                            if (mouseButtonPressed->button == sf::Mouse::Button::Left) { //LMB pressed
                                if (Tiles[i]->GetMine() == 1 && !Tiles[i]->GetFlag()) { //Tile is a mine
                                    GameLost = true;
                                    GameClock.stop();
                                    DebugMode = true;
                                } else if (!Tiles[i]->GetFlag()) { //Reveal the tile
                                    //cout << "Tiles size: " << Tiles.size() << " Boxes size: " << TileBoxes.size() << endl;
                                    Tiles[i]->SetRevealed(true);
                                    if (Tiles[i]->GetFlag()) {
                                        Tiles[i]->SetFlag(false);
                                        MinesLeft++;
                                    }
                                    if (Tiles[i]->GetValue() == 0) { //Reveal surrounding tiles
                                        Tiles[i]->RevealSurrounding(i, Tiles, stoi(Columns), stoi(Rows));
                                    }
                                    //Check if player has won
                                    int TotalRevealed = 0;
                                    for (int j = 0; j < Tiles.size(); j++) {
                                        if (Tiles[j]->GetRevealed()) {
                                            TotalRevealed++;
                                        }
                                    }
                                    //cout << "Num tiles: " << Tiles.size() << " Revealed: " << TotalRevealed << " Mines: " << Mines << endl;
                                    if (Tiles.size() - TotalRevealed == stoi(Mines)) { //Game has been won
                                        GameWon = true;
                                        GameClock.stop();
                                        MinesLeft = 0;
                                        string CompletionTime;
                                        for (int j = 0; j < Tiles.size(); j++) {
                                            if (Tiles[j]->GetMine() == 1) {
                                                Tiles[j]->SetFlag(true);
                                            }
                                        }
                                        //Update leaderboard data set if better time
                                        for (int j = 0; j < LeaderBoardData.size(); j += 2) {
                                            int Time = 0;
                                            string Temp = LeaderBoardData[j].substr(0,2);
                                            Time += stoi(Temp) * 60;
                                            Temp = LeaderBoardData[j].substr(3, 2);
                                            Time += stoi(Temp);
                                            //cerr << "Current time (seconds): " << (int)TimeElapsed.asSeconds() << " Ranking time: " << Time << endl;
                                            if ((int)TimeElapsed.asSeconds() < Time) { //Current time is better
                                                if (Minutes < 10) {
                                                    CompletionTime = "0";
                                                } else {
                                                    CompletionTime = "";
                                                }
                                                CompletionTime += to_string(Minutes);
                                                CompletionTime += ":";
                                                if (Seconds < 10) {
                                                    CompletionTime += "0";
                                                }
                                                CompletionTime += to_string(Seconds);

                                                //cerr << "Completion time: " << CompletionTime << endl;
                                                //cerr << "Name to insert: " << NameInput.substr(0, NameInput.size() - 1) << endl;

                                                for (int k = LeaderBoardData.size()-1; k > j+1; k--) { //Update data set
                                                    LeaderBoardData[k] = LeaderBoardData[k-2];
                                                }
                                                LeaderBoardData[j] = CompletionTime;
                                                LeaderBoardData[j + 1] = NameInput.substr(0, NameInput.size() - 1);
                                                LeaderBoardData[j + 1].insert(0, " ");

                                                //cerr << "Inserted new high score into data set!" << endl;
                                                break;
                                            }
                                        }
                                        //Update leaderboard file
                                        LeaderBoardFileWRITE.open("../../cmake-build-debug/files/leaderboard.txt");
                                        for (int j = 0; j < LeaderBoardData.size(); j++) {
                                            LeaderBoardFileWRITE << LeaderBoardData[j];
                                            //cerr << "Inserting: " << LeaderBoardData[j] << endl;
                                            if (j%2 == 0) { //Time
                                                LeaderBoardFileWRITE << ",";
                                            } else { //Name
                                                LeaderBoardFileWRITE << "\n";
                                            }
                                        }
                                        LeaderBoardFileWRITE.close();

                                        //cerr << "Updated leaderboard file!" << endl;

                                        //Update leaderboard in-game
                                        SetUpLeaderBoard(LeaderBoardFileREAD, LeaderBoardContent, LeaderBoardData);
                                        int Index = LeaderBoardContent.find(CompletionTime);
                                        Index += 7;
                                        Index += NameInput.size() - 1;
                                        LeaderBoardContent.insert(Index, "*");

                                        //cerr << "In-game leaderboard updated!" << endl;
                                    }
                                }
                            } else { //RMB pressed
                                if (Tiles[i]->GetFlag()) {
                                    Tiles[i]->SetFlag(false);
                                    MinesLeft++;
                                } else if (!Tiles[i]->GetRevealed()){ //Place flag if tile NOT revealed
                                    Tiles[i]->SetFlag(true);
                                    MinesLeft--;
                                }
                            }
                        }
                    }
                }
            }
        }

        //display
        GameWindow.display();
    }
}