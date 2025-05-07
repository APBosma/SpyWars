//
// Created by adele on 4/29/2025.
//
#ifndef SPYING
#define SPYING
#include "Spy.h"
#include <iostream>
#include <random>
#include <mutex>
const int MAP_SZ = 4;

std::mutex mtx;

Spy::Spy() {
  location[0] = intRand() % MAP_SZ;
  location[1] = intRand() % MAP_SZ;
}

Spy::Spy(int x, int y) {
  location[0] = x;
  location[1] = y;
}

// getters
int Spy::getLocationX() {
  return location[0];
}
int Spy::getLocationY() {
  return location[1];
}

// Other
void Spy::movement(int map[MAP_SZ][MAP_SZ], bool found[MAP_SZ][MAP_SZ]) {
  mtx.lock();
  bool good = false;
  map[location[0]][location[1]] = 0;

  while (good != true) {
    int direction = intRand() % 8;

    switch (direction) {
      case 1:
        if (location[0]-1 >= 0 && location[1]-1 >= 0) {
          location[0] = location[0]-1;
          location[1] = location[1]-1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 2:
        if (location[0]-1 >= 0) {
          location[0] = location[0]-1;
          good = true;
        }
      else {
        good = false;
      }
        break;
      case 3:
        if (location[0]-1 >= 0 && location[1]+1 < MAP_SZ) {
          location[0] = location[0]-1;
          location[1] = location[1] + 1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 4:
        if (location[1]-1 >= 0) {
          location[1] = location[1]-1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 5:
        if (location[1]+1 < MAP_SZ) {
          location[1] = location[1] + 1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 6:
        if (location[0]+1 < MAP_SZ && location[1]-1 >= 0) {
          location[0] = location[0]+1;
          location[1] = location[1]-1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 7:
        if (location[1]+1 < MAP_SZ) {
          location[1] = location[1]+1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      case 8:
        if (location[0]+1 < MAP_SZ && location[1]+1 < MAP_SZ) {
          location[0] = location[0]+1;
          location[1] = location[1]+1;
          good = true;
        }
        else {
          good = false;
        }
        break;
      default:
        good = false;
    }
  }
  found[location[0]][location[1]] = true;
  mtx.unlock();
}

// This function is used to set the locations once changed
// Bowe this was added and not a part of movement because if a spy moved to one spot that one spy was in,
// but the other spy left, it would say the spy that just moved there wasn't there
void Spy::setMap(int map[MAP_SZ][MAP_SZ]) {
  map[location[0]][location[1]] = 1;
}

// Gets me random integer numbers
int Spy::intRand() {
  static __thread std::mt19937* generator = nullptr;
  if (!generator) {
    pthread_t thread_id = pthread_self();
    unsigned int seed = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(thread_id));
    generator = new std::mt19937(seed+42);
  }
  std::uniform_int_distribution<int> distribution(0, 2147483647);
  return distribution(*generator);
}

#endif SPYING