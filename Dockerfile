FROM ubuntu:latest

RUN apt-get update && apt-get install -y g++

COPY main.cpp /tmp/main.cpp

RUN g++ /tmp/main.cpp -o /tmp/main

