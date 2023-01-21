#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
using namespace std;

char turn = 'B'; // B or R
string chosenbead;
int x, y;
int scoreline_p1;
int scoreline_p2;
bool r = false;
string board[16][31][3];
string map[3][3][3];
string beads[2][6] = {{"1B1", "1B2", "2B1", "2B2", "3B1", "3B2"},
                      {"1R1", "1R2", "2R1", "2R2", "3R1", "3R2"}};

std::fstream &GotoLine(std::fstream &file, unsigned int num)
{
    file.seekg(std::ios::beg);
    for (int i = 0; i < num - 1; ++i)
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return file;
}

int get_turnnum(char turn)
{
    if (turn == 'B')
        return 0;
    else
        return 1;
}

// The main board the game is being played on
// We name it Map
void printMap()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {

                if (map[i][j][2] != "")
                {
                    cout << map[i][j][2] << " ";
                    break;
                }

                if (map[i][j][k] == "")
                {
                    if (k == 0)
                        cout << "    ";
                    else
                        cout << map[i][j][k - 1] << " ";
                    break;
                }
            }
        }
        cout << endl;
    }
}

// Checking wether any player has won yet or not
// B: B wins
// R: R wins
// N: None, nobody has won yet

char checkWin()
{

    int e[3][3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (map[i][j][k] == "")
                {
                    e[i][j] = k - 1;
                    break;
                }
                if (k == 2)
                    e[i][j] = k;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // row
            if (e[i][0] != -1 &&
                e[i][1] != -1 &&
                e[i][2] != -1 &&
                map[i][0][e[i][0]][1] == map[i][1][e[i][1]][1] &&
                map[i][1][e[i][1]][1] == map[i][2][e[i][2]][1])
                return map[i][0][e[i][0]][1];

            // column
            if (e[0][i] != -1 &&
                e[1][i] != -1 &&
                e[2][i] != -1 &&
                map[0][i][e[0][i]][1] == map[1][i][e[1][i]][1] &&
                map[1][i][e[1][i]][1] == map[2][i][e[2][i]][1])
                return map[0][i][e[0][i]][1];

            // diagonal
            if (e[0][0] != -1 &&
                e[1][1] != -1 &&
                e[2][2] != -1 &&
                map[0][0][e[0][0]][1] == map[1][1][e[1][1]][1] &&
                map[1][1][e[1][1]][1] == map[2][2][e[2][2]][1])
                return map[0][0][e[0][0]][1];
            if (e[0][2] != -1 &&
                e[1][1] != -1 &&
                e[2][0] != -1 &&
                map[0][2][e[0][2]][1] == map[1][1][e[1][1]][1] &&
                map[1][1][e[1][1]][1] == map[2][0][e[2][0]][1])
                return map[0][2][e[0][2]][1];
        }
    }

    return 'N';
}

// Checking whether a certain coordinate has been filled with a bead previously
// throughout the game or not
bool beadExists()
{
    int turnnum = get_turnnum(turn);

    for (int i = 0; i < 6; i++)
    {
        if (chosenbead == beads[turnnum][i])
            return true;
    }

    return false;
}

bool inputChecker()
{
    int turnnum = get_turnnum(turn);
    if (turnnum == 0)
        turnnum = 1;
    else
        turnnum = 0;

    for (int i = 0; i < 3; i++)
    {
        if (map[x][y][i] == "")
        {
            if (i == 0)
                return true;
            if (turn == map[x][y][i - 1][1])
                return false;
            if (int(chosenbead[0]) <= int(map[x][y][i - 1][0]))
                return false;

            for (int j = 0; j < 6; j++)
            {
                if (beads[turnnum][j] == map[x][y][i - 1]) // unusable
                    beads[turnnum][j] = "-" + beads[turnnum][j];
            }

            return true;
        }
    }
    return false;
}

void input()
{
    int turnnum = get_turnnum(turn);
    int turnnumrev;

    if (turnnum == 0)
        turnnumrev = 1;
    else
        turnnumrev = 0;

    cout << turn << "'s turn" << endl;

    cout << "usable beads: ";
    for (int i = 0; i < 6 && beads[turnnum][i] != ""; i++)
    {
        if (beads[turnnum][i][0] != '-')
        {
            cout << beads[turnnum][i];
            if (i != 5)
                cout << " , ";
        }
    }
    cout << endl;

    cout << "Select a bead: ";
    cin >> chosenbead;
    if (beadExists())
    {
        cout << "Enter x: ";
        cin >> x;
        cout << "Enter y: ";
        cin >> y;

        x -= 1;
        y -= 1;

        // Checking whether the coordinates given by the players exist or not
        if (!(x <= 2 && x >= 0 && y <= 2 && y >= 0))
        {
            cout << "Your chosen coordinates isn't true, try again!" << endl;
            input();
            return;
        }

        // Cecking whether the chosenbead can be put over the bead in the chosen coordinate
        // AKA checking whether the chosenbead is bigger in size than the bead existing
        // in the chosen coordiante
        else if (!(inputChecker()))
        {
            cout << "Your chosen bead is the same size as the bead existing in this coordinate!";
            input();
            return;
        }

        // Checking whether the bead is in the map or not
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    if (map[i][j][k] == chosenbead)
                    {
                        map[i][j][k] = "";
                        if (k != 0)
                        {
                            for (int l = 0; l < 6; l++)
                            {
                                if (beads[turnnumrev][l] == map[i][j][k - 1])
                                {
                                    beads[turnnumrev][l].erase(beads[turnnumrev][l].begin());
                                }
                            }
                        }
                    }
                }
            }
        }

        if (turn == 'B')
            turn = 'R';
        else
            turn = 'B';

        return;
    }
    cout << "Your chosen bead does not exist; try again!";
    input();
}

// Checking whether certain players have played the game previously or not
bool playersExists(string p1, string p2)
{
    string s;
    string data = "";
    int exists = 0;
    int i = 0;
    ifstream MyReadFile("data.txt");

    while (getline(MyReadFile, s))
    {
        i++;
        if (i % 4 == 1 || i % 4 == 2)
        {
            if (i % 4 == 1)
                exists = 0;
            if (s == p1 || s == p2)
                exists++;
            else
                exists = 0;
            if (exists == 2)
            {
                if (i % 4 == 2)
                {
                    MyReadFile.close();
                    if (s == p2)
                    {
                        scoreline_p2 = i + 2;
                        scoreline_p1 = i + 1;
                        r = false;
                    }
                    else
                    {
                        scoreline_p2 = i + 1;
                        scoreline_p1 = i + 2;
                        r = true;
                    }
                    return true;
                }
                else
                    exists = 0;
            }
        }
    }

    MyReadFile.close();
    if (i == 0)
    {
        scoreline_p1 = 3;
        scoreline_p2 = 4;
    }
    else
    {
        scoreline_p1 = i + 3;
        scoreline_p2 = i + 4;
    }
    return false;
}

void fileWork()
{
    string p1, p2;

    cout << "Enter player1's name: ";
    cin >> p1;
    cout << "Enter player2's name: ";
    cin >> p2;

    if (!playersExists(p1, p2))
    {
        ofstream MyFile("data.txt", std::ios_base::app | std::ios_base::out);
        MyFile << p1 << "\n"
               << p2 << "\n"
               << "0"
               << "\n"
               << "0"
               << "\n";
        MyFile.close();
    }
    else
    {
        fstream MyReadFile("data.txt");
        string p1score, p2score;

        GotoLine(MyReadFile, scoreline_p1);
        MyReadFile >> p1score;
        GotoLine(MyReadFile, scoreline_p2);
        MyReadFile >> p2score;

        if (stoi(p1score) >= stoi(p2score))
            turn = 'B';
        else
            turn = 'R';

        MyReadFile.close();
    }
}

// Adding the result of a match to our file
void addScoreToFile(char player)
{
    ifstream MyReadFile("data.txt");
    string s;
    int i = 0;
    int scoreline;
    vector<string> data_vector;

    if (player == 'B')
        scoreline = scoreline_p1;
    else
        scoreline = scoreline_p2;

    while (getline(MyReadFile, s))
    {
        i++;
        if (i == scoreline)
            data_vector.push_back(to_string(stoi(s) + 1));
        else
            data_vector.push_back(s);
    }

    MyReadFile.close();
    ofstream MyFile("data.txt");

    for (int j = 1; j <= i; j++)
        MyFile << data_vector[j - 1] << "\n";

    MyFile.close();
}

// Our main board
void drawBoard()
{
//    string board[16][31][3];

    for (int k = 0; k <= 2; k++)
    {
        for (int i = 0 ; i <= 15; i++)
        {
            for (int j = 0; j <= 30; j++)
                board[i][j][k] = " ";
        }
    }

    for (int k = 0; k <= 2; k++)
    {
        for (int i = 0; i <= 15; i++)
        {
            for (int j = 0; j <= 30; j++)
            {
                if (i % 5 == 0)
                    board[i][j][k] = "_";
            }
        }
    }
    
    for (int k = 0; k <= 2; k++)
    {
        for (int j = 0; j <= 30; j++)
        {
            for (int i = 1; i <= 15; i++)
            {
                if (j % 10 == 0)
                    board[i][j][k] = "|";
            }
        }
    }

    for(int i = 0; i <= 15; i++)
    {
        for (int j = 0; j <= 30; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                if (board[i][j][2] != "")
                {
                    cout << board[i][j][2] << " ";
                    break;
                }

                if (board[i][j][k] == "")
                {
                    if (k == 0)
                        cout << "    ";
                    else
                        cout << board[i][j][k - 1] << " ";
                    break;
                }
            }
        }
        cout << endl;
    }  
}

//visualizing the chosenbeads
string visualizingtheBeads(string s)
{
    if (s == "1B1")
        return " 1 \n.B.";

    else if (s == "1B2")
        return " 2 \n.B.";
        
    else if (s == "2B1")
        return " 1 \n.B.\n ...";

    else if (s == "2B2")
        return " 2 \n.B.\n...";

    else if (s == "3B1")
        return " 1 \n.B.\n...\n...";

    else if (s == "3B2")
        return " 2 \n.B.\n...\n...";

    else if (s == "1R1")
        return " 1 \n.R.";

    else if (s == "1R2")
        return " 2 \n.R.";

    else if (s == "2R1")
        return " 1 \n.R.\n...";

    else if (s == "2R2")
        return " 2 \n.R.\n...";

    else if (s == "3R1")
        return " 1 \n.R.\n...\n...";

    else if (s == "3R2")
        return " 2 \n.R.\n...\n...";
    else
        return 0;
}

int main()
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                map[i][j][k] = "";
            }
        }
    }
    
    for (int k = 0; k < 3; k++)
    {
        board[3][5][k] = map[0][0][k];
        board[8][5][k] = map[1][0][k];
        board[13][5][k] = map[2][0][k];
        board[3][15][k] = map[0][1][k];
        board[8][15][k] = map[1][1][k];
        board[13][15][k] = map[2][1][k];
        board[3][25][k] = map[0][2][k];
        board[8][25][k] = map[1][2][k];
        board[13][25][k] = map[2][2][k];
    }
    
    fileWork();

    while (checkWin() == 'N')
    {

//        printMap();

        drawBoard();

        input();

        cout << "__________________________________________________" << endl;

//        for (int i = 0; i < 3; i++)
//        {
//            if (map[x][y][i] == "")
//            {
//                map[x][y][i] = visualizingtheBeads(chosenbead);
//                break;
//            }
//        }


    }

//    printMap();

    drawBoard();

    // Adding the result of the match to our file
    if (checkWin() == 'B')
    {
        cout << "Player1 win!!!";
        addScoreToFile('B');
    }
    else
    {
        cout << "Player2 win!!!";
        addScoreToFile('R');
    }

    return 0;
}

