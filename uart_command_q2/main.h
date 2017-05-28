//////////////////////////////////////////////////////////////////////////////
// prog: main.h
// comm: external definitions and prototypes for main
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

//#define OUTPUT_DATA
//#define OUTPUT_DEBUG

#define WAIT_FOREVER             0
#define LOOP_DELAY               45

// mailboxhandle- and flaghandle-array id's: [0][1][2][3]...
enum
{
	MBDISTRIB = 0,
	MBLEDS,
	MBBUZZER,
	MBDISPLAY,
	MBOX_TOTAL, // for for-loops only

	FLAGDIST = 0, // reset
	FLAGOUT,
	FLAGEXECQ,
	FLAGS_TOTAL // for for-loops only
};


#define QSIZE        15

extern OS_EVENT    *QHndl;
extern OS_EVENT    *MbHndl[];
extern OS_FLAG_GRP *FlgHndl[];


// command structure, entered from uart
typedef struct Command
{
 	INT16U  command_nr;  // counter of incoming command
    char    device_type; // device indicator
    INT16U  value;       // what to do...
	char    text[17];    // display text
} C, *PC;

extern C     dqueue[];  // data queue
extern void* pqueue[]; // message queue
extern int   qsize;

extern void InitTask      (void *pdata);
extern void DisplayOSData (void);
extern void CreateHandles (void);
extern void displayAllStackData(void);

