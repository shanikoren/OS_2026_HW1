#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlCHandler(int sig_num) {
    cout << "smash: got ctrl-C" << endl;
    pid_t fg_pid = SmallShell::getInstance().getFgPid();
    if (fg_pid != 0) {
        kill(fg_pid, SIGKILL);
        cout << "smash: process " << fg_pid << " was killed" << endl;
    }
}
