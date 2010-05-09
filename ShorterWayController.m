//
//  ShorterWayController.m
//  ShorterWayGraphic
//
//  Created by Benjamin Loulier on 15/04/10.
//  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
//

#import "ShorterWayController.h"
#import "core.h"
#import "io.h"
#import <time.h>

@implementation ShorterWayController

#pragma mark Graph generation

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
	start=clock();
	matrix = random_graph(nbNodes, complet);
	end=clock();
	[self showTimeTakenBetween:start and:end];
	/*We display it*/
	[self displayMatrix:matrix withSize:nbNodes];
}

#pragma mark Bellman ford

-(IBAction)predFord:(id)sender {
	int departure;
	departure = [departureFord intValue];
	start=clock();
	predecessorFord = bellman_ford_matrix(matrix, nbNodes, departure);
	end=clock();
	[self addLineWithPrompt:[NSString stringWithFormat:@"Predecessor matrix using Bellman-Ford with departure %d", departure]];
	[self showTimeTakenBetween:start and:end];
	[self displayPredecessor:predecessorFord withSize:nbNodes];
}

#pragma mark Dikjstra

-(IBAction)predDijkstra:(id)sender {
	int departure;
	departure = [departureDijkstra intValue];
	start=clock();
	predecessorDijkstra = dijkstra_matrix(matrix, nbNodes, departure);
	end=clock();
	[self addLineWithPrompt:[NSString stringWithFormat:@"Predecessor matrix using Dijkstra with departure %d", departure]];
	[self showTimeTakenBetween:start and:end];
	[self displayPredecessor:predecessorDijkstra withSize:nbNodes];
}

/*This function is called by the solve button*/
-(IBAction)solve:(id)sender {
	
	[self addLineWithPrompt:@"Looking for the shortest way"];
	
	[self addLineWithPrompt:@"Looking for the shortest way bellman"];
	predecessor = bellman_ford_matrix(matrix, nbNodes, 0);
	[self displayPredecessor:predecessor withSize:nbNodes];
	
	[self addLineWithPrompt:@"Looking for the shortest way dijkstra"];
	predecessor = dijkstra_matrix(matrix, nbNodes, 0);
	if(predecessor!=NULL) {
		NSLog(@"not null");
	}
	[self displayPredecessor:predecessor withSize:nbNodes];
	
	[self addLineWithPrompt:@"Looking for the shortest way Dantzig"];
	int *** mouche = dantzig_matrix(matrix, nbNodes);
	[self displayDantzigMatrix:mouche withSize:nbNodes];
}

/*Display functions*/

/*Function to display the predecessors*/
-(void)displayPredecessor:(int **)mat withSize:(int)size {
	int i, j;
	NSMutableString * line;
	for (i=0; i<size; i++) {
		line = [NSMutableString stringWithString:@""];
		for (j=0; j<2; j++) {
			int value = mat[i][j];
			if(value == INT_MAX) {
				[line appendString:@"-- "];
			}
			else [line appendFormat:@"%-2d ", mat[i][j]];
		}
		[self addLineWithoutPrompt:line];
	}
}

	 
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

/* function to display the matrix resulted */
-(void)displayDantzigMatrix:(int ***)mat withSize:(int)size {
	int i,j;
	NSMutableString * line;
	for (i=0; i<size; i++) {
		line = [NSMutableString stringWithString:@""];
		for (j=0; j<size; j++) {
			[line appendFormat:@"%-2d ", mat[i][j][1]];
		}
		[self addLineWithoutPrompt:line];
		line = [NSMutableString stringWithString:@""];
		for (j=0; j<size; j++) {
			int value = mat[i][j][0];
			if (value == INT_MAX) {
				[line appendString:@"-- "];
			}
			else [line appendFormat:@"%-2d ", value];
		}
		[self addLineWithoutPrompt:line];
	}
}

/*Function to add a line with a prompt*/
-(void)showTimeTakenBetween:(clock_t)start and:(clock_t)end  {
	double cpu_time_used;
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	[self addLineWithPrompt:[NSString stringWithFormat:@"Time taken for the operation: %f s", cpu_time_used]];
}

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

#pragma mark I/O
-(IBAction)export:(id)sender {
	
	if (matrix==NULL) {
		[self displayWarning:@"No matrix has been generated, so no matrix can be saved"];
	}
	else {
		/*We open the filechooser*/
		NSSavePanel* saveDlg = [NSSavePanel savePanel];
		
		/*When the OK button is pressed we proccess the file*/
		if ( [saveDlg runModal] == NSOKButton )
		{
			/*We get the path of the file*/
			const char * file = [[saveDlg filename] UTF8String];
			write_matrix(file, matrix, nbNodes);
		}
	}
}

-(IBAction)import:(id)sender {
	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
	[openDlg setCanChooseFiles:YES];
	[openDlg setAllowsMultipleSelection:NO];
	[openDlg setCanChooseDirectories:NO];
	
	/*When the OK button is pressed we proccess the file*/
	if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
	{
		/*We get the path of the file*/
		const char * file = [[[openDlg filenames] objectAtIndex:0] UTF8String];
		nbNodes = read_nb_nodes(file);
		matrix = read_matrix(file, nbNodes);
	}
	/*We display the loaded matrix*/
	[self addLineWithPrompt:@"Matrix imported"];
	[self displayMatrix:matrix withSize:nbNodes];
}

/******Other****/

-(void)displayWarning:(NSString *)message {
	NSAlert * alert = [NSAlert alertWithMessageText:message
									  defaultButton:nil alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
	[alert setAlertStyle:NSCriticalAlertStyle];
	[alert runModal];
}

-(void)awakeFromNib {
	[[result enclosingScrollView] setHasHorizontalScroller:YES];
	[result setHorizontallyResizable:YES];
	[result setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
	[[result textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
	[[result textContainer] setWidthTracksTextView:NO];
}


@end