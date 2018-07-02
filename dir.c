#include "config.h"
int childnum(int fileid)
{
    int i;
    int num=0;
    for (i=0;i<curdir->directory->child[fileid]->directory->childnum;i++)
    {
        if(curdir->directory->child[fileid]->directory->child[i]->status==1)
        {
            num++;
        }
    }
    return num;
}
