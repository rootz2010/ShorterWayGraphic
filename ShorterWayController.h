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
	
	/*Predecessor for the different algorithms*/
	int ** predecessorFord;
	int ** predecessorDijkstra;
	int ** predecessorDantzig;
	
	int ** predecessor;
	
	/*User entries Graph*/
	IBOutlet NSTextField * numberOfNodes;
	
	/*User entries computation*/
	IBOutlet NSTextField * departure;
	IBOutlet NSTextField * arrival;
	IBOutlet NSSlider * completeness;
	
	IBOutlet NSTextField * departureFord;
	IBOutlet NSTextField * arrivalFord;
	IBOutlet NSTextField * departureDijkstra;
	IBOutlet NSTextField * arrivalDijkstra;
	IBOutlet NSTextField * departureDantzig;
	IBOutlet NSTextField * arrivalDantzig;
	
	
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

/*IO*/
-(IBAction)import:(id)sender;
-(IBAction)export:(id)sender;

/*Actions buttons algorithm*/
-(IBAction)predFord:(id)sender;
-(IBAction)solveFord:(id)sender;
-(IBAction)predDijkstra:(id)sender;
-(IBAction)solveDijkstra:(id)sender;
-(IBAction)predDantzig:(id)sender;
-(IBAction)solveDantzig:(id)sender;

@end
