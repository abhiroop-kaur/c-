#include "fatsim.h"
#include <iostream>
#include <vector>
#include <list>
#include <cstdio>
#include <algorithm>

std::vector<long> fat_check(const std::vector<long> & fat)
{
std::vector<std::vector<long> > adj_list; // adjacency list
adj_list.resize(fat.size());    // resize the adjacency list
std::vector<long> results;    // vector to store the results
std::vector<long> terminating_index_list;       // vector to store the terminating nodes

  // create the adjacency list
for (long i = 0; i < (long)fat.size();i++){
    if(fat[i]!=-1)
      adj_list[fat[i]].push_back(i);      // store the incoming nodes
    else
      terminating_index_list.push_back(i);    // store the terminating nodes if fat[i] == -1
  }

 

  auto size_needed = adj_list.size();
  std::vector<long> distances = std::vector<long>(size_needed);   // vector to store the distances
  // measure the distances from the terminating nodes
  std::list<long> queue;  // queue to store the nodes
  std::vector<bool> visited = std::vector<bool>(size_needed);   // vector to store the visited nodes


  // breadth first search
  for (auto i =0; i<(int)terminating_index_list.size(); i++){
    
    distances.at(terminating_index_list[i])=1;//
    queue.push_back(terminating_index_list[i]);
    while (!queue.empty()){
      auto current_index = queue.front();
      queue.pop_front();
      // if (!visited.at(current_index)) {
      //   if(current_index == terminating_index_list[i])
      //     distances.at(current_index) = 1;
      // }
      for(auto j = 0; j < (int)adj_list[current_index].size(); j++){
        if (!visited.at(j)){
          // breadth-sort algorithm to have the distances counted
          visited.at(j) = false;
          queue.push_back(adj_list[current_index].at(j));
          distances[adj_list[current_index].at(j)] = distances.at(current_index) + 1;
        }
      }
    }
  }
  auto max1 = *max_element(distances.begin(), distances.end());
  for (long unsigned x = 0; x < distances.size();x++) {
    if(distances[x] == max1 && max1!=0)  // get the value where we get maximum length
      results.push_back(x);
  }
  return results;
}