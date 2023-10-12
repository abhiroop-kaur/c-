/// ============================================================================
/// Copyright (C) 2023 Pavol Federl (pfederl@ucalgary.ca)
/// All Rights Reserved. Do not distribute this file.
/// ============================================================================
///
/// You must modify this file and then submit it for grading to D2L.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, as long as you implement the detect_primes() function as
/// defined in "detectPrimes.h".

#include "detectPrimes.h"
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <cinttypes>



std::vector <int64_t> result;// initialize empty array result[]
std::atomic <bool> global_finished=false;// set global_finished = false 
// C++ barrier class (from lecture notes).
// You do not have to use it in your implementation. If you don't use it, you
// may delete it
class simple_barrier { //mechanism using mutex and condition variable to bring thread at one pont before moving further
  std::mutex m_;
  std::condition_variable cv_;
  int n_remaining_, count_;
  bool coin_;

  public:
  simple_barrier(int count = 1) { init(count); }
  void init(int count)
  {
    count_ = count;
    n_remaining_ = count_;
    coin_ = false;
  }
  bool wait()
  {
    if (count_ == 1) return true;
    std::unique_lock<std::mutex> lk(m_);
    if (n_remaining_ == 1) {
      coin_ = !coin_;
      n_remaining_ = count_;
      cv_.notify_all();
      return true;
    }
    auto old_coin = coin_;
    n_remaining_--;
    cv_.wait(lk, [&]() { return old_coin != coin_; });
    return false;
  }
};




simple_barrier barrier = simple_barrier();
std::atomic <int> k=0;
std::atomic <int64_t> j=0;
std::atomic <bool> isprime = true;

//thread function
static void is_prime_thread(int64_t n, const std::vector<int64_t> & nums, int n_threads)
{
  while(true) {
  // serial task – pick one thread using barrier
  if(barrier.wait()){ //derived from barrier.cpp 
    while(true){
      j = nums[k++];//get the next number from nums[] 
      if (j < 2){ 
        isprime = false;
        break;
      }else if (j==2||j == 3){
        isprime = true; // 2 and 3 are primes
        break;
      }else if (j % 2 == 0){ 
        isprime =  false; // handle multiples of 2
        break;
      }else if (j !=3 &&j % 3 == 0 ){
        isprime =  false; // handle multiples of 3
        break;
      }else{
        break;
      }
    }
    if (nums.size()==k){ //implemented from the pseudo code given in the assignment description
      global_finished=true;
    }   
  }
    // TA HINTS
    int64_t i = 5 + 6* n; 
    int64_t max = sqrt((double)j);
    barrier.wait(); // synchronize again before starting parallel work
    while (i <= max) {
      if(isprime==false){
        break;
      }
      if (j % i == 0) isprime =  false;
      if (j % (i + 2) == 0) isprime = false;
      i += 6*n_threads;
    }
    if(barrier.wait()){
      if(isprime==true){
          result.push_back(j);
        }else{
          isprime= true;
        }
    }
    barrier.wait();
    if (global_finished){
      break;
    }
  }
}





std::vector<int64_t> 
detect_primes(const std::vector<int64_t> & nums, int n_threads) 
{
    std::thread thread_pool[n_threads];
    barrier.init(n_threads);
    
    //start N threads
    //references taken from TA's code from tutorial number 11
    //derived from barrier.cpp
    for (int i = 0; i < n_threads; i++) {
      thread_pool[i] =std::thread(is_prime_thread, i, nums, n_threads);
      }
    // join N threads
   
    for (int i = 0; i < n_threads; i++) { 
      if (thread_pool[i].joinable()) thread_pool[i].join();
    }
    return result;
  
}
