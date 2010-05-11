#import "ShorterWayController.h"
#import "core.h"
#import "io.h"
#import <time.h>

@implementation ShorterWayController

#pragma mark Graph generation

/*This function is called by the generate button*/
-(IBAction)generate:(id)sender {
	if ([numberOfNodes intValue]!=0) {
		/*We free all the previous graph matrix and precessor matrix*/
		[self freeAll];
		/*We get the values of the text fields and of the slider*/
		nbNodes = [numberOfNodes intValue];
		complet = [completeness floatValue];
		negat = [probabilityOfNegative floatValue];
		NSLog(@"%f",probabilityOfNegative);
		[self addLineWithPrompt:@"Generation of graph started"];
		/*We call the straight-C function that generates the graph*/
		start=clock();
		if(negativeAllowed) {
			NSLog(@"%f",negat);
			matrix = random_graph(nbNodes, complet, negat);
		}
		else matrix = random_graph(nbNodes, complet, 0); /* last parameter is the negative probability */
		end=clock();
		[self showTimeTakenBetween:start and:end];
		/*We display it*/
		[self displayMatrix:matrix withSize:nbNodes];
	}
	else [self displayWarning:@"You must generate a matrix with a size superior to zero"];
}

#pragma mark Bellman ford

-(IBAction)predFord:(id)sender {
	depFord = [departureFord intValue];
	if ([self validateDeparture:depFord] && [self graphNotEmpty]) {
		start=clock();
		predecessorFord = bellman_ford_matrix(matrix, nbNodes, depFord);
		end=clock();
		[self addLineWithPrompt:[NSString stringWithFormat:@"Predecessor matrix using Bellman-Ford with departure %d", depFord]];
		[self showTimeTakenBetween:start and:end];
		if (predecessorFord) {
			[self displayPredecessor:predecessorFord withSize:nbNodes];
		}
		else [self addLineWithoutPrompt:@"Negative cycles in the graph"];
	}
}

-(IBAction)solveFord:(id)sender {
	int arr;
	struct chained_list * path;
	arr=[arrivalFord intValue];
	/*We test to see if the predecessor matrix has already been generated*/
	if(predecessorFord) {
		if ([self validateArrival:arr]) {
			start=clock();
			path=find_short(predecessorFord, depFord, arr);
			end=clock();
			[self addLineWithPrompt:[NSString stringWithFormat:@"Shorter way using Bellman-Ford with departure %d and arrival %d", depFord, arr]];
			[self showTimeTakenBetween:start and:end];
			[self displayPath:path goingFrom:depFord];
		}
	}
	else [self displayWarning:@"You need to generate the predecessor matrix before"];
}
#pragma mark Dikjstra

-(IBAction)predDijkstra:(id)sender {
	depDijkstra = [departureDijkstra intValue];
	if ([self validateDeparture:depDijkstra] && [self graphNotEmpty]) {
		start=clock();
		predecessorDijkstra = dijkstra_matrix(matrix, nbNodes, depDijkstra);
		end=clock();
		[self addLineWithPrompt:[NSString stringWithFormat:@"Predecessor matrix using Dijkstra with departure %d", depDijkstra]];
		[self showTimeTakenBetween:start and:end];
		[self displayPredecessor:predecessorDijkstra withSize:nbNodes];
	}
}

-(IBAction)solveDijkstra:(id)sender {
	int arr;
	struct chained_list* path;
	arr = [arrivalDijkstra intValue];
	/*We test to see if the predecessor matrix has already been generated*/
	if (predecessorDijkstra) {
		if ([self validateArrival:arr]) {
			start = clock();
			path = find_short(predecessorDijkstra, depDijkstra, arr);
			end = clock();
			[self addLineWithPrompt:[NSString stringWithFormat:@"Shorter way using Dijkstra with departure %d and arrival %d", depDijkstra, arr]];
			[self showTimeTakenBetween:start and:end];
			[self displayPath:path goingFrom:depDijkstra];
		}
	}
	else [self displayWarning:@"You need to generate the predecessor matrix before"];
}

#pragma mark Dantzig

-(IBAction)predDantzig:(id)sender {
	if([self graphNotEmpty]) {
		start = clock();
		predecessorDantzig = dantzig_matrix(matrix, nbNodes);
		end = clock();
		[self addLineWithPrompt:[NSString stringWithFormat:@"Predecessor matrix using Dijkstra with departure %d", depDijkstra]];
		[self showTimeTakenBetween:start and:end];
		if (predecessorDantzig) {
			[self addLineWithoutPrompt:@"The predecessor matrix is not displayed, because it is too complicated!"];
			[self addLineWithoutPrompt:@"The predecessor matrix for one departure will be displayed during shorter way computation"];
		}
		else [self addLineWithoutPrompt:@"Negative cycles in the graph"];		
	}
}

-(IBAction)solveDantzig:(id)sender {
	int arr;
	int ** tempPred;
	struct chained_list * path;
	depDantzig = [departureDantzig intValue];
	arr = [arrivalDantzig intValue];
	/*We test to see if the predecessor matrix has already been generated*/
	if(predecessorDantzig) {
		if ([self validateDeparture:depDantzig] && [self validateArrival:arr]) {
			tempPred = dantzig_extract_predecessor(predecessorDantzig, depDantzig);
			[self addLineWithPrompt:[NSString stringWithFormat:@"Intermediary predecessor matrix using Dantzig with departure %d", depDantzig]];
			[self displayPredecessor:tempPred withSize:nbNodes];
			start = clock();
			path = find_short(tempPred, depDantzig, arr);
			end = clock();
			[self addLineWithPrompt:[NSString stringWithFormat:@"Shorter way using Dantzig with departure %d and arrival %d", depDantzig, arr]];
			[self showTimeTakenBetween:start and:end];
			[self displayPath:path goingFrom:depDijkstra];
		}
	}
	else [self displayWarning:@"You need to generate the predecessor matrix before"];
}

/*Display functions*/

/*Function to display the predecessors*/
-(void)displayPredecessor:(int **)mat withSize:(int)size {
	int i, j;
	NSMutableString * line;
	line = [NSMutableString stringWithString:@"N |P|W|"];
	[self addLineWithoutPrompt:line];
	for (i=0; i<size; i++) {
		line = [NSMutableString stringWithFormat:@"%d ",i];
		for (j=1; j>=0; j--) {
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

-(void)displayPath:(struct chained_list *)path goingFrom:(int)dep {
	int compteur;
	int distance;
	struct chained_list * cursor;
	NSMutableString * line;
	compteur = 0;
	cursor = path;
	if (path) {
		line = [NSMutableString stringWithFormat:@"%d ->",dep];
		while (cursor) {
			distance+=cursor->value;
			if (cursor->next) {
				[line appendFormat:@"%d ->", cursor->number];
			}
			else {
				[line appendFormat:@"%d", cursor->number];
			}
			
			compteur++;
			if (!compteur%10) [line appendFormat:@"\n"];
			/*At the end distance will be the value of the last node*/
			distance = cursor->value;
			cursor = cursor->next;
		}
		[self addLineWithoutPrompt:line];
		line = [NSMutableString stringWithFormat:@"Length of the path: %d", distance];
		[self addLineWithoutPrompt:line];
		/*We free the path*/
		free_list(path);
	}
	else {
		line = [NSMutableString stringWithString:@"No path can be found"];
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
-(void)showTimeTakenBetween:(clock_t)deb and:(clock_t)fin  {
	double cpu_time_used;
	cpu_time_used = ((double) (fin - deb)) / CLOCKS_PER_SEC;
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
	/*We free all the previous graph matrix and precessor matrix*/
	[self freeAll];
	/*We open the save panel dialog*/
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
		/*We display the loaded matrix*/
		[self addLineWithPrompt:@"Matrix imported"];
		[self displayMatrix:matrix withSize:nbNodes];
	}
}

#pragma mark stress test

-(IBAction)stressTest:(id)sender {
	/*Variables we will use*/
	int i;
	int ** graph;
	int deb;
	int fin;
	/*We get the entries of the user*/
	int method = [[radioButtons selectedCell] tag];
	int min = [sizeMin intValue];
	int max = [sizeMax intValue];
	int inc = [increment intValue];
	/*We open the filechooser*/
	NSFileHandle* file;
	NSSavePanel* saveDlg = [NSSavePanel savePanel];
	/*When the OK button is pressed we proccess the file*/
	if ( [saveDlg runModal] == NSOKButton )
	{
		/*We create the file*/
		[[NSFileManager defaultManager] createFileAtPath:[saveDlg filename]
												contents: nil attributes: nil];
		/*We open it for writing*/
		file = [NSFileHandle fileHandleForWritingAtPath:[saveDlg filename]];
	}
	
	/*We start the stress test*/
	for(i=min; i<=max; i+=inc) {
		graph = random_graph(i, 0.5, 0);
		switch (method) {
			case 0:
				deb = clock();
				bellman_ford_matrix(graph, i, 0);
				fin = clock();
				break;
			case 1:
				deb = clock();
				dijkstra_matrix(graph, i, 0);
				fin = clock();
				break;
			case 2:
				deb = clock();
				dantzig_matrix(graph, i);
				fin = clock();
				break;
			default:
				break;
		}
		
		/*We write the result in a file*/
		[file writeData:[[NSString stringWithFormat:@"%d, %f\n",i, ((double) (fin - deb))/CLOCKS_PER_SEC] dataUsingEncoding:NSASCIIStringEncoding]];
		/*We free the graph*/
		free_2Dmatrix(graph, i);
	}
	/*We close the file*/
	[file closeFile];
	
}

#pragma mark other

/*Function to free all the matrix*/
-(void)freeAll {
	if(matrix) { free_2Dmatrix(matrix, nbNodes); matrix=NULL;}
	if(predecessorFord) { free_2Dmatrix(predecessorFord, nbNodes); predecessorFord=NULL;}
	if(predecessorDijkstra) { free_2Dmatrix(predecessorDijkstra, nbNodes); predecessorDijkstra=NULL;}
	if(predecessorDantzig) { free_3Dmatrix(predecessorDantzig, nbNodes, nbNodes); predecessorDantzig=NULL;}
}

/*Function to validate departure and arrival*/
-(BOOL)validateDeparture:(int)dep {
	if(dep<nbNodes) {
		return TRUE;
	}
	else {
		[self displayWarning:@"The departure entered is not valid (value superior to the number of nodes)"];
		return FALSE;
	}
}

-(BOOL)validateArrival:(int)arr {
	if(arr<nbNodes) {
		return TRUE;
	}
	else {
		[self displayWarning:@"The arrival entered is not valid (value superior to the number of nodes)"];
		return FALSE;
	}
}

/*Function to be sure that a graph has been generated*/
-(BOOL)graphNotEmpty {
	if(!matrix) {
		[self displayWarning:@"No graph has been generated or imported"];
		return FALSE;
	}
	else return TRUE;
}

/*Function to disable the selection of the dijkstra tab when matrix with negative values are allowed*/
- (BOOL)tabView:(NSTabView *)tabView shouldSelectTabViewItem:(NSTabViewItem *)tabViewItem {
	if (![[tabViewItem label] compare:@"Dijkstra"] && negativeAllowed==TRUE) {
		return FALSE;
	}
	return TRUE;
}

/*Function called when the negative allowed checkbox is clicked*/
-(IBAction)negativeAllowedClicked:(id)sender {
	NSButton * stateButton = (NSButton *) sender;
	int state = [stateButton state];
	switch (state) {
		case 0:
			negativeAllowed = FALSE;
			break;
		default:
			negativeAllowed = TRUE;
			break;
	}
}

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
	negativeAllowed = FALSE;
}


@end