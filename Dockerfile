FROM --platform=linux/amd64 ubuntu:latest
LABEL authors="fist_it"

RUN apt update

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata
RUN apt install -y vim build-essential git cmake net-tools gdb clang valgrind

WORKDIR /postfix
