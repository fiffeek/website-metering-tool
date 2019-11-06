# website-metering-tool

A tool for website availability & performance monitoring. Made as an assignment from Datadog on two flights and on one airport :). 
## This is an overview of a task:
- Create a console program to monitor performance and availability of websites
- Websites and check intervals are user defined
- Users can keep the console app running and monitor the websites

# Usage
```
cmake . && make 
./website-metering -f input.txt
```

# Tests
```
cmake . && make 
./alerting-test
```

# Input
I decided to go with a simple input approach, user states a path (relative or absolute) to a file with input.
The file has a following structure:
```
website seconds
website seconds
website seconds
```
For example, this is a valid input file:
```
https://www.youtube.com/ 1
https://en.wikipedia.org/wiki/HTTP_40412312123 3
```

# FAQ
## Seconds?
Why seconds? Mostly for parsing convenience, I would have gone with cron format instead but due to a lack of time I could not have done so.

## C++?
Yeah, I am always asked the same question, why c++? It would have been much easier to write in Scala or Java. And the answer is that I really like to challenge myself to learn more about c++ as a language, when I was young I fell in love with it and try to solve thing my way. This does not mean that I do not know how to program in another languages!


## Architecture?
The app is highly concurrent. That said the main process is idle, I could not find a proper usage for that, I wanted to write a client and a server so that user can dynamically add websites to the db but did not have much time to do so (I would have gone with a main process with socket then and it would not have been idle).
I used a lot of additional stuff from the internet that I could find so that I wouuld not have to implement all of that again :) (f.e. thread pools and schedulers).

## What do I need to run this?
All is specified in `CMakeLists.txt` so you can find it easily, I decided to go with static `boost` linking (linked boost version is `1.65.1.0`), you probably have to download that.
Aside from boost all is included in the repo, I wanted to use the package manager presented on cppcon 2018 but did not have much time to do so again (so most of them is header only)!

## Why only one test?
Due to a lack of time I went all the way to research useable mock libraries and could not make gtest work so I found another way and wrote a test for the main alerting fuunctionality. I should have written more unit tests and at least some ITs.

## What can be improved?
- currently (because I thought I could not use anything but console app) db is in memory on a hashmap and data is given through a reporter, it should have been `kafka` (or any other message queue) and some db like `elastic` which could handle it at ease
- aggregates are additional db's that aggregate over given period of time, that could have been done on pointers
- finding a specific time in db is done in `O(logn)` time where `n` is the number of reports in `db`, it can be done smoothly in `amortized const time`
- lack of tests (Unit and ITs [none at all])
- main thread is idle
- on stuff that takes a lot of memory I should have used heap allocations and smart pointers
- package manager and testing on different platforms should be done (tested on ubuntu 16.04 & 18.04 & debian)
- structure for tests can be vastly improved by adding interfaces `(ABCs to be exact)`

## What is included?
- all dependencies aside from boost
- source code
- tests code
- two binary files (x64)
