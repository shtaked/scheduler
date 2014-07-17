Scheduler v 0.1

Implementation of distributed computing system's logics.
Scheduler can receive 2 kind of inputs: jobs and free resurces.
Input comes on POSIX fifo (named pipe), but there can be another implementations of message receivers (sockets for example)

Scheduler receives jobs as a tuples : (3 7) (10 2) (5 5), etc., where first value means resource units that job needs, and second is
steps count.

Schedular receives resources in similar way  - couples of values, where first is free units, and second is node number.

Scheduler print all work information to console. But writing to file can be simple implemented.

For testing purpose special InputSimulator was generated.
This simulator generates two stream of both jobs and resources messages independently.
All messages are sent also through fifo. So, simulator's binary should be in the same directory as Scheduler (because fifo is a file).
Simulator has some settings. You can set interval count of messages to send per time on both lines. 
And you can set time interval for sending this messages to corresponding streams.
Also you can set available resources count.

To compile both targets just use make in root directory.
Warning: some c++11 features are used in sources, so the higher GCC version you use the better. I recommend >= 4.8.
Also you can use XCode on Mac OS X, then are special .xcodeproj target files for this

Things to do in new version:
**	Implement logging to file
**	Implement some scheduling algorythm improvements
**	Test for bugs in disconnection
**	Implement socket version of communication
**	Make interactive (create report, show queue, etc.)
**	Imrove logging mechanics (separate logger class)


