#include <vita2d.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <psp2/ctrl.h>
#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/touch.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/shellutil.h> 
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/processmgr.h> 
#include <psp2/vshbridge.h>
#include <psp2/power.h>
#include <taihen.h>

#define PAD_PACKET_MODE     0
#define EXT_PAD_PACKET_MODE 1

#define MOD_PATH "ux0:app/VTETHER01/module/vividk.skprx"

// PadPacket struct
typedef struct {
	uint32_t buttons;
	uint8_t lx;
	uint8_t ly;
	uint8_t rx;
	uint8_t ry;
	uint16_t tx;
	uint16_t ty;
	uint8_t click;
} PadPacket;

// ExtPadPacket struct
typedef struct {
	SceCtrlData pad;
	SceTouchData front;
	SceTouchData retro;
} ExtPadPacket;

// Values for click value
#define NO_INPUT 0x00
#define MOUSE_MOV 0x01
#define LEFT_CLICK 0x08
#define RIGHT_CLICK 0x10

static uint8_t mode = EXT_PAD_PACKET_MODE;

// Server thread
volatile int connected = 0;
static int server_thread(unsigned int args, void* argp){
	
	// Initializing a PadPacket
	PadPacket pkg;
	ExtPadPacket ext_pkg;
	for (;;){
			connected = 1;
			for (;;){
				sceCtrlPeekBufferPositive(0, &ext_pkg.pad, 1);
				sceTouchPeek(SCE_TOUCH_PORT_FRONT, &ext_pkg.front, 1);
				sceTouchPeek(SCE_TOUCH_PORT_BACK, &ext_pkg.retro, 1);
				switch (mode) {
				case PAD_PACKET_MODE:
					memcpy(&pkg, &ext_pkg.pad.buttons, 8); // Buttons + analogs state
					memcpy(&pkg.tx, &ext_pkg.front.report[0].x, 4); // Touch state
					uint8_t flags = NO_INPUT;
					if (ext_pkg.front.reportNum > 0) flags += MOUSE_MOV;
					if (ext_pkg.retro.reportNum > 0){
						if (ext_pkg.retro.report[0].x > 960) flags += RIGHT_CLICK;
						else flags += LEFT_CLICK;
					}
					pkg.click = flags;
					break;
				case EXT_PAD_PACKET_MODE:
					break;
				default:
					break;
				}
			}
	}
	
	return 0;
}

vita2d_pgf* debug_font = NULL;
uint32_t text_color = 0x00;
int lockPsButton = 1;
int lockQuickMenu = 1;
int lockUsbConnect = 1;
int main(int argc, char *argv[]){
	int search_param[2];
  	SceUID res = _vshKernelSearchModuleByName("vividk", search_param);
	if (res <= 0)
  	{
	SceUID mod_id;
    mod_id = taiLoadStartKernelModule(MOD_PATH, 0, NULL, 0);
    sceClibPrintf("0x%08x\n", mod_id);
    sceKernelDelayThread(1000000);
    sceAppMgrLoadExec("app0:eboot.bin", NULL, NULL);
 	}

	// Initializing graphics stuffs
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	debug_font = vita2d_load_default_pgf();
	uint32_t text_color = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
	uint32_t error_text_color = RGBA8(0xFF, 0x00, 0x00, 0xFF);
	int model = sceKernelGetModel();
	if (sceKernelGetModel() == SCE_KERNEL_MODEL_VITATV)
	{
	    vita2d_start_drawing();
		vita2d_clear_screen();
		vita2d_pgf_draw_text(debug_font, 2, 40, error_text_color, 2.0, "Fatal error: This app cannot run on the VITA TV. Exiting in 3 seconds...");
		vita2d_end_drawing();
		vita2d_wait_rendering_done();
		vita2d_swap_buffers();
		sceKernelDelayThread(3000000);  // In microseconds
		sceKernelExitProcess(1);
		return 1;
	}

	// Reduce CPU and GPU frequency to save battery
  	scePowerSetArmClockFrequency(41);
  	scePowerSetBusClockFrequency(55);
  	scePowerSetGpuClockFrequency(41);
  	scePowerSetGpuXbarClockFrequency(83);

	// Enabling analog and touch support
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);


	// Signal the kernel here to start doing stuff
	
	// Lock the PS Button and the quick menu

	//sceShellUtilInitEvents(0);
	//lockPsButton = sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN_2);
	//lockQuickMenu = sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_QUICK_MENU);
	//lockUsbConnect = sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION);
	//vita2d_pgf_draw_text(debug_font, 2, 40, lockUsbConnect ? error_text_color : text_color, 1.0, lockPsButton ? "Failed to lock the homebutton." : "The homebutton is now locked.");
	//vita2d_pgf_draw_text(debug_font, 2, 60, lockQuickMenu ? error_text_color : text_color, 1.0, lockQuickMenu ? "Failed to lock the quick menu." : "The quick menu is now locked.");
	//vita2d_pgf_draw_text(debug_font, 2, 80, lockUsbConnect ? error_text_color : text_color, 1.0, lockUsbConnect ? "Failed to lock USB connections." : "The USB connection is now locked.");

	for (;;){
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		vita2d_pgf_draw_text(debug_font, 2, 20, text_color, 1.0, "VitaTether v.0.1 by TroyWarez");
		vita2d_pgf_draw_text(debug_font, 2, 20, text_color, 1.0, "Waiting for sync");
		vita2d_end_drawing();
		vita2d_wait_rendering_done();
		vita2d_swap_buffers();
	}
	sceKernelExitProcess(0);
	return 0;
}