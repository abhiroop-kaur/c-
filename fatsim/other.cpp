#include <algorithm>
// reimplement this function
using namespace std;
vector<long> fat_check(const vector<long> & fat)
{
  auto size = fat.size();
  vector<vector<long> > adjc_list;
  vector<long> last_node;
  vector<long> results;
  vector<bool> visited;
  adjc_list.resize(size);
  visited.resize(size);
  fill(visited.begin(),visited.end(),false);
  list<long> bfs_queue ;
  vector<long> distances;
  distances.resize(size);


  //finding last nodes
  for (long i = 0; i < (int)size;i++){
    if(fat[i]== -1)
    last_node.push_back(i);//pushing last nodes to vector
    else 
      adjc_list[fat[i]].push_back(i);//pushing nodes to adjc_list
  }

 
  for (int i =0; i<(int)last_node.size(); i++){
      distances[(last_node[i])]=1;//distance from last node to itself is 1
      bfs_queue.push_back(last_node[i]);//pushing last nodes to queue
  }
    //impementing bfs
    while (!bfs_queue.empty()){
      long curr_node = bfs_queue.front();//getting the first element
      bfs_queue.pop_front();//removing the first element
      
      for(int i =0; i <(int)adjc_list[curr_node].size();i++){//looping through the adjc_list
        if (visited[i] == false){//if the node is not visited
          bfs_queue.push_back((adjc_list[curr_node])[i]);//pushing the node to queue
          distances[(adjc_list[curr_node])[i]] = distances[curr_node] + 1;//updating the distance
          visited.at(i) = false;//marking the node as visited
        }
      }
    }



    
// finding nodes w max distance
  long max_distance;
  max_distance = *max_element(distances.begin(), distances.end());
   if(max_distance!=0){
  for (long unsigned x = 0; x < distances.size();x++)
      {
        if(distances.at(x) == max_distance )  // get the value where we get maximum length
        results.push_back(x);
      }
  }
  return results;
}