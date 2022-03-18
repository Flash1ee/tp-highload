FROM ubuntu:latest as build

RUN apt-get update && apt-get install -y cmake && apt-get install -y g++ && apt-get install -y make

WORKDIR /app

COPY . .

RUN make build

WORKDIR /app/build

EXPOSE 8080

CMD ./static_web_server