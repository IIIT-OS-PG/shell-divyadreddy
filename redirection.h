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

void redirect(char** vect, int opt, int len, int inp, int out, int recfd, int &status);

