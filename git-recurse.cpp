// Requires c++17:
// g++ git-recurse.cpp --std=c++17 -o git-recurse

#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <array>
#include <vector>

namespace fs = std::filesystem;
 
std::string exec(const char* cmd) {
    std::array<char, 1048536> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

bool starts_with(std::string s1, std::string s2)
{
  if (s1.size() < s2.size())
    return false;

  for (int i=0; i<s2.size(); i++)
  {
    if (s1[i] != s2[i])
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  auto first_path = std::filesystem::current_path();

  if (argc > 1 && (std::string(argv[1]) == "help" || std::string(argv[1]) == "--help"))
  {
    std::cout << "Use:" << std::endl;
    std::cout << "git-recurse [git commands]" << std::endl;
    return 0;
  }


  std::stringstream ss_args;

  if (argc > 1)
  {
    ss_args << "git";
    for (int i=1; i<argc; i++)
    {
      ss_args << " " << argv[i];
    }
    std::cout << "Command to execute: " << ss_args.str() << std::endl;
  }

  int last = 1;

  {
    std::ifstream f(".git");
    if (f.good())
    {
      std::string line;
      std::stringstream ss, ss_0;

      if (argc > 1)
        std::cout << "----------------------------" << std::endl;

      std::cout << last << ") **** At .:  " << std::endl;
      
      if (argc > 1)
      {
        ss_0 << exec(ss_args.str().c_str());
        std::cout << ss_0.str()  << std::endl;
      }
      else
      {
        ss << exec("git log --oneline --decorate=short") << std::endl;
        getline(ss, line);
        std::cout << line << std::endl;
      }

      std::cout << std::endl;
      last++;
    }
  }
  
  std::vector<std::string> folders;

  for(auto& p: fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied))
  {
    std::ifstream f(p.path().string() + "/.git");
    if (f.good())
    {
      std::cout << p.path().string() << std::endl;
      folders.push_back(p.path().string());
    }
  }

  std::sort(folders.begin(), folders.end());

  for (std::string s : folders)
  {
    std::filesystem::current_path(s);
    std::string line;
    std::stringstream ss, ss_0;

    if (argc > 1)
      std::cout << "----------------------------" << std::endl;

    std::cout << last << ") **** At " << s <<": " << std::endl;

    if (argc > 1)
    {
      ss_0 << exec(ss_args.str().c_str());
      std::cout << ss_0.str()  << std::endl;
    }
    else
    {
      ss << exec("git log --oneline --decorate=short") << std::endl;
      getline(ss, line);
      std::cout << line << std::endl;
    }
    std::cout << std::endl;
    last++;
    std::filesystem::current_path(first_path);
  }
  return 0;
}
