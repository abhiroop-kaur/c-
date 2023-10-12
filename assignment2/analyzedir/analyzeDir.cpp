/// =========================================================================
/// Written by pfederl@ucalgary.ca in 2023, for CPSC457.
/// =========================================================================
/// You need to edit this file.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, as long as you implement the analyzeDir() function as
/// defined in "analyzeDir.h".

#include "analyzeDir.h"

#include <cassert>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>

std::unordered_map<std::string,int> hist;
Results res;    // creating object


/// check if path refers to a directory
static bool is_dir(const std::string & path)
{
  struct stat buff;
  if (0 != stat(path.c_str(), &buff)) return false;
  return S_ISDIR(buff.st_mode);
}

/// check if path refers to a file
static bool is_file(const std::string & path)
{
  struct stat buff;
  if (0 != stat(path.c_str(), &buff)) return false;
  return S_ISREG(buff.st_mode);
}

/// check if string ends with another string
static bool ends_with(const std::string & str, const std::string & suffix)
{
  if (str.size() < suffix.size()) return false;
  else
    return 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}


struct DirInfo {
  std::string path;
  long count = 0;
};

long largest_f_size = -1;
std::string largest_f_path = "";
long no_of_files = 0;
long directory_counter = 0;
int flag = 0;
// returns the directory with most files
static DirInfo scan(const std::string & dir)
{
  
  directory_counter++;
  DirInfo best { "", -1};
  auto dirp = opendir( dir.c_str());
  assert(dirp != nullptr);

  for (auto de = readdir(dirp); de != nullptr; de = readdir(dirp)) {
    std::string name = de->d_name;
    if (name == "." or name == "..") continue;
    std::string path = dir + "/" + name;

    // vacant_dirs
    if (flag == true){
      res.vacant_dirs.push_back(dir);
    }



    if (is_file(path)) {
      no_of_files ++;

      if (ends_with(path, ".txt")){
        FILE *fp;
        fp = fopen(path.c_str(), "r");
        if (fp == nullptr) {
          printf("Cannot open file %s\n", path.c_str());
          continue;
        }
        while(1){
          std::string result;
          while(1) {
            int c = fgetc(fp);
            if(c == EOF) break;
            c = tolower(c);
            if(! isalpha(c)) {
              if(result.size() == 0)
                continue;
              else{
                // for word length atleast 5
                if (result.size()>=5){
                  hist[result]++;
                  //continue;
                }
                result.clear();
                //break;
              }   
            }
            else {
              result.push_back(c);
            }
          }
          if(result.size() == 0) break;
          hist[result]++;
          
        }
        fclose(fp); 
      }
      struct stat buff;
      stat (path.c_str(), &buff);
      if (0 != stat(path.c_str(), &buff)) 
        printf("    - could not determine size of file\n");
      long file_size = buff.st_size;
      if (file_size > largest_f_size){
        largest_f_size = file_size;
        largest_f_path = path.substr(2);    //path
      }
      // all file size
      res.all_files_size += long(buff.st_size); 

      // images
      std::string cmd = "identify -format '%w %h' " + path + " 2> /dev/null";
      std::string size_of_image;
      auto fp = popen(cmd.c_str(), "r");
      assert(fp);
      char buffer[PATH_MAX];
      if( fgets(buffer, PATH_MAX, fp) != NULL) {
        size_of_image = buffer;
      }
      int status = pclose(fp);
      if( status != 0 or size_of_image[0] == '0')
      {
        size_of_image ="";
      }
      // if is an image
      if(!size_of_image.empty())
      {   
        //std::cout << "convert(\"" << size_of_image << "\")=";
        long img1, img2;
        if (2 == sscanf(size_of_image.c_str(), "%ld %ld", &img1, &img2)) {
          std::string new_path = path.substr(2);
          ImageInfo img_info = {new_path,img1,img2};
          res.largest_images.push_back(img_info);
            
        } else {
          std::cout << "Error, could not convert\n";
        }
      }
      continue;
    } 
  

  
  
    
    assert(is_dir(path));
    auto sub_result = scan(path);
    if( sub_result.count >= best.count )
      best = sub_result;
  
  }
  closedir(dirp);
  return best;
}

// ======================================================================
// You need to re-implement this function !!!!
// ======================================================================
//
// analyzeDir(n) computes stats about current directory
//   n = how many words and images to report in restuls
//
// The code below is incomplate/incorrect. Feel free to re-use any of it
// in your implementation, or delete all of it.
Results analyzeDir(int n)
{
  // The code below does a subset of the functionality you need to implement.
  // It also contains several debugging printf() statements. When you submit
  // your code for grading, please remove any dubugging pritf() statements.
  std::string dir_name = ".";
  scan(dir_name);
  res.n_dirs = directory_counter;
  res.n_files = no_of_files;
  res.largest_file_path = largest_f_path;
  res.largest_file_size = largest_f_size;
    
    // check if filename ends with .txt
  //   if (ends_with(name, ".txt")) 
  //     printf("    - is a .txt file\n");
  //   else
  //     printf("    - is not a .txt file\n");

  //   // let's see if this is an image and whether we can manage to get image info
  //   // by calling an external utility 'identify'
  //   std::string cmd = "identify -format '%wx%h' " + name + " 2> /dev/null";
  //   std::string img_size;
  //   auto fp = popen(cmd.c_str(), "r");
  //   assert(fp);
  //   char buff[PATH_MAX];
  //   if( fgets(buff, PATH_MAX, fp) != NULL) {
  //     img_size = buff;
  //   }
  //   int status = pclose(fp);
  //   if( status != 0 or img_size[0] == '0')
  //     img_size = "";
  //   if( img_size.empty())
  //     printf("    - not an image\n");
  //   else
  //     printf("    - image %s\n", img_size.c_str());
  // }
  // closedir(dir);

  // citation : ta help
  for(auto & h : hist){
    res.most_common_words.emplace_back(h.first, h.second);
  }
  auto comparefn = [](auto & a, auto & b) {
    
        return b.second < a.second;  // sort by int value, in descending order

  };
  if (res.most_common_words.size() > size_t(n)) {
    std::partial_sort(res.most_common_words.begin(), res.most_common_words.begin() + n, res.most_common_words.end(), comparefn);
    // drop all entries after the first n
    res.most_common_words.resize(n);
  } else {
    std::sort(res.most_common_words.begin(), res.most_common_words.end(), comparefn);
  }


  
  return res;
}
