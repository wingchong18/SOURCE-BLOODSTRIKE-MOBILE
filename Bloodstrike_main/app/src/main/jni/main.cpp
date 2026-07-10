#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <sys/system_properties.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>

#include "draw.h"
#include "touch.h"
int Pattern2;
int Pattern;
char buffer[80] = {0};

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



	char sign[256];
	char data[256];
	

void createFileWithProgress() {  
    const char *filePath = "/data/Zeuspubg配置";  
    FILE *file = fopen(filePath, "w");  
    if (file == NULL) {  
        perror("Error creating file");  
        exit(EXIT_FAILURE);  
    }   
    for (float progress = 0.0; progress <= 1.0; progress += 0.01) {  
        int barWidth = 60; 
        int filled = (int)(progress * barWidth);  
        printf("[+] \r%6.2f%% [", progress * 100);  
        for (int i = 0; i < filled; i++) {  
            printf(">");  
        }  
        for (int i = 0; i < barWidth - filled; i++) {  
            printf(" ");  
        }  
        printf("]");  
        fflush(stdout); 
        usleep(50000); 
    }  
    printf("\n"); 
    fclose(file);  
    }  
	
	
using namespace std;
void *print_message(void *thread_id)
{
	long tid;
	tid = (long)thread_id;
	printf("Thread %ld starting...\n", tid);
	printf("Thread %ld finished.\n", tid);
	pthread_exit(NULL);
}


char *kernel()
  {
    struct utsname unameData;      
    if (uname(&unameData) == 0){
        return unameData.release; 
    }else{
    printf("\033[31m");
        printf("[-]Unable to get kernel version。");
		exit(0);
        return NULL;
    }
  }
int main(int argc, char **argv) {  


    
              system("clear");


    new std::thread(音量);
    new std::thread(AimBotAuto);
    TouchScreenHandle();
	FPS限制.SetFps(FPS);
	FPS限制.AotuFPS_init();

	screen_config();
    init_screen_x = screen_x + screen_y;
    init_screen_y = screen_y + screen_x;
	if (init_Vulkan(init_screen_x, init_screen_y)) {
    	ImGui_init();
	} else {
		exit(0);
	}
    // new std::thread(AimBotAuto); (Removed because Aimbot feature was deleted)
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	
	while (1.1) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplAndroid_NewFrame(init_screen_x, init_screen_y);
        ImGui::NewFrame();
		tick();
		drawEnd();

		FPS限制.SetFps(FPS);
		FPF显示 = FPS限制.AotuFPS();
	}
    shutdown();
	return 0;
}
