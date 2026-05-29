#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"

using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

//vvvvvvvvvvvvvvvvvvvv Function that the segel gave as, don't touch it vvvvvvvvvv/

string _ltrim(const string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const string &s) {
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char *cmd_line, char **args) {
    FUNC_ENTRY()
    int i = 0;
    istringstream iss(_trim(string(cmd_line)).c_str());
    for (string s; iss >> s;) {
        args[i] = (char *) malloc(s.length() + 1);
        memset(args[i], 0, s.length() + 1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;
    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char *cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char *cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

//^^^^^^^^^^^^^^^^^^^ Function that the segel gave as, don't touch it^^^^^^^^^^^/


// TODO: Add your implementation for classes in Commands.h 




/**********************************************************************************/
//COMMAND CLASS
Command::Command(const char *cmd_line, SmallShell* shell) : m_cmd_line(cmd_line), m_shell(shell) {}

/**********************************************************************************/
//BUILT-IN COMMAND CLASSES
BuiltInCommand::BuiltInCommand(const char *cmd_line, SmallShell* shell) : Command(cmd_line, shell) {}


//BUILT-IN COMMAND NO. 1 ----------------------------------------------------------/
ChangePromptCommand::ChangePromptCommand(const char *cmd_line, SmallShell* shell) : BuiltInCommand(cmd_line, shell) {
    char* args[20];  
    int num_args = _parseCommandLine(cmd_line, args);  
    if (num_args >= 2) {
        this->m_new_prompt = strdup(args[1]);
    } else {
        this->m_new_prompt = nullptr;
    }
    for (int j = 0; j < num_args; ++j) {
        free(args[j]);
    }
}

ChangePromptCommand::~ChangePromptCommand() {
    if (this->m_new_prompt != nullptr) {
        free(this->m_new_prompt);
    }
}

void ChangePromptCommand::execute() {
    m_shell->promptChanger((string)m_new_prompt ? (string)m_new_prompt : "smash");
}

//BUILT-IN COMMAND NO. 2 ----------------------------------------------------------/

ShowPidCommand::ShowPidCommand(const char *cmd_line, SmallShell* shell)
 : BuiltInCommand(cmd_line, shell) {}

void ShowPidCommand::execute(){
    std::cout << "smash pid is " << getpid() << std::endl;
}


//BUILT-IN COMMAND NO. 3 ----------------------------------------------------------/
GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

void GetCurrDirCommand::execute() {
    char* cwd = getcwd(nullptr, 0);
    if (cwd != nullptr) {
        std::cout << cwd << std::endl;
        free(cwd);
    } else {
        perror("smash error: getcwd failed");
    }
}

//BUILT-IN COMMAND NO. 4 ----------------------------------------------------------/
ChangeDirCommand::ChangeDirCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {
}

ChangeDirCommand::~ChangeDirCommand() {}

void ChangeDirCommand::execute() {
    char* args[20];
    int num_args = _parseCommandLine(m_cmd_line, args);
    if (num_args > 2) {
        cerr << "smash error: cd: too many arguments" << endl;
    } else if(num_args == 2 && strcmp(args[1], "-") == 0) {
        if(m_shell->getLastPwd() == nullptr) {
            cerr << "smash error: cd: OLDPWD not set" << endl;
        } else {
            char* current_path = getcwd(nullptr, 0);
            if (current_path == nullptr) {
                perror("smash error: getcwd failed");
            } else if(chdir(m_shell->getLastPwd()) == -1) {
                perror("smash error: chdir failed");
                free(current_path);
            } else {
                m_shell->setLastPwd(current_path);
            }
        }
    } else if(num_args == 2){
        char* current_path = getcwd(nullptr, 0);
        if (current_path == nullptr) {
            perror("smash error: getcwd failed");
        } else if(chdir(args[1]) == -1) {
            perror("smash error: chdir failed");
            free(current_path);
        } else {
            m_shell->setLastPwd(current_path);
        }
    }
    for (int j = 0; j < num_args; j++) {
        free(args[j]);
    }
}

//BUILT-IN COMMAND NO. 5 ----------------------------------------------------------/
JobsCommand::JobsCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

JobsCommand::~JobsCommand() {}

void JobsCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 6 ----------------------------------------------------------/
ForegroundCommand::ForegroundCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

ForegroundCommand::~ForegroundCommand() {}

void ForegroundCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 7 ----------------------------------------------------------/
QuitCommand::QuitCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 8 ----------------------------------------------------------/
KillCommand::KillCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

KillCommand::~KillCommand() {}

void KillCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 9 ----------------------------------------------------------/
AliasCommand::AliasCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

AliasCommand::~AliasCommand() {}

void AliasCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 10 ----------------------------------------------------------/
UnAliasCommand::UnAliasCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

UnAliasCommand::~UnAliasCommand() {}

void UnAliasCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 11 ----------------------------------------------------------/
UnSetEnvCommand::UnSetEnvCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

UnSetEnvCommand::~UnSetEnvCommand() {}

void UnSetEnvCommand::execute() {
    //TODO
}

//BUILT-IN COMMAND NO. 12 ----------------------------------------------------------/
SysInfoCommand::SysInfoCommand(const char *cmd_line, SmallShell* shell)
    : BuiltInCommand(cmd_line, shell) {}

SysInfoCommand::~SysInfoCommand() {}

void SysInfoCommand::execute() {
    //TODO
}









/**********************************************************************************/
//small shell parts
SmallShell::SmallShell() : m_prompt("smash") {}

SmallShell::~SmallShell() {
    // TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char *cmd_line) {
    // For example:
    
    string cmd_s = _trim(string(cmd_line));
    string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

    if (firstWord.compare("chprompt") == 0) {
      return new ChangePromptCommand(cmd_line, this);
    }
    else if (firstWord.compare("showpid") == 0) {
      return new ShowPidCommand(cmd_line, this);
    }
    else if (firstWord.compare("pwd") == 0) {
      return new GetCurrDirCommand(cmd_line, this);
    }
    else if (firstWord.compare("cd") == 0) {
        return new ChangeDirCommand(cmd_line, this);
    }
    else if () {
        
    }
    else if () {
        
    }
    else if () {
        
    }
    else {
      return new ExternalCommand(cmd_line, this);
    }
    
    return nullptr;
}

void SmallShell::promptChanger(const string& new_prompt) {
    m_prompt = new_prompt;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    // for example:
    Command* cmd = CreateCommand(cmd_line);
    cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

string SmallShell::getPrompt() const {
    return m_prompt + "> ";
}