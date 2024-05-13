/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
// 用于向waitfg反映前台进程结束
volatile sig_atomic_t flag_pid = 0;
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);


/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
    //后台作业，fork完直接return，前台作业需要等待子进程组终止waitfg
	eval(cmdline);//此时即为整个输入的cmdline
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{
    // 1.解析命令行
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    //parseline解析同时判断是否请求为后台作业，如果是返回true
    bg = parseline(buf, argv);  // 解析buf到argv字符串数组内，即分割空格函数
    //可查看argv情况
    /*
    for(int i = 0;argv[i] != NULL;i++){
        printf("%s\n",argv[i]);
    }
    exit(0);
    */
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */
    //2.解析后的处理部分
//     在 eval 中，父级必须使用 sigprocmask 在分叉子级之前阻塞 SIGCHLD 信号，然后在通过调用 
// addjob  将子级添加到作业列表后再次使用  sigprocmask  解除这些信号的阻塞。由于子代继承
// 了父代的阻塞向量，子代必须确保在执行新程序之前解除 SIGCHLD 信号的阻塞
    sigset_t mask,prev_mask,gdb_set;
    sigemptyset(&mask);
    sigemptyset(&prev_mask);
    sigemptyset(&gdb_set);
    sigaddset(&mask,SIGCHLD);

    if (!builtin_cmd(argv)) {// 3.处理内置命令
        // 4.外部命令，子进程程序
        sigprocmask(SIG_BLOCK,&mask,&prev_mask);//父子进程均阻塞sigchld
        if ((pid = fork()) == 0) {   /* Child runs user job */
            //只要子进程执行了，就设置新进程组，并在shell添加进jobs
            //并未执行则不做这些动作
            setpgid(0,0);//此时设置子进程ID为子进程组ID
            sigprocmask(SIG_SETMASK,&prev_mask,NULL);
	        if (execve(argv[0], argv, environ) < 0) {
		        printf("%s: Command not found.\n", argv[0]);
		        exit(0);
	        }
	    }
    // 记录打开的进程,只记录了他们的最上层进程
    /* Parent waits for foreground job to terminate */
	    //前台运行
        if (!bg) {
            addjob(jobs,pid,FG,cmdline);//此addjob需要再子进程结束信号到达前运行
            //先不解除阻塞
	        waitfg(pid);//内部有临时解除阻塞
            sigprocmask(SIG_SETMASK,&prev_mask,NULL);//解除SIGCHLD的阻塞
	    }
        //后台运行
	    else{
            addjob(jobs,pid,BG,cmdline);
            int jid = pid2jid(pid);
            printf("[%d] (%d) %s",jid, pid, cmdline);
            sigprocmask(SIG_SETMASK,&prev_mask,NULL);//解除SIGCHLD的阻塞
        }
    }
// 4.回收子进程
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);
    if (!strcmp(argv[0], "jobs")){
        listjobs(jobs); 
        return 1;
    }
    if (!strcmp(argv[0], "fg") || !strcmp(argv[0], "bg")){
        do_bgfg(argv); 
        return 1;
    }
    return 0;     /* not a builtin command */
}

/*每个作业可以由进程 ID (PID)
或作业 ID (JID)标识，后者是由 tsh 分配的一个正整数。Jid 应该
在命令行中用前缀“%”表示。例如，“%5”表示 JID 5，“5”表示 PID 5。*/
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
//Bg < job > 命令通过发送 SIGCONT 信号重启 < job > ，然后在后台运行。参数 < job > 可以是 PID 或 JID
//fg < job > 命令通过发送 SIGCONT 信号重新启动 < job > ，然后在前台运行它。参数 < job > 可以是 PID 或 JID
//需要注意，bg和fg均是对整个进程组实施的

/*
struct job_t {               The job struct 
    pid_t pid;               job PID 
    int jid;                 job ID [1, 2, ...] 
    int state;               UNDEF, BG, FG, or ST 
    char cmdline[MAXLINE];   command line 
};
struct job_t jobs[MAXJOBS];  The job list 
*/
void do_bgfg(char **argv) 
{
    char* ID = argv[1];
    struct job_t* IDjob;
    pid_t pid;
    pid_t pgid;
    int jid;
    int status;
    char fg_char[] = {FG + '0', '\0'};
    char bg_char[] = {BG + '0', '\0'};
    char* fg = fg_char;
    char* bg = bg_char;
    //获取job，pid，pgid
    if(ID[0] == '%'){//输入为jid
        jid = atoi(&ID[1]);
        IDjob = getjobjid(jobs,jid);
        pid = IDjob->pid;
        pgid = __getpgid(pid);
    }
    else{
        pid = atoi(&ID[0]);
        pgid = __getpgid(pid);
        jid = pid2jid(pid);
        IDjob = getjobjid(jobs,jid);
    }
    //printf("Job [%d] (%d) %s\n",jid, pid, IDjob->cmdline);
    if(!strcmp(argv[0], "bg")){
        if(IDjob->state == ST){//继续进程的运行，但放入后台
            if(!kill(-pgid,SIGCONT)){
                IDjob->state = BG;
                printf("Job [%d] (%d) %s",jid, pid, IDjob->cmdline);
            }
        }
    }
    else if(!strcmp(argv[0], "fg")){//继续进程的运行，且放入前台
        if(IDjob->state == ST || IDjob->state == BG){
            if(!kill(-pgid,SIGCONT)){
                IDjob->state = FG;
                //printf("Job [%d] (%d) %s",jid, pid, IDjob->cmdline);
                waitfg(pid);
            }
        }
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
// 要考虑等待前台进程与sigchild的竞争问题，不要在这里使用waitpid
// 包括被前台进程被暂停的情况
void waitfg(pid_t pid)//传入的为前台进程pid
{
    int status;
    sigset_t mask;
    sigemptyset(&mask);
    // 当sigchld_handlers修改flagpid为前台进程pid时，退出循环
    while(flag_pid != pid){
        sigsuspend(&mask);//取消阻塞sigchild并休眠（该函数原子的，不间断的进行）
    }
    // if (waitpid(pid, &status, 0) < 0){
    //     if(errno == ECHILD)
    //         unix_error("foreground process don't have child process\n");
    //     else if(errno == EINTR)
    //         unix_error("waitpid 被信号中断\n");
    // }
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 * 子进程暂停或者终止时，发送SIGCHLD信号
 * 该程序运行时收回所有僵尸进程，但不会wait任何其他正在运行的子进程
 * 打印一条带有作业 PID 和违规信号描述的消息。
 * 同时还需要delete该pid的job
 */
// 收到了某个子进程（头部的）的终止或者停止信号
// 注意，子进程继承父进程的信号处理函数
// 
// 进程可以保存其直接子进程的情况，因此waitpid（-1，，）等待其直接子进程
void sigchld_handler(int sig) 
{
    //1.遍历job，获取当前前台进程id
    pid_t fg_pid = fgpid(jobs);
    int status;
    pid_t wpid;
    //遍历所有子进程，回收僵尸进程
    while((wpid = waitpid(-1, &status, WNOHANG)) > 0){
        //2.假如wpid与前台进程id一致，修改flag_pid
        if(wpid == fg_pid){
            flag_pid = wpid;
        }
        //3.删除job
        deletejob(jobs,wpid);
    }
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  Ctrlc终止所有前台进程
 */
// 已经给前台进程设置pgid为其pid
void sigint_handler(int sig) 
{
    pid_t fg_pid = fgpid(jobs);
    kill(-fg_pid, SIGINT);
    int jid = pid2jid(fg_pid);
    printf("Job [%d] (%d) terminated by signal 2\n",jid, fg_pid);
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  Ctrlz暂停所有前台进程
 */
void sigtstp_handler(int sig) 
{
    pid_t fg_pid = fgpid(jobs);
    struct job_t* IDjob;
    
    int jid = pid2jid(fg_pid);
    IDjob = getjobjid(jobs,jid);
    if (!kill(-fg_pid, SIGTSTP)){
        IDjob->state = ST;
        flag_pid = fg_pid;//终止了前台进程
    }
    printf("Job [%d] (%d) stopped by signal 20\n",jid, fg_pid);
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct 
清除作业结构中的条目
*/
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
// vscode报错不用管，gcc可编译
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 * 来自键盘的退出
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



