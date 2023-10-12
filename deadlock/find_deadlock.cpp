// this is the ONLY file you should edit and submit to D2L

#include "find_deadlock.h"
#include "common.h"
#include <iostream>
#include <vector>
#include <string>


//GRAPH
class Graph {
    public:
    std::vector<std::vector<int>> adj_list;
    std::vector<int> out_counts;

} graph;

std::vector<std::string>int2word;
const std::string RIGHT = "->";
const std::string LEFT = "<-";


Result find_deadlock(const std::vector<std::string> & edges){
    Result result;
    Graph g;// g = initialize empty g
    Word2Int w;
    for(int i=0; i< int(edges.size()); i++) {// a for loop to iterate through entire edges array
        std::string edge = edges[i];//getting the edge at i
        std::vector<std::string> tokens = split(edge);//split the edge at i
    
        //convert token splitted(0] and splitted[2] to integers n1 and n2
        int n1 = w.get(tokens[0].append("$")); //appending special character to differenciate resource
        int n2 = w.get(tokens[2]);
        int m_ax= std::max(n1,n2);//max
        
        if((m_ax+1) >(int)g.adj_list.size()){
            // resize adj_list and out _counts
            g.adj_list.resize(m_ax+1);
            g.out_counts.resize(m_ax+1);
            int2word.resize(m_ax+1 );
        }
        int2word[n1] = tokens[0];
        int2word[n2] = tokens[2];

        //update adj _list and out counts on the bases of arrow direction
        std::string sign (tokens[1]);
        if (sign == RIGHT) {
            g.adj_list[n2].push_back(n1);
            g.out_counts[n1]++;  

        } else if (sign == LEFT) {
            g.adj_list[n1].push_back(n2);
            g.out_counts[n2]++;
        }
        //topological sort
        //zeros[] = find all nodes in g with outdegree == 0
        std::vector<int> zeros;
        std::vector<int> out = g.out_counts; // copy out_counts so that we can modify it 
        for (int j = 0; j < int(out.size()); j++){
            if (out[j] == 0) {
                zeros.push_back(j);
            }
        }
    
        while(!zeros.empty()) {
            int n = zeros.back();//  n = remove one entry from zeros[] 
            zeros.pop_back(); 
            for (int value : g.adj_list[n]) { //for every n2 in adj_list[n]:
                out[value]--;  //out[n2] --
                if (out[value] == 0) {  //if out[n2] == 0:
                    zeros.push_back(value);     //append n2 to zeros[]
                }
            }
        }
        //Add the processes part of deadlock back to results
        //processes involved in deadlock are nodes n that represent a process and out[n]>0
        for (int n = 0; n < int(out.size()); n++) {
            if (out[n] > 0) {
                if (int2word[n].back() == '$' ) {
                    int2word[n].resize(int2word[n].size() - 1); // remove last character from string
                    result.procs.push_back(int2word[n]); // add modified string to result.procs
                }
            }  
        }
        if(!result.procs.empty()){
            result.index = i;
            return result;
        }
    }
    result.index = -1;
    return result;    
}
