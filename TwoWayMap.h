#ifndef TWO_WAY_MAP_H
#define TWO_WAY_MAP_H

#include <map>
#include <vector>
#include <algorithm>
#include "Relation.h"

template <typename T1, typename T2>
class TwoWayMap {
private:
  std::set<std::pair<T1,T2>> schema;
  std::vector<std::pair<T1,T2>> vectAll;
  std::vector<T1> vectT1;
  std::vector<T2> vectT2;

  struct sortSecond {
    bool operator ()(std::pair<T1,T2> a, std::pair<T1,T2> b) const {
        return  a.second < b.second;
    }
  };

public:
  friend class Relation;
  TwoWayMap () {

  }

  TwoWayMap (const TwoWayMap& other) {
    schema = other.schema;
    vectT1 = other.vectT1;
    vectT2 = other.vectT2;
  }

  TwoWayMap& operator=(TwoWayMap other) {
    schema = other.schema;
    vectT1 = other.vectT1;
    vectT2 = other.vectT2;
    return *(this);
  }

  virtual ~TwoWayMap () {

  }

  void insert(T1 first,T2 second) {
    std::pair<T1,T2> input = {first,second};
    schema.insert(input);
  }

  void insert(T2 second, T1 first) {
    std::pair<T1,T2> input = {first,second};
    schema.insert(input);
  }

  void remove(T1 look) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->first == look) {
        schema.erase(it);
        return;
      }
    }
    throw std::out_of_range ("Search");
  }

  void remove(T2 look) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->second == look) {
        schema.erase(it);
        return;
      }
    }
    throw std::out_of_range ("Search");
  }

  void replace(T1 look, T1 what) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->first == look) {
        std::pair<T1,T2> input = {what,it->second};
        schema.erase(it);
        schema.insert(input);
        return;
      }
    }
    throw std::out_of_range ("Search");
  }

  void replace(T2 look, T2 what) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->second == look) {
        std::pair<T1,T2> input = {it->first,what};
        schema.erase(it);
        schema.insert(input);
        return;
      }
    }
    throw std::out_of_range ("Search");
  }

  T1 at(T2 look) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->second == look) return it->first;
    }
    throw std::out_of_range ("Search");
  }

  T2 at(T1 look) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->first == look) return it->second;
    }
    throw std::out_of_range ("Search");
  }

  std::vector<std::pair<T1,T2>> toVector() {
    if (vectAll.size() != schema.size()) {
      vectAll.clear();
      for (auto item : schema) {
        vectAll.push_back(item);
      }
    }
    return vectAll;
  }


  std::vector<T1> getFirst() {
    if (vectT1.size() != schema.size()) {
      vectT1.clear();
      for (auto item : schema) {
        vectT1.push_back(item.first);
      }
    }
    return vectT1;
  }

  std::vector<T2> getSecond() {
    if (vectT2.size() != schema.size()) {
      vectT2.clear();
      for (auto item : schema) {
        vectT2.push_back(item.second);
      }
    }
    return vectT2;
  }

  bool contains(T1 look) {
    for (auto item : schema) {
      if (item.first == look) return true;
    }
    return false;
  }

  bool contains(T2 look) {
    for (auto item : schema) {
      if (item.second == look) return true;
    }
    return false;
  }

  void clear() {
    schema.clear();
    vectT1.clear();
    vectT2.clear();
  }

  unsigned int size() {
    return schema.size();
  }
};
#endif
