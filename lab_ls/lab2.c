#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
	struct dirent *dirent;
	DIR* dir;
	char* path;
	struct stat buff;
	struct passwd * owner;
	char is_dir, usr_r, usr_w, usr_x, grp_r, grp_w, grp_x, oth_r, oth_w, oth_x;
	struct tm timer;
	char* mon;

	if(argc < 2) path = get_current_dir_name ();
	else path = argv[1];

	dir = opendir(path);

	if (dir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }

	while((dirent = readdir(dir)) != NULL){
			stat (dirent->d_name, &buff);
			timer = *localtime(&buff.st_atime);
    		owner = getpwuid(buff.st_uid);

    		is_dir = (S_ISDIR(buff.st_mode)) ? 'd' : '-';
    		usr_r = (S_IRUSR & buff.st_mode) ? 'r' : '-';
    		usr_w = (S_IWUSR & buff.st_mode) ? 'w' : '-';
    		usr_x = (S_IXUSR & buff.st_mode) ? 'x' : '-';
    		grp_r = (S_IRGRP & buff.st_mode) ? 'r' : '-';
    		grp_w = (S_IWGRP & buff.st_mode) ? 'w' : '-';
    		grp_x = (S_IXGRP & buff.st_mode) ? 'x' : '-';
    		oth_r = (S_IROTH & buff.st_mode) ? 'r' : '-';
    		oth_w = (S_IWOTH & buff.st_mode) ? 'w' : '-';
    		oth_x = (S_IXOTH & buff.st_mode) ? 'x' : '-';

    		switch (timer.tm_mon) {
			    case 0:  mon = "JAN";     break;
			    case 1:  mon = "FEB";     break;
			    case 2:  mon = "MAR";     break;
			    case 3:  mon = "APR";     break;
			    case 4:  mon = "MAY";     break;
			    case 5:  mon = "JUN";     break;
			    case 6:  mon = "JUL";     break;
			    case 7:  mon = "AVG";     break;
			    case 8:  mon = "SEN";     break;
			    case 9:  mon = "OCT";     break;
			    case 10:  mon = "NOV";     break;
			    case 11:  mon = "DEC";     break;
			}

    		printf("%c%c%c%c%c%c%c%c%c%c\t%s  ", 
    				is_dir, usr_r, usr_w, usr_x, grp_r, grp_w, grp_x, oth_r, oth_w, oth_x, 
    				owner->pw_name
    			);

    		printf("%lld  %s  %.2d:%.2d:%.2d\t%s\n",
    				(long long)buff.st_size, 
    				mon,
    				timer.tm_hour, timer.tm_min, timer.tm_sec, 
    				dirent->d_name
    			);

		}
	closedir(dir);
	return 0;
}