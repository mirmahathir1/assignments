import numpy as np
import random

random.seed(11)

"""
The symbolic constant Q_LIMIT is set to 100, our guess (which might 
have to be 
adjusted by trial and error) as to the longest the queue will ever 
get. (As mentioned 
earlier, this guess could be eliminated if we were using dynamic 
storage allocation; 
while C supports this, we have not used it in our examples.)
"""
Q_LIMIT = 100
"""
BUSY and IDLE are defi ned to be used with the server_status 
variable, for code 
readability
"""
BUSY = 1
IDLE = 0

next_event_type = 0
num_custs_delayed = 0
num_delays_required = 0
num_events = 0
num_in_q = 0
server_status = 0

area_num_in_q = 0.0
area_server_status = 0.0
mean_interarrival = 0.0
mean_service = 0.0
sim_time = 0.0
# time_arrival = np.zeros((Q_LIMIT + 1, 1))
time_arrival = [0] * (Q_LIMIT + 1)
time_last_event = 0.0

"""
Note also that the event list, as we have discussed it so far, 
will be implemented in an array called time_next_event, whose 0th 
entry will be ignored in order 
to make the index agree with the event type
"""
time_next_event = [0] * 3
total_of_delays = 0

"""
File pointers *infile and *outfile are defined to allow us to open 
the 
input and output fi les from within the code, rather than at the 
operating-system 
level
"""
infile = None
outfile = None


def expon(mean):
    return -mean * np.log(random.uniform(0, 1))


def initialize():
    print("inside initialize")
    global time_arrival, sim_time, server_status, num_in_q, time_last_event, num_custs_delayed, total_of_delays, area_num_in_q, \
        area_server_status, time_next_event, mean_interarrival
    """
    The simulation begins with the main program invoking 
    the initialization routine. Our modeling assumption was that initially 
    the system is empty of customers and the server is idle, as depicted in the 
    “system” 
    """

    """
     Server status is 0 [we use 0 to represent an idle server 
    and 1 to represent a busy server,
    """
    server_status = IDLE
    """
    the number of customers in the queue is 0.
    """
    num_in_q = 0

    """
    There is a 
    one-dimensional array to store the times of arrival of customers 
    currently in the queue; this array is initially empty, and as the simula-
    tion progresses, its length will grow and shrink. 
    """
    # time_arrival = np.zeros((Q_LIMIT + 1, 1))
    time_arrival = [0] * (Q_LIMIT + 1)

    """
    The time of the last 
    (most recent) event is initialized to 0, so that at the time of the fi rst 
    event (when it is used), it will have its correct value.
    """
    time_last_event = 0.0
    """
    clock is set to 0,
    """
    sim_time = 0

    """
    This program is written with array indexing of 1
    """

    """
    Note that the time of the first arrival, time_next_event[1], 
    is determined by adding an exponential 
    random variate with mean mean_interarrival, namely, 
    expon(mean_interarrival), to the simulation clock
    """
    next_arrival_time = sim_time + expon(mean_interarrival)
    time_next_event[1] = next_arrival_time
    print(f"next arrival event time is updated to {next_arrival_time}")

    """
    Since there is no customer in service, it does not even make sense to 
    talk about the time of the next departure (“D” by the event list), and we 
    know that the first event will be the initial customer arrival at time 0.4. 
    However, the simulation progresses in general by looking at the event 
    list and picking the smallest value from it to determine what the next 
    event will be, so by scheduling the next departure to occur at time ` (or 
    a very large number in a computer program), we effectively eliminate 
    the departure event from consideration and force the next event to be 
    an arrival
    """
    time_next_event[2] = 1.0E30
    print(f"next arrival event time is updated to infinity")

    """
    Finally, the four statistical counters are initialized to 0. 
    """
    num_custs_delayed = 0
    total_of_delays = 0.0
    area_num_in_q = 0.0
    area_server_status = 0.0


def timing():
    print("inside timing")
    """
    When all initialization is done, control is returned to the main pro-
    gram, which then calls the timing routine to determine the next event. 
    """
    global outfile, next_event_type, num_events, time_next_event, sim_time
    min_time_next_event = 1.0E29
    next_event_type = 0
    """
    The timing function is used to 
    compare 
    time_next_event[1], time_next_event[2], . . . , 
    time_next_event[num_events] 
    (recall that num_events was set in the main function) and to 
    set next_event_type 
    equal to the event type whose time of occurrence is the 
    smallest. In case of ties, 
    the lowest-numbered event type is chosen
    """
    for i in range(1, num_events + 1):
        """
        i can only be of two values: 1 (arrival) and 2 (departure)
        """
        if time_next_event[i] < min_time_next_event:
            min_time_next_event = time_next_event[i]
            next_event_type = i
    print(f"next_event_type is {next_event_type}")
    if next_event_type == 0:
        print(f'Event list is empty at time {sim_time}', file=outfile)
        exit(1)

    """
    and the
    timing routine advances the clock to this time, then passes control 
    back to the main program with the information that the next event is to 
    be an arrival.
    the simulation clock is advanced 
    to the time of occurrence of the chosen event type, 
    min_time_next_event
    """
    sim_time = min_time_next_event
    print(f"sim_time is now {sim_time}")


def update_time_avg_stats():
    print("inside update_time_avg_stats")
    global sim_time, \
        time_last_event, \
        area_num_in_q, \
        num_in_q, \
        server_status, \
        area_server_status

    """
    Note that the time of the last event used here is its 
    old value , 
    before it is updated to its new value in this event 
    routine.
    """
    time_since_last_event = sim_time - time_last_event
    print(f"time_since_last_event is now {time_since_last_event}")
    """
     Finally, the time of the last event is brought up to 
    the current time
    """
    time_last_event = sim_time

    """
    The area under Q(t) is updated by adding in 
    the product of the previous value (i.e., the level it had 
    between the last 
    event and now) of Q(t) times the width of the 
    interval 
    of time from the last event to now, t 2 (time of last event) 
    """
    area_num_in_q += num_in_q * time_since_last_event
    print(f"area_num_in_q is now {area_num_in_q}")
    """
    Similarly, 
    the area under B(t) is updated by adding in the product of 
    its previous 
    value (0) times the width of the interval of time since the 
    last event.
    """
    area_server_status += server_status * time_since_last_event
    print(f"area_server_status is now {area_server_status}")


def arrive():
    print("inside arrive")
    global time_next_event, sim_time, mean_interarrival, server_status, num_in_q, Q_LIMIT, outfile, time_arrival, \
        total_of_delays, num_custs_delayed, mean_service

    """
    the event list is updated to reflect this customer’s 
    arrival. The arrival time of the next customer will be updated
    on the time_next_event matrix at index 1
    """
    print(f"customer arrived at sim_time: {sim_time}")
    next_arrival_time = sim_time + expon(mean_interarrival)
    time_next_event[1] = next_arrival_time
    print(f"next arrival event time is updated to {next_arrival_time}")
    if server_status == BUSY:
        print(f"server_status was BUSY")
        """
        Since this customer 
        arrives to find the server busy (status equal to 1 upon her arrival)
        and the number-in-queue variable rises to 1
        """
        num_in_q += 1
        print(f"num_in_q = {num_in_q}")
        """
        we ask whether the storage space allocated to hold the 
        queue is already full 
        """
        if num_in_q > Q_LIMIT:
            """
            If the queue is already full, an error 
            message is produced and the simulation is stopped; 
            """
            print(f'Overflow of the array time arrival at', file=outfile)
            print(f' time {sim_time}', file=outfile)
            exit(2)

        """
        she must queue up in the last location in the queue, 
        her time of arrival is 
        stored in the first location in the array, 
        """

        # np.put(time_arrival, num_in_q, sim_time)
        time_arrival[num_in_q] = sim_time
        print(f"putting arrival time {sim_time} in queue position {num_in_q}")
        """
        the time of the next 
        departure is not changed, since its value is the 
        departure time of the
        customer who is still in service at this time. Since we 
        are not observing the end of anyone’s delay in queue, the 
        number-delayed and total-delay variables are unchanged
        """
    else:
        print("server_status was not BUSY")
        """
        On the other hand, if the arriving customer finds the server 
        idle, then this customer has a delay of 0, which is counted as 
        a delay, and the number of customer delays completed is 
        incremented by 1
        """
        delay = 0.0
        total_of_delays += delay
        print(f"delay for this customer is {delay}")
        print(f"total_of_delays is now {total_of_delays}")
        """
        (which does count as a delay)
        """
        num_custs_delayed += 1
        print(f"num_custs_delayed = {num_custs_delayed}")
        """
        The server status is set to 1 to represent that the server 
        is now busy,
        """
        server_status = BUSY
        print("server_status is now BUSY")
        next_departure_time = sim_time + expon(mean_service)
        time_next_event[2] = next_departure_time
        print(f"next departure event time is updated to {next_departure_time}")
        """
        for the first customer, the queue itself is still empty
        """


def depart():
    print("inside depart")
    global num_in_q, server_status, time_next_event, sim_time, time_arrival, total_of_delays, num_custs_delayed, \
        mean_service
    print(f"customer departed at sim_time: {sim_time}")
    if num_in_q == 0:
        print("number of elements in queue is 0")
        """
        queue is now empty, the server becomes idle
        """
        server_status = IDLE
        print("server_status is now IDLE")
        """
        we must set the next departure time in the event list to 
        infinity, since the system now looks the same as it did 
        at time 0 and we want to force the next event to be the 
        arrival of next customer.
        """
        time_next_event[2] = 1.0E30
        print(f"next departure event time is updated to infinity")
    else:
        print("number of elements in queue is not equals to 0")
        """
        The server will maintain its busy status, since next customer 
        moves out of the first place in queue and into service
        """
        """
        The queue shrinks by 1,
        """
        num_in_q -= 1
        print(f"queue size is shrunk to {num_in_q}")
        """
        The delay statistics are updated, since at this time next
        customer is entering service and is completing her delay 
        in queue
        Here we make use of the time-of-arrival array, and compute 
        the second 
        delay as the current time minus the next customer’s 
        time of arrival
        """
        """
        Note that the value was stored in the 
        first location in the time-of-arrival array before it 
        was changed, so 
        this delay computation would have to be done before 
        advancing the 
        times of arrival in the array
        """
        delay = sim_time - time_arrival[1]
        total_of_delays += delay
        print(f"delay for this customer = {delay}")
        print(f"total_of_delays is now {total_of_delays}")

        num_custs_delayed += 1
        print(f"num_custs_delayed is now {num_custs_delayed}")
        """
        the time of the next departure (that of customer 2) in the 
        event list is updated to S2 time units from now
        """
        next_departure_time = sim_time + expon(mean_service)
        time_next_event[2] = next_departure_time
        print(f"next departure event time is updated to {next_departure_time}")

        """
        the time-of-arrival array is moved up one place, to 
        represent that next customer is now first in line
        """
        for i in range(1, num_in_q + 1):
            # np.put(time_arrival, i, time_arrival[i + 1])
            time_arrival[i] = time_arrival[i + 1]


def report():
    global total_of_delays, num_custs_delayed, area_num_in_q, sim_time, area_server_status, outfile
    print(f'Average delay in queue {total_of_delays / num_custs_delayed}', file=outfile)
    print(f'Average number in queue {area_num_in_q / sim_time}', file=outfile)
    print(f'Server utilization {area_server_status / sim_time}', file=outfile)
    print(f'Time simulation ended {sim_time} minutes', file=outfile)


def reportConsole():
    global total_of_delays, num_custs_delayed, area_num_in_q, sim_time, area_server_status, outfile
    print(f'Average delay in queue {total_of_delays / num_custs_delayed}')
    print(f'Average number in queue {area_num_in_q / sim_time}')
    print(f'Server utilization {area_server_status / sim_time}')
    print(f'Time simulation ended {sim_time} minutes')


def main():
    global infile, outfile, num_events, mean_interarrival, mean_service, num_delays_required, num_custs_delayed, \
        next_event_type
    print("Program started...")

    infile = open('in/mm10.5.in', 'r')
    outfile = open('mm1.out', 'w')

    num_events = 2
    [mean_interarrival, mean_service, num_delays_required] = infile.readlines()[0].split(' ')
    mean_interarrival = float(mean_interarrival)
    mean_service = float(mean_service)
    num_delays_required = float(num_delays_required)

    print("Single server queuing system", file=outfile)
    print(f'Mean interarrival time {mean_interarrival} minutes', file=outfile)
    print(f'Mean service time {mean_service} minutes', file=outfile)
    print(f'Number of customers {num_delays_required}', file=outfile)

    initialize()

    i = 0
    """
    The “while” loop then executes the simulation as long as more 
    customer delays are needed to fulfi ll the 1000-delay stopping 
    rule
    """
    while num_custs_delayed < num_delays_required:
        print(f'event count={i}' + "_" * 50)
        i += 1
        """
        Inside the “while” loop, the timing 
        function is fi rst invoked to determine the type of the next 
        event to occur and to 
        advance the simulation clock to its time. Before processing 
        this event, the function to update the areas under the Q(t) 
        and B(t) curves is invoked; by doing this at 
        this time we automatically update these areas before 
        processing each event
        """
        timing()

        update_time_avg_stats()

        """
         Then a switch statement, based on next_event_type (=1 for 
         an arrival and 2 for a departure), passes control to the 
         appropriate event function.
        """
        if next_event_type == 1:
            arrive()
        elif next_event_type == 2:
            depart()

    print("simulation done" + "_" * 50)
    report()

    infile.close()
    outfile.close()


if __name__ == "__main__":
    main()
