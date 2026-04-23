#ifndef PROJECT3_TILE_H
#define PROJECT3_TILE_H
#include <vector>

class Tile {
private:
    int Value;
    int Mine;
    bool Revealed;
    bool Flag;
    bool CheckedAlready = false;

public:
    //Constructor: 0 if not a mine, 1 if a mine
    Tile(int val = 0, int mine = 0, bool revealed = false, bool flag = false) {
        Value = val;
        Mine = mine;
        Revealed = revealed;
        Flag = flag;
    }

    //Get mine status
    int GetMine () {
        return Mine;
    }

    //Get value
    int GetValue () {
        return Value;
    }

    //Get revealed status
    bool GetRevealed () {
        return Revealed;
    }

    //Get flag status
    bool GetFlag () {
        return Flag;
    }

    //Get checked status
    bool GetChecked () {
        return CheckedAlready;
    }

    //Set mine status
    void SetMine(int mine) {
        Mine = mine;
    }

    //Set revealed status
    void SetRevealed (bool revealed) {
        Revealed = revealed;
    }

    //Set flag status
    void SetFlag(bool flag) {
        Flag = flag;
    }

    //Check surrounding tiles
    void CheckSurrounding(int position, std::vector<Tile*>& tileList, int rowL, int colL) {
        //Leftmost column
        if (position%rowL == 0) {
            if (position < rowL) { //Top left tile
                Value += tileList[position+1]->GetMine();
                Value += tileList[position+rowL]->GetMine();
                Value += tileList[position+1+rowL]->GetMine();
            } else if (position >= (colL-1)*rowL) { //Bottom left tile
                Value += tileList[position+1]->GetMine();
                Value += tileList[position-rowL]->GetMine();
                Value += tileList[position+1-rowL]->GetMine();
            } else { //Other mines in first column
                Value += tileList[position+1]->GetMine();
                Value += tileList[position-rowL]->GetMine();
                Value += tileList[position+rowL]->GetMine();
                Value += tileList[position+1-rowL]->GetMine();
                Value += tileList[position+1+rowL]->GetMine();
            }
        } else if (position%rowL == rowL-1) { //Rightmost column
            if (position <= rowL) { //Top right tile
                Value += tileList[position-1]->GetMine();
                Value += tileList[position+rowL]->GetMine();
                Value += tileList[position-1+rowL]->GetMine();
            } else if (position >= (colL-1)*rowL) { //Bottom right tile
                Value += tileList[position-1]->GetMine();
                Value += tileList[position-rowL]->GetMine();
                Value += tileList[position-1-rowL]->GetMine();
            } else { //Other mines in last column
                Value += tileList[position-1]->GetMine();
                Value += tileList[position-rowL]->GetMine();
                Value += tileList[position+rowL]->GetMine();
                Value += tileList[position-1-rowL]->GetMine();
                Value += tileList[position-1+rowL]->GetMine();
            }
        } else if (position < rowL) { //Top row (excluding corners)
            Value += tileList[position-1]->GetMine();
            Value += tileList[position+1]->GetMine();
            Value += tileList[position+rowL]->GetMine();
            Value += tileList[position-1+rowL]->GetMine();
            Value += tileList[position+1+rowL]->GetMine();
        } else if (position >= (colL-1)*rowL) { //Bottom row (excluding corners)
            Value += tileList[position-1]->GetMine();
            Value += tileList[position+1]->GetMine();
            Value += tileList[position-rowL]->GetMine();
            Value += tileList[position-1-rowL]->GetMine();
            Value += tileList[position+1-rowL]->GetMine();
        } else { // All other tiles (tiles not touching any sides)
            Value += tileList[position-1]->GetMine();
            Value += tileList[position+1]->GetMine();
            Value += tileList[position-rowL]->GetMine();
            Value += tileList[position+rowL]->GetMine();
            Value += tileList[position-1-rowL]->GetMine();
            Value += tileList[position+1-rowL]->GetMine();
            Value += tileList[position-1+rowL]->GetMine();
            Value += tileList[position+1+rowL]->GetMine();
        }
    }

    //Check surrounding tiles
    void RevealSurrounding(int position, std::vector<Tile*>& tileList, int rowL, int colL) {
        CheckedAlready = true;
        Revealed = true;
        //Leftmost column
        if (position%rowL == 0) {
            if (position < rowL) { //Top left tile
                if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                    tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
                } else if (!tileList[position+1]->GetFlag()) {
                    tileList[position+1]->SetRevealed(true);
                }
                if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                    tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
                } else if (!tileList[position+rowL]->GetFlag()) {
                    tileList[position+rowL]->SetRevealed(true);
                }
                if (tileList[position+1+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+1+rowL]->GetChecked()) {
                    tileList[position+1+rowL]->RevealSurrounding(position+1+rowL, tileList, rowL, colL);
                } else if (!tileList[position+rowL]->GetFlag()) {
                    tileList[position+1+rowL]->SetRevealed(true);
                }
            } else if (position >= (colL-1)*rowL) { //Bottom left tile
                if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                    tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
                } else if (!tileList[position+1]->GetFlag()) {
                    tileList[position+1]->SetRevealed(true);
                }
                if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                    tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
                } else if (!tileList[position-rowL]->GetFlag()) {
                    tileList[position-rowL]->SetRevealed(true);
                }
                if (tileList[position+1-rowL]->GetValue() == 0 && !tileList[position+1-rowL]->GetFlag() && !tileList[position+1-rowL]->GetChecked()) {
                    tileList[position+1-rowL]->RevealSurrounding(position+1-rowL, tileList, rowL, colL);
                } else if (!tileList[position+1-rowL]->GetFlag()) {
                    tileList[position+1-rowL]->SetRevealed(true);
                }
            } else { //Other mines in first column
                if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                    tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
                } else if (!tileList[position+1]->GetFlag()) {
                    tileList[position+1]->SetRevealed(true);
                }
                if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                    tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
                } else if (!tileList[position-rowL]->GetFlag()) {
                    tileList[position-rowL]->SetRevealed(true);
                }
                if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                    tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
                } else if (!tileList[position+rowL]->GetFlag()) {
                    tileList[position+rowL]->SetRevealed(true);
                }
                if (tileList[position+1-rowL]->GetValue() == 0 && !tileList[position+1-rowL]->GetFlag() && !tileList[position+1-rowL]->GetChecked()) {
                    tileList[position+1-rowL]->RevealSurrounding(position+1-rowL, tileList, rowL, colL);
                } else if (!tileList[position+1-rowL]->GetFlag()) {
                    tileList[position+1-rowL]->SetRevealed(true);
                }
                if (tileList[position+1+rowL]->GetValue() == 0 && !tileList[position+1+rowL]->GetFlag() && !tileList[position+1+rowL]->GetChecked()) {
                    tileList[position+1+rowL]->RevealSurrounding(position+1+rowL, tileList, rowL, colL);
                } else if (!tileList[position+1+rowL]->GetFlag()) {
                    tileList[position+1+rowL]->SetRevealed(true);
                }
            }
        } else if (position%rowL == rowL-1) { //Rightmost column
            if (position <= rowL) { //Top right tile
                if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                    tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
                } else if (!tileList[position-1]->GetFlag()) {
                    tileList[position-1]->SetRevealed(true);
                }
                if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                    tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
                } else if (!tileList[position+rowL]->GetFlag()) {
                    tileList[position+rowL]->SetRevealed(true);
                }
                if (tileList[position-1+rowL]->GetValue() == 0 && !tileList[position-1+rowL]->GetFlag() && !tileList[position-1+rowL]->GetChecked()) {
                    tileList[position-1+rowL]->RevealSurrounding(position-1+rowL, tileList, rowL, colL);
                } else if (!tileList[position-1+rowL]->GetFlag()) {
                    tileList[position-1+rowL]->SetRevealed(true);
                }
            } else if (position >= (colL-1)*rowL) { //Bottom right tile
                if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                    tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
                } else if (!tileList[position-1]->GetFlag()) {
                    tileList[position-1]->SetRevealed(true);
                }
                if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                    tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
                } else if (!tileList[position-rowL]->GetFlag()) {
                    tileList[position-rowL]->SetRevealed(true);
                }
                if (tileList[position-1-rowL]->GetValue() == 0 && !tileList[position-1-rowL]->GetFlag() && !tileList[position-1-rowL]->GetChecked()) {
                    tileList[position-1-rowL]->RevealSurrounding(position-1-rowL, tileList, rowL, colL);
                } else if (!tileList[position-1-rowL]->GetFlag()) {
                    tileList[position-1-rowL]->SetRevealed(true);
                }
            } else { //Other mines in last column
                if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                    tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
                } else if (!tileList[position-1]->GetFlag()) {
                    tileList[position-1]->SetRevealed(true);
                }
                if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                    tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
                } else if (!tileList[position-rowL]->GetFlag()) {
                    tileList[position-rowL]->SetRevealed(true);
                }
                if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                    tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
                } else if (!tileList[position+rowL]->GetFlag()) {
                    tileList[position+rowL]->SetRevealed(true);
                }
                if (tileList[position-1-rowL]->GetValue() == 0 && !tileList[position-1-rowL]->GetFlag() && !tileList[position-1-rowL]->GetChecked()) {
                    tileList[position-1-rowL]->RevealSurrounding(position-1-rowL, tileList, rowL, colL);
                } else if (!tileList[position-1-rowL]->GetFlag()) {
                    tileList[position-1-rowL]->SetRevealed(true);
                }
                if (tileList[position-1+rowL]->GetValue() == 0 && !tileList[position-1+rowL]->GetFlag() && !tileList[position-1+rowL]->GetChecked()) {
                    tileList[position-1+rowL]->RevealSurrounding(position-1+rowL, tileList, rowL, colL);
                } else if (!tileList[position-1+rowL]->GetFlag()) {
                    tileList[position-1+rowL]->SetRevealed(true);
                }
            }
        } else if (position < rowL) { //Top row (excluding corners)
            if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
            } else if (!tileList[position-1]->GetFlag()) {
                tileList[position-1]->SetRevealed(true);
            }
            if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
            } else if (!tileList[position+1]->GetFlag()) {
                tileList[position+1]->SetRevealed(true);
            }
            if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
            } else if (!tileList[position+rowL]->GetFlag()) {
                tileList[position+rowL]->SetRevealed(true);
            }
            if (tileList[position-1+rowL]->GetValue() == 0 && !tileList[position-1+rowL]->GetFlag() && !tileList[position-1+rowL]->GetChecked()) {
                tileList[position-1+rowL]->RevealSurrounding(position-1+rowL, tileList, rowL, colL);
            } else if (!tileList[position-1+rowL]->GetFlag()) {
                tileList[position-1+rowL]->SetRevealed(true);
            }
            if (tileList[position+1+rowL]->GetValue() == 0 && !tileList[position+1+rowL]->GetFlag() && !tileList[position+1+rowL]->GetChecked()) {
                tileList[position+1+rowL]->RevealSurrounding(position+1+rowL, tileList, rowL, colL);
            } else if (!tileList[position+1+rowL]->GetFlag()) {
                tileList[position+1+rowL]->SetRevealed(true);
            }
        } else if (position >= (colL-1)*rowL) { //Bottom row (excluding corners)
            if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
            } else if (!tileList[position-1]->GetFlag()) {
                tileList[position-1]->SetRevealed(true);
            }
            if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
            } else if (!tileList[position+1]->GetFlag()) {
                tileList[position+1]->SetRevealed(true);
            }
            if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
            } else if (!tileList[position-rowL]->GetFlag()) {
                tileList[position-rowL]->SetRevealed(true);
            }
            if (tileList[position-1-rowL]->GetValue() == 0 && !tileList[position-1-rowL]->GetFlag() && !tileList[position-1-rowL]->GetChecked()) {
                tileList[position-1-rowL]->RevealSurrounding(position-1-rowL, tileList, rowL, colL);
            } else if (!tileList[position-1-rowL]->GetFlag()) {
                tileList[position-1-rowL]->SetRevealed(true);
            }
            if (tileList[position+1-rowL]->GetValue() == 0 && !tileList[position+1-rowL]->GetFlag() && !tileList[position+1-rowL]->GetChecked()) {
                tileList[position+1-rowL]->RevealSurrounding(position+1-rowL, tileList, rowL, colL);
            } else if (!tileList[position+1-rowL]->GetFlag()) {
                tileList[position+1-rowL]->SetRevealed(true);
            }
        } else { // All other tiles (tiles not touching any sides)
            if (tileList[position-1]->GetValue() == 0 && !tileList[position-1]->GetFlag() && !tileList[position-1]->GetChecked()) {
                tileList[position-1]->RevealSurrounding(position-1, tileList, rowL, colL);
            } else if (!tileList[position-1]->GetFlag()) {
                tileList[position-1]->SetRevealed(true);
            }
            if (tileList[position+1]->GetValue() == 0 && !tileList[position+1]->GetFlag() && !tileList[position+1]->GetChecked()) {
                tileList[position+1]->RevealSurrounding(position+1, tileList, rowL, colL);
            } else if (!tileList[position+1]->GetFlag()) {
                tileList[position+1]->SetRevealed(true);
            }
            if (tileList[position-rowL]->GetValue() == 0 && !tileList[position-rowL]->GetFlag() && !tileList[position-rowL]->GetChecked()) {
                tileList[position-rowL]->RevealSurrounding(position-rowL, tileList, rowL, colL);
            } else if (!tileList[position-rowL]->GetFlag()) {
                tileList[position-rowL]->SetRevealed(true);
            }
            if (tileList[position+rowL]->GetValue() == 0 && !tileList[position+rowL]->GetFlag() && !tileList[position+rowL]->GetChecked()) {
                tileList[position+rowL]->RevealSurrounding(position+rowL, tileList, rowL, colL);
            } else if (!tileList[position+rowL]->GetFlag()) {
                tileList[position+rowL]->SetRevealed(true);
            }
            if (tileList[position-1-rowL]->GetValue() == 0 && !tileList[position-1-rowL]->GetFlag() && !tileList[position-1-rowL]->GetChecked()) {
                tileList[position-1-rowL]->RevealSurrounding(position-1-rowL, tileList, rowL, colL);
            } else if (!tileList[position-1-rowL]->GetFlag()) {
                tileList[position-1-rowL]->SetRevealed(true);
            }
            if (tileList[position+1-rowL]->GetValue() == 0 && !tileList[position+1-rowL]->GetFlag() && !tileList[position+1-rowL]->GetChecked()) {
                tileList[position+1-rowL]->RevealSurrounding(position+1-rowL, tileList, rowL, colL);
            } else if (!tileList[position+1-rowL]->GetFlag()) {
                tileList[position+1-rowL]->SetRevealed(true);
            }
            if (tileList[position-1+rowL]->GetValue() == 0 && !tileList[position-1+rowL]->GetFlag() && !tileList[position-1+rowL]->GetChecked()) {
                tileList[position-1+rowL]->RevealSurrounding(position-1+rowL, tileList, rowL, colL);
            } else if (!tileList[position-1+rowL]->GetFlag()) {
                tileList[position-1+rowL]->SetRevealed(true);
            }
            if (tileList[position+1+rowL]->GetValue() == 0 && !tileList[position+1+rowL]->GetFlag() && !tileList[position+1+rowL]->GetChecked()) {
                tileList[position+1+rowL]->RevealSurrounding(position+1+rowL, tileList, rowL, colL);
            } else if (!tileList[position+1+rowL]->GetFlag()) {
                tileList[position+1+rowL]->SetRevealed(true);
            }
        }
    }
};

#endif //PROJECT3_TILE_H