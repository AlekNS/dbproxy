DbProxy
========

This is very simple service for proxy tcp and dump plain queries to a file (supported only Postgres).


Docker
-------

> Linux is preferred environment.

Need to install docker first.

Run service in the container (log output to the current folder ${PWD}):

1. `docker build -f Dockerfile.build -t alekns/dbproxy .`
1. `docker run --name dbproxy-dev --rm -t -p 54321:54321 -v ${PWD}:/usr/src/app/log alekns/dbproxy 
  build/DbProxy --db-type pgsql --workers 4 --log-file log/dbproxy.log --local-port 54321 --local-ip 0.0.0.0 --upstream-port 5432 --upstream-ip YOUR_REMOTE_POSTGRES_HOST`


Run tests:

1. `docker run --name dbproxy-dev-tests --rm -ti alekns/dbproxy sh -c "cd build ; ctest"`


> Use it to force stop container `docker kill dbproxy-dev`.

Pre-requisites
---------------

1. C++14
1. CMake >= 3.5
1. Boost ~1.62


Build
------

1. `$ mkdir -p build`
1. `$ cd build`
1. `$ cmake ..`
1. `$ make`


Run
----

1. `$ ./DbProxy --db-type pgsql --workers 4 --local-port 54321 --local-ip 127.0.0.1 --upstream-port 5432 --upstream-ip 127.0.0.1`


Tests
------

1. `$ ctest`
