import random
import math
import os
import numpy as np
import shutil

np.warnings.filterwarnings('ignore', category=np.VisibleDeprecationWarning)




class MM1:
    def get_random_variable_stats(self):
        min_of_arrival_time_actual = np.min(self.interarrival_times_actual)
        max_of_arrival_time_actual = np.max(self.interarrival_times_actual)

        min_of_service_time_actual = np.min(self.service_times_actual)
        max_of_service_time_actual = np.max(self.service_times_actual)

        median_of_service_time_actual = np.median(self.service_times_actual)
        median_of_arrival_time_actual = np.median(self.interarrival_times_actual)

        min_of_uniform = np.min(self.arrival_times_uniform + self.service_times_uniform)
        max_of_uniform = np.max(self.arrival_times_uniform + self.service_times_uniform)
        median_of_uniform = np.median(self.arrival_times_uniform + self.service_times_uniform)

        cut_bins_of_uniform = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]

        beta_interarrival = self.mean_interarrival
        beta_service = self.mean_service
        cut_bins_of_actual_arrival = [0, beta_interarrival / 2, beta_interarrival, 2 * beta_interarrival,
                                      3 * beta_interarrival, 1000]
        cut_bins_of_actual_service = [0, beta_service / 2, beta_service, 2 * beta_service, 3 * beta_service, 1000]

        bins_of_actual_arrival = {}
        for i in range(1, len(cut_bins_of_actual_arrival)):
            bins_of_actual_arrival[cut_bins_of_actual_arrival[i]] = 0
        bins_of_actual_service = {}
        for i in range(1, len(cut_bins_of_actual_service)):
            bins_of_actual_service[cut_bins_of_actual_service[i]] = 0
        bins_of_uniform = {}
        for i in range(1, len(cut_bins_of_uniform)):
            bins_of_uniform[cut_bins_of_uniform[i]] = 0

        for actual_arrival in self.interarrival_times_actual:
            for i in range(1, len(cut_bins_of_actual_arrival)):
                if cut_bins_of_actual_arrival[i - 1] <= actual_arrival <= cut_bins_of_actual_arrival[i]:
                    bins_of_actual_arrival[cut_bins_of_actual_arrival[i]] += 1
                    break
        # print(bins_of_actual_arrival)

        for actual_service in self.service_times_actual:
            for i in range(1, len(cut_bins_of_actual_service)):
                if cut_bins_of_actual_service[i - 1] <= actual_service <= cut_bins_of_actual_service[i]:
                    bins_of_actual_service[cut_bins_of_actual_service[i]] += 1
                    break
        # print(bins_of_actual_service)

        for uniform_arrival in self.arrival_times_uniform:
            for i in range(1, len(cut_bins_of_uniform)):
                if cut_bins_of_uniform[i - 1] <= uniform_arrival <= cut_bins_of_uniform[i]:
                    bins_of_uniform[cut_bins_of_uniform[i]] += 1
                    break
        for uniform_service in self.service_times_uniform:
            for i in range(1, len(cut_bins_of_uniform)):
                if cut_bins_of_uniform[i - 1] <= uniform_service <= cut_bins_of_uniform[i]:
                    bins_of_uniform[cut_bins_of_uniform[i]] += 1
                    break
        # print(bins_of_uniform)

        probability_of_actual_arrival = {}
        probability_of_actual_service = {}
        probability_of_uniform = {}

        count_actual_arrival = len(self.interarrival_times_actual)
        count_actual_service = len(self.interarrival_times_actual)
        count_uniform = count_actual_arrival + count_actual_service

        for key in bins_of_actual_arrival.keys():
            probability_of_actual_arrival[key] = bins_of_actual_arrival[key] / count_actual_arrival
        # print(probability_of_actual_arrival)

        for key in bins_of_actual_service.keys():
            probability_of_actual_service[key] = bins_of_actual_service[key] / count_actual_service
        # print(probability_of_actual_service)

        for key in bins_of_uniform.keys():
            probability_of_uniform[key] = bins_of_uniform[key] / count_uniform
        # print(probability_of_uniform)

        cumulative_probability_of_actual_arrival = {}
        cumulative_probability_of_actual_service = {}
        cumulative_probability_of_uniform = {}

        keys_of_actual_arrival = probability_of_actual_arrival.keys()
        keys_of_actual_service = probability_of_actual_service.keys()
        keys_of_uniform = probability_of_uniform.keys()

        for i, key in enumerate(keys_of_actual_arrival):
            cumulation = 0
            for j, keyJ in enumerate(keys_of_actual_arrival):
                if j <= i:
                    cumulation += probability_of_actual_arrival[keyJ]
            cumulative_probability_of_actual_arrival[key] = cumulation
        # print(cumulative_probability_of_actual_arrival)

        for i, key in enumerate(keys_of_actual_service):
            cumulation = 0
            for j, keyJ in enumerate(keys_of_actual_service):
                if j <= i:
                    cumulation += probability_of_actual_service[keyJ]
            cumulative_probability_of_actual_service[key] = cumulation
        # print(cumulative_probability_of_actual_service)

        for i, key in enumerate(keys_of_uniform):
            cumulation = 0
            for j, keyJ in enumerate(keys_of_uniform):
                if j <= i:
                    cumulation += probability_of_uniform[keyJ]
            cumulative_probability_of_uniform[key] = cumulation
        # print(cumulative_probability_of_uniform)

        return {
            'min_of_actual_service': min_of_service_time_actual,
            'max_of_actual_service': max_of_service_time_actual,
            'median_of_actual_service': median_of_service_time_actual,
            'probability_of_service': probability_of_actual_service,
            'cumulative_probability_of_service': cumulative_probability_of_actual_service,

            'min_of_actual_arrival': min_of_arrival_time_actual,
            'max_of_actual_arrival': max_of_arrival_time_actual,
            'median_of_actual_arrival': median_of_arrival_time_actual,
            'probability_of_arrival': probability_of_actual_arrival,
            'cumulative_probability_of_arrival': cumulative_probability_of_actual_arrival,

            'min_of_uniform': min_of_uniform,
            'max_of_uniform': max_of_uniform,
            'median_of_uniform': median_of_uniform,
            'probability_of_uniform': probability_of_uniform,
            'cumulative_probability_of_uniform': cumulative_probability_of_uniform
        }

    def generate_random_variable_stats_report(self):
        random_variable_stats = self.get_random_variable_stats()
        outfile = open(
            f'{os.path.join("out", "mm1+random_variable" + str(self.mean_interarrival) + "+" + str(self.mean_service) + "+" + str(self.num_delays_required))}.out',
            "w")
        print("Status on Interarrival Time A: ", file=outfile)
        print(f'Min(A): {random_variable_stats["min_of_actual_arrival"]}', file=outfile)
        print(f'Max(A): {random_variable_stats["max_of_actual_arrival"]}', file=outfile)
        print(f'Median(A):  {random_variable_stats["median_of_actual_arrival"]}', file=outfile)
        print(f'P(A):', file=outfile)
        for i, key in enumerate(random_variable_stats["probability_of_arrival"]):
            print(f'< {key} = \t{random_variable_stats["probability_of_arrival"][key]}', file=outfile)
        print(f'F(A):', file=outfile)
        for i, key in enumerate(random_variable_stats["cumulative_probability_of_arrival"]):
            print(f'< {key} = \t{random_variable_stats["cumulative_probability_of_arrival"][key]}', file=outfile)
        print("_"*100, file=outfile)
        print("Status on Service Time S: ", file=outfile)
        print(f'Min(S): {random_variable_stats["min_of_actual_service"]}', file=outfile)
        print(f'Max(S): {random_variable_stats["max_of_actual_service"]}', file=outfile)
        print(f'Median(S): {random_variable_stats["median_of_actual_service"]}', file=outfile)
        print(f'P(S):', file=outfile)
        for i, key in enumerate(random_variable_stats["probability_of_service"]):
            print(f'< {key} = \t{random_variable_stats["probability_of_service"][key]}', file=outfile)
        print(f'F(S):', file=outfile)
        for i, key in enumerate(random_variable_stats["cumulative_probability_of_service"]):
            print(f'< {key} = \t{random_variable_stats["cumulative_probability_of_service"][key]}', file=outfile)
        print("_" * 100, file=outfile)
        print("Status on Uniform Random Variable U: ", file=outfile)
        print(f'Min(U): {random_variable_stats["min_of_uniform"]}',file=outfile)
        print(f'Max(U): {random_variable_stats["max_of_uniform"]}', file=outfile)
        print(f'Median(U): {random_variable_stats["median_of_uniform"]}', file=outfile)
        print(f'P(U):', file=outfile)
        for i, key in enumerate(random_variable_stats["probability_of_uniform"]):
            print(f'< {key} = \t{random_variable_stats["probability_of_uniform"][key]}', file=outfile)
        print(f'F(U):', file=outfile)
        for i, key in enumerate(random_variable_stats["cumulative_probability_of_uniform"]):
            print(f'< {key} = \t{random_variable_stats["cumulative_probability_of_uniform"][key]}', file=outfile)
        outfile.close()

    def __init__(self, mean_interarrival, mean_service, num_delays_required):
        """
            The symbolic constant Q_LIMIT is set to 100, our guess (which might
            have to be
            adjusted by trial and error) as to the longest the queue will ever
            get. (As mentioned
            earlier, this guess could be eliminated if we were using dynamic
            storage allocation;
            while C supports this, we have not used it in our examples.)
            """
        self.Q_LIMIT = 100
        """
        BUSY and IDLE are defi ned to be used with the server_status 
        variable, for code 
        readability
        """
        self.BUSY = 1
        self.IDLE = 0

        self.next_event_type = 0
        self.num_custs_delayed = 0
        self.num_delays_required = 0
        self.num_events = 0
        self.num_in_q = 0
        self.server_status = 0

        self.area_num_in_q = 0.0
        self.area_server_status = 0.0
        self.mean_interarrival = 0.0
        self.mean_service = 0.0
        self.sim_time = 0.0
        self.time_arrival = [0] * (self.Q_LIMIT + 1)
        self.time_last_event = 0.0

        """
        Note also that the event list, as we have discussed it so far, 
        will be implemented in an array called time_next_event, whose 0th 
        entry will be ignored in order 
        to make the index agree with the event type
        """
        self.time_next_event = [0] * 3
        self.total_of_delays = 0

        """
        File pointers *infile and *outfile are defined to allow us to open 
        the 
        input and output fi les from within the code, rather than at the 
        operating-system 
        level
        """
        self.infile = None
        self.outfile = None

        self.arrival_times_uniform = []
        self.service_times_uniform = []
        self.interarrival_times_actual = []
        self.service_times_actual = []

        self.num_delays_required = num_delays_required
        self.mean_interarrival = mean_interarrival
        self.mean_service = mean_service

    def get_sample(self, mean, arrival=True):
        uniform_random_sample = random.uniform(0, 1)
        actual_random_sample = -mean * math.log(uniform_random_sample)
        if arrival:
            self.arrival_times_uniform.append(uniform_random_sample)
            self.interarrival_times_actual.append(actual_random_sample)
        else:
            self.service_times_uniform.append(uniform_random_sample)
            self.service_times_actual.append(actual_random_sample)

        return actual_random_sample

    def initialize(self):
        # print("inside initialize")
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
        self.server_status = self.IDLE
        """
        the number of customers in the queue is 0.
        """
        self.num_in_q = 0

        """
        There is a 
        one-dimensional array to store the times of arrival of customers 
        currently in the queue; this array is initially empty, and as the simula-
        tion progresses, its length will grow and shrink. 
        """
        # time_arrival = np.zeros((Q_LIMIT + 1, 1))
        self.time_arrival = [0] * (self.Q_LIMIT + 1)

        """
        The time of the last 
        (most recent) event is initialized to 0, so that at the time of the fi rst 
        event (when it is used), it will have its correct value.
        """
        self.time_last_event = 0.0
        """
        clock is set to 0,
        """
        self.sim_time = 0

        """
        This program is written with array indexing of 1
        """

        """
        Note that the time of the first arrival, time_next_event[1], 
        is determined by adding an exponential 
        random variate with mean mean_interarrival, namely, 
        expon(mean_interarrival), to the simulation clock
        """
        next_arrival_time = self.sim_time + self.get_sample(self.mean_interarrival, arrival=True)
        self.time_next_event[1] = next_arrival_time
        # print(f"next arrival event time is updated to {next_arrival_time}")

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
        self.time_next_event[2] = 1.0E30
        # print(f"next arrival event time is updated to infinity")

        """
        Finally, the four statistical counters are initialized to 0. 
        """
        self.num_custs_delayed = 0
        self.total_of_delays = 0.0
        self.area_num_in_q = 0.0
        self.area_server_status = 0.0

        self.arrival_times_uniform = []
        self.service_times_uniform = []

    def timing(self):
        # print("inside timing")
        """
        When all initialization is done, control is returned to the main pro-
        gram, which then calls the timing routine to determine the next event. 
        """
        min_time_next_event = 1.0E29
        self.next_event_type = 0
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
        for i in range(1, self.num_events + 1):
            """
            i can only be of two values: 1 (arrival) and 2 (departure)
            """
            if self.time_next_event[i] < min_time_next_event:
                min_time_next_event = self.time_next_event[i]
                self.next_event_type = i
        # print(f"next_event_type is {self.next_event_type}")
        if self.next_event_type == 0:
            print(f'Event list is empty at time {self.sim_time}', file=self.outfile)
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
        self.sim_time = min_time_next_event
        # print(f"self.sim_time is now {self.sim_time}")

    def update_time_avg_stats(self):
        # print("inside update_time_avg_stats")

        """
        Note that the time of the last event used here is its 
        old value , 
        before it is updated to its new value in this event 
        routine.
        """
        time_since_last_event = self.sim_time - self.time_last_event
        # print(f"time_since_last_event is now {time_since_last_event}")
        """
         Finally, the time of the last event is brought up to 
        the current time
        """
        self.time_last_event = self.sim_time

        """
        The area under Q(t) is updated by adding in 
        the product of the previous value (i.e., the level it had 
        between the last 
        event and now) of Q(t) times the width of the 
        interval 
        of time from the last event to now, t 2 (time of last event) 
        """
        self.area_num_in_q += self.num_in_q * time_since_last_event
        # print(f"area_num_in_q is now {self.area_num_in_q}")
        """
        Similarly, 
        the area under B(t) is updated by adding in the product of 
        its previous 
        value (0) times the width of the interval of time since the 
        last event.
        """
        self.area_server_status += self.server_status * time_since_last_event
        # print(f"area_server_status is now {self.area_server_status}")

    def arrive(self):
        # print("inside arrive")

        """
        the event list is updated to reflect this customer’s 
        arrival. The arrival time of the next customer will be updated
        on the time_next_event matrix at index 1
        """
        # print(f"customer arrived at sim_time: {self.sim_time}")
        next_arrival_time = self.sim_time + self.get_sample(self.mean_interarrival, arrival=True)
        self.time_next_event[1] = next_arrival_time
        # print(f"next arrival event time is updated to {next_arrival_time}")
        if self.server_status == self.BUSY:
            # print(f"server_status was BUSY")
            """
            Since this customer 
            arrives to find the server busy (status equal to 1 upon her arrival)
            and the number-in-queue variable rises to 1
            """
            self.num_in_q += 1
            # print(f"num_in_q = {self.num_in_q}")
            """
            we ask whether the storage space allocated to hold the 
            queue is already full 
            """
            if self.num_in_q > self.Q_LIMIT:
                """
                If the queue is already full, an error 
                message is produced and the simulation is stopped; 
                """
                print(f'Overflow of the array time arrival at', file=self.outfile)
                print(f' time {self.sim_time}', file=self.outfile)
                exit(2)

            """
            she must queue up in the last location in the queue, 
            her time of arrival is 
            stored in the first location in the array, 
            """

            # np.put(time_arrival, num_in_q, sim_time)
            self.time_arrival[self.num_in_q] = self.sim_time
            # print(f"putting arrival time {self.sim_time} in queue position {self.num_in_q}")
            """
            the time of the next 
            departure is not changed, since its value is the 
            departure time of the
            customer who is still in service at this time. Since we 
            are not observing the end of anyone’s delay in queue, the 
            number-delayed and total-delay variables are unchanged
            """
        else:
            # print("server_status was not BUSY")
            """
            On the other hand, if the arriving customer finds the server 
            idle, then this customer has a delay of 0, which is counted as 
            a delay, and the number of customer delays completed is 
            incremented by 1
            """
            delay = 0.0
            self.total_of_delays += delay
            # print(f"delay for this customer is {delay}")
            # print(f"total_of_delays is now {self.total_of_delays}")
            """
            (which does count as a delay)
            """
            self.num_custs_delayed += 1
            # print(f"num_custs_delayed = {self.num_custs_delayed}")
            """
            The server status is set to 1 to represent that the server 
            is now busy,
            """
            self.server_status = self.BUSY
            # print("server_status is now BUSY")
            next_departure_time = self.sim_time + self.get_sample(self.mean_service, arrival=False)
            self.time_next_event[2] = next_departure_time
            # print(f"next departure event time is updated to {next_departure_time}")
            """
            for the first customer, the queue itself is still empty
            """

    def depart(self):
        # print("inside depart")
        # print(f"customer departed at sim_time: {self.sim_time}")
        if self.num_in_q == 0:
            # print("number of elements in queue is 0")
            """
            queue is now empty, the server becomes idle
            """
            self.server_status = self.IDLE
            # print("server_status is now IDLE")
            """
            we must set the next departure time in the event list to 
            infinity, since the system now looks the same as it did 
            at time 0 and we want to force the next event to be the 
            arrival of next customer.
            """
            self.time_next_event[2] = 1.0E30
            # print(f"next departure event time is updated to infinity")
        else:
            # print("number of elements in queue is not equals to 0")
            """
            The server will maintain its busy status, since next customer 
            moves out of the first place in queue and into service
            """
            """
            The queue shrinks by 1,
            """
            self.num_in_q -= 1
            # print(f"queue size is shrunk to {self.num_in_q}")
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
            delay = self.sim_time - self.time_arrival[1]
            self.total_of_delays += delay
            # print(f"delay for this customer = {delay}")
            # print(f"total_of_delays is now {self.total_of_delays}")

            self.num_custs_delayed += 1
            # print(f"num_custs_delayed is now {self.num_custs_delayed}")
            """
            the time of the next departure (that of customer 2) in the 
            event list is updated to S2 time units from now
            """
            next_departure_time = self.sim_time + self.get_sample(self.mean_service, arrival=False)
            self.time_next_event[2] = next_departure_time
            # print(f"next departure event time is updated to {next_departure_time}")

            """
            the time-of-arrival array is moved up one place, to 
            represent that next customer is now first in line
            """
            for i in range(1, self.num_in_q + 1):
                # np.put(time_arrival, i, time_arrival[i + 1])
                self.time_arrival[i] = self.time_arrival[i + 1]

    def report(self):
        print(f'Average delay in queue {self.total_of_delays / self.num_custs_delayed}', file=self.outfile)
        print(f'Average number in queue {self.area_num_in_q / self.sim_time}', file=self.outfile)
        print(f'Server utilization {self.area_server_status / self.sim_time}', file=self.outfile)
        print(f'Time simulation ended {self.sim_time} minutes', file=self.outfile)

    def get_stats(self):
        return {
            'mean_service_time': self.mean_service,
            'average_delay': self.total_of_delays / self.num_custs_delayed,
            'average_num_in_q': self.area_num_in_q / self.sim_time,
            'server_utilization': self.area_server_status / self.sim_time,
            'simulation_time': self.sim_time
        }

    def reportConsole(self):
        print(f'Average delay in queue {self.total_of_delays / self.num_custs_delayed}')
        print(f'Average number in queue {self.area_num_in_q / self.sim_time}')
        print(f'Server utilization {self.area_server_status / self.sim_time}')
        print(f'Time simulation ended {self.sim_time} minutes')

    def run(self):
        # print("Program started...")

        self.outfile = open(
            f'{os.path.join("out", "mm1+" + str(self.mean_interarrival) + "+" + str(self.mean_service) + "+" + str(self.num_delays_required))}.out',
            "w")

        self.num_events = 2

        # print("Single server queuing system", file=self.outfile)
        # print(f'Mean interarrival time {self.mean_interarrival} minutes', file=self.outfile)
        # print(f'Mean service time {self.mean_service} minutes', file=self.outfile)
        # print(f'Number of customers {self.num_delays_required}', file=self.outfile)

        self.initialize()

        i = 0
        """
        The “while” loop then executes the simulation as long as more 
        customer delays are needed to fulfi ll the 1000-delay stopping 
        rule
        """
        while self.num_custs_delayed < self.num_delays_required:
            # print(f'event count={i}' + "_" * 50)
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
            self.timing()

            self.update_time_avg_stats()

            """
             Then a switch statement, based on next_event_type (=1 for 
             an arrival and 2 for a departure), passes control to the 
             appropriate event function.
            """
            if self.next_event_type == 1:
                self.arrive()
            elif self.next_event_type == 2:
                self.depart()

        # print("simulation done" + "_" * 50)
        self.report()

        self.outfile.close()


if __name__ == "__main__":
    shutil.rmtree('out', ignore_errors=True)
    if not os.path.exists('out'):
        os.mkdir('out', )

    fileNames = [f for f in os.listdir('in') if os.path.isfile(os.path.join('in', f))]
    fileNames.sort()
    configs = []
    stats = []
    for fileName in fileNames:
        infile = open(os.path.join('in', fileName), 'r')
        raw_config = infile.readlines()[0].split(' ')
        configs.append({
            'mean_interarrival': float(raw_config[0]),
            'mean_service': float(raw_config[1]),
            'num_delays_required': float(raw_config[2]),
        })
    for config in configs:
        # print(config)
        mm1 = MM1(config['mean_interarrival'], config['mean_service'], config['num_delays_required'])
        mm1.run()
        stats.append(mm1.get_stats())
        mm1.generate_random_variable_stats_report()

    overall_report = open(f'{os.path.join("out","report.out")}',"w")
    print("_" * 100, file=overall_report)
    print("k\t\taverage delay\t\taverage number\t\tserver util\t\t\tsimulation time", file=overall_report)
    print("_" * 100, file=overall_report)
    for stat in stats:
        print(
            f"{stat['mean_service_time']}\t\t{stat['average_delay']}\t{stat['average_num_in_q']}\t{stat['server_utilization']}\t{stat['simulation_time']}", file=overall_report)
    print("_" * 100, file=overall_report)
    overall_report.close()


