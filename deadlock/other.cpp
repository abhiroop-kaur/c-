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

std::vector<std::string> int2word;
const std::string RIGHT = "->";
const std::string LEFT = "<-";


Result find_deadlock(const std::vector<std::string> & edges){
    Result result;
    Word2Int conversion;
    vector<string> int2word;
    vector<string> tokens;
    string edge;
    string assgn_arrow = "<-";
    string req_arrow = "->";



    for(int i =0; i<(int)edges.size();i++){
        edge = edges[i];
        tokens = split(edge);
        tokens[0].append(" ");
        int n1 = conversion.get(tokens[0]);
        int n2 = conversion.get(tokens[2]);
        int n3 = max(n2,n1) + 1 ;

        if((long)graph.adj_list.size()<n3){
           
            graph.out_counts.resize(n3);
            graph.adj_list.resize(n3);
            int2word.resize(n3);
            int2word[n1] = tokens[0];
            int2word[n2] = tokens[2];
        }
         string arrow = tokens[1];
        if(req_arrow==arrow){
            graph.adj_list[n2].push_back(n1);
            graph.out_counts[n1]++;

        }else if(assgn_arrow==arrow){
            graph.adj_list.at(n1).push_back(n2);
            graph.out_counts.at(n2)++;              
        }
        else{continue;}

        vector<int> out = graph.out_counts;
        vector<int> zeros;


        for(int i=0; i< int(out.size());i++){
            if(out[i]==0) zeros.push_back(i);
        }
        int n = 0;
        while(!zeros.empty()){ 
             n = zeros.back();
            zeros.pop_back();
            for( int n2 : graph.adj_list[n] ){
                out[n2]--;
                if(out[n2]==0) zeros.push_back(n2);
            }
        }
        for(int i=0; i<(long)out.size();i++){
            if(out.at(i)>0 && int2word.at(i).back()==' '){
                string process = int2word.at(i);
                process.resize(process.size()-1);
                result.procs.push_back(process);
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