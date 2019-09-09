#include<iostream>
#include<cstring>
#include<vector>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<iostream>
#include<cstring>
#include<vector>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void pipe_cmd(char** vect, vector<int> ind, int len, int recfd, int &status);