//
//  ShorterWayController.h
//  ShorterWayGraphic
//
//  Created by Benjamin Loulier on 15/04/10.
//  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface ShorterWayController : NSObject {
	
	/*Variables to handle the current graph*/
	int nbNodes;
	float complet;
	int ** matrix;
	
	/*User entries*/
	IBOutlet NSTextField * numberOfNodes;
	IBOutlet NSTextField * departure;
	IBOutlet NSTextField * arrival;
	IBOutlet NSSlider * completeness;
	
	/*Output*/
	IBOutlet NSTextView * result;

}

/*Text display functions*/
-(void)displayMatrix:(int **)matrix withSize:(int)size;
-(void)addLineWithPrompt:(NSString *)text;
-(void)addLineWithoutPrompt:(NSString *)text;
-(void)addLine:(NSString *)text;


-(IBAction)generate:(id)sender;
-(IBAction)solve:(id)sender;

@end
