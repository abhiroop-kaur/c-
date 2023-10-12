// -------------------------------------------------------------------------------------
// this is the only file you need to edit
// -------------------------------------------------------------------------------------
//
// (c) 2023, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "fatsim.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <cstdio>
// depth first search
void dfs(long current, const std::vector<std::vector<int>> &adj_list, std::vector<bool> &visited, std::vector<long> &dist, long depth) {
  visited[current] = true;
  dist[current] = depth;//store the distance
  
  for (long j = 0; j < (long)adj_list[current].size(); j++) {//for each adjacent node
    if (!visited[adj_list[current][j]]) {//if not visited
      dfs(adj_list[current][j], adj_list, visited, dist, depth + 1);//recursively call dfs
    }
  }
}


// reimplement this function
std::vector<long> fat_check(const std::vector<long> & fat){
  
  std::vector<bool> visited;
  visited.resize(fat.size());
  std::vector<std::vector<int>> adj_list;
  adj_list.resize(fat.size());
  std::vector<long> rest;
  std::vector<long> dist;
  dist.resize(fat.size());
  std::vector<long> longest_chains;


  //add edges to adj_list
  for(long i = 0; i <(long)fat.size(); i++){
    if(fat[i] != -1) {// (-1) denotes a NULL pointer(from assignment description)
      adj_list[fat[i]].push_back(i);
    }else{
      rest.push_back(i);
    }
  }

  // calling Depth first search
  for (long i = 0; i < (long)rest.size(); i++) {
    dfs(rest[i], adj_list, visited, dist, 1);//call dfs
  }

  //find nodes with maximum distance
  //ta help 
  long max1 = *max_element(dist.begin(), dist.end());
  if (max1 != 0) {
    for (long unsigned x = 0; x < dist.size(); x++) {
      if (dist[x] == max1) {
        longest_chains.push_back(x);
      }
    }
  }
  std::sort(longest_chains.begin(), longest_chains.end());//sort the vector
  return longest_chains;
}



