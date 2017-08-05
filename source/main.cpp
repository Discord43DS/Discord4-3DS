#include <3ds.h>
#include <stdio.h>
//#include <iostream.h>
#include <string>
#include <unistd.h>

using namespace std;

int main(void)
{
	string menu[3] = {"String1", "String2", "String3"};
	int pointer = 0;
	
	// Initialize services
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);  //TODO: Change to Bottom
	
	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame
	
	printf("Menu\n\n");
	printf(("\x1b[31m" + menu[0] + "\x1b[0m\n").c_str());
	for (int i=1; i<3; i++)
	{
		printf((menu[i] + "\n").c_str());
	}
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();

		if (kDown & KEY_START) break; // break in order to return to hbmenu
		
		//Do the keys printing only if keys have changed
		if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
		{
			consoleClear();
			printf("Menu\n\n");
			for (int i=0; i<3; ++i)
			{
				if (i == pointer)
				{
					printf(("\x1b[31m" + menu[i] + "\x1b[0m\n").c_str());
				}
				else
				{
					printf((menu[i] + "\n").c_str());
				}
			}
			
			if (kDown & KEY_DUP)
			{
				pointer -= 1;
				if (pointer == -1)
				{
					pointer = 2;
				}
				//break;
			}
			else if (kDown & KEY_DDOWN)
			{
				pointer += 1;
				if(pointer == 3)
				{
					pointer = 0;
				}
			}
		}
		
		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

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