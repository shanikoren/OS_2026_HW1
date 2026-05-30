// Ver: 04-11-2025
#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>

using namespace std;

#define COMMAND_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class SmallShell; // forward declaration

class Command {
    // TODO: Add your data members
    protected:
    const char* m_cmd_line;
    SmallShell* m_shell;
public:
    Command(const char *cmd_line, SmallShell* shell);

    virtual ~Command();

    virtual void execute() = 0;

    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
    const char* getCmdLine() const {
        return m_cmd_line;
    }
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char *cmd_line, SmallShell* shell);

    virtual ~BuiltInCommand() {
    }
};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char *cmd_line, SmallShell* shell);

    virtual ~ExternalCommand() {
    }

    void execute() override;
};


class RedirectionCommand : public Command {
    // TODO: Add your data members
public:
    explicit RedirectionCommand(const char *cmd_line, SmallShell* shell);

    virtual ~RedirectionCommand() {
    }

    void execute() override;
};

class PipeCommand : public Command {
    // TODO: Add your data members
public:
    PipeCommand(const char *cmd_line, SmallShell* shell);

    virtual ~PipeCommand() {
    }

    void execute() override;
};

class DiskUsageCommand : public Command {
public:
    DiskUsageCommand(const char *cmd_line, SmallShell* shell);

    virtual ~DiskUsageCommand() {
    }

    void execute() override;
};

class WhoAmICommand : public Command {
public:
    WhoAmICommand(const char *cmd_line, SmallShell* shell);

    virtual ~WhoAmICommand() {
    }

    void execute() override;
};

class USBInfoCommand : public Command {
    // TODO: Add your data members **BONUS: 10 Points**
public:
    USBInfoCommand(const char *cmd_line, SmallShell* shell);

    virtual ~USBInfoCommand() {
    }

    void execute() override;
};

//Built-in function no. 1
class ChangePromptCommand : public BuiltInCommand {
    // TODO: Add your data members public:
    char* m_new_prompt;
public:
    ChangePromptCommand(const char *cmd_line, SmallShell* shell);

    virtual ~ChangePromptCommand();

    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ChangeDirCommand(const char *cmd_line, SmallShell* shell);

    virtual ~ChangeDirCommand();

    void execute() override;
};


class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char *cmd_line, SmallShell* shell);

    virtual ~GetCurrDirCommand() {
    }

    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char *cmd_line, SmallShell* shell);

    virtual ~ShowPidCommand() {
    }

    void execute() override;
};

class JobsList;

class QuitCommand : public BuiltInCommand {
    JobsList* job_list;
public:
    QuitCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell);

    virtual ~QuitCommand() {
    }

    void execute() override;
};

class JobsList {
public:
    class JobEntry {
    public:
        int id;
        pid_t pid;
        bool is_stopped = false;
        string cmd;
        time_t start_time;

        JobEntry(int id, int pid, const char* cmd, bool is_stopped) : id(id), pid(pid),
        cmd(cmd), is_stopped(is_stopped) {
            this->start_time = time(nullptr);
        }
        ~JobEntry() = default;

    };
    vector <JobEntry*> job_entries;
    // TODO: Add your data members
public:
    JobsList();

    ~JobsList() {
        for (JobEntry* job : job_entries) {
            delete job;
        }
    }

    void addJob(Command *cmd, pid_t pid, bool isStopped = false);

    void printJobsList();

    void killAllJobs();

    void removeFinishedJobs();

    JobEntry *getJobById(int jobId);

    void removeJobById(int jobId);

    JobEntry *getLastJob(int *lastJobId);

    JobEntry *getLastStoppedJob(int *jobId);

    // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
    JobsList* jobs_list;
    // TODO: Add your data members
public:
    JobsCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell);

    virtual ~JobsCommand() {
    }

    void execute() override;
};

class KillCommand : public BuiltInCommand {
    JobsList* job_list;
public:
    KillCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell);

    virtual ~KillCommand() {
    }

    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
    JobsList* jobs_list;
public:
    ForegroundCommand(const char *cmd_line, JobsList *jobs, SmallShell* shell);

    virtual ~ForegroundCommand() {
    }

    void execute() override;
};

class AliasCommand : public BuiltInCommand {
public:
    AliasCommand(const char *cmd_line, SmallShell* shell);

    virtual ~AliasCommand() {
    }

    void execute() override;
};

class UnAliasCommand : public BuiltInCommand {
public:
    UnAliasCommand(const char *cmd_line, SmallShell* shell);

    virtual ~UnAliasCommand() {
    }

    void execute() override;
};

class UnSetEnvCommand : public BuiltInCommand {
public:
    UnSetEnvCommand(const char *cmd_line, SmallShell* shell);

    virtual ~UnSetEnvCommand() {
    }

    void execute() override;
};

class SysInfoCommand : public BuiltInCommand {
public:
    SysInfoCommand(const char *cmd_line, SmallShell* shell);

    virtual ~SysInfoCommand() {
    }

    void execute() override;
};

class SmallShell {
private:
    // TODO: Add your data members
    string m_prompt = "smash";
    char* m_lastPwd = nullptr; //here we save last path
    JobsList jobs_list;
    pid_t fg_pid = 0;
    SmallShell();

public:
    Command *CreateCommand(const char *cmd_line);

    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator
    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    ~SmallShell();

    void executeCommand(const char *cmd_line);

    void promptChanger(const string& new_prompt);

    string getPrompt() const;

    pid_t getFgPid() const {
        return fg_pid;
    }

    void setFgPid(pid_t pid) {
        fg_pid = pid;
    }

    char* getLastPwd() const{
        return m_lastPwd;
    }

    void setLastPwd(char* newLastPwd) {
        if (m_lastPwd != nullptr) {
            free(m_lastPwd);
        }
        m_lastPwd = newLastPwd;
    }

    // TODO: add extra methods as needed
};


#endif //SMASH_COMMAND_H_
