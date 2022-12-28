#include <iostream>
using namespace std;


char turn = 'B'; // B or R
string chosenbead;
int x, y;


void printMap(string map[3][3][3]){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++){
                if (map[i][j][k] == ""){
                    if (k == 0)
                        cout << "   ";
                    else
                        cout << map[i][j][k-1] << " ";
                }
            }
        }
        cout << endl;
    }
}


char checkWin(){ // B or R or E:equal or N:none
// todo
}


bool beadExists(string beads[2][6]){
    int turnnum;
    if (turn == 'B') turnnum = 0;
    else turnnum = 1;

    for (int i = 0; i < 6; i++){
        if (chosenbead == beads[turnnum][i])
            return true;
    }

    return false;
}


bool inputChecker(string map[3][3][3]){
    if (turn == 'R')
        cout << "";
    for (int i = 0; i < 3; i++){
        if (map[x][y][i] == ""){
            if (i == 0)
                return true;
            if (turn == map[x][y][i-1][0])
                return false;
            if (int(chosenbead[1]) <= int(map[x][y][i-1][1]))
                return false;
            return true;
        }
    }
    return false;
}


void input(string beads[2][6], string map[3][3][3]){ // output: 0:chosenbead , 1:x , 2:y
    int turnnum;

    cout << turn << "'s turn" << endl;

    if (turn == 'B') turnnum = 0;
    else turnnum = 1;

    cout << "unused beads: ";
    for (int i = 0; i < 6 && beads[turnnum][i] != ""; i++)
        cout << beads[turnnum][i] << " , ";
    cout << endl;

    cout << "Select a bead: ";
    cin >> chosenbead;
    if (beadExists(beads)) {
        cout << "Enter x: ";
        cin >> x;
        cout << "Enter y: ";
        cin >> y;

        x -= 1;
        y -= 1;

        if (!(x <= 2 && x >= 0 && y <= 2 && y >= 0 && inputChecker(map))) {
            cout << "Your chosen coordinates isn't true; try again!";// todo
            input(beads, map);
            return;
        }

        if (turn == 'B') turn = 'R';
        else turn = 'B';

        // todo: agar zire mohre dige raft az beads[][] kharej she

        return;
    }
    cout << "Your chosen bead does not exist; try again!";
    input(beads, map);
}


int main() {

    string map[3][3][3];

    string beads[2][6] = {{"B1", "B1", "B2", "B2", "B3", "B3"},
                        {"R1", "R1", "R2", "R2", "R3", "R3"}};

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                map[i][j][k] = "";
            }
        }
    }

    while (checkWin() != 'N') { // todo: ==

        printMap(map);

        input(beads, map);

        for (int i = 0; i < 3; i++){
            if (map[x][y][i] == ""){
                map[x][y][i] = chosenbead;
                break;
            }
        }


//        cout << "chosenbead: " << chosenbead << endl << "x: " << x << endl << "y: " << y;
    }

    return 0;
}
