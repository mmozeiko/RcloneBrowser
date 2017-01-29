#include "osx_helper.h"
#include <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

QIcon osxGetIcon(const QString& extension)
{
    QIcon icon;
    @autoreleasepool
    {
        NSImage* image = [[NSWorkspace sharedWorkspace] iconForFileType:extension.toNSString()];
        NSRect rect = NSMakeRect(0, 0, image.size.width, image.size.height);
        CGImageRef imageRef = [image CGImageForProposedRect:&rect context:NULL hints:nil];
        if (imageRef)
        {
            icon = QtMac::fromCGImageRef(imageRef);
        }
    }
    return icon;
}

void osxShowDockIcon()
{
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
}

void osxHideDockIcon()
{
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, kProcessTransformToUIElementApplication);
}
