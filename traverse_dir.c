#include "indexer.h"

//recursively traveres a directory or gets file.
int traverse_Dir(char *dirName){
	char *path = dirName; 
	char *filePath;
	DIR *dir;
	struct dirent *entry;
	if( (dir = opendir(dirName)) == 0){	//if argument is a file, hash file
		hash_Tokens(path);
		return 0;
	}
	while( (entry = readdir(dir) ) != 0){	//visit each file and/or directoy in the argument 
		if(entry->d_type == DT_REG && strcmp(entry->d_name, ".DS_Store")){	// type is a file and is not hidden
			filePath = path;
			filePath = Concat(path, '/');	//concatenate "/" and file name
			filePath = ConcatString(filePath, entry->d_name);
			hash_Tokens(filePath);	//hash file
		
		}else if(entry->d_type == DT_DIR){	//type is a directory
			
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){	//if directory is a . or .. skip it
				continue;
		
			}else{
				
				path = Concat(path, '/');
				path = ConcatString(path, entry->d_name);	//concatenate "/" and directory name for recursing
				traverse_Dir(path);
				path = dirName;	//after recursing reset path to current directory
			
			}	
			
		}
	}
	closedir(dir);
	return 0;
}
