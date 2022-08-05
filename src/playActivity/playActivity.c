#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <stdbool.h>  
#include <sys/stat.h>  
#include <fcntl.h>
#include <time.h>

#include "cjson/cJSON.h"
#include "../common/utils.h"

// Max number of records in the DB
#define MAXVALUES 1000
#define MAXBACKUPFILES 80
#define PLAY_ACTIVITY_DB_PATH "/mnt/SDCARD/Saves/CurrentProfile/saves/playActivity.db"


static struct rom_s
{
	char name[100];
	int playTime;
}
rom_list[MAXVALUES];
static int rom_list_len = 0;

int readRomDB(void)
{
  	// Check to avoid corruption
  	if (!file_exists(PLAY_ACTIVITY_DB_PATH))
		return 1;

	FILE * file = fopen(PLAY_ACTIVITY_DB_PATH, "rb");

	if (file == NULL)
		// The file exists but could not be opened
		// Something went wrong, the program is terminated
		return -1;

	fread(rom_list, sizeof(rom_list), 1, file);
	rom_list_len = 0;
	int i;
	
	for (i = 0; i < MAXVALUES; i++) {
		if (strlen(rom_list[i].name) > 0)
			rom_list_len = i + 1;
	}

	fclose(file);
}

void writeRomDB(void)
{
	if (rom_list_len > 0){
		remove(PLAY_ACTIVITY_DB_PATH);
		FILE * file= fopen(PLAY_ACTIVITY_DB_PATH, "wb");
		if (file != NULL) {
    		fwrite(rom_list, sizeof(rom_list), 1, file);
    		fclose(file);
    		system("sync");
		}
	}
}


void displayRomDB(void){
	printf("--------------------------------\n");
	for (int i = 0 ; i < rom_list_len ; i++) {	
			printf("romlist name: %s\n", rom_list[i].name);
			
			char cPlayTime[15];
			sprintf(cPlayTime, "%d", rom_list[i].playTime);
			printf("playtime: %s\n", cPlayTime);
	 }
	printf("--------------------------------\n");

}
	
int searchRomDB(char* romName){
	int position = -1;
	
	for (int i = 0 ; i < rom_list_len ; i++) {
		if (strcmp(rom_list[i].name,romName) == 0){
			position = i;
			break;
		}
	}
	return position;
}


void backupDB(void){

	char fileNameToBackup[120];
	char fileNameNextSlot[120];
	char command[250];	
	int i;
	mkdir("/mnt/SDCARD/Saves/CurrentProfile/saves/PlayActivityBackup", 0700);
	for (i=0; i<MAXBACKUPFILES; i++) {
		snprintf(fileNameToBackup,sizeof(fileNameToBackup),"/mnt/SDCARD/Saves/CurrentProfile/saves/PlayActivityBackup/playActivityBackup%02d.db",i);
		if ( access(fileNameToBackup, F_OK) != 0 ) break;
	} 
			
	// Backup		
	if (i<MAXBACKUPFILES){
		snprintf(fileNameNextSlot,sizeof(fileNameNextSlot),"/mnt/SDCARD/Saves/CurrentProfile/saves/PlayActivityBackup/playActivityBackup%02d.db",i+1);
	}else{
		snprintf(fileNameToBackup,sizeof(fileNameToBackup),"/mnt/SDCARD/Saves/CurrentProfile/saves/PlayActivityBackup/play<ActivityBackup00.db");
		snprintf(fileNameNextSlot,sizeof(fileNameNextSlot),"/mnt/SDCARD/Saves/CurrentProfile/saves/PlayActivityBackup/playActivityBackup01.db");
	}
	// Next slot for backup
	remove(fileNameToBackup);
	remove(fileNameNextSlot);
			
	sprintf(command, "cp /mnt/SDCARD/Saves/CurrentProfile/saves/playActivity.db %s", fileNameToBackup);
	system(command);

}



int main(int argc, char *argv[]) {
  	
  	
	if (argc > 1){
		if (strcmp(argv[1],"init") == 0) {
			int epochTime = (int)time(NULL);
			char baseTime[15];
			sprintf(baseTime, "%d", epochTime);
			
			remove("initTimer");
			int init_fd = open("initTimer", O_CREAT | O_WRONLY);
			if (init_fd>0) {
        		write(init_fd, baseTime, strlen(baseTime));
        		close(init_fd);
        		system("sync");
        
			}			
		}
	
		else {
			if (strcmp(argv[1],"") != 0) {
				FILE *fp;
				long lSize;
				char *baseTime;
				char *gameName = (char *)basename(argv[1]);
				gameName[99] = '\0';
				fp = fopen ( "initTimer" , "rb" );
			
				if( fp > 0 ) {
					fseek( fp , 0L , SEEK_END);
					lSize = ftell( fp );
					rewind( fp );
					baseTime = (char*)calloc( 1, lSize+1 );
					if( !baseTime ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
				
					if( 1!=fread( baseTime , lSize, 1 , fp) )
  					fclose(fp),free(baseTime),fputs("entire read fails",stderr),exit(1);
					fclose(fp);
				
		
					int iBaseTime = atoi(baseTime) ;
    				
    				int iEndEpochTime = (int)time(NULL);
					char cEndEpochTime[15];
					sprintf(cEndEpochTime, "%d", iEndEpochTime);
	
					char cTempsDeJeuSession[15];	
    				int iTempsDeJeuSession = iEndEpochTime - iBaseTime ;
    				sprintf(cTempsDeJeuSession, "%d", iTempsDeJeuSession);
			
					// Loading DB
					if (readRomDB()  == -1){
						// To avoid a DB overwrite
					 	return EXIT_SUCCESS;
					}
    			 	
    				//Addition of the new time
    				int totalPlayTime;
    				int searchPosition = searchRomDB(gameName);
    				if (searchPosition>=0){
    					// Game found
    					rom_list[searchPosition].playTime += iTempsDeJeuSession;
    					totalPlayTime = rom_list[searchPosition].playTime;
						}
    				else {
    					// Game inexistant, add to the DB	
    					if (rom_list_len<MAXVALUES-1){
    						rom_list[rom_list_len].playTime = iTempsDeJeuSession;
    						totalPlayTime = iTempsDeJeuSession ;
    						strcpy (rom_list[rom_list_len].name, gameName);	
    						rom_list_len ++;
    					}
    					else {
    						totalPlayTime = -1;
    					}	
    				}
    					
    				// Write total current time for the onion launcher				 	
					char cTotalTimePlayed[50];	 	
					remove("currentTotalTime");
					int totalTime_fd = open("currentTotalTime", O_CREAT | O_WRONLY);
	
					if (totalTime_fd > 0) {
					
						if (totalPlayTime >=  0) {				
							int h = (totalPlayTime/3600);
							int m = (totalPlayTime -(3600*h))/60;	
							sprintf(cTotalTimePlayed, "%d:%02d", h,m);
					 		
						}
						else {
							if (totalPlayTime == -1) {
								// DB full, needs to be cleaned
							strcpy(cTotalTimePlayed,"DB:FU");
							}
							else{
							strcpy(cTotalTimePlayed,"ERROR");
							
							}
						}
				
						write(totalTime_fd, cTotalTimePlayed, strlen(cTotalTimePlayed));
        				close(totalTime_fd);		
        				system("sync");
					}
					
   					// DB Backup
   					backupDB();
   					
   					// We save the DB
   					writeRomDB();		
					
					remove("initTimer");   					
   		
   						
				}
			
			}
			
		
			
		}		
		
	}
	
    return EXIT_SUCCESS;
}