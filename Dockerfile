FROM ubuntu:26.04
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install g++ make libboost-program-options-dev -y
RUN mkdir app
WORKDIR app
COPY . .
RUN make
RUN make install
CMD ["ndmserver"]
