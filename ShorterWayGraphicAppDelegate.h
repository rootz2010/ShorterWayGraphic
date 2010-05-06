//
//  ShorterWayGraphicAppDelegate.h
//  ShorterWayGraphic
//
//  Created by Benjamin Loulier on 15/04/10.
//  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ShorterWayGraphicAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
