#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <wait.h>

using namespace std;

int main() {
  vector<string> args;
  string curr;
  string prompt;

  while (1) {
    cout << ">>> ";
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

    vector<char *> c_args;
    for (int i = 0; i < args.size(); i++) {
      c_args.push_back(const_cast<char *>(args[i].c_str()));
    }

    pid_t pid = fork();
    if (pid < 0) {
      cerr << "Cannot create process." << endl;
      exit(0);
    } else if (pid == 0) {
      execvp(c_args[0], c_args.data());
      cerr << "Command not found or error." << endl;
      exit(1);
    } else {
      int statloc;
      waitpid(-1, &statloc, 0);
    }

    args.clear();
    curr.clear();
    prompt.clear();
  }
}
