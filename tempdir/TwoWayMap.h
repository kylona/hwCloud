#ifndef TWO_WAY_MAP_H
#define TWO_WAY_MAP_H

#include <map>
#include <vector>
#include <algorithm>
#include "Relation.h"

template <typename T1, typename T2>
class TwoWayMap {
private:
  std::map<T1,T2> schema;
  std::vector<std::pair<T1,T2>> sortByIndex;

  struct sortSecond {
    bool operator ()(std::pair<T1,T2> a, std::pair<T1,T2> b) const {
        return  a.second < b.second;
    }
  };

public:
  friend class Relation;
  TwoWayMap () {

  }

  TwoWayMap (const TwoWayMap& old) {
    this->schema = old.schema;
  }

  TwoWayMap& operator=(TwoWayMap other) {
    schema = other.schema;
    sortByIndex = other.sortByIndex;
  }

  virtual ~TwoWayMap () {

  }

  void insert(T1 first,T2 second) {
    schema[first] = second;
  }

  void remove(T1 first) {
    schema.erase(first);
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

  T1 at(T2 look) {
    for (auto it = schema.begin(); it != schema.end(); it++) {
      if (it->second == look) return it->first;
    }
    throw std::out_of_range ("Search");
  }

  T2 at(T1 look) {
    return schema.at(look);
  }


  T1 getFirst(int index) {
    if (sortByIndex.empty()) {
      for (auto it = schema.begin(); it != schema.end(); it++) {
        sortByIndex.push_back(*it);
      }
    }
    sort(sortByIndex.begin(),sortByIndex.end(),sortSecond());
    return sortByIndex[index].first;
  }

  T2 getSecond(int index) {
  //  typedef std::map<T1,T2>::const_iterator MapIterator;
    //MapIterator it = schemaLookup.cbegin();
    //return *(it + index);
  }

  void clear() {
    schema.clear();
    sortByIndex.clear();
  }

  unsigned int size() {
    return schema.size();
  }
};
#endif
