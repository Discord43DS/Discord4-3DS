#include <3ds.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

using namespace std;

// Get length of an array
template <typename T,unsigned S>
inline unsigned arraysize(const T (&v)[S]) {
    return S;
}

int main(void)
{
	string menu[] = {"Server1", "Server2", "Server3", "Server4", "Server5", "Server6", "Server7", "Server8", "Server9", "Server10", "Server11", "Server12", "Server13", "Server14", "Server15", "Server16", "Server17", "Server18", "Server19", "Server20"};
	//Will be set by Discord API for each server in a Channel type array
	string channels[] = {"Channel 1", "Channel 2", "Channel 3"};
	unsigned int pointer = 0;
	unsigned int pointerChan = 0;

	// Initialize services
	gfxInitDefault();
	//We need one PrintConsole for each screen
	PrintConsole topScreen, bottomScreen;
	//Initialize console for both screen
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

    //Initialize servers menu on the bottom screen
    consoleSelect(&bottomScreen);
	printf("Menu\n\n");
	printf(("\x1b[31m" + menu[0] + "\x1b[0m\n").c_str());
	for (unsigned int i=1; i< arraysize(menu); i++)
		printf((menu[i] + "\n").c_str());

	while (aptMainLoop())
	{
	    printf("\x1b[30;1H%i", pointer);
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		//Do the keys printing only if keys have changed
		if (kDown & KEY_DUP || kDown & KEY_DDOWN || kDown & KEY_A){
			if (kDown & KEY_DUP){
				pointer -= 1;
				if (pointer == (unsigned) -1)
					pointer = arraysize(menu) - 1;
			}
			else if (kDown & KEY_DDOWN){
				pointer += 1;
				if(pointer == arraysize(menu))
					pointer = 0;
			}
			else if (kDown & KEY_A){
                    consoleClear();
                    printf((menu[pointer] + "\n\n").c_str());
                    printf(("\x1b[31m" + channels[0] + "\x1b[0m\n").c_str());
                    for (unsigned int i=1; i< arraysize(channels); i++){
                        printf((channels[i] + "\n").c_str());
                    }

                while (true){
                    hidScanInput();
                    kDown = hidKeysDown();
                    if (kDown & KEY_B) break;

                    if (kDown & KEY_DUP || kDown & KEY_DDOWN){
                        if (kDown & KEY_DUP){
                            pointerChan -= 1;
                            if (pointerChan == (unsigned) -1)
                                pointerChan = arraysize(channels) - 1;
                        }
                        else if (kDown & KEY_DDOWN){
                            pointerChan += 1;
                            if(pointerChan == arraysize(channels))
                                pointerChan = 0;
                        }

                        consoleClear();
                        printf((menu[pointer] + "\n\n").c_str());
                        for (unsigned int i=0; i< arraysize(channels); ++i){
                            if (i == pointerChan)
                                printf(("\x1b[31m" + channels[i] + "\x1b[0m\n").c_str());
                            else
                                printf((channels[i] + "\n").c_str());
                        }
                    }
                }
			}

			consoleClear();
			printf("Menu\n\n");

			for (unsigned int i=0; i< arraysize(menu); ++i){
				if (i == pointer)
					printf(("\x1b[31m" + menu[i] + "\x1b[0m\n").c_str());
				else
					printf((menu[i] + "\n").c_str());
			}
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}
	// Exit services
	gfxExit();
	return 0;
}
