#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: SLIGHTLY MODIFIED
 FROM VERSION 1.1 of J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
       are other packets in the channel for GBN), but can be larger
   - frames can be corrupted (either the header or the data portion)
       or lost, according to user-defined probabilities
   - frames will be delivered in the order in which they were sent
       (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 1 /* change to 1 if you're doing extra credit */
/* and write a routine called B_output */

/* a "pkt" is the data unit passed from layer 3 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 3 via the students transport level protocol entities.         */
struct pkt
{
    char data[20];
};

/* a frame is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined frame structure, which all   */
/* students must follow. */
#define HARD_CODED_PARAMETER_ACTIVATION false
#define HARD_CODED_NUMBER_OF_PACKETS 10
#define HARD_CODED_LOSS_PROBABILITY 0.1
#define HARD_CODED_CORRUPTION_PROBABILITY 0.3
#define HARD_CODED_DELAY 1000
#define HARD_CODED_TRACE 2
#define HARD_CODED_PIGGY_ENABLE true

#define ENABLE_MAHATHIR_LOG false

#define TYPE_DATA 0
#define TYPE_ACK 1
#define TYPE_PIGGY 2
#define TYPE_DONT_CARE 3

struct frm
{
    int seqnum;
    int acknum;
    int checksum;
    char payload[20];
    int type;
};

/********* FUNCTION PROTOTYPES. DEFINED IN THE LATER PART******************/
void starttimer(int AorB, float increment);
void stoptimer(int AorB);
void tolayer1(int AorB, struct frm frame);
void tolayer3(int AorB, char datasent[20]);

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 3, passed the data to be sent to other side */



#define device_A 0
#define device_B 1
#define timer_duration 500

//variables for A
bool is_A_waiting_for_ack = false;
int sending_seq_of_A = 0;
int receiving_seq_of_B = 0;
struct frm last_sent_by_A;
bool outstanding_ack_of_A = false;
int outstanding_ack_seq_of_A = 0;

int frame_count_A=0;
int number_of_frames_received_A = 0;
int dropped_frame_A_output = 0;

//variables for B
bool is_B_waiting_for_ack = false;
int sending_seq_of_B = 0;
int receiving_seq_of_A = 0;
struct frm last_sent_by_B;
bool outstanding_ack_of_B = false;
int outstanding_ack_seq_of_B = 0;

int frame_count_B = 0;
int number_of_frames_received_B = 0;
int dropped_frame_B_output = 0;

//control variables
bool piggy_backing_enable;

int get_checksum(struct frm frame)
{
    int checksum = 0;
    checksum+= (frame.seqnum+frame.acknum);

    int i=0;
    for(i=0; i<20; i++)
    {
        checksum+=(int)frame.payload[i];
    }

    return checksum;

}

void drop_frame(int device)
{
    if(device == device_A)
    {
        printf("device A: dropped frame in device A\n");
    }
    else
    {
        printf("device B: dropped frame in device B\n");
    }
}

void print_frame(int device, struct frm frame)
{
    if(ENABLE_MAHATHIR_LOG)
    {
        printf("_______frame INFO:_______\n");
        if(frame.seqnum == TYPE_DONT_CARE)
        {
            printf("seqnum= DONT_CARE\n");
        }
        else
        {
            printf("seqnum= %d\n",frame.seqnum);
        }

        printf("acknum= %d\n",frame.acknum);
        printf("ckecksum= %d\n",frame.checksum);
        printf("payload= %s\n",frame.payload);

        if(frame.type== TYPE_ACK)
        {
            printf("type= ACK\n");
        }
        else if(frame.type == TYPE_DATA)
        {
            printf("type= DATA\n");
        }
        else if(frame.type == TYPE_PIGGY)
        {
            printf("type= PIGGY\n");
        }
        else
        {
            printf("type= ERROR");
        }

        if(device == device_A)
        {
            printf("device= A\n");
        }
        else
        {
            printf("device= B\n");
        }

        printf("__________________________\n");
    }

}

void print_packet(int AorB, struct pkt packet)
{
    if(ENABLE_MAHATHIR_LOG)
    {
        printf("_________packet:_________\n");
        printf("data= %s\n",packet.data);
        if(AorB == device_A)
        {
            printf("device= A\n");
        }
        else
        {
            printf("device= B\n");
        }
        printf("__________________________\n");
    }

}

void send_ack_to_layer_1(int device, int seq)
{
    struct frm ackframe;
    ackframe.acknum = seq;
    strcpy(ackframe.payload, "TYPE_DONT_CARE");
    ackframe.seqnum = TYPE_DONT_CARE;
    ackframe.type= TYPE_ACK;
    ackframe.checksum = get_checksum(ackframe);

    if(device==device_A)
    {
        printf("device A: sending ack frame\n");
    }
    else
    {
        printf("device B: sending ack frame\n");
    }
    print_frame(device, ackframe);
    tolayer1(device, ackframe);
}

void A_output(struct pkt packet)
{
    printf("\ndevice A: inside A_output function\n");
    frame_count_A++;
    print_packet(device_A, packet);

    if(is_A_waiting_for_ack)
    {
        printf("device A: new frame but A is waiting for acknowledgment\n");
        dropped_frame_A_output++;
        drop_frame(device_A);
        return;
    }

    struct frm sending_frame;

    if(outstanding_ack_of_A)
    {
        printf("device A: outstanding ack found\n");
        sending_frame.acknum = outstanding_ack_seq_of_A;
        sending_frame.type = TYPE_PIGGY;
        strcpy(sending_frame.payload,packet.data);
        sending_frame.seqnum = sending_seq_of_A;
        sending_frame.checksum = get_checksum(sending_frame);
        printf("device A: piggy frame created\n");
    }
    else
    {
        sending_frame.acknum = TYPE_DONT_CARE;
        sending_frame.type = TYPE_DATA;
        strcpy(sending_frame.payload,packet.data);
        sending_frame.seqnum = sending_seq_of_A;
        sending_frame.checksum = get_checksum(sending_frame);
        printf("device A: data frame created\n");
    }



    is_A_waiting_for_ack=true;

    printf("device A: sending frame to layer 1\n");
    print_frame(device_A,sending_frame);
    last_sent_by_A = sending_frame;
    tolayer1(device_A, sending_frame);


    printf("device A: started timer for re-transmission if it goes off\n");
    starttimer(device_A,timer_duration);

}

/* need be completed only for extra credit */
void B_output(struct pkt packet)
{
    printf("\ndevice B: inside B_output function\n");
    frame_count_B++;
    print_packet(device_B, packet);

    if(is_B_waiting_for_ack)
    {
        printf("device B: new frame but B is waiting for acknowledgment\n");
        dropped_frame_B_output++;
        drop_frame(device_B);
        return;
    }

    struct frm sending_frame;

    if(outstanding_ack_of_B)
    {
        printf("device B: outstanding ack found\n");
        sending_frame.acknum = outstanding_ack_seq_of_B;
        sending_frame.type = TYPE_PIGGY;
        strcpy(sending_frame.payload,packet.data);
        sending_frame.seqnum = sending_seq_of_B;
        sending_frame.checksum = get_checksum(sending_frame);
        printf("device B: piggy frame created\n");
    }
    else
    {
        sending_frame.acknum = TYPE_DONT_CARE;
        sending_frame.type = TYPE_DATA;
        strcpy(sending_frame.payload,packet.data);
        sending_frame.seqnum = sending_seq_of_B;
        sending_frame.checksum = get_checksum(sending_frame);
        printf("device B: data frame created\n");
    }



    is_B_waiting_for_ack=true;

    printf("device B: sending frame to layer 1\n");
    print_frame(device_B,sending_frame);
    last_sent_by_B = sending_frame;
    tolayer1(device_B, sending_frame);


    printf("device B: started timer for re-transmission if it goes off\n");
    starttimer(device_B,timer_duration);

}

void A_input(struct frm frame)
{
    printf("device A: inside A_input\n");
    print_frame(device_A,frame);

    if(frame.checksum!=get_checksum(frame))
    {
        printf("device A: corrupted frame received\n");

        if(is_A_waiting_for_ack)
        {
            printf("device A: resending last sent frame\n");
            print_frame(device_A,last_sent_by_A);
            tolayer1(device_A,last_sent_by_A);

            printf("device A: resetting timer\n");
            stoptimer(device_A);
            starttimer(device_A,timer_duration);

            return;
        }
        else
        {
            struct frm negative_ack;
            negative_ack.acknum = 1-receiving_seq_of_A;
            negative_ack.seqnum = TYPE_DONT_CARE;
            negative_ack.type= TYPE_ACK;
            strcpy(negative_ack.payload, frame.payload);
            negative_ack.checksum = get_checksum(negative_ack);

            printf("device A: sending negative ack to layer 1\n");
            print_frame(device_A,negative_ack);
            tolayer1(device_A, negative_ack);

            return;
        }
    }

    if(frame.type==TYPE_DATA || frame.type == TYPE_PIGGY)
    {
        if(frame.seqnum == receiving_seq_of_A)
        {
            if(piggy_backing_enable)
            {
                printf("device A: piggy backing enabled\n");
                printf("device A: saving incoming frame sequence for sending later for ack\n");
                outstanding_ack_of_A = true;
                outstanding_ack_seq_of_A = frame.seqnum;

                printf("device A: started timer. ack will be sent as TYPE_ACK if off\n");
                starttimer(device_A,timer_duration);

            }
            else
            {
                printf("device A: piggy backing disabled\n");
                send_ack_to_layer_1(device_A, frame.seqnum);
            }

            printf("device A: sending payload %s to layer 3\n",frame.payload);
            tolayer3(device_A,frame.payload);
            receiving_seq_of_A = 1 - receiving_seq_of_A;
            number_of_frames_received_A++;
        }
        else
        {
            printf("device A: duplicate frame- sequence received\n");

            if(piggy_backing_enable)
            {
                printf("device A: saving incoming frame sequence for sending later for ack\n");
                outstanding_ack_of_A = true;
                outstanding_ack_seq_of_A = frame.seqnum;

                printf("device A: started timer. ack will be sent as TYPE_ACK if off\n");
                starttimer(device_A,timer_duration);
            }
            else
            {
                printf("device A: piggy backing disabled\n");
                send_ack_to_layer_1(device_A, frame.seqnum);
                return;
            }
        }

    }

    if(frame.type==TYPE_ACK || frame.type==TYPE_PIGGY)
    {
        printf("device A: received acknowledgment\n");

        if(!is_A_waiting_for_ack)
        {
            printf("device A: A wasn't expecting any acknowledgment\n");
            drop_frame(device_A);
            return;
        }

        if(sending_seq_of_A != frame.acknum)
        {
            printf("device A: sequence of acknowledgment is not the one expected\n");

            printf("device A: resending last sent frame\n");
            print_frame(device_A,last_sent_by_A);
            tolayer1(device_A,last_sent_by_A);

            printf("device A: started timer\n");
            starttimer(device_A,timer_duration);
        }
        else
        {
            printf("device A: proper acknowledgment received\n");

            printf("device A: changing sending sequence\n");
            sending_seq_of_A = 1- sending_seq_of_A;

            printf("device A: stopping timer\n");
            stoptimer(device_A);

            printf("device A: stop waiting for acknowledgment\n");
            is_A_waiting_for_ack = false;
        }

    }


}

void B_input(struct frm frame)
{
    printf("device B: inside B_input\n");
    print_frame(device_B,frame);

    if(frame.checksum!=get_checksum(frame))
    {
        printf("device B: corrupted frame received\n");

        if(is_B_waiting_for_ack)
        {
            printf("device B: resending last sent frame\n");
            print_frame(device_B,last_sent_by_B);
            tolayer1(device_B,last_sent_by_B);

            printf("device B: resetting timer\n");
            stoptimer(device_B);
            starttimer(device_B,timer_duration);

            return;
        }
        else
        {
            struct frm negative_ack;
            negative_ack.acknum = 1-receiving_seq_of_B;
            negative_ack.seqnum = TYPE_DONT_CARE;
            negative_ack.type= TYPE_ACK;
            strcpy(negative_ack.payload, frame.payload);
            negative_ack.checksum = get_checksum(negative_ack);

            printf("device B: sending negative ack to layer 1\n");
            print_frame(device_B,negative_ack);
            tolayer1(device_B, negative_ack);


            return;
        }
    }

    if(frame.type==TYPE_DATA || frame.type == TYPE_PIGGY)
    {
        if(frame.seqnum == receiving_seq_of_B)
        {
            if(piggy_backing_enable)
            {
                printf("device B: saving incoming frame sequence for sending later for ack\n");
                outstanding_ack_of_B = true;
                outstanding_ack_seq_of_B = frame.seqnum;

                printf("device B: started timer. ack will be sent as TYPE_ACK if off\n");
                starttimer(device_B,timer_duration);
            }
            else
            {

                printf("device B: piggy backing disabled\n");
                send_ack_to_layer_1(device_B, frame.seqnum);
            }
            printf("device B: sending payload %s to layer 3\n",frame.payload);
            tolayer3(device_B,frame.payload);
            receiving_seq_of_B = 1 - receiving_seq_of_B;
            number_of_frames_received_B++;
        }
        else
        {
            printf("device B: duplicate frame- sequence received\n");


            if(piggy_backing_enable)
            {
                printf("device B: saving incoming frame sequence for sending later for ack\n");
                outstanding_ack_of_B = true;
                outstanding_ack_seq_of_B = frame.seqnum;

                printf("device B: started timer. ack will be sent as TYPE_ACK if off\n");
                starttimer(device_B,timer_duration);
            }
            else
            {
                printf("device B: piggy backing disabled\n");
                send_ack_to_layer_1(device_B, frame.seqnum);
                return;
            }
        }

    }

    if(frame.type==TYPE_ACK || frame.type==TYPE_PIGGY)
    {
        printf("device B: received acknowledgment\n");

        if(!is_B_waiting_for_ack)
        {
            printf("device B: B wasn't expecting any acknowledgment\n");
            drop_frame(device_B);
            return;
        }

        if(sending_seq_of_B != frame.acknum)
        {
            printf("device B: sequence of acknowledgment is not the one expected\n");

            printf("device B: resending last sent frame\n");
            print_frame(device_B,last_sent_by_B);
            tolayer1(device_B,last_sent_by_B);

            printf("device B: started timer\n");
            starttimer(device_B,timer_duration);
        }
        else
        {
            printf("device B: proper acknowledgment received\n");

            printf("device B: changing sending sequence\n");
            sending_seq_of_B = 1- sending_seq_of_B;

            printf("device B: stopping timer\n");
            stoptimer(device_B);

            printf("device B: stop waiting for acknowledgment\n");
            is_B_waiting_for_ack = false;
        }

    }

}


/* called when A's timer goes off */
void A_timerinterrupt(void)
{
    printf("device A: inside A_timerinterrupt\n");

    if(outstanding_ack_of_A==true)
    {
        printf("device A: outstanding acknowledgment found\n");

        outstanding_ack_of_A=false;

        send_ack_to_layer_1(device_A, outstanding_ack_seq_of_A);

        return;
    }

    if(!is_A_waiting_for_ack)
    {
        printf("device A: not waiting for acknowledgment\n");
        return;
    }
    else
    {
        printf("device A: resending last sent frame again\n");
        print_frame(device_A,last_sent_by_A);
        tolayer1(device_A,last_sent_by_A);

        printf("device A: started timer\n");
        starttimer(device_A, timer_duration);
    }

}

/* called when B's timer goes off */
void B_timerinterrupt(void)
{
    printf("device B: inside B_timerinterrupt\n");

    if(outstanding_ack_of_B==true)
    {
        printf("device B: outstanding acknowledgment found\n");

        outstanding_ack_of_B=false;

        send_ack_to_layer_1(device_B, outstanding_ack_seq_of_B);

        return;
    }

    if(!is_B_waiting_for_ack)
    {
        printf("device B: not waiting for acknowledgment\n");
        return;
    }
    else
    {
        printf("device B: resending last sent frame again\n");
        print_frame(device_B,last_sent_by_B);
        tolayer1(device_B,last_sent_by_B);

        printf("device B: started timer\n");
        starttimer(device_B, timer_duration);
    }
}



/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void)
{

}

/* Note that with simplex transfer from a-to-B, there is no B_output() */





/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void)
{

}

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 1 and below network environment:
    - emulates the tranmission and delivery (possibly with bit-level corruption
        and frame loss) of frames across the layer 1/4 interface
    - handles the starting/stopping of a timer, and generates timer
        interrupts (resulting in calling students timer handler).
    - generates packet to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

struct event
{
    float evtime;       /* event time */
    int evtype;         /* event type code */
    int eventity;       /* entity where event occurs */
    struct frm *frmptr; /* ptr to frame (if any) assoc w/ this event */
    struct event *prev;
    struct event *next;
};
struct event *evlist = NULL; /* the event list */

/* possible events: */
#define TIMER_INTERRUPT 0
#define FROM_layer3 1
#define FROM_layer1 2

#define OFF 0
#define ON 1
#define A 0
#define B 1

int TRACE = 1;     /* for my debugging */
int nsim = 0;      /* number of packets from 5 to 4 so far */
int nsimmax = 0;   /* number of pkts to generate, then stop */
float time = 0.000;
float lossprob;    /* probability that a frame is dropped  */
float corruptprob; /* probability that one bit is frame is flipped */
float lambda;      /* arrival rate of packets from layer 3 */
int ntolayer1;     /* number sent into layer 1 */
int nlost;         /* number lost in media */
int ncorrupt;      /* number corrupted by media*/

void init();
void generate_next_arrival(void);
void insertevent(struct event *p);

int main()
{
    struct event *eventptr;
    struct pkt pkt2give;
    struct frm frm2give;

    int i, j;
    char c;

    init();
    A_init();
    B_init();

    while (1)
    {
        eventptr = evlist; /* get next event to simulate */
        if (eventptr == NULL)
            goto terminate;
        evlist = evlist->next; /* remove this event from event list */
        if (evlist != NULL)
            evlist->prev = NULL;
        if (TRACE >= 2)
        {
            printf("\nEVENT time: %f,", eventptr->evtime);
            printf("  type: %d", eventptr->evtype);
            if (eventptr->evtype == 0)
                printf(", timerinterrupt  ");
            else if (eventptr->evtype == 1)
                printf(", fromlayer3 ");
            else
                printf(", fromlayer1 ");
            printf(" entity: %d\n", eventptr->eventity);
        }
        time = eventptr->evtime; /* update time to next event time */
        if (eventptr->evtype == FROM_layer3)
        {
            if (nsim < nsimmax)
            {
                if (nsim + 1 < nsimmax)
                    generate_next_arrival(); /* set up future arrival */
                /* fill in pkt to give with string of same letter */
                j = nsim % 26;
                for (i = 0; i < 20; i++)
                    pkt2give.data[i] = 97 + j;
                pkt2give.data[19] = 0;
                if (TRACE > 2)
                {
                    printf("          MAINLOOP: data given to student: ");
                    for (i = 0; i < 20; i++)
                        printf("%c", pkt2give.data[i]);
                    printf("\n");
                }
                nsim++;
                if (eventptr->eventity == A)
                    A_output(pkt2give);
                else
                    B_output(pkt2give);
            }
        }
        else if (eventptr->evtype == FROM_layer1)
        {
            frm2give.seqnum = eventptr->frmptr->seqnum;
            frm2give.acknum = eventptr->frmptr->acknum;
            frm2give.checksum = eventptr->frmptr->checksum;
            frm2give.type = eventptr->frmptr->type;
            for (i = 0; i < 20; i++)
                frm2give.payload[i] = eventptr->frmptr->payload[i];
            if (eventptr->eventity == A) /* deliver frame by calling */
                A_input(frm2give); /* appropriate entity */
            else
                B_input(frm2give);
            free(eventptr->frmptr); /* free the memory for frame */
        }
        else if (eventptr->evtype == TIMER_INTERRUPT)
        {
            if (eventptr->eventity == A)
                A_timerinterrupt();
            else
                B_timerinterrupt();
        }
        else
        {
            printf("INTERNAL PANIC: unknown event type \n");
        }
        free(eventptr);
    }

terminate:
    printf(
        " Simulator terminated at time %f\n after sending %d pkts from layer3\n",
        time, nsim);

    //WRITTEN BY MAHATHIR
    printf("Device A stats: \n");
    printf("Number of frames sent from A: %d\n",frame_count_A);
    printf("Number of received frames in B: %d\n",number_of_frames_received_B);
    printf("Number of dropped at A_output: %d\n",dropped_frame_A_output);

    printf("Device B stats: \n");
    printf("Number of frames sent from B: %d\n",frame_count_B);
    printf("Number of received frames in A: %d\n",number_of_frames_received_A);
    printf("Number of dropped at B_output: %d\n",dropped_frame_B_output);

}

void init() /* initialize the simulator */
{
    int i;
    float sum, avg;
    float jimsrand();

    printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");

    if(HARD_CODED_PARAMETER_ACTIVATION)
    {
        nsimmax = HARD_CODED_NUMBER_OF_PACKETS;
        lossprob = HARD_CODED_LOSS_PROBABILITY;
        corruptprob = HARD_CODED_CORRUPTION_PROBABILITY;
        lambda = HARD_CODED_DELAY;
        TRACE = HARD_CODED_TRACE;
        piggy_backing_enable = HARD_CODED_PIGGY_ENABLE;
    }
    else
    {
        printf("Enter the number of packets to simulate: ");
        scanf("%d",&nsimmax);
        printf("Enter  frame loss probability [enter 0.0 for no loss]:");
        scanf("%f",&lossprob);
        printf("Enter frame corruption probability [0.0 for no corruption]:");
        scanf("%f",&corruptprob);
        printf("Enter average time between packets from sender's layer3 [ > 0.0]:");
        scanf("%f",&lambda);
        printf("Enter TRACE:");
        scanf("%d",&TRACE);

        printf("Enter piggy enable [ 1 for enable, 0 for disable]:");
        int option;
        scanf("%d",&option);
        piggy_backing_enable=option==1?true:false;
    }

    srand(9999); /* init random number generator */
    sum = 0.0;   /* test random number generator for students */
    for (i = 0; i < 1000; i++)
        sum = sum + jimsrand(); /* jimsrand() should be uniform in [0,1] */
    avg = sum / 1000.0;
    if (avg < 0.25 || avg > 0.75)
    {
        printf("It is likely that random number generation on your machine\n");
        printf("is different from what this emulator expects.  Please take\n");
        printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(1);
    }

    ntolayer1 = 0;
    nlost = 0;
    ncorrupt = 0;

    time = 0.0;              /* initialize time to 0.0 */
    generate_next_arrival(); /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand(void)
{
    double mmm = RAND_MAX;
    float x;                 /* individual students may need to change mmm */
    x = rand() / mmm;        /* x should be uniform in [0,1] */
    return (x);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival(void)
{
    double x, log(), ceil();
    struct event *evptr;
    float ttime;
    int tempint;

    if (TRACE > 2)
        printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

    x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
    /* having mean of lambda        */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + x;
    evptr->evtype = FROM_layer3;
    if (BIDIRECTIONAL && (jimsrand() > 0.5))
        evptr->eventity = B;
    else
        evptr->eventity = A;
    insertevent(evptr);
}

void insertevent(struct event *p)
{
    struct event *q, *qold;

    if (TRACE > 2)
    {
        printf("            INSERTEVENT: time is %lf\n", time);
        printf("            INSERTEVENT: future time will be %lf\n", p->evtime);
    }
    q = evlist;      /* q points to header of list in which p struct inserted */
    if (q == NULL)   /* list is empty */
    {
        evlist = p;
        p->next = NULL;
        p->prev = NULL;
    }
    else
    {
        for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
            qold = q;
        if (q == NULL)   /* end of list */
        {
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        }
        else if (q == evlist)     /* front of list */
        {
            p->next = evlist;
            p->prev = NULL;
            p->next->prev = p;
            evlist = p;
        }
        else     /* middle of list */
        {
            p->next = q;
            p->prev = q->prev;
            q->prev->next = p;
            q->prev = p;
        }
    }
}

void printevlist(void)
{
    struct event *q;
    int i;
    printf("--------------\nEvent List Follows:\n");
    for (q = evlist; q != NULL; q = q->next)
    {
        printf("Event time: %f, type: %d entity: %d\n", q->evtime, q->evtype,
               q->eventity);
    }
    printf("--------------\n");
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB /* A or B is trying to stop timer */)
{
    struct event *q, *qold;

    if (TRACE > 2)
        printf("          STOP TIMER: stopping timer at %f\n", time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
        {
            /* remove this event */
            if (q->next == NULL && q->prev == NULL)
                evlist = NULL;          /* remove first and only event on list */
            else if (q->next == NULL) /* end of list - there is one in front */
                q->prev->next = NULL;
            else if (q == evlist)   /* front of list - there must be event after */
            {
                q->next->prev = NULL;
                evlist = q->next;
            }
            else     /* middle of list */
            {
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            free(q);
            return;
        }
    printf("Warning: unable to cancel your timer. It wasn't running.\n");
}

void starttimer(int AorB /* A or B is trying to start timer */, float increment)
{
    struct event *q;
    struct event *evptr;

    if (TRACE > 2)
        printf("          START TIMER: starting timer at %f\n", time);
    /* be nice: check to see if timer is already started, if so, then  warn */
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
        {
            printf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + increment;
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = AorB;
    insertevent(evptr);
}

/************************** TOlayer1 ***************/
void tolayer1(int AorB, struct frm frame)
{
    struct frm *myfrmptr;
    struct event *evptr, *q;
    float lastime, x;
    int i;

    ntolayer1++;

    /* simulate losses: */
    if (jimsrand() < lossprob)
    {
        nlost++;
        if (TRACE > 0)
            printf("          TOlayer1: frame being lost\n");
        return;
    }

    /* make a copy of the frame student just gave me since he/she may decide */
    /* to do something with the frame after we return back to him/her */
    myfrmptr = (struct frm *)malloc(sizeof(struct frm));
    myfrmptr->seqnum = frame.seqnum;
    myfrmptr->acknum = frame.acknum;
    myfrmptr->checksum = frame.checksum;
    myfrmptr->type = frame.type;
    for (i = 0; i < 20; i++)
        myfrmptr->payload[i] = frame.payload[i];
    if (TRACE > 2)
    {
        printf("          TOlayer1: seq: %d, ack %d, check: %d ", myfrmptr->seqnum,
               myfrmptr->acknum, myfrmptr->checksum);
        for (i = 0; i < 20; i++)
            printf("%c", myfrmptr->payload[i]);
        printf("\n");
    }

    /* create future event for arrival of frame at the other side */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtype = FROM_layer1;      /* frame will pop out from layer1 */
    evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
    evptr->frmptr = myfrmptr;         /* save ptr to my copy of frame */
    /* finally, compute the arrival time of frame at the other end.
       medium can not reorder, so make sure frame arrives between 1 and 10
       time units after the latest arrival time of frames
       currently in the medium on their way to the destination */
    lastime = time;
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == FROM_layer1 && q->eventity == evptr->eventity))
            lastime = q->evtime;
    evptr->evtime = lastime + 1 + 9 * jimsrand();

    /* simulate corruption: */
    if (jimsrand() < corruptprob)
    {
        ncorrupt++;
        if ((x = jimsrand()) < .75)
            myfrmptr->payload[0] = 'Z'; /* corrupt payload */
        else if (x < .875)
            myfrmptr->seqnum = 999999;
        else
            myfrmptr->acknum = 999999;
        if (TRACE > 0)
            printf("          TOlayer1: frame being corrupted\n");
    }

    if (TRACE > 2)
        printf("          TOlayer1: scheduling arrival on other side\n");
    insertevent(evptr);
}

void tolayer3(int AorB, char datasent[20])
{
    int i;
    if (TRACE > 2)
    {
        printf("          TOlayer3: data received: ");
        for (i = 0; i < 20; i++)
            printf("%c", datasent[i]);
        printf("\n");
    }
}
