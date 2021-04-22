FROM amazonlinux
WORKDIR /home
COPY ./lab56.cpp .
RUN yum install -y gcc-c++
RUN yum install -y nano
RUN yum install -y gdb
RUN g++ lab56.cpp -lpthread -o lab56
ENTRYPOINT /bin/bash