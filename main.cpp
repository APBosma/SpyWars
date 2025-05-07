//
// Created by adele on 4/29/2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "spying.cpp"
const int MAX_SPIES = 5;

void printInstructions();
void showBoard(int map[MAP_SZ][MAP_SZ], bool found[MAP_SZ][MAP_SZ]); // Shows nice board
void endOfDay(int *money, int numSpies, bool *alive); // Incraments money and checks status
bool checkWin(bool found[MAP_SZ][MAP_SZ]); // Checks if you have discovered everything

int main() {
  //srand(time(0));
  int map[MAP_SZ][MAP_SZ] = {0};
  bool discovered[MAP_SZ][MAP_SZ] = {false};
  std::vector<std::thread> threadsMovement;
  std::string choice;

  std::cout << "Welcome my king to Spy Wars!" << std::endl << std::endl;
  do {
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "1. Instructions" << std::endl;
    std::cout << "2. Play" << std::endl;
    std::cout << "3. Exit" <<std:: endl;
    std::cin >> choice;

    if (choice == "1") {
      printInstructions();
    }
    else if (choice == "3") {
      std::cout << "Goodbye!" << std::endl;
    }
    else if (choice != "2") {
      std::cout << "Unfortunately that is not an option my king." << std::endl;
    }
  } while (choice != "3" && choice != "2");

  if (choice == "2") {
    int numSpies = 2;
    int day = 0;
    bool alive = true;
    int money = 30;
    Spy spy1(0,0);
    Spy spy2(MAP_SZ-1,MAP_SZ-1);
    Spy spies[MAX_SPIES] = {spy1, spy2};
    map[0][0] = 1;
    map[MAP_SZ-1][MAP_SZ-1] = 1;
    discovered[0][0] = true;
    discovered[MAP_SZ-1][MAP_SZ-1] = true;

    while (alive) {
      day++;
      std::cout << "Day " << day << std::endl;
      std::cout << "Treasury: " << money << " coins" << std::endl;
      showBoard(map, discovered);

      std::cout << "What do you wish to do my king?" << std::endl;
      std::cout << "1. Purchase another spy" << std::endl;
      std::cout << "2. Next day" << std::endl;
      std::cout << "3. Surrender" << std::endl;
      std::cin >> choice;

      while (choice != "1" && choice != "2" && choice != "3") {
        std::cout << "That is not an option my king." << std::endl;
        std::cout << "What do you wish to do my king?" << std::endl;
        std::cout << "1. Purchase another spy" << std::endl;
        std::cout << "2. Next day" << std::endl;
        std::cout << "3. Surrender" << std::endl;
        std::cin >> choice;
      }

      switch (stoi(choice)) {
        case 1:
          std::cout << "Purchasing another spy will cost 5 gold up front and 2 gold a day. Are you sure you still want to purchase another one? Y/N" << std:: endl;
          std::cin >> choice;
          if ((choice == "Y" || choice == "y") && money > 5 && numSpies < MAX_SPIES) {
            money -= 5;
            spies[numSpies] = Spy();
            numSpies++;
            std::cout << "Congratulations my king on your new spy!" << std::endl;
          }
          else if (numSpies == MAX_SPIES) {
            std::cout << "I'm afraid there are no more spies to hire." << std::endl;
          }
          else if (money < 5) {
            std::cout << "I'm afraid the treasury is too small my king." << std::endl;
          }
          day--;
          break;
        case 2:
          if (day == 30 && !checkWin(discovered)) {
            alive = false;
            break;
          }

          endOfDay(&money, numSpies, &alive);

          // Parallelized movement
          for (int i=0; i < numSpies; i++) {
            threadsMovement.emplace_back(&Spy::movement, &spies[i], map, discovered);
          }
          for (auto& th : threadsMovement) {
            th.join();
          }
          threadsMovement.clear();
          // Adjusts map for new locations, MUST BE DONE AFTER SPIES MOVE
          for (int i=0; i < numSpies; i++) {
            threadsMovement.emplace_back(&Spy::setMap, &spies[i], map);
          }
          for (auto& th : threadsMovement) {
            th.join();
          }
          threadsMovement.clear();
          break;
        case 3:
          alive = false;
          break;
        default:
          std::cout << "Your humble servant has fucked up, my king." << std::endl;
      }
      if (checkWin(discovered)) {
        break;
      }
    }
    if (alive == true) {
      std::cout << "Congratulations my king! You will surely win the war now!" << std::endl;
    }
    else {
      std::cout << "Fair well, my king. Hopefully at least the tale of your rule outlasts us." << std::endl;
    }
  }
  return 0;
}

void printInstructions() {
  std::cout << "-- Instructions --" << std::endl;
  std::cout << "You are a king of a kingdom that will go to war in 30 days.\n";
  std::cout << "You have 30 moves to explore your enemy's territory with your spies. You will start with 2 spies. " <<
               "Each spy moves randomly due your kingdom's lacking investment in geography in schools.\n";
  std::cout << "At the start of each move you will gain money from taxes, but you will also lose money from " <<
               "paying your spies." << " Each turn you will have an opportunity to buy more spies, but beware!" <<
               "An unwise king may go bankrupt if they purchase too many spies. This would lead to an instant loss.\n";
}

void showBoard(int map[MAP_SZ][MAP_SZ], bool found[MAP_SZ][MAP_SZ]) {
  for (int i = 0; i < MAP_SZ; i++) {
    for (int j = 0; j < MAP_SZ; j++) {
      if (!found[i][j]) {
        std::cout << " - ";
      }
      else if (map[i][j] == 1) {
        std::cout << " S ";
      }
      else {
        std::cout << " O ";
      }
    }
    std::cout << std::endl;
  }
}

void endOfDay(int *money, int numSpies, bool *alive) {
  *money += 8; // taxes
  for (int i=0; i < numSpies; i++) { // Salaries
    *money -= 2;
  }
  if (*money < 1) {
    *alive = false;
  }
}

bool checkWin(bool found[MAP_SZ][MAP_SZ]) {
  for (int i = 0; i < MAP_SZ; i++) {
    for (int j = 0; j < MAP_SZ; j++) {
      if (!found[i][j]) {
        return false;
      }
    }
  }
  return true;
}




