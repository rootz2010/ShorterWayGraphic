//
//  ShorterWayController.m
//  ShorterWayGraphic
//
//  Created by Benjamin Loulier on 15/04/10.
//  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
//

#import "ShorterWayController.h"
#import "core.h"

@implementation ShorterWayController

/*This function is called by the generate button*/
-(IBAction)generate:(id)sender {
	/*We free the previous matrix*/
	if(matrix!=NULL) {
		free_matrix(matrix, nbNodes);
		NSLog(@"Matrix freed");
	}
	/*We get the values of the text fields and of the slider*/
	nbNodes = [numberOfNodes intValue];
	complet = [completeness floatValue];
	[self addLineWithPrompt:@"Generation of graph started"];
	/*We call the straight-C function that generates the graph*/
	matrix = random_graph(nbNodes, complet);
	/*We display it*/
	[self displayMatrix:matrix withSize:nbNodes];
}

/*This function is called by the solve button*/
-(IBAction)solve:(id)sender {
	[self addLineWithPrompt:@"Looking for the shortest way"];
}

/*Display functions*/

/*Function to display the matrix of a graph*/
-(void)displayMatrix:(int **)mat withSize:(int)size {
	int i, j;
	NSMutableString * line;
	for (i=0; i<size; i++) {
		line = [NSMutableString stringWithString:@""];
		for (j=0; j<size; j++) {
			int value = mat[i][j];
			if(value == INT_MAX) {
				[line appendString:@"-- "];
			}
			else [line appendFormat:@"%-2d ", mat[i][j]];
		}
		[self addLineWithoutPrompt:line];
	}
}

/*Function to add a line with a prompt*/
-(void)addLineWithPrompt:(NSString *)text {
	[self addLine:[NSString stringWithFormat:@"> %@\n", text]];
}

/*Function to add a line without a prompt*/
-(void)addLineWithoutPrompt:(NSString *)text {
	[self addLine:[NSString stringWithFormat:@"  %@\n", text]];
}

-(void)addLine:(NSString *)text {
	NSAttributedString *string = [[NSAttributedString alloc] initWithString:text];
	
	NSTextStorage *storage = [result textStorage];
	
	[storage appendAttributedString:string];
	
	[storage setFont:[NSFont fontWithName:@"Courier" size:13.0]];
	
	//We scroll to the end so that we can view the whole text
	NSRange range;
	range = NSMakeRange ([[result string] length], 0);
	[result scrollRangeToVisible: range];
	
	[string release];
}

-(void)awakeFromNib {
	[[result enclosingScrollView] setHasHorizontalScroller:YES];
	[result setHorizontallyResizable:YES];
	[result setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
	[[result textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
	[[result textContainer] setWidthTracksTextView:NO];
}


@end