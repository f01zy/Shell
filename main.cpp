#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <wait.h>

#define PATH_MAX 512

using namespace std;

void changePath(string &path) {
  char buffer[PATH_MAX];
  if (getcwd(buffer, PATH_MAX)) {
    path = buffer;
  }
}

int main(int argc, char **argv) {
  while (1) {
    vector<string> args;
    string curr;
    string prompt;
    string path;

    changePath(path);
    cout << path << "$ ";
    if (!getline(cin, prompt)) {
      break;
    }
    if (prompt.empty()) {
      continue;
    }
    for (int i = 0; i < prompt.size(); i++) {
      char ch = prompt[i];
      if (ch == ' ') {
        args.push_back(curr);
        curr.clear();
      } else {
        curr.push_back(ch);
      }
    }

    if (!curr.empty()) {
      args.push_back(curr);
    }
    if (args.empty()) {
      continue;
    }

    if (args[0] == "cd") {
      const char *newPath;
      if (args.size() == 1) {
        newPath = "/";
      } else {
        newPath = args[1].c_str();
      }
      if (chdir(newPath) != 0) {
        cout << "Failed to change directory." << endl;
      }
      continue;
    } else if (args[0] == "exit") {
      exit(0);
    }

    vector<char *> c_args;
    for (int i = 0; i < args.size(); i++) {
      c_args.push_back(const_cast<char *>(args[i].c_str()));
    }
    c_args.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
      cerr << "Cannot create process." << endl;
    } else if (pid == 0) {
      execvp(c_args[0], c_args.data());
      cerr << "Command not found or error." << endl;
      exit(1);
    } else {
      int statloc;
      waitpid(pid, &statloc, 0);
    }
  }
}
