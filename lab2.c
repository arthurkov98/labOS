#include <stdio.h>
#include <dirent.h>
#include <unistd.h> 

int main(int argc, char* argv[]){
	struct dirent *dirent;
	DIR* dir;

	if(argc < 2){
		char* path;
		path = get_current_dir_name ();
		dir = opendir(path);

		for(;(dirent = readdir(dir)) != NULL;printf("%s\n", dirent->d_name));
		closedir(dir);

	}
	else{

		dir = opendir(argv[1]);
		if (dir == NULL) {
            printf ("Cannot open directory '%s'\n", argv[1]);
            return 1;
        }

		for(;(dirent = readdir(dir)) != NULL;printf("%s\n", dirent->d_name));
		closedir(dir);
	}
}