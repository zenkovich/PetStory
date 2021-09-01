#import <Cocoa/Cocoa.h>
#import "o2/Application/Mac/AppDelegate.h"

int main(int argc, const char * argv[]) {
	@autoreleasepool {
		[NSApplication sharedApplication];
		[NSApp setDelegate:[[AppDelegate alloc] init]];
		[NSApp run];
		
//		[NSApplication sharedApplication];
//		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
//		id applicationName = [[NSProcessInfo processInfo] processName];
//		id window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 1200, 1200)
//												styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO];
//		[window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
//		[window setTitle: applicationName];
//		[window makeKeyAndOrderFront:nil];
//		[NSApp activateIgnoringOtherApps:YES];
//		
//		id applicationMenuBar = [NSMenu new];
//		id appMenuItem        = [NSMenuItem new];
//		[appMenuItem setTitle:@"Hello"];
//		[applicationMenuBar addItem:appMenuItem];
//		[NSApp setMainMenu: applicationMenuBar];
//		
//		[NSApp run];
	}
	return 0;
}
