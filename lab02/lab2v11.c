/******* Lab2.c file *******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <libgen.h> //used for getting dirname and basename

struct NODE{
	char Name[64];           
	char Type[1];             /* D for directory F for file */
	struct NODE *childPtr, *siblingPtr, *parentPtr;
};

struct NODE *root, *cwd;                 /* root and CWD pointers */
char line[128];                   /* user input line */
char command[16], pathname[64];   /* user inputs */
char dname[64], bname[64];   /* string holders*/
int main(){                                                 //WORKS!!!
	// FILE *tempFile;
	int ID;
	initialize();
	cwd = root;
	// printf("%c\n", *cwd->Name);
	while(1){
		char *command;
		printf("root@simulation:~");
		pwd(cwd);
		printf("$ ");
		fgets(line, 128, stdin);
		command = strtok(line, " \n/");	// this breaks up the input into segments
		if(command != NULL){
			ID = findCommand(command);
		}
		else{
			ID = -1;
		}
		command = strtok(NULL, " \n");	// this breaks up the input into segments
		//       printf("\n%d_%s\n", ID, command);
		// printf("%s", cwd->childPtr->
		switch(ID){ 
			case -1 : printf("%s: command not found\n", line); break;
			case 0 : return 0;
			case 1 : mkdir(command);  break;
			case 2 : rmdir(command);  break;
			case 3 : cd(&cwd, command);break;
			case 4 : ls();            break;
			case 5 : pwd(cwd);	printf("\n");           break;
			case 6 : create(command);	      break;
			case 7 : rm(command);     break;
			case 8 : save(root, command);	 break;
			case 9 : reload(root, command);   break;
			case 10 : help(); break;
			//                   ...................

			// }
		}
	}
	return 0;
}
help(){                                                     //WORKS!!!
	printf(" mkdir [dname]   : make a new directiry for the pathname\n rmdir [dname]   : rm the directory, if it is empty\n cd [dname]      : change CWD to pathname, or to / if no pathname\n ls                : list the directory contents of pathname or CWD\n pwd               : print the (absolute) pathname of CWD\n create [FileName] : create a FILE node\n rm [FileName]     : rm a FILE node\n save [FileName]   : save the current file system tree in a file\n reload [FileName] : re-initalize the file system tree from a file\n help              : save the file system tree, then terminate the program\n");
}
reload(struct NODE *inputCWD, char *inputFile){             //Need to finish
	// First we need to clear the existing directory
	FILE *infile;
	char *tempName, tempDirName[128];
	if (inputFile == NULL){
		printf("ERROR: No input file name.\n");
		return 0;
	}
	else{
		cwd = root;
		if(root->childPtr){
			recursiveDelete(root->childPtr);
			root->childPtr = NULL;
		}
		infile = fopen(inputFile, "r");
		if(fgets(line, sizeof(line), infile) != NULL && line[0] == '/'){
			//          printf("read '%s'",line);

			while (fgets(line, sizeof(line), infile) != NULL){
				//pwd(cwd);
				tempName = strtok(line, "\n");
				printf("read '%s' == '%s'\n",line, tempName);
				if(tempName && tempName[0] == '~'){
					tempName = strtok(line, "~\n");
					while(tempName){
						printf("create '%s'\n",tempName);
						reloadCreate(tempName);
						tempName = strtok(NULL, "~");
					}
				}
				if(tempName && tempName[0] == '/'){
					printf("mkdirs: '%s'\n",tempName);
					// cd(&cwd, "/");
					strcpy(tempDirName, tempName);
					printf("cd: '%s'\n", dirname(tempDirName));
					printf("mkdirs: '%s'\n",tempName);
					cd(&cwd, tempDirName);
					reloadMkdir(basename(tempName));
					cd(&cwd, basename(tempName));
					printf("Now in:");
					pwd(cwd);
					printf("\n");
				}
			}
		}
	}
	cd(&cwd, "/");
	return 1;
}
reloadMkdir(char *dname){                                         //WORKS!!!
	char *FileName;
	struct NODE *temp;
	temp = cwd;
	strcpy(bname, basename(dname));
	if(0 == strcmp(basename(dname), "/")){
		//      printf("cannot create directory '/'");
		return 0;
	}
	if(!dname || bname[0] == '.' || bname[0] == '/'){
		return 0;
	}
	char *character = dirname(dname);
	if(strcmp(character, ".")){
		//  printf("%s\n", character);
		if(cd(&temp, character) == 0){
			//      printf("mkdir: cannot create directory '%s': directory does not exist\n", dname);
			return 0;
		};
	}
	//  printf("%s ###: ", character);
	//  pwd(temp);
	//  printf(" ###\n");
	//  printf("Dir: '%s' File: '%s'\n",dname, bname);
	if(temp->childPtr == NULL){      
		temp->childPtr = (struct NODE *)malloc(sizeof(struct NODE));
		strcpy(temp->childPtr->Name, bname);
		temp->childPtr->parentPtr = temp;
		strcpy(temp->childPtr->Type, "D");
	}
	else{
		temp = temp->childPtr;
		if(0 == strcmp(temp->Name, bname)){
			//    printf("mkdir: cannot create directory '%s': directory exists\n", bname);
			return 0;
		}
		while(temp->siblingPtr != NULL){
			temp = temp->siblingPtr;
			//   printf("%s != %s\n", temp->Name, dname);
			if(0 == strcmp(temp->Name, bname)){
				//   printf("mkdir: cannot create directory '%s': directory exists\n", dname);
				return 0;
			}
		}
		temp->siblingPtr = (struct NODE *)malloc(sizeof(struct NODE));
		strcpy(temp->siblingPtr->Name, bname);
		temp->siblingPtr->parentPtr = temp->parentPtr;
		strcpy(temp->siblingPtr->Type, "D");
	}
	//printf("Directory: %s Created.\n", dname);
}
reloadCreate(char *dname){                                        //Need to finish
	struct NODE *temp;
	char *character = dirname(dname);
	char *FileName = basename(dname);
	temp = cwd;
	strcpy(bname, basename(dname));

	//  printf("FileName is '%s'\n", FileName);
	if(temp->childPtr == NULL){
		temp->childPtr = (struct NODE *)malloc(sizeof(struct NODE));
		strcpy(temp->childPtr->Name, FileName);
		temp->childPtr->parentPtr = temp;
		strcpy(temp->childPtr->Type, "F");
	}
	else{
		temp = temp->childPtr;
		while(temp->siblingPtr != NULL){
			temp = temp->siblingPtr;
			//   printf("%s != %s\n", temp->Name, FileName);
		}
		temp->siblingPtr = (struct NODE *)malloc(sizeof(struct NODE));
		strcpy(temp->siblingPtr->Name, FileName);
		temp->siblingPtr->parentPtr = temp->parentPtr;
		strcpy(temp->siblingPtr->Type, "F");
	}
	//printf("Directory: %s Created.\n", FileName);
}


int saveDir(struct NODE *inputCWD, char *outputFile){
	struct NODE *Temp;
	FILE *outfile;
	outfile = fopen(outputFile, "a"); //appends file
	//   fprintf(outfile, "\n");
	fclose(outfile);
	rwd(inputCWD, outputFile); // writes cwd to file

	outfile = fopen(outputFile, "a"); //appends file
	fprintf(outfile, "\n");
	if(inputCWD->childPtr){
		Temp = inputCWD->childPtr;
		if(*Temp->Type == 'F'){  // checking to see if first childPtr of inputCWD is a file
			fprintf(outfile, "\n"); 
			fprintf(outfile, "~%s", Temp->Name);
		}
		while(Temp->siblingPtr){ // going through directory and fprintf all files within
			Temp = Temp->siblingPtr;
			if(*Temp->Type == 'F'){  // checking to see if first childPtr of inputCWD is a file
				fprintf(outfile, "~%s", Temp->Name);
			}
		}
		fprintf(outfile, "\n");
		fclose(outfile);

		// Now we recursively print the folders within the input directory
		Temp = inputCWD->childPtr;
		if(*Temp->Type == 'D'){  // checking to see if first childPtr of inputCWD is a file
			saveDir(Temp, outputFile);
		}
		while(Temp->siblingPtr){ // going through directory and fprintf all files within
			Temp = Temp->siblingPtr;
			if(*Temp->Type == 'D'){  // checking to see if first childPtr of inputCWD is a file
				//                outfile = fopen(outputFile, "a"); //appends file
				//              fprintf(outfile, "\n");
				//            fclose(outfile);
				saveDir(Temp, outputFile);
			}
		}
	}

	return 1;
}
save(struct NODE *inputCWD, char *outputFile){
	FILE *outfile;
	if (outputFile == NULL){
		printf("ERROR: No output file name.\n");
		return 0;
	}
	outfile = fopen(outputFile, "w"); // creates output file
	if(outfile){
		fclose(outfile);                  // closes root
	}
	else{
		printf("error with file name '%s' try different name\n", outputFile);
		return 0;
	}
	if(saveDir(inputCWD, outputFile)){// begins examining root and saving directory information. returns true if successful.
		printf("save successful\n");
		return 1;        
	}
	else{
		printf("Error: an unknown error occured in saveDir()\n");
	}

}
/*
 save(struct NODE *inputCWD, char *outputFile){              //WORKS Sort of.
	 if (outputFile == NULL){
		 printf("ERROR: No output file name.\n");
		 return 0;
		 }
		 //print CWD then on next line ~file~file2~file3~etc
		 //then go up dir and down dir and savedir(dir)
		 /*
		  savedir(dir, &outfile){
			  fprintf(cwd\n);
			  if(dir->childPtr == NULL){
				  return 0;
				  }
				  dir = dir->childPtr;
				  do{
					  if(dir->Type == 'D'){
						  fprintf("~%s", dir->Name);
						  }
						  }while(dir->siblingPtr && dir = dir->siblingPtr);

						  temp = temp->parentPtr->childPtr;
						  do{
							  if (temp->Type == 'D'){
								  savedir(temp->childPtr, &outfile);
								  }
								  }while(temp->siblingPtr && temp = temp->siblingPtr;);
								  */
		 //  printf("started with NODE: %s, output file %s", inputCWD->Name, outputFile);
		 /*  int stop = 0;
		  struct NODE *tempFile;
		  tempFile = inputCWD;
		  FILE *outfile;
		  printf("\n");
		  pwd(inputCWD);
		  printf("\n");
		  if(inputCWD->childPtr != NULL){
			  tempFile = inputCWD->childPtr;
			  }
			  else{
				  if(strcmp(tempFile->Name, "F")){
					  //    printf("######%s", tempFile->Name);////////////////////////////////////////////////////////
					  rwd(tempFile, outputFile);
					  }
					  outfile = fopen(outputFile, "a"); //appends file
					  fprintf(outfile, "\n");
					  return 0;
					  }

					  if(inputCWD == root){
						  outfile = fopen(outputFile, "w");
						  fprintf(outfile, "000/\n");
						  }
						  else{
							  if(strcmp(tempFile->Name, "F")){
								  //     printf("######%s", tempFile->Name);////////////////////////////////////////////////////////
								  rwd(tempFile, outputFile);
								  }
								  outfile = fopen(outputFile, "a"); //appends file
								  fprintf(outfile, "\n");
								  }
								  printf("%s:", tempFile->Type);
								  while(stop != 1){
									  if(!strcmp(tempFile->Type , "F")){
										  //      printf("######%s", tempFile->Name);////////////////////////////////////////////////////////

										  printf("~%s", tempFile->Name);
										  fprintf(outfile, "~%s",tempFile->Name);
										  }
										  if(tempFile->siblingPtr != NULL){
											  tempFile = tempFile->siblingPtr;
											  }    else{
												  stop = 1;
												  }
												  }

												  fprintf(outfile, "\n");

												  stop = 0;
												  tempFile = inputCWD->childPtr; //sets starting point.
												  printf("\nFolders:\n");

												  while(stop != 1){// looking for folders
													  if(0 == strcmp(tempFile->Type, "D")){
														  fclose(outfile);  // preps file for appendages
														  save(tempFile, outputFile);
														  outfile = fopen(outputFile, "a"); //appends file
														  if(tempFile->childPtr == NULL){
															  fprintf(outfile, "\n");
															  }
															  }
															  if(tempFile->siblingPtr != NULL){
																  tempFile = tempFile->siblingPtr;
																  }
																  else{
																	  stop = 1;
																	  }
																	  }
																	  fclose(outfile);
																	  }*/
		 initialize(){                                               //WORKS!!!
			 root = (struct NODE *)malloc(sizeof(struct NODE));
			 strcpy(root->Name, "/");
			 strcpy(root->Type, "D");
		 }
		 mkdir(char *dname){                                         //WORKS!!!
			 char *FileName;
			 struct NODE *temp;
			 temp = cwd;
			 strcpy(bname, basename(dname));
			 if(0 == strcmp(basename(dname), "/")){
				 //      printf("cannot create directory '/'");
				 return 0;
			 }
			 if(!dname || bname[0] == '.' || bname[0] == '/'){
				 return 0;
			 }
			 char *character = dirname(dname);
			 if(strcmp(character, ".")){
				 printf("%s\n", character);
				 if(cd(&temp, character) == 0){
					 printf("mkdir: cannot create directory '%s': directory does not exist\n", dname);
					 return 0;
				 };
			 }
			 //  printf("%s ###: ", character);
			 //  pwd(temp);
			 //  printf(" ###\n");
			 //  printf("Dir: '%s' File: '%s'\n",dname, bname);
			 if(temp->childPtr == NULL){      
				 temp->childPtr = (struct NODE *)malloc(sizeof(struct NODE));
				 strcpy(temp->childPtr->Name, bname);
				 temp->childPtr->parentPtr = temp;
				 strcpy(temp->childPtr->Type, "D");
			 }
			 else{
				 temp = temp->childPtr;
				 if(0 == strcmp(temp->Name, bname)){
					 printf("mkdir: cannot create directory '%s': directory exists\n", bname);
					 return 0;
				 }
				 while(temp->siblingPtr != NULL){
					 temp = temp->siblingPtr;
					 //   printf("%s != %s\n", temp->Name, dname);
					 if(0 == strcmp(temp->Name, bname)){
						 printf("mkdir: cannot create directory '%s': directory exists\n", dname);
						 return 0;
					 }
				 }
				 temp->siblingPtr = (struct NODE *)malloc(sizeof(struct NODE));
				 strcpy(temp->siblingPtr->Name, bname);
				 temp->siblingPtr->parentPtr = temp->parentPtr;
				 strcpy(temp->siblingPtr->Type, "D");
			 }
			 //printf("Directory: %s Created.\n", dname);
		 }




		 create(char *dname){                                        //Need to finish
			 char *FileName = basename(dname);
			 strcpy(bname, basename(dname));
			 if(dname == NULL || FileName[0] == '.' || FileName[0] == '/'){
				 return 0;
			 }
			 struct NODE *temp;
			 char *character = dirname(dname);
			 temp = cwd;
			 strcpy(bname, basename(dname));
			 //  printf("%s == %s\n", dirname(dname), dname);
			 if(strcmp(character, ".") || character[0] == '/'){
				 //    printf("dirname is . or /: %s\n", character);
				 if(cd(&temp, character) == 0){
					 printf("create: cannot create file '%s': directory does not exist\n", bname);
					 return 0;
				 };
			 }
			 else{
				 printf("dir: '%s'\n",character);
				 if(strcmp(character, ".") && !cd(&temp, character)){
					 return 0;
				 }
			 }

			 printf("FileName is '%s'\n", FileName);
			 if(temp->childPtr == NULL){
				 temp->childPtr = (struct NODE *)malloc(sizeof(struct NODE));
				 strcpy(temp->childPtr->Name, FileName);
				 temp->childPtr->parentPtr = temp;
				 strcpy(temp->childPtr->Type, "F");
			 }
			 else{
				 temp = temp->childPtr;
				 if(0 == strcmp(temp->Name, FileName)){
					 printf("create: cannot create file '%s': File exists\n", FileName);
					 return 0;
				 }
				 while(temp->siblingPtr != NULL){
					 temp = temp->siblingPtr;
					 //   printf("%s != %s\n", temp->Name, FileName);
					 if(0 == strcmp(temp->Name, FileName)){
						 printf("create: cannot create file '%s': File exists\n", FileName);
						 return 0;
					 }
				 }
				 temp->siblingPtr = (struct NODE *)malloc(sizeof(struct NODE));
				 strcpy(temp->siblingPtr->Name, FileName);
				 temp->siblingPtr->parentPtr = temp->parentPtr;
				 strcpy(temp->siblingPtr->Type, "F");
			 }
			 //printf("Directory: %s Created.\n", FileName);
		 }
		 int findCommand(char *command){                             //WORKS!!!
			 if(0 == strcmp(command, "quit")){return 0;}
			 if(0 == strcmp(command, "mkdir")){return 1;}
			 if(0 == strcmp(command, "rmdir")){return 2;}
			 if(0 == strcmp(command, "cd")){return 3;}
			 if(0 == strcmp(command, "ls")){return 4;}
			 if(0 == strcmp(command, "pwd")){return 5;}
			 if(0 == strcmp(command, "create")){return 6;}
			 if(0 == strcmp(command, "rm")){return 7;}
			 if(0 == strcmp(command, "save")){return 8;}
			 if(0 == strcmp(command, "reload")){return 9;}
			 if(0 == strcmp(command, "help")){return 10;}
			 if(0 == strcmp(command, "h")){return 10;}
			 if(0 == strcmp(command, "dir")){return 4;}		// more used to using dir than ls
			 else{return -1;}
		 }
		 ls(){                                                       //WORKS!!!
			 struct NODE *temp;
			 if(cwd->childPtr != NULL){
				 temp = cwd->childPtr;
				 printf("%c: %s\n",temp->Type[0], temp->Name);
				 while(temp->siblingPtr != NULL){
					 temp = temp->siblingPtr;
					 printf("%c: %s\n",temp->Type[0], temp->Name);
				 }	
			 }
			 else{
				 printf("Directory is empty.\n");
			 }
		 }						

		 struct NODE *temp;
		 if(cwd->childPtr != NULL){
			 temp = cwd->childPtr;
			 printf("%c: %s\n",temp->Type[0], temp->Name);
			 while(temp->siblingPtr != NULL){
				 temp = temp->siblingPtr;
				 printf("%c: %s\n",temp->Type[0], temp->Name);
			 }	
		 }
		 else{
			 printf("Directory is empty.\n");
		 }
		 }						
		 rmdir(char *DirName){
			 struct NODE *temp;
			 struct NODE *temp2;
			 struct NODE *lastNode;
			 char directory[128];
			 strcpy(directory, DirName);
			 //printf("dir: '%s'\n", directory);
			 strcpy(directory, dirname(directory));
			 // printf("dir: '%s'\n", directory);
			 temp2 = cwd;
			 if(strcmp(directory, ".")){
				 //   printf("cd: '%s'\n", directory);
				 if(0 == cd(cwd, directory)){
					 return 0;
				 }
			 }
			 //  printf("base: '%s'\n", DirName);
			 strcpy(DirName, basename(DirName));
			 // printf("base: '%s'\n", DirName);
			 if(cwd->childPtr == NULL){
				 if(DirName == NULL){
					 return 0;
				 }
				 printf("rmdir: failed to remove '%s': No such file or directory\n", DirName);
				 return 0;
			 }
			 else{
				 //    printf("looking for '%s'...\n", DirName);
				 temp = cwd->childPtr;
				 if(0 == strcmp(temp->Name, DirName)){
					 //  printf("Directory found...");
					 if(temp->siblingPtr != NULL){
						 cwd->childPtr = temp->siblingPtr;
					 }
					 else{
						 cwd->childPtr = NULL;
					 }
					 if(temp->childPtr != NULL){
						 printf("rmdir: failed to remove '%s': Directory is not empty\n", DirName);
						 cwd = temp2;
						 return 0;
					 }
					 free(temp);
					 return 1; 
				 }
				 while(temp->siblingPtr != NULL){
					 lastNode = temp;
					 temp = temp->siblingPtr;
					 if(0 == strcmp(temp->Name, DirName)){
						 //      printf("Directory found...");
						 if(temp->siblingPtr != NULL){
							 lastNode->siblingPtr = temp->siblingPtr;
						 }
						 if(temp->childPtr != NULL){
							 printf("rmdir: failed to remove '%s': Directory is not empty\n", DirName);
							 cwd = temp2;
							 return 0;
						 }
						 free(temp);
						 lastNode->siblingPtr = NULL;
						 return 1;
					 }
				 }
				 printf("rmdir: failed to remove '%s': No such file or directory\n", DirName);
				 return 0;
			 }
		 }
		 recursiveDelete(struct NODE *DirectoryToDelete){            //WORKS!!!
			 if(DirectoryToDelete->childPtr != NULL){
				 recursiveDelete(DirectoryToDelete->childPtr);
				 DirectoryToDelete->childPtr = NULL;
			 }
			 if(DirectoryToDelete->siblingPtr != NULL){
				 recursiveDelete(DirectoryToDelete->siblingPtr);
				 DirectoryToDelete->siblingPtr = NULL;
			 }
			 free(DirectoryToDelete);
		 }
		 int cd(struct NODE ** tempVerified, char *dname){           //WORKS!!! ex: cd(Pointer2Change, DestinationString)
			 struct NODE *temp;
			 temp = *tempVerified;
			 int found = 0;
			 //printf("looking for '/' dir '%s'\n", dname);    // Used when troubleshooting.

			 if(dname && dname[0] == '/'){ //This checks for '/'
				 //     printf("looking for '/' dir '%s'\n", dname);    // Used when troubleshooting.
				 temp = root;
				 found = 1;    
			 }

			 dname = strtok(dname, " \n/");	// this sets dname to the next segment of code or NULL if none exists    

			 while (dname){
				 //    printf("looking for 2 '%s'\n", dname);
				 if(0 == strcmp(dname, "..")){
					 if(temp == root){
						 return 0;
					 }
					 temp = temp->parentPtr;
					 found = 1;
				 }
				 else if(temp->childPtr){
					 temp = temp->childPtr;
					 while(temp->siblingPtr != NULL && strcmp(temp->Name, dname)){ // Progressing through directory looking for next directory
						 temp = temp->siblingPtr;
					 }
					 if(0 == strcmp(temp->Name, dname)){
						 found = 1;
					 }
					 else{
						 return 0;
					 }
				 }
				 //  printf("comparing '%s' in '%s'\n", dname, temp->Name);    // Used when troubleshooting


				 dname = strtok(NULL, " \n/");	// this sets dname to the next segment of code or NULL if none exists    
			 }
			 if(found == 1){
				 if(0 == strcmp(temp->Type, "F")){
					 return 0;
				 }
				 //    printf("Found\n");
				 *tempVerified = temp;
				 return 1;
			 }
			 else{
				 return 0;
			 }
		 }
		 /*
		  if(dname && dname[0] == '/'){ //This checks for '/'
			  printf("looking for '/' dir '%s'\n", dname);    // Used when troubleshooting.
			  temp = root;
			  dname = strtok(dname, " \n/");	// this sets dname to the next segment of code or NULL if none exists
			  found = 1;
			  if(dname){
				  printf("looking for '%s'\n", dname);    // Used when troubleshooting.
				  found = cd(&temp, dname);
				  if(found){
					  *tempVerified = temp;
					  return 1;
					  }
					  else{
						  return 0;
						  }
						  }
						  else{
							  *tempVerified = temp;
							  return 1;
							  }
							  }
							  else{ // since dname didn't start with a '/' we know it's not the absolute directory
								  dname = strtok(dname, " \n/");	// this removes any characters at the beginning
								  //      printf("looking for 2 '%s'\n", dname);    // Used when troubleshooting.
								  if(0 == strcmp(dname, "..")){
									  dname = strtok(NULL, " \n/");
									  if(temp == root){
										  //  printf("temp is root\n");
										  return 0;
										  }
										  else{
											  // printf("going '..'\n");
											  temp = temp->parentPtr;
											  // printf("DirDone\n");
											  if(dname){
												  //     printf("%s\n", dname);
												  }
												  }
												  found = 1;
												  if(dname){
													  // printf("%s\n", dname);
													  found = cd(&temp, dname);
													  if(found){
														  *tempVerified = temp;
														  return 1;
														  }
														  else{
															  return 0;
															  }
															  }
															  else{
																  *tempVerified = temp;
																  return 1;
																  }
																  }
																  }
																  // printf("looking for dir '%s' in '%s'\n", dname, temp->Name);    // Used when troubleshooting.
																  if(temp->childPtr){
																	  temp = temp->childPtr;
																	  }
																	  else{
																		  return 0;
																		  }
																		  while(temp->siblingPtr != NULL && strcmp(temp->Name, dname)){ // Progressing through directory folders/files looking for next directory
																			  temp = temp->siblingPtr;
																			  }
																			  //  printf("comparing '%s' in '%s'\n", dname, temp->Name);    // Used when troubleshooting.
																			  if(0 == strcmp(temp->Name, dname)){
																				  printf("looking for 2 '%s'\n", dname);    // Used when troubleshooting.

																				  dname = strtok(NULL, " \n/");	// this sets dname to the next segment of code or NULL if none exists
																				  if(dname){
																					  //         printf("Looking for '%s'\n", dname);
																					  printf("looking for 3 '%s'\n", dname);    // Used when troubleshooting.
																					  found = cd(&temp, dname);
																					  if(found){
																						  *tempVerified = temp;
																						  return 1;
																						  }
																						  else{
																							  return 0;
																							  }
																							  }
																							  else{
																								  //       printf("found\n");
																								  *tempVerified = temp;
																								  //       printf("%s\n",temp->Name);
																								  return 1;
																								  }
																								  }
																								  else{
																									  return 0;
																									  }
																									  }*/
		 /*Bunch of old code  
		  if(!dname || 0 == strcmp(dname, "..")){
			  //if(temp == cwd){
			  if(!dname){ // if no dname passed then return root.
				  temp = root;
				  //	printf("Moved to root dir.\n");
				  return 1;
				  }
				  if(0 == strcmp(dname, "..")){
					  if(temp != root){
						  temp = temp->parentPtr;
						  dname = strtok(NULL, " \n/");	// this sets dname to the next segment of code or NULL if none exists
						  if(!dname){
							  return 1;
							  }
							  if(cd(cwd, dname)){
								  return 1;
								  }
								  else{
									  cwd = temp;
									  return 0;
									  }
									  }
									  else{
										  return 0;
										  }
										  }
										  // }
										  else{// This occurs if directory has moved since cd was initially called
											  //     printf("found!\n");
											  cwd = temp;
											  return 1;
											  }
											  }
											  else{
												  if(temp->childPtr == NULL){
													  printf("Directory temp->childPtr '%s' not found.\n", dname);
													  return 0;
													  }
													  else{
														  if(0 == strcmp(temp->childPtr->Name, dname)){ // if next directory is found move to this directory
															  if (!strcmp(temp->childPtr->Type, "F")){
																  printf("'%s' is a File and not a Directory\n", dname);
																  return 0;
																  }
																  //	printf("moved to directory\n");
																  temp = temp->childPtr;
																  dname = strtok(NULL, " /\n");
																  found = cd(temp, dname); // Proceed to next directory
																  }
																  else{
																	  temp = temp->childPtr; // Only happens when childPtr exists
																	  while(temp->siblingPtr != NULL && strcmp(temp->Name, dname)){ // Progressing through directory folders/files looking for next directory
																		  temp = temp->siblingPtr;
																		  }
																		  //	printf("%s/", temp->Name); // name of directory ended at;
																		  if(strcmp(temp->Name, dname)){
																			  printf("Directory temp->name '%s' not found.\n", dname);
																			  return 0;
																			  }
																			  else{  // If directory found.
																				  if (!strcmp(temp->Type, "F")){
																					  printf("'%s' is a File and not a Directory\n", dname);
																					  return 0;
																					  }
																					  else{
																						  dname = strtok(NULL, " ");
																						  found = cd(temp, dname); // Proceed to next directory
																						  }
																						  }
																						  }
																						  }
																						  }
																						  return found;
																						  }
																						  */
		 rm(char *dname){
			 struct NODE *temp;
			 struct NODE *lastNode;
			 // printf("looking for '%s'...\n", dname);
			 if(cwd->childPtr == NULL){
				 printf("rm: cannot remove '%s': No such file or directory\n", dname);
				 return 0;
			 }
			 else{
				 if(!dname){
					 printf("no parameters set.\n");
					 return 0;
				 }  
				 temp = cwd->childPtr;
				 if(0 == strcmp(temp->Name, dname)){
					 //printf("File found...");
					 if(temp->siblingPtr != NULL){
						 cwd->childPtr = temp->siblingPtr;
					 }
					 else{
						 cwd->childPtr = NULL;
					 }
					 free(temp);
					 //printf(" and removed\n");
					 return 1; 
				 }
				 while(temp->siblingPtr != NULL){
					 lastNode = temp;
					 temp = temp->siblingPtr;
					 if(0 == strcmp(temp->Name, dname)){
						 //    printf("File found...");
						 if(temp->siblingPtr != NULL){
							 lastNode->siblingPtr = temp->siblingPtr;
						 }
						 else{
							 lastNode->siblingPtr = NULL;
						 }
						 free(temp);

						 //	printf(" and removed\n");
						 return 1;                               // Will remove all files within directory before returning.
					 }
				 }
				 printf("rm: cannot remove '%s': No such file or directory\n", dname);
				 return 0;
			 }
		 }
		 pwd(struct NODE *inputDir){                                 //WORKS!!!
			 struct NODE *temp;
			 if(inputDir == root){
				 printf("%s", root->Name);
			 }
			 if(inputDir != root && inputDir->parentPtr != root){
				 temp = inputDir->parentPtr;
				 pwd(temp);
				 printf("/%s", inputDir->Name);
			 }
			 if(inputDir->parentPtr == root){
				 temp = inputDir->parentPtr;
				 pwd(temp);
				 printf("%s", inputDir->Name);
			 }
		 }
		 rwd(struct NODE *inputDir, char *outputFile){ // Saves working director to file
			 struct NODE *temp;
			 FILE *outfile;
			 outfile = fopen(outputFile, "a"); //appends file

			 if(inputDir == root){
				 fprintf(outfile, "/");
			 }
			 if(inputDir != root && inputDir->parentPtr != root){
				 temp = inputDir->parentPtr;
				 fclose(outfile);
				 rwd(temp, outputFile);
				 outfile = fopen(outputFile, "a");
				 fprintf(outfile, "%s/",inputDir->Name);
			 }
			 if(inputDir->parentPtr == root){
				 temp = inputDir->parentPtr;
				 fclose(outfile);
				 rwd(temp, outputFile);
				 outfile = fopen(outputFile, "a");
				 fprintf(outfile, "%s/",inputDir->Name);    
			 }
			 printf("%s/\n", inputDir->Name);
			 fclose(outfile);
		 }
		 