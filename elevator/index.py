import random
import numpy as np

TOTAL_MAX_CUSTOMERS = 400
TOTAL_ELEVATORS = 4
TOTAL_FLOORS = 12

# STEP 1
DELTIME = 0
ELEVTIME = 0
MAXDEL = 0
MAXELEV = 0
QUELEN = 0
QUETIME = 0
MAXQUE = 0
quetotal = 0
remain = 0

between = np.random.randint(1, 100, (TOTAL_MAX_CUSTOMERS+1,))
floor = np.random.randint(2, TOTAL_FLOORS+1, (TOTAL_MAX_CUSTOMERS+1,))
delivery = np.random.randint(1, 100, (TOTAL_MAX_CUSTOMERS+1,))

first = np.full((TOTAL_ELEVATORS+1,), 0, dtype=int)
occup = np.full((TOTAL_ELEVATORS+1,), 0, dtype=int)
selvec = np.full((TOTAL_ELEVATORS+1, TOTAL_FLOORS+1), 0, dtype=int)
flrvec = np.full((TOTAL_ELEVATORS+1, TOTAL_FLOORS+1), 0, dtype=int)
elevator = np.full((TOTAL_MAX_CUSTOMERS+1,), 0, dtype=int)
eldel = np.full((TOTAL_ELEVATORS+1,), 0, dtype=int)
arrive = np.full((TOTAL_MAX_CUSTOMERS+1,), 0, dtype=int)
queue_length_zero = False

# STEP 2
i = 0
delivery[i] = 15

# STEP 3
TIME = between[i]
return_to_ground = np.full((4+1,), TIME, dtype=int)
stop = np.full((4+1,), 0, dtype=int)
operate = np.full((4+1,), 0, dtype=int)
wait = np.full((TOTAL_MAX_CUSTOMERS+1,), 0, dtype=int)

j = 0
# STEP 4
while TIME <= 4800:

    if not queue_length_zero:
        # STEP 5
        j = 0
        if TIME >= return_to_ground[1]:
            j = 1
        elif TIME >= return_to_ground[2]:
            j = 2
        elif TIME >= return_to_ground[3]:
            j = 3
        elif TIME >= return_to_ground[4]:
            j = 4

    if not j == 0 or queue_length_zero:

        if not queue_length_zero:
            # STEP 6
            first[j] = i
            occup[j] = 0

            for k in range(1, TOTAL_FLOORS+1):
                selvec[j][k] = 0
                flrvec[j][k] = 0

        # STEP 7
        while 1:
            if not queue_length_zero:
                selvec[j][floor[i]] = 1
                flrvec[j][floor[i]] = flrvec[j][floor[i]] + 1
                occup[j] = occup[j] + 1

            queue_length_zero = False
            # STEP 8
            i = i+1
            TIME = TIME + between[i]
            delivery[i] = 15

            # STEP 9
            for k in range(TOTAL_ELEVATORS+1):
                if TIME >= return_to_ground[k]:
                    return_to_ground[k] = TIME

            # STEP 10
            limit = 0
            if between[i] <= 15 and occup[j] < 12:
                for k in range(first[j], i-1+1):
                    delivery[k] = delivery[k] + between[i]
                continue # GO TO STEP 7
            else:
                limit = i - 1
                break

        # STEP 11
        for k in range(first[j], limit+1):
            # step 12
            N = floor[k] - 1
            elevator[k] = 10 * N + 3*np.sum(flrvec[j][1:N+1]) + 3 + 10 * np.sum(selvec[j][1:N+1]) + 5
            # step 13
            delivery[k] = delivery[k] + elevator[k]
            # step 14
            DELTIME = DELTIME + delivery[k]
            # step 15
            if delivery[k] > MAXDEL:
                MAXDEL = delivery[k]
            # step 16
            if elevator[k] > MAXELEV:
                MAXELEV = elevator[k]

        # step 17
        stop[j] = stop[j] + np.sum(selvec[j][1:TOTAL_FLOORS+1])

        Max = np.argmax(selvec[j])

        eldel[j] = 20 * (Max - 1) + 3 * np.sum(flrvec[j][1:TOTAL_FLOORS+1]) + 10 * np.sum(selvec[j][1:TOTAL_FLOORS+1])

        return_to_ground[j] = TIME + eldel[j]

        operate[j] = operate[j] + eldel[j]

        # step 18
        continue

    # step 19
    quecust = i
    startque = TIME
    queue = 1
    arrive[i] = TIME

    while 1:
        # step 20
        i = i+1
        TIME = TIME + between[i]
        arrive[i] = TIME
        queue = queue + 1

        # step 21
        if TIME >= return_to_ground[1]:
            j = 1
            break
        elif TIME >= return_to_ground[2]:
            j = 2
            break
        elif TIME >= return_to_ground[3]:
            j = 3
            break
        elif TIME >= return_to_ground[4]:
            j = 4
            break
        else:
            continue

    # step 22
    for k in range(1, TOTAL_FLOORS+1):
        selvec[j][k] = 0
        flrvec[j][k] = 0

    remain = queue - 12

    # step 23
    if remain <= 0:
        R = i
        occup[j] = queue
    else:
        R = quecust + 11
        occup[j] = 12

    # step 24
    for k in range(quecust, R+1):
        selvec[j][floor[k]] = 1
        flrvec[j][floor[k]] = flrvec[j][floor[k]] + 1

    # step 25
    if queue >= QUELEN:
        QUELEN = queue

    # step 26
    quetotal = quetotal + occup[j]

    QUETIME = QUETIME + np.sum(TIME - arrive[quecust:R+1])

    # step 27
    if TIME - startque >= MAXQUE:
        MAXQUE = TIME - startque

    # step 28
    first[j] =  quecust

    # step 29
    for k in range(first[j], R+1):
        delivery[k] = 15+(TIME-arrive[k])
        wait[k] = TIME - arrive[k]

    # step 30
    if remain <= 0:
        queue = 0
        queue_length_zero = True
        continue  # go to step 8
    else:
        limit = R
        for k in range(first[j], limit+1):
            # step 12
            N = floor[k] - 1
            elevator[k] = 10 * N + 3*np.sum(flrvec[j][1:N+1]) + 3 + 10 * np.sum(selvec[j][1:N+1]) + 5
            # step 13
            delivery[k] = delivery[k] + elevator[k]
            # step 14
            DELTIME = DELTIME + delivery[k]
            # step 15
            if delivery[k] > MAXDEL:
                MAXDEL = delivery[k]
            # step 16
            if elevator[k] > MAXELEV:
                MAXELEV = elevator[k]

        # step 17
        stop[j] = stop[j] + np.sum(selvec[j][1:TOTAL_FLOORS+1])
        Max = np.argmax(selvec[j])
        eldel[j] = 20 * (Max - 1) + 3 * np.sum(flrvec[j][1:TOTAL_FLOORS+1]) + 10 * np.sum(selvec[j][1:TOTAL_FLOORS+1])
        return_to_ground[j] = TIME + eldel[j]
        operate[j] = operate[j] + eldel[j]




