//
// Created by adele on 4/29/2025.
//

class Spy {
  private:
    int location[2];

  public:
    // Constructors
    Spy();
    Spy(int x, int y);

    // getters
    int getLocationX();
    int getLocationY();

    // Other
    void movement(int map[4][4], bool found[4][4]);
    void setMap(int map[4][4]);
    int intRand();
};

