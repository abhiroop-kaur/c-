// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pixels() counts the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
// Currently the function ignores the n_threads parameter. Your job is to
// parallelize the function so that it uses n_threads threads to do
// the computation.

#include "calcpi.h"
#include <thread>
#include <vector>
#include <cmath>


uint64_t count_pixels(int r, int n_threads) //number of threads 
{
  // make sure your multithreaded solution returns the same number of
  // pixels as the single-threaded solution below!!!
  double rsq = double(r) * r;  //r-radius
  uint64_t count = 0; 
  std::vector<std::pair<int, int>> sections(n_threads);
  int num_sections = static_cast<int>(ceil(double(r) / double(n_threads)));

  
  int i = 0;
  while (i < n_threads) //assigning the starting and ending indices of each sec
  {
    sections[i].first = i * num_sections;
    sections[i].second = (i + 1) * num_sections - 1;
    i++;
  }
  sections[n_threads - 1].second = r - 1; // Adjust the last sec to include any remaining rows

  // Create and join the threads
  // We create a vector of threads here to keep track of all the threads that we spawn.
  std::vector<std::thread> threads(n_threads); //create a std::vector of std::thread objects named threads, with n_threads elements  

  for (int i = 0; i < n_threads; i++) // loop1 to go through the each section
    {
      // creates a new thread object 
      // and assigns it to the i-th element of the threads vector.
      // splitting them into smaller sub 
        threads[i] = std::thread([&count, r, rsq](const std::pair<int, int>& sec)
        
        {
          uint64_t local_count = 0;
          for( double x = 1 ; x <= r ; x ++)// loop2 for x coordinates
          {
            for (double y = sec.first; y <= sec.second; y++)//loop3 for y coordinates
            {
             if( x*x + y*y <= rsq) // checks the sum of sqrs with coordinates x, y ; where rsq holds the square of the radius of the circle.
             {
              local_count ++;//reference from class notes
             } 
            }
          }
          count+=local_count;
        },sections[i]); // passing the lamda function; retrives i-th element of the "sec"
    }
    

    
    for( auto& thread : threads) 
    {
      thread.join(); 
    }
      
  return count * 4 + 1;//total count of pixels, multiplied by 4 and added by 1 to account for the center pixel
}


