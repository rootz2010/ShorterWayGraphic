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
	float negat;
	int ** matrix;
	
	/*Variable to retain the chosen departure*/
	int depFord;
	int depDijkstra;
	int depDantzig;
	
	/*Predecessor for the different algorithms*/
	int ** predecessorFord;
	int ** predecessorDijkstra;
	int *** predecessorDantzig;
	
	int ** predecessor;
	
	/*User entries Graph*/
	IBOutlet NSTextField * numberOfNodes;
	IBOutlet NSSlider * completeness;
	BOOL negativeAllowed;
	IBOutlet NSSlider * probabilityOfNegative;
	
	IBOutlet NSTextField * departureFord;
	IBOutlet NSTextField * arrivalFord;
	IBOutlet NSTextField * departureDijkstra;
	IBOutlet NSTextField * arrivalDijkstra;
	IBOutlet NSTextField * departureDantzig;
	IBOutlet NSTextField * arrivalDantzig;
	
	/*Output*/
	IBOutlet NSTextView * result;
	
	/*Time measurment*/
	clock_t start, end;
	
	/*Stress test*/
	IBOutlet NSMatrix * radioButtons;
	IBOutlet NSTextField * sizeMin;
	IBOutlet NSTextField * sizeMax;
	IBOutlet NSTextField * increment;
	
}

/*Text display functions*/
-(void)displayMatrix:(int **)matrix withSize:(int)size;
-(void)displayPredecessor:(int **)mat withSize:(int)size;
-(void)displayPath:(struct chained_list *)path goingFrom:(int)departure;
//-(void)displayPathDantzig:(struct chained_list *)path goingFrom:(int)departure;
-(void)displayWarning:(NSString *)message;
-(void)addLineWithPrompt:(NSString *)text;
-(void)addLineWithoutPrompt:(NSString *)text;
-(void)addLine:(NSString *)text;


-(IBAction)generate:(id)sender;

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

/*Negative allowed Checkbox*/
-(IBAction)negativeAllowedClicked:(id)sender;

/*Stress test*/
-(IBAction)stressTest:(id)sender;

/*Time measurment*/
-(void)showTimeTakenBetween:(clock_t)start and:(clock_t)end;

/*Function to validate departure and arrival*/
-(BOOL)validateDeparture:(int)dep;
-(BOOL)validateArrival:(int)arr;

/*Function to see if a grpah has been generated or imported and if not display a warning*/
-(BOOL)graphNotEmpty;

/*utilities*/
-(void)freeAll;

@end
