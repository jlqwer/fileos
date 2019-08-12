#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
    #include <conio.h>
    #define CLS system("cls");
#else
    int getch();
    #define CLS system("clear");
#endif // WIN32
#include<string.h>
#include <malloc.h>
#include <time.h>
#define HOSTNAME "filesys"
#define PASSWORD "toor"
#define PWD "/"
int block[5000];
int disk;
char* inputpassword(char *passwd);
char* inputpasswordlinux(char * passwd);
struct alluser{
    char name[100];
    char passowrd[100];
    int status;
    struct filetree *home;
}user[20];
struct cmd{
    char cmds[200];
    char cmd[20][200];
    int cmdn;
}acmd;

struct file{
    char content[1000];
    int file_size;
    int pos;
};

struct directory{
    int childnum;
    struct filetree *child[100];
};
struct filetree *cwd[30];
struct filetree *rootdir;
struct filetree *home;
struct filetree{
    struct filetree *pre;
    struct file *file;
    struct directory *directory;
    char filetype;  //d和f
    long long int create_time;
    int authority[3];
    int user_id;
    char name[100];
    int status;
};

struct path{
    char paths[200];
    char path[20][200];
    int pathn;
    struct filetree *cd;
}cutpaths;

int usernum; //用户总数
int uid; //当前用户uid
int passworderrornum; //密码输错的次数
char cmdusername[100];  //当前用户
struct filetree *curdir;
char cwdstr[500];
int mkdir(struct filetree *pre, struct directory *fpath,char *dirname);
long long int curtime();
int check_rPermission(struct filetree *curfile);
int hasfile(char *dirname,struct filetree *finddir);
int childnum(int fileid);
int rm();
int rmdir();
void chmod();
char *rwx(int aqx,char *aqxstr);
char *getauthority(struct filetree *qx,char *qxstr);
void ls();
void gettimestr(long long int t,char *timestr);
void ll();
int cat(char *filename);
int modcwd(char *dirname);
void getcwd();
int mkdir(struct filetree *pre, struct directory *fpath,char *dirname);
struct file* createfile(struct filetree *pre,struct directory *fpath,char *filename);
int cp(char *yname,char *mname);
int mv(char *yname,char *mname);
int vi(struct filetree *pre,struct directory *fpath,char *filename);
int passwd();
int adduser();
int deluser();
void init();
int hasuser(char *loginname);
char* inputpassword(char *passwd);
int login(char *loginname);
int su(char *uname);
void showmenu();
char isadmin(char * username);
int getcmd();
int cutcmd();
int exec();
#endif // CONFIG_H_INCLUDED
