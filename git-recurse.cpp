// Requires c++17:
// g++ git-recurse.cpp --std=c++17 -o git-recurse

#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <cstdlib>

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

int main()
{
  auto first_path = std::filesystem::current_path();

  {
    std::ifstream f(".git");
    if (f.good())
    {
      std::string line;
      std::stringstream ss;
      ss << exec("git log --oneline --decorate=short") << std::endl;
      getline(ss, line);
      std::cout << "**** At .:  " << std::endl << line << std::endl;
    }
  }
  
  for(auto& p: fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied))
  {
    std::ifstream f(p.path().string() + "/.git");
    if (f.good())
    {
      std::filesystem::current_path(p.path().string());

      std::string line;
      std::stringstream ss;
      ss << exec("git log --oneline --decorate=short") << std::endl;
      getline(ss, line);
      std::cout << "**** At " << p.path().string() << ":  " << std::endl << line << std::endl;

      std::filesystem::current_path(first_path);
    }
  }
  return 0;
}
