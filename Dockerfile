FROM ubuntu:26.04
RUN apt-get update
RUN apt-get upgrade -y
RUN mkdir app
WORKDIR app
COPY ./install/ndmserver*.deb .
RUN apt-get install ./ndmserver*.deb -y
CMD ["ndmserver"]
